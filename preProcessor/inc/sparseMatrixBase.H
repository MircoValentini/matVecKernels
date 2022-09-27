/*
 * sparseMatrix02.H
 *
 *  Created on: May 17, 2022
 *      Author: valentini
 */

#ifndef INC_SPARSEMATRIXBASE_H_
#define INC_SPARSEMATRIXBASE_H_


#include "../inc/timing.H"
#include "../inc/randomMatrix.H"
#include "../inc/permutations/blockRowPerm.H"

#include <math.h>


/**
 * @class abstract class used for handling sparse Matrices
 */
class sparseMatrix
{

  protected:
    /** Object used to compute the time of the matrix product */
    simpleClock_t m_clock;

    const connectivityIO& m_IO;

  public:
    /**
     * @brief constructor
     *
     * @param [in] block size used in the compression algorithm for saving in hdf5 format
     */
    sparseMatrix
    (
      const connectivityIO& IO
    ):
      m_IO(IO)
    {
      // Exit point
      return;
    };

    /**
     * @brief destructor for the generic
     */
    ~sparseMatrix()
    {
      return;
    };


    /**
     * @brief benchmark routine
     */
    virtual void createDataStructure
    (
	const blockRowPerm& Permutation
    ) = 0;


    virtual timeProfile run
    (
	const blockRowPerm& Permutation,
	const randomMatrix& randomMatrix
    ) = 0;

};


#endif /* INC_SPARSEMATRIXBASE_H_ */
