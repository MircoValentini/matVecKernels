#ifndef __MSR_IMPLICIT_DIV_KERNELS_H__
#define __MSR_IMPLICIT_DIV_KERNELS_H__

#define __MY_RESTRICT__ __restrict__

#ifdef __TEST_CPU__
void
MSR_implicitDivUpwind_InternalFaces_CPU
(
    const idx_k                               nInternalFaces,
    const int                                 sign,
    const idx_k*        const __MY_RESTRICT__ owner,
    const idx_k*        const __MY_RESTRICT__ neigh,
    const idx_k*        const __MY_RESTRICT__ permutation,
    const fld_k*        const __MY_RESTRICT__ phi,
          LDUdiag_k*    const __MY_RESTRICT__ diag,
          LDUoffDiag_k* const __MY_RESTRICT__ values
)
{

  // Local variables
  idx_k iFace;
  idx_k iComp;
  idx_k iPrev;
  idx_k iNext;
  fld_k tmp;

  // Main loop
  for ( iFace=0; iFace<=nInternalFaces; iFace++ )
  {
    // Get the indices of the previous and the next cell
    iPrev  = owner[iFace];
    iNext  = neigh[iFace];
    // Extract the flux from the surface field
    tmp = phi[iFace];
    // Assembly the matrix components
    values[permutation[iFace+nInternalFaces]] += sign*__MMIN__( -tmp, 0.0 );
    values[permutation[iFace]]                += sign*__MMIN__(  tmp, 0.0 );
    diag [iPrev] += sign*__MMAX__(  tmp, 0.0 );
    diag [iNext] += sign*__MMAX__( -tmp, 0.0 );
  };
  // Exit point
  return;
};
#endif


#ifdef __TEST_NATIVE_CUDA__

#endif


#ifdef __TEST_OPEN_ACC__

#endif


#ifdef __TEST_OPEN_MP__

#endif

#endif
