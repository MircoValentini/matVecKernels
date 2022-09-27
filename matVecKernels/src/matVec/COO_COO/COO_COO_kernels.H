#ifndef __MATVEC_COO_COO_KERNELS_H__
#define __MATVEC_COO_COO_KERNELS_H__


#define __MY_RESTRICT__ __restrict__

#ifdef __TEST_CPU__
void
matVec_COO_COO_CPU
(
    const idx_k                               nCells,
    const idx_k                               nFaces,
    const idx_k                               nInternalFaces,
    const idx_k                               cellsStride,
    const idx_k                               facesStride,
    const idx_k                               iComp,
    const idx_k*        const __MY_RESTRICT__  owner,
    const idx_k*        const __MY_RESTRICT__ neigh,
    const LDUoffDiag_k* const __MY_RESTRICT__  upper,
    const LDUoffDiag_k* const __MY_RESTRICT__ lower,
    const fld_k*        const __MY_RESTRICT__ psi,
          fld_k*              __MY_RESTRICT__ Apsi
)
{
  // Local variables
  idx_k iFace;
  idx_k iOwn;
  idx_k iNei;
  idx_k iPrev;
  idx_k iNext;
  // relax the linear system
  for ( iFace=0; iFace<nInternalFaces; iFace++ )
  {
    iOwn = owner[iFace];
    iNei = neigh[iFace];
    iPrev = iComp*cellsStride+iOwn;
    iNext = iComp*cellsStride+iNei;
    Apsi[iOwn] += upper[iFace]*psi[iNext];
    Apsi[iNei] += lower[iFace]*psi[iPrev];
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
matVec_COO_COO_nativeCUDA
(
    const idx_k                               nCells,
    const idx_k                               nFaces,
    const idx_k                               nInternalFaces,
    const idx_k                               cellsStride,
    const idx_k                               facesStride,
    const idx_k                               iComp,
    const idx_k*        const __MY_RESTRICT__ owner,
    const idx_k*        const __MY_RESTRICT__ neigh,
    const LDUoffDiag_k* const __MY_RESTRICT__ upper,
    const LDUoffDiag_k* const __MY_RESTRICT__ lower,
    const fld_k*        const __MY_RESTRICT__ psi,
          fld_k*        const __MY_RESTRICT__ Apsi
)
{
  // Local variables
  fld_k const * const __restrict__ loc_psi=&psi[iComp*cellsStride];
  // Compute the index of hte face
  int iFace = blockIdx.x*blockDim.x + threadIdx.x;
  // relax the linear system
  //if ( iFace < nInternalFaces )
  if ( iFace < nInternalFaces )
  {
    // Indirect access indices
    idx_k iOwn = owner[iFace];
    idx_k iNei = neigh[iFace];
    // compute the increments
    fld_k tmp1 = upper[iFace]*loc_psi[iNei];
    fld_k tmp2 = lower[iFace]*loc_psi[iOwn];
    // Accumulate the multiplication
    MYatomicAdd( &(Apsi[iOwn]), tmp1 );
    MYatomicAdd( &(Apsi[iNei]), tmp2 );
  };
  // Exit point
  return;
};
#endif


#ifdef __TEST_OPEN_ACC__
void
matVec_COO_COO_openACC
(
    const idx_k                               nCells,
    const idx_k                               nFaces,
    const idx_k                               nInternalFaces,
    const idx_k                               cellsStride,
    const idx_k                               facesStride,
    const idx_k                               iComp,
    const idx_k*        const __MY_RESTRICT__ owner,
    const idx_k*        const __MY_RESTRICT__ neigh,
    const LDUoffDiag_k* const __MY_RESTRICT__ upper,
    const LDUoffDiag_k* const __MY_RESTRICT__ lower,
    const fld_k*        const __MY_RESTRICT__ psi,
          fld_k*        const __MY_RESTRICT__ Apsi
)
{
  // Local variables
  fld_k const * const __restrict__ loc_psi=&psi[iComp*cellsStride];
  // relax the linear system
  #pragma acc parallel loop deviceptr(owner,neigh,upper,lower,loc_psi,Apsi)
  for ( idx_k iFace=0; iFace<nInternalFaces; iFace++ )
  {
    // Indirect access indices
    idx_k iOwn = owner[iFace];
    idx_k iNei = neigh[iFace];
    #pragma acc atomic update
    Apsi[iOwn] += upper[iFace]*loc_psi[iNei];

    #pragma acc atomic update
    Apsi[iNei] += lower[iFace]*loc_psi[iOwn];
  };
  // Exit point
  return;
};
#endif

#ifdef __TEST_OPEN_MP__
void
matVec_COO_COO_openMP
(
    const idx_k                               nCells,
    const idx_k                               nFaces,
    const idx_k                               nInternalFaces,
    const idx_k                               cellsStride,
    const idx_k                               facesStride,
    const idx_k                               iComp,
    const idx_k*        const __MY_RESTRICT__ owner,
    const idx_k*        const __MY_RESTRICT__ neigh,
    const LDUoffDiag_k* const __MY_RESTRICT__ upper,
    const LDUoffDiag_k* const __MY_RESTRICT__ lower,
    const fld_k*        const __MY_RESTRICT__ psi,
          fld_k*        const __MY_RESTRICT__ Apsi
)
{
  // Local variables
  idx_k iFace;
  idx_k iOwn;
  idx_k iNei;
  idx_k iPrev;
  idx_k iNext;
  // relax the linear system
  for ( iFace=0; iFace<nInternalFaces; iFace++ )
  {
    iOwn = owner[iFace];
    iNei = neigh[iFace];
    iPrev = iComp*cellsStride+iOwn;
    iNext = iComp*cellsStride+iNei;
    Apsi[iOwn] += upper[iFace]*psi[iNext];
    Apsi[iNei] += lower[iFace]*psi[iPrev];
  };
  // Exit point
  return;
};
#endif

#endif
