/*
 * @file unitTestBase.H
 *
 * @brief Definition of the base class for handling unit tests
 *
 *  @date   18/Giu/2022
 *  @author Mirco Valentini
 */


#ifndef __UNIT_TEST_BASE_H__
#define __UNIT_TEST_BASE_H__


/**
 * Local include
 */
#include "dataTypes.H"
#include "timing.H"

/**
 * Class used as a base class for all the unit tests
 */
class unitTestBase
{
  protected:

    /**
     * Data used fo constructing the test case file name. 
     * The final name of the unit test data will be created as:
     *
     * >>> <m_UTdir>/<m_prefix>_<name>_<%6.6d - index>.h5
     *
     */

    const char* m_outputFile;

    /**
     * Name of the directory in which the data are saved
     */
    const char* m_UTdir;

    /**
     * Name of the prefix to be prepended tothe name of the unit tests
     * i.e. LidDrivenCavity3D, MotorbikeCavity, ...
     */
    const char* m_prefix;

    /**
     * Name of hte kernel to be tested
     */
    const char* m_name;

    /**
     * Index of hte test to be tested
     */
    idx_k m_index;

    /**
     * Tolerance to be used to check the result
     */
    double m_tolerance;

  public:


    /**
     * Constructor of the unit test Abstract class, starting from the 
     * standard data
     */
    unitTestBase
    (
      const char* outputFile,
      const char* UTdir,
      const char* prefix,
      const char* name,
      idx_k       index
    ):
      m_outputFile(outputFile)	    
     ,m_UTdir(UTdir)
     ,m_prefix(prefix)
     ,m_name(name)
     ,m_index(index)
    {};

    virtual const char* kernelFlavour() const = 0;
    const char* outputFile() const { return(m_outputFile); };
    const char* kernel() const { return( m_name  ); };
    const char* prefix() const { return( m_prefix ); };
    const char* name()   const { return( m_name   ); };
    idx_k       index()  const { return( m_index  ); };


    /**
     * Virtual destructor 
     */
    virtual ~unitTestBase()
    {
      m_index = 0;
      m_tolerance = 0.0;
      /* Exit point */
      return;
    };

    virtual fld_k bandWidth() const = 0;
    virtual fld_k jumpParam() const = 0;

    /*
     * @brief Load input data into the CPU memory
     */
    virtual void loadInputData( ) = 0;


    /*
     * @brief Load output data into the CPU memory
     */
    virtual void loadOutputData( ) = 0;


    /*
     * @brief Prepare the data for the kernel
     */
    virtual void Prepare( ) = 0;
    virtual void Dispose( ) = 0;


    /*
     * @brief Allocate memory on the device
     */
    virtual void allocateDevice() = 0;


    /*
     * @brief copy memory to the device
     */
    virtual void copyToDevice() = 0;


    /*
     * @brief copy memory from the device
     */
    virtual void copyFromDevice() = 0;


    /*
     * @brief run the testcase and measure the time
     */
    virtual void run( timeProfile& profileTime ) = 0;


    /*
     * @brief check the result
     */
    virtual bool check( ) = 0;


    /*
     * @brief Free al the memory of the unit test
     */
    virtual void Free() = 0;

};

#endif
