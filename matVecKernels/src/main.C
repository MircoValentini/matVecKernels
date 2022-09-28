/**
 * @file main.cu
 *
 * @brief main file used to handle all the unit test wiht a single 
 *        entry point.
 *        Specific tests can be selected through the comman line parameters
 *
 * @date   18 Giu 2022
 * @author Mirco Valentini
 */

/*
 * System include
 */
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#ifdef __MPI__
#include "mpi.h"
#endif

/**
 * Nvidia headers
 */
#if defined(__NATIVE_CUDA__) || defined(__OPEN_ACC__) || defined(__OPEN_MP__)
#include <cuda_runtime_api.h>
#include <cuda.h>
#endif

/**
 * Local include
 */
#include "unitTestFactory.H"
#include "dataBase.H"

/**
 * Main entry point for all the test cases
 */
int
main
(
  int    argc,
  char** argv
)
{

#ifdef __MPI__
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
#endif

    // Profiling data
    timeProfile profileCheckTime;
    timeProfile profileWarmupTime;
    timeProfile profileRaceTime;

    // Declare the object that handles the unit test
    unitTestFactory testFactory( argc, argv );

    // Create the test
    unitTestBase* ut = testFactory.allocate();

    // Create the database object
    sql OutDB( *ut );

    // Load input data from the file
    ut->loadInputData();

    // Load expected output data from the file
    ut->loadOutputData();

    // Prepare data for the specific kernel
    ut->Prepare();

    // Allocate data on the device if the unitt est i son the device
    ut->allocateDevice();

    // Copy data to the device 
    ut->copyToDevice();

    // Run the computational kernel
    ut->warmup( profileWarmupTime );
/*
    ut->warmup( profileWarmupTime );
    ut->race( profileRaceTime, testFactory.nLaps() );
*/
    // Dispose the test case
    ut->Dispose();

    // Copy back data from the device
    ut->copyFromDevice();

    // Check if the result is correct
    bool check = ut->check( );

    if ( check )
    {

      // Rerun the warmup in ordr to fix the cache
      ut->warmup( profileCheckTime );

      // Run the race 
      ut->race( profileRaceTime, testFactory.numberOfRepetitions() );


#ifdef __MPI__
      double* warmup_executionTime = nullptr;
      double* warmup_clockTime     = nullptr;
      double* warmup_cudaTime      = nullptr;
      double* race_executionTime = nullptr;
      double* race_clockTime     = nullptr;
      double* race_cudaTime      = nullptr;
      double tmp;
      if ( world_rank == 0 )
      {
        warmup_executionTime = (double*)malloc( sizeof(double)*world_size );
        warmup_clockTime     = (double*)malloc( sizeof(double)*world_size );
        warmup_cudaTime      = (double*)malloc( sizeof(double)*world_size );
        race_executionTime = (double*)malloc( sizeof(double)*world_size );
        race_clockTime     = (double*)malloc( sizeof(double)*world_size );
        race_cudaTime      = (double*)malloc( sizeof(double)*world_size );
      }
      tmp=profileWarmupTime.executionTime();
      MPI_Gather( &tmp, 1, MPI_DOUBLE, warmup_executionTime, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      tmp=profileWarmupTime.clockTime();
      MPI_Gather( &tmp, 1, MPI_DOUBLE, warmup_clockTime, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      tmp=profileWarmupTime.cudaTime();
      MPI_Gather( &tmp, 1, MPI_DOUBLE, warmup_cudaTime, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      tmp=profileRaceTime.executionTime()/testFactory.numberOfRepetitions();
      MPI_Gather( &tmp, 1, MPI_DOUBLE, race_executionTime, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      tmp=profileRaceTime.clockTime()/testFactory.numberOfRepetitions();
      MPI_Gather( &tmp, 1, MPI_DOUBLE, race_clockTime, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      tmp=profileRaceTime.cudaTime()/testFactory.numberOfRepetitions();
      MPI_Gather( &tmp, 1, MPI_DOUBLE, race_cudaTime, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

      if ( world_rank == 0 )
      {
        // Open the database
        OutDB.open( );
        // Create table
        OutDB.createTable();
        for ( int i=0; i<world_size; ++i )
        {
          OutDB.write
          (
            i,
            world_size,
            testFactory.numberOfRepetitions(),
            warmup_executionTime[i],
            warmup_clockTime[i],
            warmup_cudaTime[i],
            race_executionTime[i],
            race_clockTime[i],
            race_cudaTime[i]
          );
        }
        // printf( "confronto :: %lf - ", profileWarmupTime.executionTime() );
        // printf( "%lf\n", executionTime[0] );
        // Close the database
        OutDB.close();
        free(warmup_executionTime);
        free(warmup_clockTime);
        free(warmup_cudaTime);
        free(race_executionTime);
        free(race_clockTime);
        free(race_cudaTime);
      }
#else
      // Open the database
      OutDB.open( "test01.db" );
      // Create table
      OutDB.createTable();
      // Write data
      OutDB.write
      (
        0,
        1,
        testFactory.numberOfRepetitions(),
        profileWarmupTime.executionTime(),
        profileWarmupTime.clockTime(),
        profileWarmupTime.cudaTime(),
        profileRaceTime.executionTime(),
        profileRaceTime.clockTime(),
        profileRaceTime.cudaTime()
      );
      // Close the database
      OutDB.close();
#endif
    }

    // Free the unit test
    ut->Free();

    // Print the result of the tests
    if ( check )
    {
      if ( testFactory.verbose() )
      {
        profileWarmupTime.print();
        profileRaceTime.print( testFactory.numberOfRepetitions() );
      }
    }
    else
    {
#ifdef __TEST_CPU__
-      printf( "ERROR :: check not pased (CPU) :: %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__ );
#endif
#ifdef __TEST_NATIVE_CUDA__
-      printf( "ERROR :: check not pased (native CUDA) :: %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__ );
#endif
#ifdef __TEST_OPEN_ACC__
-      printf( "ERROR :: check not pased (openACC) :: %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__ );
#endif
#ifdef __TEST_OPEN_MP__
-      printf( "ERROR :: check not pased (openMP) :: %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__ );
#endif
      abort();
    };

    // Free the factory
    testFactory.free();

#ifdef __MPI__
    // Finalize the MPI environment.
    MPI_Finalize();
#endif

    // Exit point
    return(0);

};
