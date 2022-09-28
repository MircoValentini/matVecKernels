/*
 * @file CSC_CSR_IO.H
 *
 * @brief Definition of the routines and data to be used to test the 
 * LDUSYSCOOmatVecCompOffDiagStageOneOutput kernel
 *
 *  @date   18/Giu/2022
 *  @author Mirco Valentini
 */


#ifndef __CSC_CSR_IO_H__
#define __CSC_CSR_IO_H__


/**
 * Local include
 */
#include "unitTestMatVec_IO.H"


/**
 * Include the kernels
 */
#if !defined( __DRY_RUN__ )
#include "CSC_CSR_kernels.H"
#endif


class CSC_CSR_IO:
  public unitTestMatVec
{
  private:

    idx_k* m_rowStartInp_CPU;
    idx_k* m_rowStartOut_CPU;


void 
ComputeRowStart
(
    const idx_k                        nCells,
    const idx_k                        nInternalFaces,
    const idx_k* const __MY_RESTRICT__ owner,
          idx_k* const __MY_RESTRICT__ rowStart
)
{
  // Local variables
  idx_k iFace;
  idx_k nOwnStart;
  idx_k curOwn;
  idx_k idx;
  // Compute row start
  nOwnStart   = 0;
  rowStart[0] = 0;
  idx         = 1;
  for ( iFace=0; iFace<nInternalFaces; iFace++ )
  {
    curOwn = owner[iFace];
    if ( curOwn > nOwnStart )
    {
      while ( idx <= curOwn )
      {
        rowStart[idx] = iFace;
        idx++;
      };
      nOwnStart = curOwn;
    };
  };
  // Fill the remaining part of the ros start vector
  for (iFace=idx;iFace<nCells+1;iFace++)
  {
    rowStart[iFace]=nInternalFaces;
  };
  // Exit point
  return;
};

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
    CSC_CSR_IO
    (
      const char* outputFile,
      const char* UTdir,
      const char* prefix,
      idx_k       index,
      double      tolerance
    ):
      unitTestMatVec(outputFile,UTdir,prefix,index,tolerance)
     ,m_rowStartInp_CPU(nullptr)
     ,m_rowStartOut_CPU(nullptr)
    {};


    /**
     * @brief Destructor for the class
     */
    ~CSC_CSR_IO()
    {
      Free();
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
    CSC_CSR_IO
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
    CSC_CSR_IO
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
    CSC_CSR_IO
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
    CSC_CSR_IO():
      unitTestMatVec
      (
        "LidDrivenCavity3D",
        0
      )
    {};


    const char* kernelFlavour() const { return( "CSC_CSR" ); };

    /**
     * @brief load input data for the kernel
     */
    void 
    loadInputData( )
    {
      unitTestMatVec::loadInputData();
      // Allocate memory
      m_rowStartInp_CPU = reinterpret_cast<idx_k*> ( malloc( (m_nCellsInp_CPU+1)*sizeof(idx_k) ) );
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
      m_rowStartOut_CPU = reinterpret_cast<idx_k*> ( malloc( (m_nCellsInp_CPU+1)*sizeof(idx_k) ) );
      /* Exit point */
      return;
    };


    /**
     * @brief Prepare data for the specific kernel
     */
    void
    Prepare()
    {
      /* Compute the rowstart vector needd for CSR */
      ComputeRowStart( m_nCellsInp_CPU, m_nInternalFacesInp_CPU, m_ownerInp_CPU, m_rowStartInp_CPU );
      ComputeRowStart( m_nCellsInp_CPU, m_nInternalFacesInp_CPU, m_ownerInp_CPU, m_rowStartOut_CPU );
      /* Exit point */
      return;
    };


    /**
     * @brief allocate data on the device
     */
    void
    allocateDevice()
    {
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
      if ( !m_allocatedDevice )
      {
        // Allocate memory
        cudaMalloc( (void**)&m_rowStart_Device, (m_nCellsInp_CPU+1)*sizeof(idx_k) );
        // Synchronize the device
        cudaDeviceSynchronize();
      }
      else
      {
        printf( "ERROR :: data already allocated:: %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__ );
        abort();
      };
#endif
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
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
      if ( m_allocatedInput && m_allocatedDevice )
      {
        // Allocate memory
        cudaMemcpy( m_rowStart_Device, m_rowStartInp_CPU, (m_nCellsInp_CPU+1)*sizeof(idx_k), cudaMemcpyHostToDevice );
        // Synchronize the device
        cudaDeviceSynchronize();
      }
      else
      {
        printf( "ERROR :: data are not allocated:: %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__ );
        abort();
      };
#endif
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
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
      if ( m_allocatedInput && m_allocatedDevice )
      {
        // Allocate memory
        cudaMemcpy( m_rowStartInp_CPU, m_rowStart_Device, (m_nCellsInp_CPU+1)*sizeof(idx_k), cudaMemcpyDeviceToHost );
        // Synchronize the device
        cudaDeviceSynchronize();
      }
      else
      {
        printf( "ERROR :: data are not allocated:: %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__ );
        abort();
      };
#endif
      /* Exit point */
      return;
    };


    /**
     * @brief Run the kernels 
     *
     * @param[in,out] profileTime time spent in the computation
     */
    void
    warmup
    (
      timeProfile& profileTime
    )
    {
      /* Start the profiling */
      m_profiler.tic();
      /* Call the kernel */
#if defined( __TEST_CPU__ ) && !defined( __DRY_RUN__ )
    matVec_CSC_CSR_CPU
    (
      m_nCellsInp_CPU,
      m_nFacesInp_CPU,
      m_nInternalFacesInp_CPU,
      m_cellsStrideInp_CPU,
      m_facesStrideInp_CPU,
      m_iCompInp_CPU,
      m_ownerInp_CPU,
      m_neighInp_CPU,
      m_rowStartInp_CPU,
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
      matVec_CSC_CSR_nativeCUDA<<<nBlocks,nThreadsPerBlock>>>
      (
        m_nCellsInp_CPU,
        m_nFacesInp_CPU,
        m_nInternalFacesInp_CPU,
        m_cellsStrideInp_CPU,
        m_facesStrideInp_CPU,
        m_iCompInp_CPU,
        m_owner_Device,
        m_neigh_Device,
        m_rowStart_Device,
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
      matVec_CSC_CSR_openACC
      (
        m_nCellsInp_CPU,
        m_nFacesInp_CPU,
        m_nInternalFacesInp_CPU,
        m_cellsStrideInp_CPU,
        m_facesStrideInp_CPU,
        m_iCompInp_CPU,
        m_owner_Device,
        m_neigh_Device,
        m_rowStart_Device,
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
      matVec_CSC_CSR_openMP
      (
        m_nCellsInp_CPU,
        m_nFacesInp_CPU,
        m_nInternalFacesInp_CPU,
        m_cellsStrideInp_CPU,
        m_facesStrideInp_CPU,
        m_iCompInp_CPU,
        m_owner_Device,
        m_neigh_Device,
        m_rowStart_Device,
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
     * @brief Run the kernels 
     *
     * @param[in,out] profileTime time spent in the computation
     */
    void
    race
    (
      timeProfile& profileTime,
      idx_k        numberOfRepetitions
    )
    {
      /* Start the profiling */
      m_profiler.tic();
      /* Call the kernel */
#if defined( __TEST_CPU__ ) && !defined( __DRY_RUN__ )
      for ( idx_k i=0; i<numberOfRepetitions; ++i )
      {
        matVec_CSC_CSR_CPU
        (
          m_nCellsInp_CPU,
          m_nFacesInp_CPU,
          m_nInternalFacesInp_CPU,
          m_cellsStrideInp_CPU,
          m_facesStrideInp_CPU,
          m_iCompInp_CPU,
          m_ownerInp_CPU,
          m_neighInp_CPU,
          m_rowStartInp_CPU,
          m_upperInp_CPU,
          m_lowerInp_CPU,
          m_psiInp_CPU,
          m_ApsiInp_CPU
        );
      }
#endif

/* Call the CUDA kernel */
#if defined( __TEST_NATIVE_CUDA__ ) && !defined( __DRY_RUN__ )
      int nThreadsPerBlock = 256;
      int nBlocks = (m_nInternalFacesInp_CPU+255)/256;
      matVec_CSC_CSR_nativeCUDA<<<nBlocks,nThreadsPerBlock>>>
      (
        m_nCellsInp_CPU,
        m_nFacesInp_CPU,
        m_nInternalFacesInp_CPU,
        m_cellsStrideInp_CPU,
        m_facesStrideInp_CPU,
        m_iCompInp_CPU,
        m_owner_Device,
        m_neigh_Device,
        m_rowStart_Device,
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
      matVec_CSC_CSR_openACC
      (
        m_nCellsInp_CPU,
        m_nFacesInp_CPU,
        m_nInternalFacesInp_CPU,
        m_cellsStrideInp_CPU,
        m_facesStrideInp_CPU,
        m_iCompInp_CPU,
        m_owner_Device,
        m_neigh_Device,
        m_rowStart_Device,
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
      matVec_CSC_CSR_openMP
      (
        m_nCellsInp_CPU,
        m_nFacesInp_CPU,
        m_nInternalFacesInp_CPU,
        m_cellsStrideInp_CPU,
        m_facesStrideInp_CPU,
        m_iCompInp_CPU,
        m_owner_Device,
        m_neigh_Device,
        m_rowStart_Device,
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
      if ( m_allocatedInput )
      {
        free(m_rowStartInp_CPU);
      };
      if ( m_allocatedOutput )
      {
        free(m_rowStartOut_CPU);
      };
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
      if ( m_allocatedDevice )
      {
        cudaFree(m_rowStart_Device);
      };
#endif
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
__unitTest__( CSC_CSR_IO );
#endif
