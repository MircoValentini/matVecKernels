/*
 * @file unitTestFactory.H
 *
 * @brief This is the factory of unit tests. According to the command line 
 *         parameters this class allocate different unit tests in order to 
 *         test different kernels.
 *
 *  @date   18/Giu/2022
 *  @author Mirco Valentini
 */


#ifndef __UNIT_TEST_FACTORY_H__
#define __UNIT_TEST_FACTORY_H__

/*
 * System include
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * Include the unit tests for each kernel
 */
#include "unitTestBase.H"

/**
 * Include the unit tests for each kernel
 */
#include "unitTests.H"

class unitTestFactory
{
  private:

    /* Data used fo constructing the test case file name */
    const char*   m_UTdir;
    const char*   m_prefix;
    const char*   m_name;
    const char*   m_testKind;
    const char*   m_outputFile;
    idx_k         m_index;
    double        m_tolerance;
    unsigned char m_kid;
    /* Pointer to the test case*/
    unitTestBase* m_test;
    bool          m_allocated;

    /* 
     * Show the help for the unit tests
     */
    void
    showHelp( )
    {
      printf( " Example:\n test -n <kernel name> -d <directory of the unit tests> -p <prefix to the name of the test>  -i <test index> -k <test kind> [-t <tolerance>]\n"  );
      printf( " Example:\n test -n LDUSYSCOOmatVecCompOffDiagStageOneOutput -d \"../../\" -p \"SHMmats-RCM_5krows_processor71\" -i 0 -k \"COO_COO\"\n"  );
      printf( "\n - Kernels name can be:\n\n" );
      printf( " +   0) matVec\n"                          ); /* 0  */
      // Exit point
      return;
    };

    /*
     * Convert the kernel name to kernel ID (used for the allocation switch)
     */
    bool
    kernelName2kernelId(  )
    {
      m_kid = 255;
      if ( strcmp( m_name, "matVec" ) == 0 ) { m_kid=0;  }; /* 0  */
      bool check;
      if ( m_kid == 255 )
      {
        check = false;
      }
      else
      {
        check = true;
      }
      return( check );
    };

  public:

    /**
     * Constructor of the unit test Abstract class, starting from the 
     * standard data
     */
    unitTestFactory
    (
      int    argc,
      char** argv
    ):
      m_UTdir("../matVecData")
     ,m_prefix("LDUSYSCOOmatVecCompOffDiagStageOneOutput")
     ,m_name("matVec")
     ,m_index(0)
     ,m_tolerance(0.0000000001)
     ,m_outputFile("test01.db")	
     ,m_kid(0)
     ,m_testKind("")
     ,m_test(nullptr)
     ,m_allocated(false)
    {

      /**
       * Local variables
       */
      opterr = 0;
      int c;
      bool verbose=false;

      /**
       * Parse command line arguments
       */
      while ( (c = getopt (argc, argv, "n:d:p:i:t:k:o:vh")) != -1)
      {
        /**
         * Loop over the input options
         */
        switch (c)
        {
        /**
         * Name of the kernel
         */
        case 'n':
          {
            *const_cast<char**>(&m_name) = optarg;
            if ( !kernelName2kernelId( ) )
            {
              printf( "ERROR :: wrong kernel name :: %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__ );
              abort();
            };
            break;
          }
	case 'o':
	  {
            *const_cast<char**>(&m_outputFile) = optarg;
            break;
	  }
        /* Direcotry where the unit tests are saved */
        case 'd':
          {
            *const_cast<char**>(&m_UTdir) = optarg;
            break;
          }
        /* kind of the unit test */
        case 'k':
          {
            *const_cast<char**>(&m_testKind) = optarg;
            break;
          }
        /* Prefix of the kernel name */
        case 'p':
          {
            *const_cast<char**>(&m_prefix) = optarg;
            break;
          }
        /* Index of hte testcase */
        case 'i':
          {
            m_index = atoi(optarg);
            break;
          }
        /* Tolerance used to chekc (default 1.0E-12) */
        case 't':
          {
            char* tmp;
            m_tolerance = strtod(optarg, &tmp);
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
            showHelp();
            abort();
          }
        };
      };
      /** Logging the input data */
      if ( verbose )
      {
        printf( " + Prefix    :: %s\n", m_prefix );
        printf( " + Name      :: %s\n", m_name   );
        printf( " + Directory :: %s\n", m_UTdir  );
        printf( " + Index     :: %d\n", m_index  );
      };
      // Exit point
      return;
    };


    /**
     * Allocate the unit test
     */
    unitTestBase*
    allocate()
    {
      switch ( m_kid )
      {
        case (0):
        {
          matVecTestFactory tmp
          (
	    m_outputFile,
            m_UTdir,
            m_prefix,
            m_testKind,
            m_index,
            m_tolerance
          );
          m_test = tmp.allocate();
          m_allocated = true;
          break;
        }
        default:
        {
          printf( "ERROR :: unit test (%s) still not implemented:: %s, %s, %d\n", m_name, __FUNCTION__, __FILE__, __LINE__ );
          printf( "\t + directory :: %s\n", m_UTdir );
          printf( "\t + prefix    :: %s\n", m_prefix );
          printf( "\t + name      :: %s\n", m_name );
          printf( "\t + index     :: %d\n", m_index );
          printf( "\t + kernel ID :: %u\n", m_kid );
          abort();
        }
      };
      return(m_test);
    };


    /**
     * Free the unit test
     */
    void
    free()
    {
      if ( m_allocated )
      {
        // To call the c function and not the method add the "::" in front

        ::free( m_test );
      };
      // Exit point
      return;
    };

};


#endif
