/*
 * sparseBlkMatrix02.H
 *
 *  Created on: May 17, 2022
 *      Author: valentini
 */

#ifndef INC_MATRICES_SPARSEBLKMATRIX_H_
#define INC_MATRICES_SPARSEBLKMATRIX_H_


#include "../inc/permutations/blockRowPerm.H"
#include "../sparseMatrixBase.H"
#include "../inc/randomMatrix.H"
#include "../inc/connectivityIO.H"
#include "../inc/timing.H"

#include <stdio.h>


template<const int N>
inline void
product
(
        int*          const              cnt,
  const int                              lo,
  const int                              hi,
  const double*       const __restrict__ upper,
  const double*       const __restrict__ lower,
  const unsigned int* const __restrict__ owner,
  const unsigned int* const __restrict__ neigh,
  const double*       const __restrict__ psi,
        double*       const __restrict__ Apsi
)
{
  double xr;
  double tmp;
  int k;
  int iNei;
  int iRow;
  for ( iRow=lo; iRow<hi; ++iRow )
  {
    xr = psi[iRow]; // const  double &
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




class sparseBlkMatrix:
    public sparseMatrix
{
  private:

    const connectivityIO&       m_IO;
    unsigned int*  __restrict__ m_blockStart;
    unsigned char* __restrict__ m_blockKind;
    double*        __restrict__ m_result;

  public:

    sparseBlkMatrix( const connectivityIO& IO ):
      sparseMatrix(IO),
      m_IO(IO),
      m_blockStart(nullptr),
      m_blockKind(nullptr),
      m_result(nullptr)
    {
      // allocate the remaining data, dimension is an upper bound
      m_result     = static_cast<double*>       ( malloc( 1*m_IO.matSize()*sizeof(double       ) ) );
      m_blockStart = static_cast<unsigned int*> ( malloc( 3*m_IO.matSize()*sizeof(unsigned int ) ) );
      m_blockKind  = static_cast<unsigned char*>( malloc( 3*m_IO.matSize()*sizeof(unsigned char) ) );
      // Exit point
      return;
    };

    ~sparseBlkMatrix()
    {
      // free local variables
      free( m_blockStart );
      free( m_blockKind );
      free( m_result );
      // Exit point
      return;
    };


    /**
     * @brief Load the matrix from the hdf5 files
     *
     * @param [in] name name of the hdf5 file to be loaded
     */
    void
    createDataStructure
    (
	const blockRowPerm& Permutation
    )
    {
      // Initialized the partially compressed indices
      int i;
      for ( i=0; i<Permutation.nSubBlocks(); ++i )
      {
        m_blockStart[i] = (Permutation.subBlocks())[4*i];
        m_blockKind [i] = static_cast<unsigned char>( (Permutation.subBlocks())[4*i+3] );
      };
      m_blockStart[Permutation.nSubBlocks()] = (Permutation.subBlocks())[4*(Permutation.nSubBlocks()-1)+1]+1;
      // Exit point
      return;
    };

    /**
     * @brief Run the benchmark routine
     *
     * @param[in] idx index of the test to be performed
     */
    timeProfile
    run
    (
	const blockRowPerm& Permutation,
	const randomMatrix& randomMatrix
    )
    {
      // Local variables
      unsigned short i;
      int lo;
      int hi;
      int cnt;
      const int M = m_IO.matSize();
      const int N = Permutation.nSubBlocks();
      //
      double       const * const  __restrict__ d=Permutation.diag(  randomMatrix.diag()  );
      double       const * const  __restrict__ u=Permutation.upper( randomMatrix.upper() );
      double       const * const  __restrict__ l=Permutation.lower( randomMatrix.lower() );
      double       const * const  __restrict__ x=Permutation.x(     randomMatrix.x()     );
      unsigned int const * const  __restrict__ o=Permutation.pOwner();
      unsigned int const * const  __restrict__ n=Permutation.pNeigh();
      double             * const  __restrict__ b=m_result;

      // Start timer
      m_clock.tic();

      // Diagonal part of the product
      for ( cnt=0; cnt<M; ++cnt )
        {
          b[cnt] = d[cnt]*x[cnt];
        };

      // Off diagonal part of the product
      lo = 0;
      hi = 0;
      cnt = 0;
      for ( i=0; i<N; ++i )
      {
        lo = hi;
        hi = m_blockStart[i+1];
        // printf( "%d, %d, %d \n", lo, hi, m_blockKind[i] );
        switch ( m_blockKind[i] )
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
          default: { printf( "ERROR :: unhandled case in the switch:: %d;\n", m_blockKind[i] ); abort(); };
        };
      };

      // Stop timer
      return( m_clock.toc() );

    };

};


#endif /* INC_MATRICES_SPARSEBLKMATRIX_H_ */
