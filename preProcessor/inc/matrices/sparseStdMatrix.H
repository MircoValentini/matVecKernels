/*
 * sparseStdMatrix.H
 *
 *  Created on: May 18, 2022
 *      Author: valentini
 */

#ifndef INC_MATRICES_SPARSESTDMATRIX_H_
#define INC_MATRICES_SPARSESTDMATRIX_H_


#include "../inc/permutations/blockRowPerm.H"
#include "../sparseMatrixBase.H"
#include "../inc/randomMatrix.H"
#include "../inc/connectivityIO.H"
#include "../inc/timing.H"

#include <stdio.h>



class sparseStdMatrix:
    public sparseMatrix
{
  private:

    const connectivityIO& m_IO;
    double*  __restrict__ m_result;

  public:

    sparseStdMatrix( const connectivityIO& IO ):
      sparseMatrix(IO),
      m_IO(IO),
      m_result(nullptr)
    {
      // allocate the remaining data, dimension is an upper bound
      m_result = static_cast<double*>( malloc( m_IO.matSize()*sizeof(double) ) );
      // Exit point
      return;
    };

    ~sparseStdMatrix()
    {
      // free local variables
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
      int i;
      //
      double       const * const  __restrict__ d=randomMatrix.diag();
      double       const * const  __restrict__ u=randomMatrix.upper();
      double       const * const  __restrict__ l=randomMatrix.lower();
      double       const * const  __restrict__ x=randomMatrix.x();
      unsigned int const * const  __restrict__ o=m_IO.owner();
      unsigned int const * const  __restrict__ n=m_IO.neigh();
      double             * const  __restrict__ b=m_result;

      // Start timer
      m_clock.tic();

      // Diagonal part of the product
      for ( i=0; i<m_IO.matSize(); ++i )
        {
          b[i] = d[i]*x[i];
        }

      // Off diagonal part of the product
      for ( i=0; i<m_IO.matNNZ(); ++i )
	{
	  b[o[i]] = u[i]*x[n[i]];
	  b[n[i]] = l[i]*x[o[i]];
	};

      // Stop timer
      return( m_clock.toc() );

    };

};

#endif /* INC_MATRICES_SPARSESTDMATRIX_H_ */
