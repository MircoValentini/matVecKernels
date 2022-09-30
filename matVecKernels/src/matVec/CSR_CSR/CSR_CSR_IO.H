/*
 * @file LDUSYSCSRmatVecCompOffDiagStageOneOutput_001_IO.H
 *
 * @brief Definition of the routines and data to be used to test the 
 * LDUSYSCSRmatVecCompOffDiagStageOneOutput_004 kernel
 *
 *  @date   18/Giu/2022
 *  @author Mirco Valentini
 */


#ifndef __CSR_CSR_IO_H__
#define __CSR_CSR_IO_H__

/**
 * Local include
 */
#include "unitTestMatVec_IO.H"

/**
 * Include the kernels
 */
#if !defined( __DRY_RUN__ )
#include "CSR_CSR_kernels.H"
#endif


class CSR_CSR_IO:
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
    idx_k*        m_rowStartOut_CPU;

    idx_k*        m_pownerInp_CPU;
    idx_k*        m_pneighInp_CPU;
    idx_k*        m_prowStartInp_CPU;
    LDUoffDiag_k* m_plowerInp_CPU;

#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
    // GPU Arrays
    // idx_k*        m_owner_Device;
    //idx_k*        m_neigh_Device;
    idx_k*        m_rowStart_Device;
    idx_k*        m_powner_Device;
    idx_k*        m_pneigh_Device;
    idx_k*        m_prowStart_Device;
    //fld_k*        m_psi_Device;
    //fld_k*        m_Apsi_Device;
    //LDUoffDiag_k* m_upper_Device;
    //LDUoffDiag_k* m_lower_Device;
    LDUoffDiag_k* m_plower_Device;
#endif

    /**
     * Structure used to sort an array
     */
    struct sortItem
    {
      unsigned int  m_NID;
      unsigned int* m_idx;
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
      idx_k*        powner,
      idx_k*        pneigh,
      LDUoffDiag_k* plower,
      idx_k*        prowStart
    )
    {

      // Allocate temporary meory needed for sorting operations
      idx_k* temporaryArray = (idx_k*)malloc( 2*nInternalFaces*sizeof(idx_k) );
      sortItem* sortItems   = (sortItem*)malloc( 2*nInternalFaces*sizeof(sortItem) );

      // Prepare the data structure to run the sorting algorithm
      for ( idx_k i=0; i<nInternalFaces; ++i )
      {
        temporaryArray[i] = i;
        sortItems[i].m_NID = neigh[i];
        sortItems[i].m_idx = &temporaryArray[i];
      };

      // Sort the array accordingly to the strategy implemented in the comparator
      qsort( (void*)sortItems, nInternalFaces, sizeof(sortItem), &comparator );

      // Fill the permutation vector
      for ( idx_k i=0; i<nInternalFaces; ++i )
      {
        idx_k tmp = *(sortItems[i].m_idx);
        powner[i] = owner[tmp];
        pneigh[i] = neigh[tmp];
        plower[i] = lower[tmp];
      };

      // Compute the permuted rowstart
      ComputeRowStart
      (
        nCells,
        nInternalFaces,
        pneigh,
        prowStart
      );

      // Free memory
      free( temporaryArray );
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
    CSR_CSR_IO
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
     ,m_pownerInp_CPU(nullptr)
     ,m_pneighInp_CPU(nullptr)
     ,m_prowStartInp_CPU(nullptr)
     ,m_plowerInp_CPU(nullptr)
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
     ,m_owner_Device(nullptr)
     ,m_neigh_Device(nullptr)
     ,m_rowStart_Device(nullptr)
     ,m_powner_Device(nullptr)
     ,m_pneigh_Device(nullptr)
     ,m_prowStart_Device(nullptr)
     ,m_psi_Device(nullptr)
     ,m_Apsi_Device(nullptr)
     ,m_upper_Device(nullptr)
     ,m_lower_Device(nullptr)
     ,m_plower_Device(nullptr)
#endif
    {};


    /**
     * @brief Destructor for the class
     */
    ~CSR_CSR_IO()
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
      m_pownerInp_CPU = nullptr;
      m_pneighInp_CPU = nullptr;
      m_prowStartInp_CPU = nullptr;
      m_plowerInp_CPU = nullptr;
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
      m_owner_Device = nullptr;
      m_neigh_Device = nullptr;
      m_rowStart_Device = nullptr;
      m_powner_Device = nullptr;
      m_pneigh_Device = nullptr;
      m_prowStart_Device = nullptr;
      m_psi_Device = nullptr;
      m_Apsi_Device = nullptr;
      m_upper_Device = nullptr;
      m_lower_Device = nullptr;
      m_plower_Device = nullptr;
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
    CSR_CSR_IO
    (
      const char*  prefix,
      idx_k  index
    ):
      CSR_CSR_IO
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
    CSR_CSR_IO
    (
      const char* prefix
    ):
      CSR_CSR_IO
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
    CSR_CSR_IO
    (
      idx_k  index
    ):
      CSR_CSR_IO
      (
        "LidDrivenCavity3D",
        index
      )
    {};

    /**
     * @brief Empty constructor for the IO object
     * 
     */
    CSR_CSR_IO():
      CSR_CSR_IO
      (
        "LidDrivenCavity3D",
        0
      )
    {};


    const char* kernelFlavour() const { return( "CSR_CSR" ); };

    /**
     * @brief load input data for the kernel
     */
    void 
    loadInputData( )
    {
      unitTestMatVec::loadInputData();
      // Allocate memory
      m_rowStartInp_CPU  = reinterpret_cast<idx_k*>       ( malloc( (m_nCellsInp_CPU+1)*    sizeof(idx_k)        )  );
      m_pownerInp_CPU    = reinterpret_cast<idx_k*>       ( malloc( m_nInternalFacesInp_CPU*sizeof(idx_k)        )  );
      m_pneighInp_CPU    = reinterpret_cast<idx_k*>       ( malloc( m_nInternalFacesInp_CPU*sizeof(idx_k)        )  );
      m_prowStartInp_CPU = reinterpret_cast<idx_k*>       ( malloc( (m_nCellsInp_CPU+1)*    sizeof(idx_k)        )  );
      m_plowerInp_CPU    = reinterpret_cast<LDUoffDiag_k*>( malloc( m_nInternalFacesInp_CPU*sizeof(LDUoffDiag_k) )  );
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
      // Compute the permuted values
      computePermutation
      (
        m_nCellsInp_CPU,
        m_nInternalFacesInp_CPU, 
        m_ownerInp_CPU,
        m_neighInp_CPU,
        m_lowerInp_CPU,
        m_pownerInp_CPU,
        m_pneighInp_CPU,
        m_plowerInp_CPU,
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
        //cudaMalloc( (void**)&m_owner_Device,     m_nInternalFacesInp_CPU*            sizeof(idx_k)        );
        //cudaMalloc( (void**)&m_neigh_Device,     m_nInternalFacesInp_CPU*            sizeof(idx_k)        );
        cudaMalloc( (void**)&m_rowStart_Device,  (m_nCellsInp_CPU+1)*                sizeof(idx_k)        );
        cudaMalloc( (void**)&m_powner_Device,    m_nInternalFacesInp_CPU*            sizeof(idx_k)        );
        cudaMalloc( (void**)&m_pneigh_Device,    m_nInternalFacesInp_CPU*            sizeof(idx_k)        );
        cudaMalloc( (void**)&m_prowStart_Device, (m_nCellsInp_CPU+1)*                sizeof(idx_k)        );
        //cudaMalloc( (void**)&m_lower_Device,     m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k) );
        cudaMalloc( (void**)&m_plower_Device,    m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k) );
        //cudaMalloc( (void**)&m_upper_Device,     m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k) );
        //cudaMalloc( (void**)&m_psi_Device,       m_cellsStrideInp_CPU*m_nCompInp_CPU*sizeof(fld_k)        );
        //cudaMalloc( (void**)&m_Apsi_Device,      m_cellsStrideInp_CPU*               sizeof(fld_k)        );
        // Synchronize the device
        cudaDeviceSynchronize();
        // Update the allocation flag
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
        //cudaMemcpy( m_owner_Device,     m_ownerInp_CPU,     m_nInternalFacesInp_CPU*            sizeof(idx_k),        cudaMemcpyHostToDevice );
        //cudaMemcpy( m_neigh_Device,     m_neighInp_CPU,     m_nInternalFacesInp_CPU*            sizeof(idx_k),        cudaMemcpyHostToDevice );
        cudaMemcpy( m_rowStart_Device,  m_rowStartInp_CPU,  (m_nCellsInp_CPU+1)*                sizeof(idx_k),        cudaMemcpyHostToDevice );
        cudaMemcpy( m_powner_Device,    m_pownerInp_CPU,    m_nInternalFacesInp_CPU*            sizeof(idx_k),        cudaMemcpyHostToDevice );
        cudaMemcpy( m_pneigh_Device,    m_pneighInp_CPU,    m_nInternalFacesInp_CPU*            sizeof(idx_k),        cudaMemcpyHostToDevice );
        cudaMemcpy( m_prowStart_Device, m_prowStartInp_CPU, (m_nCellsInp_CPU+1)*                sizeof(idx_k),        cudaMemcpyHostToDevice );
        //cudaMemcpy( m_lower_Device,     m_lowerInp_CPU,     m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k), cudaMemcpyHostToDevice );
        cudaMemcpy( m_plower_Device,    m_plowerInp_CPU,    m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k), cudaMemcpyHostToDevice );
        //cudaMemcpy( m_upper_Device,     m_upperInp_CPU,     m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k), cudaMemcpyHostToDevice );
        //cudaMemcpy( m_psi_Device,       m_psiInp_CPU,       m_cellsStrideInp_CPU*m_nCompInp_CPU*sizeof(fld_k),        cudaMemcpyHostToDevice );
        //cudaMemcpy( m_Apsi_Device,      m_ApsiInp_CPU,      m_cellsStrideInp_CPU*               sizeof(fld_k),        cudaMemcpyHostToDevice );
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
        // cudaMemcpy( m_ownerInp_CPU,     m_owner_Device,     m_nInternalFacesInp_CPU*            sizeof(idx_k),        cudaMemcpyDeviceToHost );
        // cudaMemcpy( m_neighInp_CPU,     m_neigh_Device,     m_nInternalFacesInp_CPU*            sizeof(idx_k),        cudaMemcpyDeviceToHost );
        cudaMemcpy( m_rowStartInp_CPU,  m_rowStart_Device,  (m_nCellsInp_CPU+1)*                sizeof(idx_k),        cudaMemcpyDeviceToHost );
        cudaMemcpy( m_pownerInp_CPU,    m_powner_Device,    m_nInternalFacesInp_CPU*            sizeof(idx_k),        cudaMemcpyDeviceToHost );
        cudaMemcpy( m_pneighInp_CPU,    m_pneigh_Device,    m_nInternalFacesInp_CPU*            sizeof(idx_k),        cudaMemcpyDeviceToHost );
        cudaMemcpy( m_prowStartInp_CPU, m_prowStart_Device, (m_nCellsInp_CPU+1)*                sizeof(idx_k),        cudaMemcpyDeviceToHost );
        // cudaMemcpy( m_lowerInp_CPU,     m_lower_Device,     m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k), cudaMemcpyDeviceToHost );
        cudaMemcpy( m_plowerInp_CPU,    m_plower_Device,    m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k), cudaMemcpyDeviceToHost );
        //cudaMemcpy( m_upperInp_CPU,     m_upper_Device,     m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k), cudaMemcpyDeviceToHost );
        //cudaMemcpy( m_psiInp_CPU,       m_psi_Device,       m_cellsStrideInp_CPU*m_nCompInp_CPU*sizeof(fld_k),        cudaMemcpyDeviceToHost );
        //cudaMemcpy( m_ApsiInp_CPU,      m_Apsi_Device,      m_cellsStrideInp_CPU*               sizeof(fld_k),        cudaMemcpyDeviceToHost );
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
    CSR_CSR_CPU
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
      m_pownerInp_CPU,
      m_pneighInp_CPU,
      m_prowStartInp_CPU,
      m_upperInp_CPU,
      m_lowerInp_CPU,
      m_plowerInp_CPU,
      m_psiInp_CPU,
      m_ApsiInp_CPU
    );
#endif

/* Call the CUDA kernel */
#if defined( __TEST_NATIVE_CUDA__ ) && !defined( __DRY_RUN__ )
      CSR_CSR_nativeCUDA<<<(2*m_nInternalFacesInp_CPU*__N_BLOCKS__+255)/256, 256>>>
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
        m_powner_Device,
        m_pneigh_Device,
        m_prowStart_Device,
        m_upper_Device,
        m_lower_Device,
        m_plower_Device,
        m_psi_Device,
        m_Apsi_Device
      );
      // Synchronize the device
      cudaDeviceSynchronize();
#endif

/* Call the OPEN_ACC kernel */
#if defined( __TEST_OPEN_ACC__ ) && !defined( __DRY_RUN__ )
      CSR_CSR_openACC
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
        m_powner_Device,
        m_pneigh_Device,
        m_prowStart_Device,
        m_upper_Device,
        m_lower_Device,
        m_plower_Device,
        m_psi_Device,
        m_Apsi_Device      );
      // Synchronize the device
      cudaDeviceSynchronize();
#endif

    /* Call the OPEN_MP kernel */
#if  defined( __TEST_OPEN_MP__ ) && !defined( __DRY_RUN__ )
      CSR_CSR_openMP
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
        m_powner_Device,
        m_pneigh_Device,
        m_prowStart_Device,
        m_upper_Device,
        m_lower_Device,
        m_plower_Device,
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
        CSR_CSR_CPU
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
          m_pownerInp_CPU,
          m_pneighInp_CPU,
          m_prowStartInp_CPU,
          m_upperInp_CPU,
          m_lowerInp_CPU,
          m_plowerInp_CPU,
          m_psiInp_CPU,
          m_ApsiInp_CPU
        );
      }
#endif

/* Call the CUDA kernel */
#if defined( __TEST_NATIVE_CUDA__ ) && !defined( __DRY_RUN__ )
      CSR_CSR_nativeCUDA<<<(2*m_nInternalFacesInp_CPU*__N_BLOCKS__+255)/256, 256>>>
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
        m_powner_Device,
        m_pneigh_Device,
        m_prowStart_Device,
        m_upper_Device,
        m_lower_Device,
        m_plower_Device,
        m_psi_Device,
        m_Apsi_Device
      );
      // Synchronize the device
      cudaDeviceSynchronize();
#endif

/* Call the OPEN_ACC kernel */
#if defined( __TEST_OPEN_ACC__ ) && !defined( __DRY_RUN__ )
      CSR_CSR_openACC
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
        m_powner_Device,
        m_pneigh_Device,
        m_prowStart_Device,
        m_upper_Device,
        m_lower_Device,
        m_plower_Device,
        m_psi_Device,
        m_Apsi_Device      );
      // Synchronize the device
      cudaDeviceSynchronize();
#endif

    /* Call the OPEN_MP kernel */
#if  defined( __TEST_OPEN_MP__ ) && !defined( __DRY_RUN__ )
      CSR_CSR_openMP
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
        m_powner_Device,
        m_pneigh_Device,
        m_prowStart_Device,
        m_upper_Device,
        m_lower_Device,
        m_plower_Device,
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
        free(m_pownerInp_CPU);
        free(m_pneighInp_CPU);
        free(m_prowStartInp_CPU);
        free(m_plowerInp_CPU);
        // Update allocation flag
        m_allocatedInput = false;
      };
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
      if ( m_allocatedDevice )
      {
        cudaFree(m_pownerInp_CPU);
        cudaFree(m_pneighInp_CPU);
        cudaFree(m_prowStartInp_CPU);
        cudaFree(m_plowerInp_CPU);
        // Synchronize the device
        cudaDeviceSynchronize();
        // Update allocation flag
        m_allocatedDevice = false;
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
__unitTest__( CSR_CSR_IO );
#endif
