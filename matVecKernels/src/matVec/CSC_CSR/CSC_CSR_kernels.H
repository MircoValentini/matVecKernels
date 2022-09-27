#ifndef __MATVEC_CSC_CSR_KERNELS_H__
#define __MATVEC_CSC_CSR_KERNELS_H__


#define __MY_RESTRICT__ __restrict__

#ifdef __TEST_CPU__
void
matVec_CSC_CSR_CPU
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
    const LDUoffDiag_k* const __MY_RESTRICT__ upper,
    const LDUoffDiag_k* const __MY_RESTRICT__ lower,
    const fld_k*        const __MY_RESTRICT__ psi,
          fld_k*        const __MY_RESTRICT__ Apsi
)
{
  // Local variables
  fld_k const * const __restrict__ loc_psi=&psi[iComp*cellsStride];
  // Perform computation
  for ( idx_k iRow=0; iRow<nCells; iRow++ )
  {
    fld_k xr = loc_psi[iRow];
    fld_k tmp = 0;
    for ( idx_k iCol=rowStart[iRow]; iCol<rowStart[iRow+1]; iCol++ )
    {
      idx_k iNei  = neigh[iCol];
      tmp   += upper[iCol] * loc_psi[iNei];
      Apsi[iNei] += lower[iCol]*xr;
    }
    Apsi[iRow] += tmp;
  };
  // Exit point
  return;
};
#endif

#ifdef __TEST_NATIVE_CUDA__
__device__ double MYatomicAdd(fld_k* address, fld_k val)
{
    unsigned long long int* address_as_ull = (unsigned long long int*)address;
    unsigned long long int old = *address_as_ull, assumed;
    do {
        assumed = old;
        old = atomicCAS(address_as_ull, assumed,
                __double_as_longlong(val + __longlong_as_double(assumed)));
    } while (assumed != old);
    return __longlong_as_double(old);
}

__global__
void
matVec_CSC_CSR_nativeCUDA
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
    const LDUoffDiag_k* const __MY_RESTRICT__ upper,
    const LDUoffDiag_k* const __MY_RESTRICT__ lower,
    const fld_k*        const __MY_RESTRICT__ psi,
          fld_k*        const __MY_RESTRICT__ Apsi
)
{
  // Local variables
  fld_k const * const __restrict__ loc_psi=&psi[iComp*cellsStride];

  idx_k iRow = blockIdx.x*blockDim.x + threadIdx.x;
  // Perform computation
  if ( iRow < nCells )
  {
    fld_k xr = loc_psi[iRow];
    fld_k tmp = 0;
    for ( idx_k iCol=rowStart[iRow]; iCol<rowStart[iRow+1]; iCol++ )
    {
      idx_k iNei  = neigh[iCol];
      tmp   += upper[iCol] * loc_psi[iNei];
      atomicAdd( &Apsi[iNei],  lower[iCol]*xr);
    }
    atomicAdd( &Apsi[iRow], tmp);
  };
  // Exit point
  return;
};
#endif


#ifdef __TEST_OPEN_ACC__
void
matVec_CSC_CSR_openACC
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
    const LDUoffDiag_k* const __MY_RESTRICT__ upper,
    const LDUoffDiag_k* const __MY_RESTRICT__ lower,
    const fld_k*        const __MY_RESTRICT__ psi,
          fld_k*        const __MY_RESTRICT__ Apsi
)
{
  // Local variables
  fld_k const * const __restrict__ loc_psi=&psi[iComp*cellsStride];
  // Perform computation
  #pragma acc parallel loop deviceptr(owner,neigh,upper,lower,loc_psi,Apsi,rowStart)
  for ( idx_k iRow=0; iRow<nCells; iRow++ )
  {
    fld_k xr  = loc_psi[iRow];
    fld_k tmp = 0.0;
    #pragma acc loop seq 
    for ( idx_k iCol=rowStart[iRow]; iCol<rowStart[iRow+1]; iCol++ )
    {
      idx_k iNei  = neigh[iCol];
      tmp   += upper[iCol] *loc_psi[iNei];
      #pragma acc atomic update
      Apsi[iNei] += lower[iCol]*xr;
    };
    #pragma acc atomic update
    Apsi[iRow] += tmp;
  };
  // Exit point
  return;
};
#endif

#ifdef __TEST_OPEN_MP__
void
matVec_CSC_CSR_openMP
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
    const LDUoffDiag_k* const __MY_RESTRICT__ upper,
    const LDUoffDiag_k* const __MY_RESTRICT__ lower,
    const fld_k*        const __MY_RESTRICT__ psi,
          fld_k*        const __MY_RESTRICT__ Apsi
)
{
  // Local variables
  fld_k const * const __restrict__ loc_psi=&psi[iComp*cellsStride];
  // Perform computation
  for ( idx_k iRow=0; iRow<nCells; iRow++ )
  {
    fld_k xr = loc_psi[iRow];
    fld_k tmp = 0;
    for ( idx_k iCol=rowStart[iRow]; iCol<rowStart[iRow+1]; iCol++ )
    {
      idx_k iNei  = neigh[iCol];
      tmp   += upper[iCol] * loc_psi[iNei];
      Apsi[iNei] += lower[iCol]*xr;
    }
    Apsi[iRow] += tmp;
  };
  // Exit point
  return;
};
#endif

#endif
