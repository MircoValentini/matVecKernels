#ifndef __SINGLE_TEST_DEVEL_H__
#define __SINGLE_TEST_DEVEL_H__

/*
 * System include
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


/**
 * @brief Command line parser
 *
 * @param[in]  argc      number of input arguments
 * @param[in]  argv      value for the input arguments
 * @param[out] UTdir     unit test directory
 * @param[out] prefix    prefix for the unit test name
 * @param[out] index     index for the unit test name
 * @param[out] tolerance tolerance used for the check of the unit test
 */
void
parseInputArguments
(
  int          argc,
  char**       argv,
  const char** UTdir,
  const char** prefix,
  int*         index,
   double*     tolerance
)
{

  /**
   * Local variables
   */
  opterr = 0;
  int c;
  bool verbose=false;

  /**
   * Set the default values
   */
  *UTdir     = "defaultDirectory";
  *prefix    = "defaultPrefix";
  *index     = 999999;
  *tolerance = 1.0E-12;

  /**
   * Parse command line arguments
   */
  while ( (c = getopt (argc, argv, "n:d:p:i:t:vh")) != -1)
  {
    /**
     * Loop over the input options
     */
    switch (c)
    {
    /* Direcotry where the unit tests are saved */
    case 'd':
      {
        *const_cast<char**>(UTdir) = optarg;
        break;
      }
    /* Prefix ofhte kenrlen name */
    case 'p':
      {
        *const_cast<char**>(prefix) = optarg;
        break;
      }
    /* Index of hte testcase */
    case 'i':
      {
        *index = atoi(optarg);
        break;
      }
    /* Tolerance used to chekc (default 1.0E-12) */
    case 't':
      {
        char* tmp;
        *tolerance = strtod(optarg, &tmp);
        break;
      }
    /* verbose to log the input data */
    case 'v':
      {
        verbose=true;
        break;
      }
    case 'h':
    case '?':
    default:
      {
        printf( " + HELP\n" );
        printf( " Example:\n test -d <directory of the unit tests> -p <prefix to the name of the test> -i <test index> [-t <tolerance>]\n"  );
        printf( " Example:\n test -d \"../../data/\" -p \"LidDrivenCavity3D\" -n \"LDUSYSmatVecCSR_OffDiagStage\" -i 0\n"  );
        printf( " \n \n" );
      }
    };
  };
  /* Logging the input data */
  if ( verbose )
  {
    printf( " + Prefix    :: %s\n",  *prefix );
    printf( " + Directory :: %s\n",  *UTdir  );
    printf( " + Index     :: %d\n",  *index  );
    printf( " + Tolerance :: %lf\n", *tolerance  );
    exit(1);
  };
  /* Exit point */
  return;

};


/*
 * @brief temporary main for running single unit Tests in development phase.
 *        This define has to be added at the end of a unitTest_IO.H file.
 *        It expand to a temporary main. 
 */
#define __single_test_main__( TEST_NAME, TEST_TYPE )                           \
int main                                                                       \
(                                                                              \
  int    argc,                                                                 \
  char** argv                                                                  \
)                                                                              \
{                                                                              \
                                                                               \
  const char* UTdir;                                                           \
  const char* prefix;                                                          \
  int         index;                                                           \
  double      tolerance;                                                       \
  timeProfile profileTime;                                                     \
                                                                               \
  parseInputArguments                                                          \
  (                                                                            \
    argc,                                                                      \
    argv,                                                                      \
    &UTdir,                                                                    \
    &prefix,                                                                   \
    &index,                                                                    \
    &tolerance                                                                 \
  );                                                                           \
                                                                               \
                                                                               \
    TEST_NAME ut                                                               \
    (                                                                          \
      UTdir,                                                                   \
      prefix,                                                                  \
      index,                                                                   \
      tolerance                                                                \
    );                                                                         \
                                                                               \
    ut.loadInputData();                                                        \
                                                                               \
    ut.loadOutputData();                                                       \
                                                                               \
    ut.allocateDevice();                                                       \
                                                                               \
    ut.copyToDevice();                                                         \
                                                                               \
    ut.run( profileTime );                                                     \
                                                                               \
    ut.copyFromDevice();                                                       \
                                                                               \
    bool check = ut.check( );                                                  \
                                                                               \
                                                                               \
    if ( check )                                                               \
    {                                                                          \
      profileTime.print();                                                     \
    }                                                                          \
    else                                                                       \
    {                                                                          \
      printf( "ERROR :: check not pased for (%s):: %s, %s, %d\n",              \
              #TEST_TYPE, __FUNCTION__, __FILE__, __LINE__ );                  \
      abort();                                                                 \
    };                                                                         \
                                                                               \
    return(0);                                                                 \
                                                                               \
};


/*
 * @brief specializations for unit Tests in development phase.
 *
 */
#ifdef __TEST_CPU__
#define __unitTest__( NAME ) \
__single_test_main__( NAME, CPU );
#endif
#ifdef __TEST_NATIVE_CUDA__
#define __unitTest__( NAME ) \
__single_test_main__( NAME, NATIVE_CUDA );
#endif
#ifdef __TEST_OPEN_ACC__
#define __unitTest__( NAME ) \
__single_test_main__( NAME, OPEN_ACC );
#endif
#ifdef __TEST_OPEN_MP__
#define __unitTest__( NAME ) \
__single_test_main__( NAME, OPEN_MP );
#endif


#endif
