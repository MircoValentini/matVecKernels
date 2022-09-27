/*
 * converter.C
 *
 *  Created on: May 19, 2022
 *      Author: valentini
 */
// #include "../inc/permutations/blockRowPerm.H"
#include "connectivityIO.H"
#include "../inc/randomMatrix.H"

// #include "../inc/matrices/sparseMatrices.H"
// #include "../inc/matrices/sparseStdMatrix.H"
// #include "../inc/matrices/sparseBlkMatrix.H"
// #include "../inc/timing.H"

// Generate unite test data of the same 
#include "UnitTestsIOManager.H"

void
usage()
{

  printf( "./converter.x -[hao:n:d:p:s:l:q:c:r:t:]\n" );
  printf( " -h: show this message \n" );
  printf( " -a: simulate only\n" );
  printf( " -o: owner file         \n" );
  printf( " -n: neigh file         \n" );
  printf( " -d: out directory      \n" );
  printf( " -p: prefix             \n" );
  printf( " -s: seed               \n" );
  printf( " -l: min Value          \n" );
  printf( " -q: max Value          \n" );
  printf( " -c: number of decimals \n" );
  printf( " -r: number of matrices \n" );
  printf( " -t: number of solutions\n" );

  return;

};


int
main
(
  int argc,
  char *argv[]
)
{

  // Options for the software
  char* ownerFname; //="./owner"; // o
  char* neighFname; // ="./neigh"; // n

  char* UTdir; // =".";          // d
  char* prefix; // ="Motorbike"; // p

  unsigned int seed = initSeed(); // s
  int minVal    = -100;           // l
  int maxVal    = 100;            // q
  int nDecimals = 5;              // c
  int nMat = 1;                   // r
  int nSol = 1;                   // t
  
  bool b_owner=false;
  bool b_neigh=false;
  bool b_UTdir=false;
  bool b_prefix=false;
  bool b_simulate=false;

  // Parse input arguments
  int opt;
  while((opt = getopt(argc, argv, "hao:n:d:p:s:l:q:c:r:t:")) != -1) 
  { 
    switch(opt) 
    { 
        case 'a': 
          b_simulate = true;
          break;
        case 'o': 
          ownerFname=optarg;
          b_owner = true;
          break;
        case 'n':
          neighFname=optarg;
          b_neigh = true;
          break;
        case 'd':
          UTdir=optarg;
          b_UTdir = true;
          break;
        case 'p':
          prefix=optarg;
          b_prefix = true;
          break;
        case 's':
          seed = atoi( optarg );
          break;
        case 'l':
          minVal = atoi( optarg );
          break;
        case 'q':
          maxVal = atoi( optarg );
          break;
        case 'c':
          nDecimals = atoi( optarg );
          break;
        case 'r':
          nMat = atoi( optarg );
          break;
        case 't':
          nSol = atoi( optarg );
          break;
        case 'h':
          usage();
          abort();
          break;
        case '?':
            printf("unknown option: %c\n", optopt);
            usage();
            abort();
            break;
    };
  };

  if ( !b_owner )
  {
    printf( "ERROR: owner file not provided\n" );
    usage();
    abort();
  };

  if ( !b_neigh )
  {
    printf( "ERROR: neighbour file not provided\n" );
    usage();
    abort();
  };
  
  if ( !b_UTdir )
  {
    printf( "ERROR: target directory not provided\n" );
    usage();
    abort();
  };
  
  if ( !b_prefix )
  {
    printf( "ERROR: test case prefix not provided\n" );
    usage();
    abort();
  };

  // log input
  printf( " - owner file          :: %s\n", ownerFname );
  printf( " - neigh file          :: %s\n", neighFname );
  printf( " - out directory       :: %s\n", UTdir );
  printf( " - prefix              :: %s\n", prefix );
  printf( " - seed                :: %d\n", seed );
  printf( " - min Value           :: %d\n", minVal );
  printf( " - max Value           :: %d\n", maxVal );
  printf( " - number of decimals  :: %d\n", nDecimals );
  printf( " - number of matrices  :: %d\n", nMat );
  printf( " - number of solutions :: %d\n", nSol );
  
  if ( b_simulate )
  {
    printf( "Exit because simulation is enabled\n" );
    exit(0);
  };

  // Initialize objects
  connectivityIO IO( ownerFname, neighFname );


  // Read the connectivity from openFOAM files
  IO.readTXT();


  // Manage random matrices
  randomMatrix randMat( seed, IO );
  unsigned int nTest = 0;

  // This function is used to compute a random matrix with ranges for the values
  for ( int i=0; i<nMat; ++i )
  {

    randMat.computeRandomMatrix( minVal, maxVal, nDecimals );
    for ( int j=0; j<nSol; ++j )
    {

      randMat.initPsi( minVal, maxVal, nDecimals );
      randMat.initApsi();



      unsigned int nCells         = IO.nCells();
      unsigned int nFaces         = IO.nFaces();
      unsigned int nInternalFaces = IO.nInternalFaces();
      unsigned int cellsStride    = IO.nCells();
      unsigned int facesStride    = IO.nFaces();
      unsigned int nComp          = 1;
      unsigned int iComp          = 0;

      HDF5kernels Out( UTdir, prefix, "LDUSYSCOOmatVecCompOffDiagStageOneOutput", nTest );
      Out.Create();
      Out.addDataset( "nCells",           flavour_e::input, 1, &nCells );
      Out.addDataset( "nFaces",           flavour_e::input, 1, &nFaces );
      Out.addDataset( "nInternalFaces",   flavour_e::input, 1, &nInternalFaces );
      Out.addDataset( "cellStride",       flavour_e::input, 1, &cellsStride );
      Out.addDataset( "faceStride",       flavour_e::input, 1, &facesStride );
      Out.addDataset( "nComp",            flavour_e::input, 1, &nComp );
      Out.addDataset( "iComp",            flavour_e::input, 1, &iComp );
      Out.addDataset( "owner",            flavour_e::input, nInternalFaces, IO.owner() );
      Out.addDataset( "neigh",            flavour_e::input, nInternalFaces, IO.neigh() );
      Out.addDataset( "upper",            flavour_e::input, nInternalFaces, randMat.upper() );
      Out.addDataset( "lower",            flavour_e::input, nInternalFaces, randMat.lower() );
      Out.addDataset( "psi",              flavour_e::input, cellsStride*nComp, randMat.psi() );
      Out.addDataset( "Apsi",             flavour_e::input, cellsStride, randMat.Apsi() );


      randMat.computeSpMv();

      Out.addDataset( "nCells",           flavour_e::output, 1, &nCells );
      Out.addDataset( "nFaces",           flavour_e::output, 1, &nFaces );
      Out.addDataset( "nInternalFaces",   flavour_e::output, 1, &nInternalFaces );
      Out.addDataset( "cellStride",       flavour_e::output, 1, &cellsStride );
      Out.addDataset( "faceStride",       flavour_e::output, 1, &facesStride );
      Out.addDataset( "nComp",            flavour_e::output, 1, &nComp );
      Out.addDataset( "iComp",            flavour_e::output, 1, &iComp );
      Out.addDataset( "owner",            flavour_e::output, nInternalFaces, IO.owner() );
      Out.addDataset( "neigh",            flavour_e::output, nInternalFaces, IO.neigh() );
      Out.addDataset( "upper",            flavour_e::output, nInternalFaces, randMat.upper() );
      Out.addDataset( "lower",            flavour_e::output, nInternalFaces, randMat.lower() );
      Out.addDataset( "psi",              flavour_e::output, cellsStride*nComp, randMat.psi() );
      Out.addDataset( "Apsi",             flavour_e::output, cellsStride, randMat.Apsi() );
      Out.Close();

      // Update test case id
      nTest++;

    };

  };
  
  // Exit point
  return(0);

};



