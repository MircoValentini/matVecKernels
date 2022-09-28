/*
 * @file matVecTestFactory.H
 *
 * @brief This is the factory of unit tests. According to the command line 
 *         parameters this class allocate different unit tests in order to 
 *         test different kernels.
 *
 *  @date   18/Giu/2022
 *  @author Mirco Valentini
 */


#ifndef __MATVEC_TEST_FACTORY_H__
#define __MATVEC_TEST_FACTORY_H__

/*
 * System include
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


/**
 * Include the different unit tests
 */
#include "COO_COO_IO.H"
#include "CSC_CSR_IO.H"
#include "BlockRowPerm_IO.H"
#include "BlockRowPermCC_IO.H"
#include "BlockRowPerm_sELL_IO.H"
#include "CSR_CSR_IO.H"
#include "CSR_IO.H"




/**
 * Unit tests for matrix vector product
 */
class matVecTestFactory
{
  private:

    /* Data used fo constructing the test case file name */
    const char*   m_UTdir;
    const char*   m_prefix;
    const char*   m_test;
    const char*   m_outputFile;
    idx_k         m_index;
    double        m_tolerance;
    idx_k         m_kid;
    /* Pointer to the test case*/
    unitTestBase* m_matVecTest;
    bool          m_allocated;

    /* 
     * Show the help for the unit tests
     */
    void
    showHelp( )
    {
      printf( "Create a unit test object of the selected kind. Kind can be:\n" );
      printf( " 0) \"COO_COO\"\n\n" );
      printf( " 1) \"CSC_CSR\"\n\n" );
      printf( " 2) \"BlockRowPerm\"\n\n" );
      printf( " 3) \"BlockRowPermCC\"\n\n" );
      printf( " 4) \"BlockRowPerm_sELL\"\n\n" );
      printf( " 5) \"CSR_CSR\"\n\n" );
      printf( " 6) \"CSR\"\n\n" );
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
      if ( strcmp( m_test, "COO_COO"  ) == 0 )              { m_kid=0;  }; /* 0  */
      if ( strcmp( m_test, "CSC_CSR"  ) == 0 )              { m_kid=1;  }; /* 1  */
      if ( strncmp( m_test, "BlockRowPerm", 12  ) == 0 )    { m_kid=2;  }; /* 2  */
      if ( strncmp( m_test, "BlockRowPermCC", 14  ) == 0 )  { m_kid=3;  }; /* 2  */
      if ( strncmp( m_test, "BlockRowPermELL", 15  ) == 0 ) { m_kid=4;  }; /* 2  */
      if ( strcmp( m_test, "CSR_CSR"  ) == 0 )              { m_kid=5;  }; /* 3  */
      if ( strcmp( m_test, "CSR"      ) == 0 )              { m_kid=6;  }; /* 4  */
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
    matVecTestFactory
    (
      const char* outputFile,
      const char* UTdir,
      const char* prefix,
      const char* test,
      idx_k       index,
      double      tolerance
    ):
      m_outputFile(outputFile)	    
     ,m_UTdir(UTdir)
     ,m_prefix(prefix)
     ,m_test(test)
     ,m_index(index)
     ,m_tolerance(tolerance)
     ,m_kid(0)
     ,m_matVecTest(nullptr)
    {
      if ( !kernelName2kernelId() )
      {
        printf( "ERROR:: wrong test type: %s\n", m_test );
        abort();
      }
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
          m_matVecTest = 
          new COO_COO_IO
          (
            m_outputFile,
            m_UTdir,
            m_prefix,
            m_index,
            m_tolerance
          );
          m_allocated = true;
          break;
        }
        case (1):
        {
          m_matVecTest = 
          new CSC_CSR_IO
          (
            m_outputFile,
            m_UTdir,
            m_prefix,
            m_index,
            m_tolerance
          );
          m_allocated = true;
          break;
        }
        case (2):
        {
          idx_k nBlks = 10;
          int l = strlen(m_test);
          if ( l > 12 )
          {
            const char* tmp = &m_test[13];
            nBlks = atoi( tmp );
          };
          m_matVecTest = 
          new BlockRowPerm_IO
          (
            m_outputFile,
            m_UTdir,
            m_prefix,
            m_index,
            m_tolerance,
            nBlks
          );
          m_allocated = true;
          break;
        }
        case (3):
        {
          idx_k nBlks = 10;
          int l = strlen(m_test);
          if ( l > 14 )
          {
            const char* tmp = &m_test[15];
            nBlks = atoi( tmp );
          };
          m_matVecTest = 
          new BlockRowPermCC_IO
          (
            m_outputFile,
            m_UTdir,
            m_prefix,
            m_index,
            m_tolerance,
            nBlks
          );
          m_allocated = true;
          break;
        }
        case (4):
        {
          m_matVecTest = 
          new BlockRowPerm_sELL_IO
          (
            m_outputFile,
            m_UTdir,
            m_prefix,
            m_index,
            m_tolerance,
            m_test
          );
          m_allocated = true;
          break;
        }
        case (5):
        {
          m_matVecTest = 
          new CSR_CSR_IO
          (
            m_outputFile,
            m_UTdir,
            m_prefix,
            m_index,
            m_tolerance
          );
          m_allocated = true;
          break;
        }
        case (6):
        {
          m_matVecTest =
          new CSR_IO
          (
            m_outputFile,
            m_UTdir,
            m_prefix,
            m_index,
            m_tolerance
          );
          m_allocated = true;
          break;
        }
        default:
        {
          printf( "ERROR :: unit test (%s) still not implemented:: %s, %s, %d\n", m_test, __FUNCTION__, __FILE__, __LINE__ );
          printf( "\t + directory :: %s\n", m_UTdir );
          printf( "\t + prefix    :: %s\n", m_prefix );
          printf( "\t + test      :: %s\n", m_test );
          printf( "\t + index     :: %d\n", m_index );
          printf( "\t + kernel ID :: %u\n", m_kid );
          abort();
        }
      };
      return(m_matVecTest);
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

        delete m_matVecTest;
      };
      // Exit point
      return;
    };

};


#endif
