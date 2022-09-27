#ifndef __MATVEC_BlockRowPermCC_KERNELS_H__
#define __MATVEC_BlockRowPermCC_KERNELS_H__


#define __MY_RESTRICT__ __restrict__

#ifdef __TEST_CPU__
#define __V2__
#ifdef __V0__
template<const int N>
inline void
productCC
(
        idx_k*         const                      cnt,
  const idx_k                                     clo,
  const idx_k                                     rlo,
  const idx_k                                     rhi,
        LDUoffDiag_k   const * const __restrict__ upper,
        LDUoffDiag_k   const * const __restrict__ lower,
        unsigned short const * const __restrict__ dneigh,
        fld_k          const * const __restrict__ psi,
        fld_k                * const __restrict__ Apsi
)
{
  fld_k xr;
  fld_k tmp;
  idx_k k;
  idx_k iNei;
  idx_k iRow;
  for ( iRow=rlo; iRow<rhi; ++iRow )
  {
    xr = psi[iRow];
    tmp = 0.0;
    for ( k=0; k<N; ++k)
    {
      iNei = clo + dneigh[*cnt];
      tmp += upper[*cnt]*psi[iNei];
      Apsi[iNei] += lower[*cnt]*xr;
      (*cnt)++;
    };
    Apsi[iRow] += tmp;
  };
  // Exit point
  return;
};
#endif


#ifdef __V1__
template<const int N>
inline void
productCC
(
        idx_k*         const                      cnt,
  const idx_k                                     clo,
  const idx_k                                     rlo,
  const idx_k                                     rhi,
        LDUoffDiag_k   const * const __restrict__ upper,
        LDUoffDiag_k   const * const __restrict__ lower,
        unsigned short const * const __restrict__ dneigh,
        fld_k          const * const __restrict__ psi,
        fld_k                * const __restrict__ Apsi
)
{
  fld_k xr;
  fld_k tmp;
  idx_k k;
  idx_k iNei;
  idx_k iRow;
  for ( iRow=rlo; iRow<rhi; ++iRow )
  {
    xr = psi[iRow];
    tmp = 0.0;
    iNei = clo;
    for ( k=0; k<N; ++k)
    {
      iNei += dneigh[*cnt];
      tmp += upper[*cnt]*psi[iNei];
      Apsi[iNei] += lower[*cnt]*xr;
      (*cnt)++;
    };
    Apsi[iRow] += tmp;
  };
  // Exit point
  return;
};
#endif


#ifdef __V2__
template<const int N>
inline void
productCC
(
        idx_k*         const                      cnt,
  const idx_k                                     clo,
  const idx_k                                     rlo,
  const idx_k                                     rhi,
        LDUoffDiag_k   const * const __restrict__ upper,
        LDUoffDiag_k   const * const __restrict__ lower,
        unsigned short const * const __restrict__ dneigh,
        fld_k          const * const __restrict__ psi,
        fld_k                * const __restrict__ Apsi
)
{
  fld_k xr;
  fld_k tmp;
  idx_k k;
  idx_k iNei;
  idx_k iRow;
  for ( iRow=rlo; iRow<rhi; ++iRow )
  {
    xr = psi[iRow];
    tmp = 0.0;
    iNei = clo;
    for ( k=0; k<N; ++k)
    {
      iNei += dneigh[*cnt];
      tmp += upper[*cnt]*psi[iNei];
      Apsi[iNei] += lower[*cnt]*xr;
      (*cnt)++;
    };
    Apsi[iRow] += tmp;
  };
  // Exit point
  return;
};
#endif


void
matVec_BlockRowPermCC_CPU
(
  idx_k M, // = m_IO.matSize();
  idx_k N, // = Permutation.nSubBlocks();
  //
  idx_k          const * const  __restrict__ blkStart,
  unsigned short const * const  __restrict__ blkDeltaCol,
  unsigned char  const * const  __restrict__ blkKind,
  unsigned short const * const  __restrict__ dn, //=Permutation.pNeigh();
  LDUoffDiag_k   const * const  __restrict__ u, //=Permutation.upper( randomMatrix.upper() );
  LDUoffDiag_k   const * const  __restrict__ l, //=Permutation.lower( randomMatrix.lower() );
  fld_k          const * const  __restrict__ x, //=Permutation.x(     randomMatrix.x()     );
  fld_k                * const  __restrict__ b  //=m_result;
)
{
  // Local variables
  unsigned short i;
  idx_k rlo;
  idx_k rhi;
  idx_k clo;
  idx_k cnt;

  // Off diagonal part of the product
  rlo = 0;
  rhi = 0;
  clo = 0;
  cnt = 0;
  for ( i=0; i<N; ++i )
  {
    // Row index range
    rlo = rhi;
    rhi = blkStart[i+1];
    // Minimum column index of the block
    clo = blkDeltaCol[i];
    // Call the kernel
    switch ( blkKind[i] )
    {
      case (0):   continue;
      case (1):   productCC<1 >( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (2):   productCC<2 >( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (3):   productCC<3 >( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (4):   productCC<4 >( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (5):   productCC<5 >( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (6):   productCC<6 >( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (7):   productCC<7 >( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (8):   productCC<8 >( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (9):   productCC<9 >( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (10):  productCC<10>( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (11):  productCC<11>( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (12):  productCC<12>( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (13):  productCC<13>( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (14):  productCC<14>( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (15):  productCC<15>( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (16):  productCC<16>( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (17):  productCC<17>( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (18):  productCC<18>( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (19):  productCC<19>( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (20):  productCC<20>( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      case (21):  productCC<21>( &cnt, clo, rlo, rhi, u, l, dn, x, b ); break;
      default: { printf( "ERROR :: unhandled case in the switch:: %d;\n", blkKind[i] ); abort(); };
    };
  };

  // Stop timer
  return;

};
#endif

#ifdef __TEST_NATIVE_CUDA__
__device__ double MYatomicAddCC(fld_k* address, fld_k val)
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
matVec_BlockRowPermCC_nativeCUDA
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
    MYatomicAddCC( &(Apsi[iOwn]), tmp1 );
    MYatomicAddCC( &(Apsi[iNei]), tmp2 );
  };
  // Exit point
  return;
};
#endif


#ifdef __TEST_OPEN_ACC__
void
matVec_BlockRowPermCC_openACC
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
matVec_BlockRowPermCC_openMP
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
