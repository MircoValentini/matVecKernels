/*
 * @file unitTestMatVec.H
 *
 * @brief Definition of the routines and data to be used to test the 
 * LDUSYSCOOmatVecCompOffDiagStageOneOutput kernel
 *
 *  @date   18/Giu/2022
 *  @author Mirco Valentini
 */


#ifndef __MATVEC_IO_H__
#define __MATVEC_IO_H__


/**
 * System include
 */
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "math.h"


/**
 * Local include
 */
#include "dataTypes.H"
#include "LDUdataTypes.H"
#include "timing.H"
#include "unitTestBase.H"
#include "UnitTestsIOManager.H"


class unitTestMatVec:
  public unitTestBase
{

  protected:

    /* tolerance used for the check */
    double m_tolerance;

    /* Tracking allocations */
    bool m_allocatedInput;
    bool m_allocatedOutput;
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
    bool m_allocatedDevice;
#endif

    /*  Read the input data for  the Lernels */
    idx_k m_nCellsInp_CPU;
    idx_k m_nFacesInp_CPU;
    idx_k m_nInternalFacesInp_CPU;
    idx_k m_cellsStrideInp_CPU;
    idx_k m_facesStrideInp_CPU;
    idx_k m_nCompInp_CPU;
    idx_k m_iCompInp_CPU;

    // CPU Arrays
    idx_k*        m_ownerInp_CPU;
    idx_k*        m_neighInp_CPU;
    fld_k*        m_psiInp_CPU;
    fld_k*        m_ApsiInp_CPU;
    LDUoffDiag_k* m_upperInp_CPU;
    LDUoffDiag_k* m_lowerInp_CPU;

    /*  Read the expected output data for  the Lernels */
    idx_k m_nCellsOut_CPU;
    idx_k m_nFacesOut_CPU;
    idx_k m_nInternalFacesOut_CPU;
    idx_k m_cellsStrideOut_CPU;
    idx_k m_facesStrideOut_CPU;
    idx_k m_nCompOut_CPU;
    idx_k m_iCompOut_CPU;

    // CPU Arrays
    idx_k*        m_ownerOut_CPU;
    idx_k*        m_neighOut_CPU;
    fld_k*        m_psiOut_CPU;
    fld_k*        m_ApsiOut_CPU;
    LDUoffDiag_k* m_upperOut_CPU;
    LDUoffDiag_k* m_lowerOut_CPU;

#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
    // GPU Arrays
    idx_k*        m_owner_Device;
    idx_k*        m_neigh_Device;
    fld_k*        m_psi_Device;
    fld_k*        m_Apsi_Device;
    LDUoffDiag_k* m_upper_Device;
    LDUoffDiag_k* m_lower_Device;
#endif

    /* Clock used for the profiler */
    simpleClock_t m_profiler;


    /* Compute matrix statistics */
    fld_k
    computeBandwidth
    (
      idx_k               N,
      idx_k               M,
      idx_k const * const owner,
      idx_k const * const neigh
    ) const
    {
      idx_k bandWidth=0;
      for ( idx_k i=1; i<M; ++i )
      {
        if ( neigh[i] > owner[i] )
        {
          idx_k candidateBandwidth = neigh[i] - owner[i] + 1;
          bandWidth = (bandWidth>candidateBandwidth) ? bandWidth : candidateBandwidth;
        }
      };
      fld_k tmp =  ((fld_k)bandWidth)/N;
      /* Exit point */
      return(tmp);
    };

    fld_k
    computeJumpParameter
    (
      idx_k               N,
      idx_k               M,
      idx_k const * const owner
    ) const
    {
      idx_k dnnz = 0;
      idx_k* nnz  = (idx_k*)malloc( N*sizeof(idx_k) );
      for ( idx_k i=0; i<N; ++i )
      {
        nnz[i] = 0;
      };
      for ( idx_k i=0; i<M; ++i )
      {
        nnz[owner[i]] += 1;
      };
      for ( idx_k i=0; i<N-1; ++i )
      {
        if ( nnz[i] != nnz[i+1] )
        {
          dnnz += 1;
        };
      };
      free(nnz);
      fld_k tmp = ((fld_k)dnnz)/N;
      /* Exit point */
      return(tmp);
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
    unitTestMatVec
    ( const char* outputFile,
      const char* UTdir,
      const char* prefix,
      idx_k       index,
      double      tolerance
    ):
      unitTestBase(outputFile,UTdir,prefix,"matVec",index)
     ,m_tolerance(tolerance)
     ,m_allocatedInput(false)
     ,m_allocatedOutput(false)
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
     ,m_allocatedDevice(false)
#endif
     ,m_profiler()
     ,m_nCellsInp_CPU(0)
     ,m_nFacesInp_CPU(0)
     ,m_nInternalFacesInp_CPU(0)
     ,m_cellsStrideInp_CPU(0)
     ,m_facesStrideInp_CPU(0)
     ,m_nCompInp_CPU(0)
     ,m_iCompInp_CPU(0)
     ,m_ownerInp_CPU(nullptr)
     ,m_neighInp_CPU(nullptr)
     ,m_psiInp_CPU(nullptr)
     ,m_ApsiInp_CPU(nullptr)
     ,m_upperInp_CPU(nullptr)
     ,m_lowerInp_CPU(nullptr)
     ,m_nCellsOut_CPU(0)
     ,m_nFacesOut_CPU(0)
     ,m_nInternalFacesOut_CPU(0)
     ,m_cellsStrideOut_CPU(0)
     ,m_facesStrideOut_CPU(0)
     ,m_nCompOut_CPU(0)
     ,m_iCompOut_CPU(0)
     ,m_ownerOut_CPU(nullptr)
     ,m_neighOut_CPU(nullptr)
     ,m_psiOut_CPU(nullptr)
     ,m_ApsiOut_CPU(nullptr)
     ,m_upperOut_CPU(nullptr)
     ,m_lowerOut_CPU(nullptr)
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
     ,m_owner_Device(nullptr)
     ,m_neigh_Device(nullptr)
     ,m_psi_Device(nullptr)
     ,m_Apsi_Device(nullptr)
     ,m_upper_Device(nullptr)
     ,m_lower_Device(nullptr)
#endif
    {};


    /**
     * @brief Destructor for the class
     */
    ~unitTestMatVec()
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
      m_nCellsInp_CPU = 0;
      m_nFacesInp_CPU = 0;
      m_nInternalFacesInp_CPU = 0;
      m_cellsStrideInp_CPU = 0;
      m_facesStrideInp_CPU = 0;
      m_nCompInp_CPU = 0;
      m_iCompInp_CPU = 0;
      m_ownerInp_CPU = nullptr;
      m_neighInp_CPU = nullptr;
      m_psiInp_CPU = nullptr;
      m_ApsiInp_CPU = nullptr;
      m_upperInp_CPU = nullptr;
      m_lowerInp_CPU = nullptr;
      m_nCellsOut_CPU = 0;
      m_nFacesOut_CPU = 0;
      m_nInternalFacesOut_CPU = 0;
      m_cellsStrideOut_CPU = 0;
      m_facesStrideOut_CPU = 0;
      m_nCompOut_CPU = 0;
      m_iCompOut_CPU = 0;
      m_ownerOut_CPU = nullptr;
      m_neighOut_CPU = nullptr;
      m_psiOut_CPU = nullptr;
      m_ApsiOut_CPU = nullptr;
      m_upperOut_CPU = nullptr;
      m_lowerOut_CPU = nullptr;
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
      m_owner_Device = nullptr;
      m_neigh_Device = nullptr;
      m_psi_Device = nullptr;
      m_Apsi_Device = nullptr;
      m_upper_Device = nullptr;
      m_lower_Device = nullptr;
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
    unitTestMatVec
    (
      const char*  prefix,
      idx_k        index
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
    unitTestMatVec
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
    unitTestMatVec
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
    unitTestMatVec():
      unitTestMatVec
      (
        "LidDrivenCavity3D",
        0
      )
    {};


    void
    Dispose()
    {
      return;	    
    };    


    /**
     * @brief load input data for the kernel
     */
    void 
    loadInputData( )
    {
      if ( !m_allocatedInput )
      {
        // Open the test case file
        HDF5kernels IO( m_UTdir, m_prefix, m_name, m_index );
        IO.Open();
        IO.getDataset( "nCells",         flavour_e::input,  &m_nCellsInp_CPU );
        IO.getDataset( "nFaces",         flavour_e::input,  &m_nFacesInp_CPU );
        IO.getDataset( "nInternalFaces", flavour_e::input,  &m_nInternalFacesInp_CPU );
        IO.getDataset( "cellStride",     flavour_e::input,  &m_cellsStrideInp_CPU );
        IO.getDataset( "faceStride",     flavour_e::input,  &m_facesStrideInp_CPU );
        IO.getDataset( "iComp",          flavour_e::input,  &m_iCompInp_CPU );
        IO.getDataset( "nComp",          flavour_e::input,  &m_nCompInp_CPU );
        // Allocate memory
        m_ownerInp_CPU = reinterpret_cast<idx_k*>       ( malloc( m_nInternalFacesInp_CPU*            sizeof(idx_k) )         );
        m_neighInp_CPU = reinterpret_cast<idx_k*>       ( malloc( m_nInternalFacesInp_CPU*            sizeof(idx_k) )         );
        m_upperInp_CPU = reinterpret_cast<LDUoffDiag_k*>( malloc( m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k) )  );
        m_lowerInp_CPU = reinterpret_cast<LDUoffDiag_k*>( malloc( m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k) )  );
        m_psiInp_CPU   = reinterpret_cast<fld_k*>       ( malloc( m_cellsStrideInp_CPU*m_nCompInp_CPU*sizeof(fld_k) )         );
        m_ApsiInp_CPU  = reinterpret_cast<fld_k*>       ( malloc( m_cellsStrideInp_CPU*               sizeof(fld_k) )         );
        // REad vectors
        IO.getDataset( "owner", flavour_e::input, m_ownerInp_CPU );
        IO.getDataset( "neigh", flavour_e::input, m_neighInp_CPU );
        IO.getDataset( "upper", flavour_e::input, m_upperInp_CPU );
        IO.getDataset( "lower", flavour_e::input, m_lowerInp_CPU );
        IO.getDataset( "psi",   flavour_e::input, m_psiInp_CPU );
        IO.getDataset( "Apsi",  flavour_e::input, m_ApsiInp_CPU );
        IO.Close();
        // Update the allocation flag
        m_allocatedInput = true;
      }
      else
      {
        printf( "ERROR :: data already allocated:: %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__ );
        abort();
      };
      /* Exit point */
      return;
    };


    /**
     * @brief load output data for the kernel
     */
    void 
    loadOutputData( )
    {
      if ( !m_allocatedOutput )
      {
        // Open the test case file
        HDF5kernels IO( m_UTdir, m_prefix, m_name, m_index );
        IO.Open();
        IO.getDataset( "nCells",         flavour_e::output,  &m_nCellsOut_CPU );
        IO.getDataset( "nFaces",         flavour_e::output,  &m_nFacesOut_CPU );
        IO.getDataset( "nInternalFaces", flavour_e::output,  &m_nInternalFacesOut_CPU );
        IO.getDataset( "cellStride",     flavour_e::output,  &m_cellsStrideOut_CPU );
        IO.getDataset( "faceStride",     flavour_e::output,  &m_facesStrideOut_CPU );
        IO.getDataset( "iComp",          flavour_e::output,  &m_iCompOut_CPU );
        IO.getDataset( "nComp",          flavour_e::output,  &m_nCompOut_CPU );
        // Allocate memory
        m_ownerOut_CPU = reinterpret_cast<idx_k*>       ( malloc( m_nInternalFacesOut_CPU*            sizeof(idx_k) )         );
        m_neighOut_CPU = reinterpret_cast<idx_k*>       ( malloc( m_nInternalFacesOut_CPU*            sizeof(idx_k) )         );
        m_upperOut_CPU = reinterpret_cast<LDUoffDiag_k*>( malloc( m_nInternalFacesOut_CPU*            sizeof(LDUoffDiag_k) )  );
        m_lowerOut_CPU = reinterpret_cast<LDUoffDiag_k*>( malloc( m_nInternalFacesOut_CPU*            sizeof(LDUoffDiag_k) )  );
        m_psiOut_CPU   = reinterpret_cast<fld_k*>       ( malloc( m_cellsStrideOut_CPU*m_nCompOut_CPU*sizeof(fld_k) )         );
        m_ApsiOut_CPU  = reinterpret_cast<fld_k*>       ( malloc( m_cellsStrideOut_CPU*               sizeof(fld_k) )         );
        // REad vectors
        IO.getDataset( "owner", flavour_e::output, m_ownerOut_CPU );
        IO.getDataset( "neigh", flavour_e::output, m_neighOut_CPU );
        IO.getDataset( "upper", flavour_e::output, m_upperOut_CPU );
        IO.getDataset( "lower", flavour_e::output, m_lowerOut_CPU );
        IO.getDataset( "psi",   flavour_e::output, m_psiOut_CPU );
        IO.getDataset( "Apsi",  flavour_e::output, m_ApsiOut_CPU );
        IO.Close();
        // Update the allocation flag
        m_allocatedOutput = true;
      }
      else
      {
        printf( "ERROR :: data already allocated:: %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__ );
        abort();
      };
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
        cudaMalloc( (void**)&m_owner_Device, m_nInternalFacesInp_CPU*            sizeof(idx_k)        );
        cudaMalloc( (void**)&m_neigh_Device, m_nInternalFacesInp_CPU*            sizeof(idx_k)        );
        cudaMalloc( (void**)&m_lower_Device, m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k) );
        cudaMalloc( (void**)&m_upper_Device, m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k) );
        cudaMalloc( (void**)&m_psi_Device,   m_cellsStrideInp_CPU*m_nCompInp_CPU*sizeof(fld_k)        );
        cudaMalloc( (void**)&m_Apsi_Device,  m_cellsStrideInp_CPU*               sizeof(fld_k)        );
        // Synchronize the device
        cudaDeviceSynchronize();
        // Update the allocation flag
        m_allocatedDevice = true;
      }
      else
      {
        printf( "ERROR :: data already allocated:: %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__ );
        abort();
      };
#endif
      /* Exit point */
      return;
    };


    /**
     * @brief copy data to the device
     */
    void
    copyToDevice()
    {
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
      if ( m_allocatedInput && m_allocatedDevice )
      {
        // Allocate memory
        cudaMemcpy( m_owner_Device, m_ownerInp_CPU, m_nInternalFacesInp_CPU*            sizeof(idx_k),        cudaMemcpyHostToDevice );
        cudaMemcpy( m_neigh_Device, m_neighInp_CPU, m_nInternalFacesInp_CPU*            sizeof(idx_k),        cudaMemcpyHostToDevice );
        cudaMemcpy( m_lower_Device, m_lowerInp_CPU, m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k), cudaMemcpyHostToDevice );
        cudaMemcpy( m_upper_Device, m_upperInp_CPU, m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k), cudaMemcpyHostToDevice );
        cudaMemcpy( m_psi_Device,   m_psiInp_CPU,   m_cellsStrideInp_CPU*m_nCompInp_CPU*sizeof(fld_k),        cudaMemcpyHostToDevice );
        cudaMemcpy( m_Apsi_Device,  m_ApsiInp_CPU,  m_cellsStrideInp_CPU*               sizeof(fld_k),        cudaMemcpyHostToDevice );
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
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
      if ( m_allocatedInput && m_allocatedDevice )
      {
        // Allocate memory
        cudaMemcpy( m_ownerInp_CPU, m_owner_Device, m_nInternalFacesInp_CPU*            sizeof(idx_k),        cudaMemcpyDeviceToHost );
        cudaMemcpy( m_neighInp_CPU, m_neigh_Device, m_nInternalFacesInp_CPU*            sizeof(idx_k),        cudaMemcpyDeviceToHost );
        cudaMemcpy( m_lowerInp_CPU, m_lower_Device, m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k), cudaMemcpyDeviceToHost );
        cudaMemcpy( m_upperInp_CPU, m_upper_Device, m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k), cudaMemcpyDeviceToHost );
        cudaMemcpy( m_psiInp_CPU,   m_psi_Device,   m_cellsStrideInp_CPU*m_nCompInp_CPU*sizeof(fld_k),        cudaMemcpyDeviceToHost );
        cudaMemcpy( m_ApsiInp_CPU,  m_Apsi_Device,  m_cellsStrideInp_CPU*               sizeof(fld_k),        cudaMemcpyDeviceToHost );
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


    fld_k
    bandWidth() const
    {
      return( computeBandwidth
              (
                m_nCellsInp_CPU,
                m_nInternalFacesInp_CPU,
                m_ownerInp_CPU,
                m_neighInp_CPU
              )
            );
    };


    fld_k
    jumpParam() const
    {
      return( computeJumpParameter
              (
                m_nCellsInp_CPU,
                m_nInternalFacesInp_CPU,
                m_ownerInp_CPU
              )
            );
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
        free(m_lowerInp_CPU);
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
        free(m_lowerOut_CPU);
        free(m_upperOut_CPU);
        free(m_psiOut_CPU);
        free(m_ApsiOut_CPU);
        // Update allocation flag
        m_allocatedOutput  = false;
      };
#if defined(__TEST_NATIVE_CUDA__) || defined(__TEST_OPEN_ACC__) || defined(__TEST_OPEN_MP__)
      if ( m_allocatedDevice )
      {
        cudaFree(m_owner_Device);
        cudaFree(m_neigh_Device);
        cudaFree(m_lower_Device);
        cudaFree(m_upper_Device);
        cudaFree(m_psi_Device);
        cudaFree(m_Apsi_Device);
        // Synchronize the device
        cudaDeviceSynchronize();
        // Update allocation flag
        m_allocatedDevice = false;
      };
#endif
      /* Exit point */
      return;
    };


    /**
     * @brief Check the result of the kernels
     *
     * @return true if the result is correct
     */
    bool
    check(  )
    {
      /* Local variabels */
      bool check=true;
      if ( m_allocatedInput && m_allocatedOutput )
      {
        for ( idx_k i=0; i<m_nCellsInp_CPU; i++ )
        {
          if ( fabs( m_ApsiInp_CPU[i] - m_ApsiOut_CPU[i]) > m_tolerance ) 
          {
            check = false;
          };
        };
      }
      else
      {
        printf( "ERROR :: data are not allocated:: %s, %s, %d\n", __FUNCTION__, __FILE__, __LINE__ );
        abort();
      }
      /* Exit point */
      return( check );
    };

};

#endif

