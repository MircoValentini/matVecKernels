/*
 * @file LDUSYSCSRmatVecCompOffDiagStageOneOutput_001_IO.H
 *
 * @brief Definition of the routines and data to be used to test the 
 * CSR kernel
 *
 *  @date   18/Giu/2022
 *  @author Mirco Valentini
 */


#ifndef __CSR_IO_H__
#define __CSR_IO_H__


/**
 * Local include
 */
#include "unitTestMatVec_IO.H"


/**
 * Include the kernels
 */
#if !defined( __DRY_RUN__ )
#include "CSR_kernels.H"
#endif


class CSR_IO:
  public unitTestMatVec
{
  private:

    /* tolerance used for the check */
    double m_tolerance;

    /* Tracking allocations */
    bool m_allocatedInput;
    bool m_allocatedOutput;
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
    bool m_allocatedDevice;
#endif
    /* Clock used for the profiler */
    simpleClock_t m_profiler;


    // CPU Arrays
    idx_k*        m_rowStartInp_CPU;
    idx_k*        m_rowIdxInp_CPU;
    idx_k*        m_colIdxInp_CPU;
    idx_k*        m_prowStartInp_CPU;
    LDUoffDiag_k* m_valuesInp_CPU;

    // CPU Arrays
    idx_k*        m_rowStartOut_CPU;


#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
    // GPU Arrays
    idx_k*        m_rowStart_Device;
    idx_k*        m_rowIdx_Device;
    idx_k*        m_colIdx_Device;
    idx_k*        m_prowStart_Device;
    LDUoffDiag_k* m_values_Device;
#endif

    /**
     * Structure used to sort an array
     */
    struct sortItem
    {
      unsigned int  m_NID;
      unsigned int* m_idx;
      unsigned int  m_kind; 
    };

    /**
     * Comparator used to sort data
     */
    static int
    comparator(const void *p, const void *q)
    {
      int check;
      unsigned int l = (((struct sortItem *)p)->m_NID);
      unsigned int r = (((struct sortItem *)q)->m_NID);
      if ( r > l ) check = -1;
      if ( l > r ) check = +1;
      if ( l == r ) check = 0;
      return( check );
    };

    /**
     * Compute the row start vector given the ordere row indices
     */
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

    /**
     * Compute the permutation needed to represent in CSR the 
     * lower part of the matrix
     */
    void computePermutation
    (
      idx_k         nCells,
      idx_k         nInternalFaces, 
      idx_k*        owner, 
      idx_k*        neigh, 
      LDUoffDiag_k* lower,
      LDUoffDiag_k* upper,
      idx_k*        rowIdx,
      idx_k*        colIdx,
      LDUoffDiag_k* values,
      idx_k*        rowStart
    )
    {

      // Allocate temporary meomry needed for sorting operations
      idx_k* temporaryArray01 = (idx_k*)malloc( 2*nInternalFaces*sizeof(idx_k) );
      idx_k* temporaryArray02 = (idx_k*)malloc( 2*nInternalFaces*sizeof(idx_k) );
      sortItem* sortItems     = (sortItem*)malloc( 2*nInternalFaces*sizeof(sortItem) );

      // Prepare the data structure to run the sorting algorithm
      for ( idx_k i=0; i<nInternalFaces; ++i )
      {
        temporaryArray01[i] = i;
        // sortItems[i].m_NID  = owner[i]*nCells + neigh[i];
        sortItems[i].m_NID  = owner[i];
        sortItems[i].m_idx  = &temporaryArray01[i];
        sortItems[i].m_kind = 0; // owner
      };
      for ( idx_k i=0; i<nInternalFaces; ++i )
      {
        temporaryArray02[i] = i;
        // sortItems[i+nInternalFaces].m_NID  = neigh[i]*nCells + owner[i];
        sortItems[i+nInternalFaces].m_NID  = neigh[i];
        sortItems[i+nInternalFaces].m_idx  = &temporaryArray02[i];
        sortItems[i+nInternalFaces].m_kind = 1; // neighbour
      };
      // Sort the array accordingly to the strategy implemented in the comparator
      qsort( (void*)sortItems, 2*nInternalFaces, sizeof(sortItem), &comparator );

      // Fill the permutation vector
      for ( idx_k i=0; i<2*nInternalFaces; ++i )
      {
        idx_k tmp  = *(sortItems[i].m_idx);
        idx_k kind = sortItems[i].m_kind;
        if ( kind == 0 )
        {
          rowIdx[i] = owner[tmp];
          colIdx[i] = neigh[tmp];
          values[i] = upper[tmp];
        }
        if ( kind == 1)
        {
          rowIdx[i] = neigh[tmp];
          colIdx[i] = owner[tmp];
          values[i] = lower[tmp];
        };
    };

      // Compute the permuted rowstart
      ComputeRowStart
      (
        nCells,
        2*nInternalFaces,
        rowIdx,
        rowStart
      );

      // Free memory
      free( temporaryArray01 );
      free( temporaryArray02 );
      free( sortItems );
    
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
    CSR_IO
    (
      const char* outputFile,
      const char* UTdir,
      const char* prefix,
      idx_k       index,
      double      tolerance
    ):
      unitTestMatVec(outputFile,UTdir,prefix,index,tolerance)
     ,m_tolerance(tolerance)
     ,m_allocatedInput(false)
     ,m_allocatedOutput(false)
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
     ,m_allocatedDevice(false)
#endif
     ,m_profiler()
     ,m_rowStartInp_CPU(nullptr)
     ,m_rowIdxInp_CPU(nullptr)
     ,m_colIdxInp_CPU(nullptr)
     ,m_prowStartInp_CPU(nullptr)
     ,m_valuesInp_CPU(nullptr)
     ,m_rowStartOut_CPU(nullptr)
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
     ,m_rowStart_Device(nullptr)
     ,m_rowIdx_Device(nullptr)
     ,m_colIdx_Device(nullptr)
     ,m_prowStart_Device(nullptr)
     ,m_values_Device(nullptr)
#endif
    {};


    /**
     * @brief Destructor for the class
     */
    ~CSR_IO()
    {
      /* Free the dynamic memory */
      Free();

      /* Reset all the private members*/
      m_tolerance = 0.0;
      m_allocatedInput = false;
      m_allocatedOutput = false;
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
      m_allocatedDevice = false;
#endif
      m_rowStartInp_CPU = nullptr;
      m_rowIdxInp_CPU = nullptr;
      m_colIdxInp_CPU = nullptr;
      m_prowStartInp_CPU = nullptr;
      m_valuesInp_CPU = nullptr;
      m_rowStartOut_CPU = nullptr;
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
      m_rowStart_Device = nullptr;
      m_rowIdx_Device = nullptr;
      m_colIdx_Device = nullptr;
      m_prowStart_Device = nullptr;
      m_values_Device = nullptr;
#endif
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
    CSR_IO
    (
      const char*  prefix,
      idx_k  index
    ):
      CSR_IO
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
    CSR_IO
    (
      const char* prefix
    ):
      CSR_IO
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
    CSR_IO
    (
      idx_k  index
    ):
      CSR_IO
      (
        "LidDrivenCavity3D",
        index
      )
    {};

    /**
     * @brief Empty constructor for the IO object
     * 
     */
    CSR_IO():
      CSR_IO
      (
        "LidDrivenCavity3D",
        0
      )
    {};



    const char* kernelFlavour() const { return( "CSR" ); };



    /**
     * @brief load input data for the kernel
     */
    void 
    loadInputData( )
    {
      unitTestMatVec::loadInputData();
      // Allocate memory
      m_rowStartInp_CPU  = reinterpret_cast<idx_k*>       ( malloc( (m_nCellsInp_CPU+1)*                sizeof(idx_k) )         );
      m_rowIdxInp_CPU    = reinterpret_cast<idx_k*>       ( malloc( 2*m_nInternalFacesInp_CPU*          sizeof(idx_k) )         );
      m_colIdxInp_CPU    = reinterpret_cast<idx_k*>       ( malloc( 2*m_nInternalFacesInp_CPU*          sizeof(idx_k) )         );
      m_prowStartInp_CPU = reinterpret_cast<idx_k*>       ( malloc( 2*(m_nCellsInp_CPU+1)*              sizeof(idx_k) )         );
      m_valuesInp_CPU    = reinterpret_cast<LDUoffDiag_k*>( malloc( 2*m_nInternalFacesInp_CPU*          sizeof(LDUoffDiag_k) )  );
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

    /**
     * @brief Prepare data for the specific kernel
     */
    void
    Prepare()
    {
      // Compute the permuted values
      computePermutation
      (
        m_nCellsInp_CPU,
        m_nInternalFacesInp_CPU, 
        m_ownerInp_CPU,
        m_neighInp_CPU,
        m_lowerInp_CPU,
        m_upperInp_CPU,
        m_rowIdxInp_CPU,
        m_colIdxInp_CPU,
        m_valuesInp_CPU,
        m_prowStartInp_CPU
      );
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
        cudaMalloc( (void**)&m_rowStart_Device,  (m_nCellsInp_CPU+1)*                sizeof(idx_k)        );
        cudaMalloc( (void**)&m_rowIdx_Device,    2*m_nInternalFacesInp_CPU*          sizeof(idx_k)        );
        cudaMalloc( (void**)&m_colIdx_Device,    2*m_nInternalFacesInp_CPU*          sizeof(idx_k)        );
        cudaMalloc( (void**)&m_prowStart_Device, 2*(m_nCellsInp_CPU+1)*              sizeof(idx_k)        );
        cudaMalloc( (void**)&m_values_Device,    2*m_nInternalFacesInp_CPU*          sizeof(LDUoffDiag_k) );
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
        cudaMemcpy( m_rowStart_Device,  m_rowStartInp_CPU,  (m_nCellsInp_CPU+1)*                sizeof(idx_k),        cudaMemcpyHostToDevice );
        cudaMemcpy( m_rowIdx_Device,    m_rowIdxInp_CPU,    2*m_nInternalFacesInp_CPU*          sizeof(idx_k),        cudaMemcpyHostToDevice );
        cudaMemcpy( m_colIdx_Device,    m_colIdxInp_CPU,    2*m_nInternalFacesInp_CPU*          sizeof(idx_k),        cudaMemcpyHostToDevice );
        cudaMemcpy( m_prowStart_Device, m_prowStartInp_CPU, 2*(m_nCellsInp_CPU+1)*              sizeof(idx_k),        cudaMemcpyHostToDevice );
        cudaMemcpy( m_values_Device,    m_valuesInp_CPU,    2*m_nInternalFacesInp_CPU*          sizeof(LDUoffDiag_k), cudaMemcpyHostToDevice );
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
        cudaMemcpy( m_rowStartInp_CPU,  m_rowStart_Device,  (m_nCellsInp_CPU+1)*                sizeof(idx_k),        cudaMemcpyDeviceToHost );
        cudaMemcpy( m_rowIdxInp_CPU,    m_rowIdx_Device,    2*m_nInternalFacesInp_CPU*          sizeof(idx_k),        cudaMemcpyDeviceToHost );
        cudaMemcpy( m_colIdxInp_CPU,    m_colIdx_Device,    2*m_nInternalFacesInp_CPU*          sizeof(idx_k),        cudaMemcpyDeviceToHost );
        cudaMemcpy( m_prowStartInp_CPU, m_prowStart_Device, 2*(m_nCellsInp_CPU+1)*              sizeof(idx_k),        cudaMemcpyDeviceToHost );
        cudaMemcpy( m_valuesInp_CPU,    m_values_Device,    2*m_nInternalFacesInp_CPU*          sizeof(LDUoffDiag_k), cudaMemcpyDeviceToHost );
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
    matVec_CSR_CPU
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
      m_rowIdxInp_CPU,
      m_colIdxInp_CPU,
      m_prowStartInp_CPU,
      m_upperInp_CPU,
      m_lowerInp_CPU,
      m_valuesInp_CPU,
      m_psiInp_CPU,
      m_ApsiInp_CPU
    );
#endif

/* Call the CUDA kernel */
#if defined( __TEST_NATIVE_CUDA__ ) && !defined( __DRY_RUN__ )
      matVec_CSR_nativeCUDA<<<(2*m_nInternalFacesInp_CPU*__N_BLOCKS__+255)/256, 256>>>
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
        m_rowIdx_Device,
        m_colIdx_Device,
        m_prowStart_Device,
        m_upper_Device,
        m_lower_Device,
        m_values_Device,
        m_psi_Device,
        m_Apsi_Device
      );
      // Synchronize the device
      cudaDeviceSynchronize();
#endif

/* Call the OPEN_ACC kernel */
#if defined( __TEST_OPEN_ACC__ ) && !defined( __DRY_RUN__ )
      matVec_CSR_openACC
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
        m_rowIdx_Device,
        m_colIdx_Device,
        m_prowStart_Device,
        m_upper_Device,
        m_lower_Device,
        m_values_Device,
        m_psi_Device,
        m_Apsi_Device      );
      // Synchronize the device
      cudaDeviceSynchronize();
#endif

    /* Call the OPEN_MP kernel */
#if  defined( __TEST_OPEN_MP__ ) && !defined( __DRY_RUN__ )
      matVec_CSR_openMP
      (
        m_nCellsInp_CPU,
        m_nFacesInp_CPU,
        m_nInternalFacesInp_CPU,
        m_cellsStrideInp_CPU,
        m_facesStrideInp_CPU,
        m_iComp_Device,
        m_owner_Device,
        m_neigh_Device,
        m_rowStart_Device,
        m_rowIdx_Device,
        m_colIdx_Device,
        m_prowStart_Device,
        m_upper_Device,
        m_lower_Device,
        m_values_Device,
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
        matVec_CSR_CPU
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
          m_rowIdxInp_CPU,
          m_colIdxInp_CPU,
          m_prowStartInp_CPU,
          m_upperInp_CPU,
          m_lowerInp_CPU,
          m_valuesInp_CPU,
          m_psiInp_CPU,
          m_ApsiInp_CPU
        );
      }
#endif

/* Call the CUDA kernel */
#if defined( __TEST_NATIVE_CUDA__ ) && !defined( __DRY_RUN__ )
      matVec_CSR_nativeCUDA<<<(2*m_nInternalFacesInp_CPU*__N_BLOCKS__+255)/256, 256>>>
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
        m_rowIdx_Device,
        m_colIdx_Device,
        m_prowStart_Device,
        m_upper_Device,
        m_lower_Device,
        m_values_Device,
        m_psi_Device,
        m_Apsi_Device
      );
      // Synchronize the device
      cudaDeviceSynchronize();
#endif

/* Call the OPEN_ACC kernel */
#if defined( __TEST_OPEN_ACC__ ) && !defined( __DRY_RUN__ )
      matVec_CSR_openACC
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
        m_rowIdx_Device,
        m_colIdx_Device,
        m_prowStart_Device,
        m_upper_Device,
        m_lower_Device,
        m_values_Device,
        m_psi_Device,
        m_Apsi_Device      );
      // Synchronize the device
      cudaDeviceSynchronize();
#endif

    /* Call the OPEN_MP kernel */
#if  defined( __TEST_OPEN_MP__ ) && !defined( __DRY_RUN__ )
      matVec_CSR_openMP
      (
        m_nCellsInp_CPU,
        m_nFacesInp_CPU,
        m_nInternalFacesInp_CPU,
        m_cellsStrideInp_CPU,
        m_facesStrideInp_CPU,
        m_iComp_Device,
        m_owner_Device,
        m_neigh_Device,
        m_rowStart_Device,
        m_rowIdx_Device,
        m_colIdx_Device,
        m_prowStart_Device,
        m_upper_Device,
        m_lower_Device,
        m_values_Device,
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
        free(m_ownerInp_CPU);
        free(m_neighInp_CPU);
        free(m_rowStartInp_CPU);
        free(m_rowIdxInp_CPU);
        free(m_colIdxInp_CPU);
        free(m_prowStartInp_CPU);
        free(m_lowerInp_CPU);
        free(m_valuesInp_CPU);
        free(m_upperInp_CPU);
        free(m_psiInp_CPU);
        free(m_ApsiInp_CPU);
        // Update allocation flag
        m_allocatedInput = false;
      };
      if ( m_allocatedOutput )
      {
        free(m_ownerOut_CPU);
        free(m_neighOut_CPU);
        free(m_rowStartOut_CPU);
        free(m_lowerOut_CPU);
        free(m_upperOut_CPU);
        free(m_psiOut_CPU);
        free(m_ApsiOut_CPU);
        // Update allocation flag
        m_allocatedOutput  =false;
      };
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
      if ( m_allocatedDevice )
      {
        cudaFree(m_ownerInp_CPU);
        cudaFree(m_neighInp_CPU);
        cudaFree(m_rowStartInp_CPU);
        cudaFree(m_rowIdxInp_CPU);
        cudaFree(m_colIdxInp_CPU);
        cudaFree(m_prowStartInp_CPU);
        cudaFree(m_lowerInp_CPU);
        cudaFree(m_valuesInp_CPU);
        cudaFree(m_upperInp_CPU);
        cudaFree(m_psiInp_CPU);
        cudaFree(m_ApsiInp_CPU);
        // Synchronize the device
        cudaDeviceSynchronize();
        // Update allocation flag
        m_allocatedDevice = false;
      };
#endif
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
__unitTest__( CSR_IO );
#endif
