#ifndef __CSR_CSR_KERNELS_H__
#define __CSR_CSR_KERNELS_H__

#define __MY_RESTRICT__ __restrict__

#ifdef __TEST_CPU__
void
CSR_CSR_CPU
(
    const idx_k                               nCells,
    const idx_k                               nFaces,
    const idx_k                               nInternalFaces,
    const idx_k                               cellsStride,
    const idx_k                               facesStride,
    const idx_k                               iComp,
    const idx_k*        const __MY_RESTRICT__ owner,
    const idx_k*        const __MY_RESTRICT__ neigh,
    const idx_k*        const __MY_RESTRICT__ rowStart,
    const idx_k*        const __MY_RESTRICT__ powner,
    const idx_k*        const __MY_RESTRICT__ pneigh,
    const idx_k*        const __MY_RESTRICT__ prowStart,
    const LDUoffDiag_k* const __MY_RESTRICT__ upper,
    const LDUoffDiag_k* const __MY_RESTRICT__ lower,
    const LDUoffDiag_k* const __MY_RESTRICT__ plower,
    const fld_k*        const __MY_RESTRICT__ psi,
          fld_k*        const __MY_RESTRICT__ Apsi
)
{
  // Local variables
  fld_k const * const __restrict__ loc_psi=&psi[iComp*cellsStride];
  
  // Perform computation (Upper)
  for ( idx_k iRow=0; iRow<nCells; iRow++ )
  {
    fld_k tmp = 0;
    for ( idx_k iCol=rowStart[iRow]; iCol<rowStart[iRow+1]; iCol++ )
    {
      idx_k iNei  = neigh[iCol];
      tmp   += upper[iCol] * loc_psi[iNei];
    }
    Apsi[iRow] += tmp;
  };


  // Perform computation (Lower)
  for ( idx_k iRow=0; iRow<nCells; iRow++ )
  {
    fld_k tmp = 0;
    for ( idx_k iCol=prowStart[iRow]; iCol<prowStart[iRow+1]; iCol++ )
    {
      idx_k iNei  = powner[iCol];
      tmp   += plower[iCol] * loc_psi[iNei];
    }
    Apsi[iRow] += tmp;
  };

  // Exit point
  return;
};
#endif


#ifdef __TEST_NATIVE_CUDA__
__global__
void
CSR_CSR_nativeCUDA
(
    const idx_k                               nCells,
    const idx_k                               nFaces,
    const idx_k                               nInternalFaces,
    const idx_k                               cellsStride,
    const idx_k                               facesStride,
    const idx_k                               iComp,
    const idx_k*        const __MY_RESTRICT__ owner,
    const idx_k*        const __MY_RESTRICT__ neigh,
    const idx_k*        const __MY_RESTRICT__ rowStart,
    const idx_k*        const __MY_RESTRICT__ powner,
    const idx_k*        const __MY_RESTRICT__ pneigh,
    const idx_k*        const __MY_RESTRICT__ prowStart,
    const LDUoffDiag_k* const __MY_RESTRICT__ upper,
    const LDUoffDiag_k* const __MY_RESTRICT__ lower,
    const LDUoffDiag_k* const __MY_RESTRICT__ plower,
    const fld_k*        const __MY_RESTRICT__ psi,
          fld_k*        const __MY_RESTRICT__ Apsi
)
{
  // Blocks of the matrix
  const idx_k blk=__N_BLOCKS__;
  const int  pp = blk-1;
  // Local variables
  fld_k const * const __restrict__ loc_psi=&psi[iComp*cellsStride];
  // Compute the thread idx
  idx_k tmp = blockIdx.x*blockDim.x + threadIdx.x;
  // Compute the indices of row and the index of thread in each column
  idx_k iRow   = tmp/blk;
  idx_k ithCol = tmp%blk;
  // Perform computation (Upper)
  if ( iRow<nCells )
  {
    // Initialize local variables
    fld_k tmp = 0;
    // Compute the total number of columns in the current row (iRow)
    idx_k nCol = rowStart[iRow+1] - rowStart[iRow];
    // Compute the number of elemnts in each column-thread
    idx_k nColBlk=(nCol+blk-1)/blk;
    // Compute the bounds of the loop on each thread (loverbound and upperbound)
    idx_k nColLo = nColBlk*ithCol;
    idx_k nColHi = nColBlk*(ithCol+1);
    // Correction to the last upper bound of the last thread
    nColHi = nColHi>nCol ? nCol : nColHi;
    // Do operation only if the thread has something to do
    if ( nColLo<nCol )
    {
      // Loop over the elements owned by the column thread
      for ( idx_k iCol=rowStart[iRow]+nColLo; iCol<rowStart[iRow]+nColHi; iCol++ )
      {
        idx_k iNei  = neigh[iCol];
        tmp   += upper[iCol] * loc_psi[iNei];
      };
    };
    __syncthreads();
    // Use the shift to accumulate tmp inside the block
    for (int i=1; i<blk; i*=2)
    {
      fld_k n = __shfl_up_sync(0xffffffff, tmp, i, blk );
      if ( (ithCol & pp ) >= i) {tmp += n;};
    };
    if ( ithCol == blk-1)
    {
      // Add data to the current line
      atomicAdd( &Apsi[iRow], tmp);
    };
  };


  //Perform computation (Lower)
  if ( iRow>=nCells && iRow<2*nCells )
  {
    iRow=iRow-nCells;
    // Initialize local variables
    fld_k tmp = 0;
    // Compute the total number of columns in the current row (iRow)
    idx_k nCol = prowStart[iRow+1] - prowStart[iRow];
    // Compute the number of elemnts in each column-thread
    idx_k nColBlk=(nCol+blk-1)/blk;
    // Compute the bounds of the loop on each thread (loverbound and upperbound)
    idx_k nColLo = nColBlk*ithCol;
    idx_k nColHi = nColBlk*(ithCol+1);
    // Correction to the last upper bound of the last thread
    nColHi = nColHi>nCol ? nCol : nColHi;
    // Do operation only if the thread has something to do
    if ( nColLo<nCol )
    {
      // Loop over the elements owned by the column thread
      for ( idx_k iCol=prowStart[iRow]+nColLo; iCol<prowStart[iRow]+nColHi; iCol++ )
      {
        idx_k iNei  = powner[iCol];
        tmp   += plower[iCol] * loc_psi[iNei];
      };
    };
    __syncthreads();
    // Use the shift to accumulate tmp inside the block
    for (int i=1; i<blk; i*=2)
    {
      fld_k n = __shfl_up_sync(0xffffffff, tmp, i, blk );
      if ( (ithCol & pp ) >= i) {tmp += n;};
    };
    if ( ithCol == blk-1)
    {
      // Add data to the current line
      atomicAdd( &Apsi[iRow], tmp);
    };
  };

  // Exit point
  return;
};

#endif


#ifdef __TEST_OPEN_ACC__
void
CSR_CSR_openACC
(
    const idx_k                               nCells,
    const idx_k                               nFaces,
    const idx_k                               nInternalFaces,
    const idx_k                               cellsStride,
    const idx_k                               facesStride,
    const idx_k                               iComp,
    const idx_k*        const __MY_RESTRICT__ owner,
    const idx_k*        const __MY_RESTRICT__ neigh,
    const idx_k*        const __MY_RESTRICT__ rowStart,
    const idx_k*        const __MY_RESTRICT__ powner,
    const idx_k*        const __MY_RESTRICT__ pneigh,
    const idx_k*        const __MY_RESTRICT__ prowStart,
    const LDUoffDiag_k* const __MY_RESTRICT__ upper,
    const LDUoffDiag_k* const __MY_RESTRICT__ lower,
    const LDUoffDiag_k* const __MY_RESTRICT__ plower,
    const fld_k*        const __MY_RESTRICT__ psi,
          fld_k*        const __MY_RESTRICT__ Apsi
)
{
  // Local variables
  fld_k const * const __restrict__ loc_psi=&psi[iComp*cellsStride];
  // Perform computation

  #pragma acc parallel loop deviceptr(neigh,upper,loc_psi,Apsi,rowStart)
  for ( idx_k iRow=0; iRow<nCells; iRow++ )
  {
    fld_k tmp = 0;
    #pragma acc loop seq
    for ( idx_k iCol=rowStart[iRow]; iCol<rowStart[iRow+1]; iCol++ )
    {
      idx_k iNei  = neigh[iCol];
      tmp += upper[iCol] * loc_psi[iNei];
    }
    Apsi[iRow] += tmp;
  };


  #pragma acc parallel loop deviceptr(powner,plower,loc_psi,Apsi,prowStart)
  for ( idx_k iRow=0; iRow<nCells; iRow++ )
  {
    fld_k tmp = 0;
    #pragma acc loop seq
    for ( idx_k iCol=prowStart[iRow]; iCol<prowStart[iRow+1]; iCol++ )
    {
      idx_k iNei  = powner[iCol];
      tmp += plower[iCol] * loc_psi[iNei];
    }
    Apsi[iRow] += tmp;
  };

  // Exit point
  return;
};
#endif


#ifdef __TEST_OPEN_MP__
void
CSR_CSR_openMP
(
    const idx_k                               nCells,
    const idx_k                               nFaces,
    const idx_k                               nInternalFaces,
    const idx_k                               cellsStride,
    const idx_k                               facesStride,
    const idx_k                               iComp,
    const idx_k*        const __MY_RESTRICT__ owner,
    const idx_k*        const __MY_RESTRICT__ neigh,
    const idx_k*        const __MY_RESTRICT__ rowStart,
    const idx_k*        const __MY_RESTRICT__ powner,
    const idx_k*        const __MY_RESTRICT__ pneigh,
    const idx_k*        const __MY_RESTRICT__ prowStart,
    const LDUoffDiag_k* const __MY_RESTRICT__ upper,
    const LDUoffDiag_k* const __MY_RESTRICT__ lower,
    const LDUoffDiag_k* const __MY_RESTRICT__ plower,
    const fld_k*        const __MY_RESTRICT__ psi,
          fld_k*        const __MY_RESTRICT__ Apsi
)
{
  // Local variables
  fld_k const * const __restrict__ loc_psi=&psi[iComp*cellsStride];
  
  // Perform computation (Upper)
  for ( idx_k iRow=0; iRow<nCells; iRow++ )
  {
    fld_k tmp = 0;
    for ( idx_k iCol=rowStart[iRow]; iCol<rowStart[iRow+1]; iCol++ )
    {
      idx_k iNei  = neigh[iCol];
      tmp   += upper[iCol] * loc_psi[iNei];
    }
    Apsi[iRow] += tmp;
  };


  // Perform computation (Lower)
  for ( idx_k iRow=0; iRow<nCells; iRow++ )
  {
    fld_k tmp = 0;
    for ( idx_k iCol=prowStart[iRow]; iCol<prowStart[iRow+1]; iCol++ )
    {
      idx_k iNei  = powner[iCol];
      tmp   += plower[iCol] * loc_psi[iNei];
    }
    Apsi[iRow] += tmp;
  };

  // Exit point
  return;
};
#endif

#endif
