/*
 * mainPermutation.C
 *
 *  Created on: May 13, 2022
 *      Author: valentini
 */

#include "../inc/permutations/blockRowPerm.H"
#include "../inc/randomMatrix.H"

#include "../inc/matrices/sparseMatrices.H"
#include "../inc/matrices/sparseStdMatrix.H"
#include "../inc/matrices/sparseBlkMatrix.H"
#include "../inc/timing.H"

int
main()
{
  int i;
  int j;
  int k;

  int nBlk=15;
  int nRep=100;

  const char* ownerFname="./owner";
  const char* neighFname="./neigh";

  unsigned int seed=123456;

  int* nBlocks;

  // Initialize objects
  connectivityIO IO( ownerFname, neighFname, 400 );
  // Read the connectivity from openFOAM files
  IO.readTXT();
  IO.writeHDF5( "test.h5" );
  IO.writeOctave();

  randomMatrix RandomMatrix( seed, IO );
  blockRowPerm Permutation( IO );
  sparseMatrices MatrixFormatList( IO );
  timeProfile timeMat[MatrixFormatList.size()];

  // Reset the permutation
  Permutation.resetMemory();

  // Compute the permutation
  Permutation.computePermutation( nBlk );
  Permutation.print();
  Permutation.writeOctave();


  // Number of repetitions for each product
  for ( j=0; j<nRep; j++ )
    {
      // Generate Random Matrix
      RandomMatrix.computeRandomMatrix( -100, 100, 4 );

      // Compute the matrix product with standard matrix
      timeMat[0] += MatrixFormatList[0].run( Permutation, RandomMatrix );

      for ( k=1; k<MatrixFormatList.size(); ++k )
        {
          // Create the specific data structure needed for the matrix vector product
	  MatrixFormatList[k].createDataStructure( Permutation );
          // Compute the matrix product with modified product
          timeMat[k] += MatrixFormatList[k].run( Permutation, RandomMatrix );
        };

    };

  for ( i=0; i<MatrixFormatList.size(); ++i )
    {
      timeMat[i].print();
    };
  // Write results on hdf5
  // WriteOutput(  );

  // Exit point
  return(0);

};

