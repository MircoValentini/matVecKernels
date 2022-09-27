/*
 * @file COO_COO_IO.H
 *
 * @brief Definition of the routines and data to be used to test the 
 * LDUSYSCOOmatVecCompOffDiagStageOneOutput kernel
 *
 *  @date   18/Giu/2022
 *  @author Mirco Valentini
 */


#ifndef __COO_COO_IO_H__
#define __COO_COO_IO_H__


/**
 * Local include
 */
#include "unitTestMatVec_IO.H"


/**
 * Include the kernels
 */
#if !defined( __DRY_RUN__ )
#include "COO_COO_kernels.H"
#endif


class COO_COO_IO:
  public unitTestMatVec
{
  private:


  public:

    /**
     * @brief Constructor for the IO object given all the parameters
     *
     * The name of the unit test (the searched file ) will be generated as follows:
     *
     *  UTname = UTdir/${name}/${prefix}_${name}_${%6.6d-index}.h5
     *
     * @param[in] UTdir  direcotry where the unit tests are stored
     * @param[in] prefix prefix to be applied to the neame of the unit tests
     * @param[in] index  index of hte unit thest
     */
    COO_COO_IO
    (
      const char* outputFile,
      const char* UTdir,
      const char* prefix,
      idx_k       index,
      double      tolerance
    ):
      unitTestMatVec(outputFile,UTdir,prefix,index,tolerance)
    {};


    /**
     * @brief Destructor for the class
     */
    ~COO_COO_IO()
    {
      // unitTestMatVec::~unitTestMatVec();
      /* Exit point */
      return;
    };


    /**
     * @brief Constructor for the IO object.
     *        Unit test dir assumed equal to "../../data"
     *
     * The name of the unit test (the searched file ) will be generated as follows:
     *
     *  UTname = UTdir/${name}/${prefix}_${name}_${%6.6d-index}.h5
     *
     * @param[in] prefix prefix to be applied to the neame of the unit tests
     * @param[in] index  index of hte unit thest
     */
    COO_COO_IO
    (
      const char*  prefix,
      idx_k  index
    ):
      unitTestMatVec
      (
        "test01.db",
        "../../data/",
        prefix,
        index,
        1.0E-12
      )
    {};


    /**
     * @brief Constructor for the IO object.
     *        Index assumed equal to "0"
     *
     * The name of the unit test (the searched file ) will be generated as follows:
     *
     *  UTname = UTdir/${name}/${prefix}_${name}_${%6.6d-index}.h5
     *
     * @param[in] prefix prefix to be applied to the neame of the unit tests
     * @param[in] index  index of hte unit thest
     */
    COO_COO_IO
    (
      const char* prefix
    ):
      unitTestMatVec
      (
        prefix,
        0
      )
    {};


    /**
     * @brief Constructor for the IO object.
     *        Prefix assumed equal to "LidDrivenCavity3D"
     *
     * The name of the unit test (the searched file ) will be generated as follows:
     *
     *  UTname = UTdir/${name}/${prefix}_${name}_${%6.6d-index}.h5
     *
     * @param[in] prefix prefix to be applied to the neame of the unit tests
     * @param[in] index  index of hte unit thest
     */
    COO_COO_IO
    (
      idx_k  index
    ):
      unitTestMatVec
      (
        "LidDrivenCavity3D",
        index
      )
    {};

    /**
     * @brief Empty constructor for the IO object
     * 
     */
    COO_COO_IO():
      unitTestMatVec
      (
        "LidDrivenCavity3D",
        0
      )
    {};

    const char* kernelFlavour() const { return( "COO_COO" ); };

    /**
     * @brief load input data for the kernel
     */
    void 
    loadInputData( )
    {
      unitTestMatVec::loadInputData();
      /* Exit point */
      return;
    };


    /**
     * @brief load output data for the kernel
     */
    void 
    loadOutputData( )
    {
      unitTestMatVec::loadOutputData();
      /* Exit point */
      return;
    };


    void
    Prepare()
    {
      /* Exit point */
      return;
    };


    /**
     * @brief allocate data on the device
     */
    void
    allocateDevice()
    {
      unitTestMatVec::allocateDevice();
      /* Exit point */
      return;
    };


    /**
     * @brief copy data to the device
     */
    void
    copyToDevice()
    {
      unitTestMatVec::copyToDevice();
      /* Exit point */
      return;
    };


    /**
     * @brief copy data from the device
     */
    void
    copyFromDevice()
    {
      unitTestMatVec::copyFromDevice();
      /* Exit point */
      return;
    };


    /**
     * @brief Run the kernels 
     *
     * @param[in,out] profileTime time spent in the computation
     */
    void
    run
    (
      timeProfile& profileTime
    )
    {
      /* Start the profiling */
      m_profiler.tic();
      /* Call the kernel */
#if defined( __TEST_CPU__ ) && !defined( __DRY_RUN__ )
    matVec_COO_COO_CPU
    (
      m_nCellsInp_CPU,
      m_nFacesInp_CPU,
      m_nInternalFacesInp_CPU,
      m_cellsStrideInp_CPU,
      m_facesStrideInp_CPU,
      m_iCompInp_CPU,
      m_ownerInp_CPU,
      m_neighInp_CPU,
      m_upperInp_CPU,
      m_lowerInp_CPU,
      m_psiInp_CPU,
      m_ApsiInp_CPU
    );
#endif

/* Call the CUDA kernel */
#if defined( __TEST_NATIVE_CUDA__ ) && !defined( __DRY_RUN__ )
      int nThreadsPerBlock = 256;
      int nBlocks = (m_nInternalFacesInp_CPU+255)/256;
      matVec_COO_COO_nativeCUDA<<<nBlocks,nThreadsPerBlock>>>
      (
        m_nCellsInp_CPU,
        m_nFacesInp_CPU,
        m_nInternalFacesInp_CPU,
        m_cellsStrideInp_CPU,
        m_facesStrideInp_CPU,
        m_iCompInp_CPU,
        m_owner_Device,
        m_neigh_Device,
        m_upper_Device,
        m_lower_Device,
        m_psi_Device,
        m_Apsi_Device
      );
      // Synchronize the device
      cudaDeviceSynchronize();
#endif

/* Call the OPEN_ACC kernel */
#if defined( __TEST_OPEN_ACC__ ) && !defined( __DRY_RUN__ )
      matVec_COO_COO_openACC
      (
        m_nCellsInp_CPU,
        m_nFacesInp_CPU,
        m_nInternalFacesInp_CPU,
        m_cellsStrideInp_CPU,
        m_facesStrideInp_CPU,
        m_iCompInp_CPU,
        m_owner_Device,
        m_neigh_Device,
        m_upper_Device,
        m_lower_Device,
        m_psi_Device,
        m_Apsi_Device
      );
      // Synchronize the device
      cudaDeviceSynchronize();
#endif

    /* Call the OPEN_MP kernel */
#if  defined( __TEST_OPEN_MP__ ) && !defined( __DRY_RUN__ )
      matVec_COO_COO_openMP
      (
        m_nCellsInp_CPU,
        m_nFacesInp_CPU,
        m_nInternalFacesInp_CPU,
        m_cellsStrideInp_CPU,
        m_facesStrideInp_CPU,
        m_iCompInp_CPU,
        m_owner_Device,
        m_neigh_Device,
        m_upper_Device,
        m_lower_Device,
        m_psi_Device,
        m_Apsi_Device
      );
      // Synchronize the device
      cudaDeviceSynchronize();
#endif
      /* Record the profiling time */
      m_profiler.toc( profileTime );
      /* Exit point */
      return;
    };


    /**
     * @brief free memory
     */
    void 
    Free()
    {
      unitTestMatVec::Free();
      /* Exit point */
      return;
    };

};

#endif





/**
 * In development phase, it is necessary to build a single unit test in order
 * to be more flexible you can enable the following define and focus on only 
 * one source file.
 */ 
#ifdef __SINGLE_MAIN_DEVEL__
#include "singleTestDevel.H"
__unitTest__( COO_COO_IO );
#endif
