/*
 * @file implicitDivTestFactory.H
 *
 * @brief This is the factory of unit tests. According to the command line 
 *         parameters this class allocate different unit tests in order to 
 *         test different kernels.
 *
 *  @date   18/Giu/2022
 *  @author Mirco Valentini
 */


#ifndef __IMPLICIT_DIV_TEST_FACTORY_H__
#define __IMPLICIT_DIV_TEST_FACTORY_H__

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
#include "LDU-orderedCOO-implicitDiv_IO.H"
#include "MSR-implicitDiv_IO.H"
#include "LDU-CSR-implicitDiv_IO.H"


/**
 * Unit tests for matrix vector product
 */
class implicitDivTestFactory
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
    unitTestBase* m_implicitDivTest;
    bool          m_allocated;

    /* 
     * Show the help for the unit tests
     */
    void
    showHelp( )
    {
      printf( "Create a unit test object of the selected kind. Kind can be:\n" );
      printf( " 0) \"LDU-orderedCOO\"\n\n" );
      printf( " 1) \"LDU-CSR\"\n\n" );
      printf( " 6) \"MSR\"\n\n" );
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
      if ( strcmp( m_test, "LDU-orderedCOO"  ) == 0 ) { m_kid=0;  }; /* 0  */
      if ( strcmp( m_test, "LDU_CSR"  ) == 0 )        { m_kid=1;  }; /* 1  */
      if ( strcmp( m_test, "MSR"      ) == 0 )        { m_kid=2;  }; /* 4  */
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
    implicitDivTestFactory
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
     ,m_implicitDivTest(nullptr)
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
          m_implicitDivTest = 
          new LDUorderedCOOimplicitDiv_IO
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
          m_implicitDivTest = 
          new LDU_CSR_implicitDiv_IO
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
          m_implicitDivTest = 
          new MSR_ImplicitDiv_IO
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
      return(m_implicitDivTest);
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

        delete m_implicitDivTest;
      };
      // Exit point
      return;
    };

};


#endif
