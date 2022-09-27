#ifndef __MATVEC_BlockRowPerm_KERNELS_H__
#define __MATVEC_BlockRowPerm_KERNELS_H__


#define __MY_RESTRICT__ __restrict__

#ifdef __TEST_CPU__
template<const int N>
inline void
product
(
        idx_k*       const                      cnt,
  const idx_k                                   lo,
  const idx_k                                   hi,
        LDUoffDiag_k const * const __restrict__ upper,
        LDUoffDiag_k const * const __restrict__ lower,
        idx_k        const * const __restrict__ owner,
        idx_k        const * const __restrict__ neigh,
        fld_k        const * const __restrict__ psi,
        fld_k              * const __restrict__ Apsi
)
{
  fld_k xr;
  fld_k tmp;
  idx_k k;
  idx_k iNei;
  idx_k iRow;
  for ( iRow=lo; iRow<hi; ++iRow )
  {
    xr = psi[iRow];
    tmp = 0.0;
    for ( k=0; k<N; ++k)
    {
      iNei = neigh[*cnt];
      tmp += upper[*cnt]*psi[iNei];
      Apsi[iNei] += lower[*cnt]*xr;
      (*cnt)++;
    };
    Apsi[iRow] += tmp;
  };
  // Exit point
  return;
};


void
matVec_BlockRowPerm_CPU
(
  idx_k M, // = m_IO.matSize();
  idx_k N, // = Permutation.nSubBlocks();
  //
  idx_k         const * const  __restrict__ blkStart,
  unsigned char const * const  __restrict__ blkKind,
  idx_k         const * const  __restrict__ o, //=Permutation.pOwner();
  idx_k         const * const  __restrict__ n, //=Permutation.pNeigh();
  LDUoffDiag_k  const * const  __restrict__ u, //=Permutation.upper( randomMatrix.upper() );
  LDUoffDiag_k  const * const  __restrict__ l, //=Permutation.lower( randomMatrix.lower() );
  fld_k         const * const  __restrict__ x, //=Permutation.x(     randomMatrix.x()     );
  fld_k               * const  __restrict__ b  //=m_result;
)
{
  // Local variables
  unsigned short i;
  idx_k lo;
  idx_k hi;
  idx_k cnt;

  // Off diagonal part of the product
  lo = 0;
  hi = 0;
  cnt = 0;
  for ( i=0; i<N; ++i )
  {
    lo = hi;
    hi = blkStart[i+1];
    switch ( blkKind[i] )
    {
      case (0):   continue;
      case (1):   product<1 >( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (2):   product<2 >( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (3):   product<3 >( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (4):   product<4 >( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (5):   product<5 >( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (6):   product<6 >( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (7):   product<7 >( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (8):   product<8 >( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (9):   product<9 >( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (10):  product<10>( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (11):  product<11>( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (12):  product<12>( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (13):  product<13>( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (14):  product<14>( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (15):  product<15>( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (16):  product<16>( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (17):  product<17>( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (18):  product<18>( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (19):  product<19>( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (20):  product<20>( &cnt, lo, hi, u, l, o, n, x, b ); break;
      case (21):  product<21>( &cnt, lo, hi, u, l, o, n, x, b ); break;
      default: { printf( "ERROR :: unhandled case in the switch:: %d;\n", blkKind[i] ); abort(); };
    };
  };

  // Stop timer
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
matVec_BlockRowPerm_nativeCUDA
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
matVec_BlockRowPerm_openACC
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
matVec_BlockRowPerm_openMP
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
