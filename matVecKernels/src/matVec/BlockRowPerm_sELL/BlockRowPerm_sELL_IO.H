/*
 * @file BlockRowPerm_IO.H
 *
 * @brief Definition of the routines and data to be used to test the 
 * LDUSYSCOOmatVecCompOffDiagStageOneOutput kernel
 *
 *  @date   18/Giu/2022
 *  @author Mirco Valentini
 */


#ifndef __BlockRowPerm_sELL_IO_H__
#define __BlockRowPerm_sELL_IO_H__

/**
 * c includes
 */
#include <climits>

/**
 * Local include
 */
#include "unitTestMatVec_IO.H"
#include "BlkRowPerm_sELL.H"
#include "blockCompressor.H"
#include "parser_sELL.H"

/**
 * Include the kernels
 */
#if !defined( __DRY_RUN__ )
#include "BlockRowPerm_sELL_kernels.H"
#endif


class BlockRowPerm_sELL_IO:
  public unitTestMatVec
{
  private:

    parseELL m_parser;

    blockRowPerm_sELL* m_permutation;
    blockCompressor*   m_compressor;

    idx_k  m_AVXsize;
    idx_k  m_nBlks;
    double m_scale;
    double m_limitScale;
    char*  m_name;
    blockConstructionKind m_blockConstructionKind;
    cellSortingStrategy   m_cellSortingStrategy;
    memorySortingStrategy m_memorySortingStrategy;

    /* Local permuted variables variables*/
    idx_k*        __restrict__ m_pOwnerInp;
    idx_k*        __restrict__ m_pNeighInp;
    fld_k*        __restrict__ m_pPsiInp;
    fld_k*        __restrict__ m_pApsiInp;
    LDUoffDiag_k* __restrict__ m_pUpperInp;
    LDUoffDiag_k* __restrict__ m_pLowerInp;
    bool m_locAllocatedInput;

    idx_k*        __restrict__ m_pOwnerOut;
    idx_k*        __restrict__ m_pNeighOut;
    fld_k*        __restrict__ m_pPsiOut;
    fld_k*        __restrict__ m_pApsiOut;
    LDUoffDiag_k* __restrict__ m_pUpperOut;
    LDUoffDiag_k* __restrict__ m_pLowerOut;
    bool m_locAllocatedOutput;


  public:

    /**
     * @brief Constructor for the IO object given all the parameters
     *
     * The name of the unit test (the searched file ) will be generated as follows:
     *
     *  UTname = UTdir/${name}/${prefix}_${name}_${%6.6d-index}.h5
     *
     * @param[in] outputFile           name of the output file
     * @param[in] UTdir                direcotry where the unit tests are stored
     * @param[in] prefix               prefix to be applied to the neame of the unit tests
     * @param[in] index                index of hte unit thest
     * @param[in] tolerance            tolerance used for the check
     * @param[in] configurationString  number of blocks
     */
    BlockRowPerm_sELL_IO
    (
      const char* outputFile,
      const char* UTdir,
      const char* prefix,
      idx_k       index,
      double      tolerance,
      const char* configurationString
    ):
      // Inheritance
      unitTestMatVec(outputFile,UTdir,prefix,index,tolerance)
      // Members
     ,m_permutation(nullptr)
     ,m_compressor(nullptr)
     ,m_AVXsize(8)
     ,m_nBlks(0)
     ,m_scale(0.0)
     ,m_limitScale(0.0)
     ,m_name(nullptr)
     ,m_blockConstructionKind(blockConstructionKind::oneParameter)
     ,m_cellSortingStrategy(cellSortingStrategy::bandwidth)
     ,m_memorySortingStrategy(memorySortingStrategy::ell)
     ,m_pOwnerInp(nullptr)
     ,m_pNeighInp(nullptr)
     ,m_pPsiInp(nullptr)
     ,m_pApsiInp(nullptr)
     ,m_pUpperInp(nullptr)
     ,m_pLowerInp(nullptr)
     ,m_locAllocatedInput(false)
     ,m_pOwnerOut(nullptr)
     ,m_pNeighOut(nullptr)
     ,m_pPsiOut(nullptr)
     ,m_pApsiOut(nullptr)
     ,m_pUpperOut(nullptr)
     ,m_pLowerOut(nullptr)
     ,m_locAllocatedOutput(false)
    {
      /* Construct the name o f the kernel */
      // printf( "Configuration string :: %s\n", configurationString );
      m_parser.readConfiguration( configurationString, false );
      if ( m_parser.lastParserStatus() == false )
      {
        printf( "%s\n", m_parser.lastErrorMsg() );
        printf( "%d\n", m_parser.lastErrorLine() );
        printf( "%d\n", m_parser.lastErrorIdx() );
        abort();
      }
      else
      {
        
        m_blockConstructionKind = m_parser.getBlockConstructionKind();
        m_AVXsize               = m_parser.AVXsize();
        m_nBlks                 = m_parser.nBlocks();
        m_scale                 = m_parser.scaleFactor();
        m_limitScale            = m_parser.limitScaleFactor();
        m_cellSortingStrategy   = m_parser.cellSorting();
        m_memorySortingStrategy = m_parser.memorySorting();
        m_name                  = m_parser.configurationString();
      }
      /* Exit point */
      return;
    };

    /**
     * @brief Constructor for the IO object given all the parameters
     *
     * The name of the unit test (the searched file ) will be generated as follows:
     *
     *  UTname = UTdir/${name}/${prefix}_${name}_${%6.6d-index}.h5
     *
     * @param[in] outputFile name of the output file
     * @param[in] UTdir      direcotry where the unit tests are stored
     * @param[in] prefix     prefix to be applied to the neame of the unit tests
     * @param[in] index      index of hte unit thest
     * @param[in] tolerance  tolerance used for the check
     * @param[in] nBlks      number of blocks
     */
    BlockRowPerm_sELL_IO
    (
      const char* outputFile,
      const char* UTdir,
      const char* prefix,
      idx_k       index,
      double      tolerance,
      idx_k       nBlks
    ):
      // Inheritance
      unitTestMatVec(outputFile,UTdir,prefix,index,tolerance)
      // Members
     ,m_permutation(nullptr)
     ,m_compressor(nullptr)
     ,m_AVXsize(8)
     ,m_nBlks(nBlks)
     ,m_scale(0.0)
     ,m_limitScale(0.0)
     ,m_name(nullptr)
     ,m_blockConstructionKind(blockConstructionKind::oneParameter)
     ,m_cellSortingStrategy(cellSortingStrategy::bandwidth)
     ,m_memorySortingStrategy(memorySortingStrategy::ell)
     ,m_pOwnerInp(nullptr)
     ,m_pNeighInp(nullptr)
     ,m_pPsiInp(nullptr)
     ,m_pApsiInp(nullptr)
     ,m_pUpperInp(nullptr)
     ,m_pLowerInp(nullptr)
     ,m_locAllocatedInput(false)
     ,m_pOwnerOut(nullptr)
     ,m_pNeighOut(nullptr)
     ,m_pPsiOut(nullptr)
     ,m_pApsiOut(nullptr)
     ,m_pUpperOut(nullptr)
     ,m_pLowerOut(nullptr)
     ,m_locAllocatedOutput(false)
    {
      /* Construct the name o f the kernel */
      m_name = (char*)malloc(32);
      memset( m_name, 0, 32 );
      sprintf( m_name, "BlockRowPerm_%d", m_nBlks );
      /* Exit point */
      return;
    };


    /**
     * @brief Constructor for the IO object given all the parameters
     *
     * The name of the unit test (the searched file ) will be generated as follows:
     *
     *  UTname = UTdir/${name}/${prefix}_${name}_${%6.6d-index}.h5
     *
     * @param[in] outputFile name of the output file
     * @param[in] UTdir      direcotry where the unit tests are stored
     * @param[in] prefix     prefix to be applied to the neame of the unit tests
     * @param[in] index      index of hte unit thest
     * @param[in] tolerance  tolerance used for the check
     * @param[in] nBlks      number of blocks
     */
    BlockRowPerm_sELL_IO
    (
      const char* outputFile,
      const char* UTdir,
      const char* prefix,
      idx_k       index,
      double      tolerance,
      idx_k       AVXsize,
      idx_k       nBlks
    ):
      // Inheritance
      unitTestMatVec(outputFile,UTdir,prefix,index,tolerance)
      // Members
     ,m_permutation(nullptr)
     ,m_compressor(nullptr)
     ,m_AVXsize(AVXsize)
     ,m_nBlks(nBlks)
     ,m_scale(0.0)
     ,m_limitScale(0.0)
     ,m_name(nullptr)
     ,m_blockConstructionKind(blockConstructionKind::oneParameter)
     ,m_cellSortingStrategy(cellSortingStrategy::bandwidth)
     ,m_memorySortingStrategy(memorySortingStrategy::ell)
     ,m_pOwnerInp(nullptr)
     ,m_pNeighInp(nullptr)
     ,m_pPsiInp(nullptr)
     ,m_pApsiInp(nullptr)
     ,m_pUpperInp(nullptr)
     ,m_pLowerInp(nullptr)
     ,m_locAllocatedInput(false)
     ,m_pOwnerOut(nullptr)
     ,m_pNeighOut(nullptr)
     ,m_pPsiOut(nullptr)
     ,m_pApsiOut(nullptr)
     ,m_pUpperOut(nullptr)
     ,m_pLowerOut(nullptr)
     ,m_locAllocatedOutput(false)
    {
      /* Error handling */
      if
      (
        m_AVXsize != 1 &&
        m_AVXsize != 2 &&
        m_AVXsize != 4 &&
        m_AVXsize != 8
      )
      {
        printf( "ERROR:: wrong AVX size %lu\n", m_AVXsize );
        printf( "        valid valuse can be: 1, 2, 4, 8\n" );
        abort();
      };
      /* Construct the name o f the kernel */
      m_name = (char*)malloc(32);
      memset( m_name, 0, 32 );
      sprintf( m_name, "BlockRowPerm_%d", m_nBlks );
      /* Exit point */
      return;
    };


    /**
     * @brief Constructor for the IO object given all the parameters
     *
     * The name of the unit test (the searched file ) will be generated as follows:
     *
     *  UTname = UTdir/${name}/${prefix}_${name}_${%6.6d-index}.h5
     *
     * @param[in] outputFile name of the output file
     * @param[in] UTdir      direcotry where the unit tests are stored
     * @param[in] prefix     prefix to be applied to the neame of the unit tests
     * @param[in] index      index of hte unit thest
     * @param[in] tolerance  tolerance used for the check
     * @param[in] nBlks      number of blocks
     */
    BlockRowPerm_sELL_IO
    (
      const char*           outputFile,
      const char*           UTdir,
      const char*           prefix,
      idx_k                 index,
      double                tolerance,
      idx_k                 AVXsize,
      idx_k                 nBlks,
      cellSortingStrategy   cellSort,
      memorySortingStrategy memSort
    ):
      // Inheritance
      unitTestMatVec(outputFile,UTdir,prefix,index,tolerance)
      // Members
     ,m_permutation(nullptr)
     ,m_compressor(nullptr)
     ,m_AVXsize(AVXsize)
     ,m_nBlks(nBlks)
     ,m_scale(0.0)
     ,m_limitScale(0.0)
     ,m_name(nullptr)
     ,m_blockConstructionKind(blockConstructionKind::oneParameter)
     ,m_cellSortingStrategy(cellSort)
     ,m_memorySortingStrategy(memSort)
     ,m_pOwnerInp(nullptr)
     ,m_pNeighInp(nullptr)
     ,m_pPsiInp(nullptr)
     ,m_pApsiInp(nullptr)
     ,m_pUpperInp(nullptr)
     ,m_pLowerInp(nullptr)
     ,m_locAllocatedInput(false)
     ,m_pOwnerOut(nullptr)
     ,m_pNeighOut(nullptr)
     ,m_pPsiOut(nullptr)
     ,m_pApsiOut(nullptr)
     ,m_pUpperOut(nullptr)
     ,m_pLowerOut(nullptr)
     ,m_locAllocatedOutput(false)
    {
      /* Error handling */
      if
      (
        m_AVXsize != 1 &&
        m_AVXsize != 2 &&
        m_AVXsize != 4 &&
        m_AVXsize != 8
      )
      {
        printf( "ERROR:: wrong AVX size %lu\n", m_AVXsize );
        printf( "        valid valuse can be: 1, 2, 4, 8\n" );
        abort();
      };
      /* Construct the name o f the kernel */
      m_name = (char*)malloc(32);
      memset( m_name, 0, 32 );
      sprintf( m_name, "BlockRowPerm_%d", m_nBlks );
      /* Exit point */
      return;
    };


    /**
     * @brief Constructor for the IO object given all the parameters
     *
     * The name of the unit test (the searched file ) will be generated as follows:
     *
     *  UTname = UTdir/${name}/${prefix}_${name}_${%6.6d-index}.h5
     *
     * @param[in] outputFile name of the output file
     * @param[in] UTdir      direcotry where the unit tests are stored
     * @param[in] prefix     prefix to be applied to the neame of the unit tests
     * @param[in] index      index of hte unit thest
     * @param[in] tolerance  tolerance used for the check
     * @param[in] nBlks      number of blocks
     */
    BlockRowPerm_sELL_IO
    (
      const char* outputFile,
      const char* UTdir,
      const char* prefix,
      idx_k       index,
      double      tolerance,
      idx_k       AVXsize,
      double      scale,
      double      limitScale
    ):
      // Inheritance
      unitTestMatVec(outputFile,UTdir,prefix,index,tolerance)
      // Members
     ,m_permutation(nullptr)
     ,m_compressor(nullptr)
     ,m_AVXsize(AVXsize)
     ,m_nBlks(0)
     ,m_scale(scale)
     ,m_limitScale(limitScale)
     ,m_name(nullptr)
     ,m_blockConstructionKind(blockConstructionKind::twoParameters)
     ,m_cellSortingStrategy(cellSortingStrategy::bandwidth)
     ,m_memorySortingStrategy(memorySortingStrategy::ell)
     ,m_pOwnerInp(nullptr)
     ,m_pNeighInp(nullptr)
     ,m_pPsiInp(nullptr)
     ,m_pApsiInp(nullptr)
     ,m_pUpperInp(nullptr)
     ,m_pLowerInp(nullptr)
     ,m_locAllocatedInput(false)
     ,m_pOwnerOut(nullptr)
     ,m_pNeighOut(nullptr)
     ,m_pPsiOut(nullptr)
     ,m_pApsiOut(nullptr)
     ,m_pUpperOut(nullptr)
     ,m_pLowerOut(nullptr)
     ,m_locAllocatedOutput(false)
    {
      /* Error handling */
      if
      (
        m_AVXsize != 1 &&
        m_AVXsize != 2 &&
        m_AVXsize != 4 &&
        m_AVXsize != 8
      )
      {
        printf( "ERROR:: wrong AVX size %lu\n", m_AVXsize );
        printf( "        valid valuse can be: 1, 2, 4, 8\n" );
        abort();
      };
      /* Construct the name o f the kernel */
      m_name = (char*)malloc(32);
      memset( m_name, 0, 32 );
      sprintf( m_name, "BlockRowPerm_%d", m_nBlks );
      /* Exit point */
      return;
    };


    /**
     * @brief Constructor for the IO object given all the parameters
     *
     * The name of the unit test (the searched file ) will be generated as follows:
     *
     *  UTname = UTdir/${name}/${prefix}_${name}_${%6.6d-index}.h5
     *
     * @param[in] outputFile name of the output file
     * @param[in] UTdir      direcotry where the unit tests are stored
     * @param[in] prefix     prefix to be applied to the neame of the unit tests
     * @param[in] index      index of hte unit thest
     * @param[in] tolerance  tolerance used for the check
     * @param[in] nBlks      number of blocks
     */
    BlockRowPerm_sELL_IO
    (
      const char*           outputFile,
      const char*           UTdir,
      const char*           prefix,
      idx_k                 index,
      double                tolerance,
      idx_k                 AVXsize,
      double                scale,
      double                limitScale,
      cellSortingStrategy   cellSort,
      memorySortingStrategy memSort
    ):
      // Inheritance
      unitTestMatVec(outputFile,UTdir,prefix,index,tolerance)
      // Members
     ,m_permutation(nullptr)
     ,m_compressor(nullptr)
     ,m_AVXsize(AVXsize)
     ,m_nBlks(0)
     ,m_scale(scale)
     ,m_limitScale(limitScale)
     ,m_name(nullptr)
     ,m_blockConstructionKind(blockConstructionKind::twoParameters)
     ,m_cellSortingStrategy(cellSort)
     ,m_memorySortingStrategy(memSort)
     ,m_pOwnerInp(nullptr)
     ,m_pNeighInp(nullptr)
     ,m_pPsiInp(nullptr)
     ,m_pApsiInp(nullptr)
     ,m_pUpperInp(nullptr)
     ,m_pLowerInp(nullptr)
     ,m_locAllocatedInput(false)
     ,m_pOwnerOut(nullptr)
     ,m_pNeighOut(nullptr)
     ,m_pPsiOut(nullptr)
     ,m_pApsiOut(nullptr)
     ,m_pUpperOut(nullptr)
     ,m_pLowerOut(nullptr)
     ,m_locAllocatedOutput(false)
    {
      /* Error handling */
      if
      (
        m_AVXsize != 1 &&
        m_AVXsize != 2 &&
        m_AVXsize != 4 &&
        m_AVXsize != 8
      )
      {
        printf( "ERROR:: wrong AVX size %lu\n", m_AVXsize );
        printf( "        valid valuse can be: 1, 2, 4, 8\n" );
        abort();
      };
      /* Construct the name o f the kernel */
      m_name = (char*)malloc(32);
      memset( m_name, 0, 32 );
      sprintf( m_name, "BlockRowPerm_%d", m_nBlks );
      /* Exit point */
      return;
    };


    /**
     * @brief Destructor for the class
     */
    ~BlockRowPerm_sELL_IO()
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
    BlockRowPerm_sELL_IO
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
    BlockRowPerm_sELL_IO
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
    BlockRowPerm_sELL_IO
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
    BlockRowPerm_sELL_IO():
      unitTestMatVec
      (
        "LidDrivenCavity3D",
        0
      )
    {};

    const char* kernelFlavour() const { return( m_name ); };

    /**
     * @brief load input data for the kernel
     */
    void 
    loadInputData( )
    {
      unitTestMatVec::loadInputData();
      /* Local data */
      if ( !m_locAllocatedInput )
      {
        m_pOwnerInp     = reinterpret_cast<idx_k*>         ( malloc( m_nInternalFacesInp_CPU*            sizeof(idx_k) )         );
        m_pNeighInp     = reinterpret_cast<idx_k*>         ( malloc( m_nInternalFacesInp_CPU*            sizeof(idx_k) )         );
        m_pUpperInp     = reinterpret_cast<LDUoffDiag_k*>  ( malloc( m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k) )  );
        m_pLowerInp     = reinterpret_cast<LDUoffDiag_k*>  ( malloc( m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k) )  );
        m_pPsiInp       = reinterpret_cast<fld_k*>         ( malloc( m_cellsStrideInp_CPU*m_nCompInp_CPU*sizeof(fld_k) )         );
        m_pApsiInp      = reinterpret_cast<fld_k*>         ( malloc( m_cellsStrideInp_CPU*               sizeof(fld_k) )         );
        /* Prmutation object*/
        m_permutation = new blockRowPerm_sELL
                            (
                              m_nCellsInp_CPU, 
                              m_nInternalFacesInp_CPU,
                              m_AVXsize,
                              m_ownerInp_CPU,
                              m_neighInp_CPU
                            );
        /* Set the allocation status flag */
        m_locAllocatedInput = true;
      }
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
      /* Local data */
      if ( !m_locAllocatedOutput )
      {
        m_pOwnerOut     = reinterpret_cast<idx_k*>       ( malloc( m_nInternalFacesInp_CPU*            sizeof(idx_k) )         );
        m_pNeighOut     = reinterpret_cast<idx_k*>       ( malloc( m_nInternalFacesInp_CPU*            sizeof(idx_k) )         );
        m_pUpperOut     = reinterpret_cast<LDUoffDiag_k*>( malloc( m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k) )  );
        m_pLowerOut     = reinterpret_cast<LDUoffDiag_k*>( malloc( m_nInternalFacesInp_CPU*            sizeof(LDUoffDiag_k) )  );
        m_pPsiOut       = reinterpret_cast<fld_k*>       ( malloc( m_cellsStrideInp_CPU*m_nCompInp_CPU*sizeof(fld_k) )         );
        m_pApsiOut      = reinterpret_cast<fld_k*>       ( malloc( m_cellsStrideInp_CPU*               sizeof(fld_k) )         );
        /* Set the allocation status flag */
        m_locAllocatedOutput = true;
      }
      /* Exit point */
      return;
    };


    void
    Prepare()
    {
      /* Compute the permutation */
      switch (m_blockConstructionKind)
      {
        case (blockConstructionKind::oneParameter):
        {
          m_permutation->computePermutation( m_nBlks, m_cellSortingStrategy, m_memorySortingStrategy );
          break;
        }
        case (blockConstructionKind::twoParameters):
        {
          m_permutation->computePermutation( m_scale, m_limitScale, m_cellSortingStrategy, m_memorySortingStrategy );
          break;
        }
        default:
        {
          printf( "ERROR: wrong block construction kind\n" );
          abort();
        }
      }
      /* Extract the permuted Input data */
      /* extract permutations*/
      m_permutation->pOwner( m_ownerInp_CPU, m_pOwnerInp );
      m_permutation->pNeigh( m_neighInp_CPU, m_pNeighInp );
      m_permutation->pLower( m_lowerInp_CPU, m_pLowerInp );
      m_permutation->pUpper( m_upperInp_CPU, m_pUpperInp );
      m_permutation->pX    ( m_psiInp_CPU,   m_pPsiInp   );
      m_permutation->pAx   ( m_ApsiInp_CPU,  m_pApsiInp  );
      /* Extract the permuted Input data */
      m_permutation->pOwner( m_ownerOut_CPU, m_pOwnerOut );
      m_permutation->pNeigh( m_neighOut_CPU, m_pNeighOut );
      m_permutation->pLower( m_lowerOut_CPU, m_pLowerOut );
      m_permutation->pUpper( m_upperOut_CPU, m_pUpperOut );
      m_permutation->pX    ( m_psiOut_CPU,   m_pPsiOut   );
      m_permutation->pAx   ( m_ApsiOut_CPU,  m_pApsiOut  );
      // Compute compressed blocks
      m_compressor = new blockCompressor( *m_permutation );
      // Print owner neighbour
      // printf( "Mapping permuted owner/neighbour\n" );
      // printf( "--------------------------------\n" );
      // for ( idx_k i=0; i<m_nInternalFacesInp_CPU; ++i )
      //{
      //    printf( "%8u -> %8lu, %8lu, %8lu\n", i, m_permutation->ELLperm(i), m_pOwnerInp[i], m_pNeighInp[i] );
      //};
      /* Exit point */
      return;
    };


    void
    Dispose()
    {
      m_permutation->Ax( m_pApsiInp, m_ApsiInp_CPU );
      return;
    };


    fld_k
    bandWidth()
    {
      return( computeBandwidth
              (
                m_nCellsInp_CPU,
                m_nInternalFacesInp_CPU,
                m_pOwnerInp,
                m_pNeighInp
              )
            );
    };


    fld_k
    jumpParam()
    {
      return( computeJumpParameter
              (
                m_nCellsInp_CPU,
                m_nInternalFacesInp_CPU,
                m_pOwnerInp
              )
            );
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
      matVec_BlockRowPerm_sELL_CPU
      (
        m_compressor->nBlocks(),
        m_compressor->lo(),
        m_compressor->hi(),
        m_compressor->productKernels(),
        m_pNeighInp,
        m_pUpperInp,
        m_pLowerInp,
        m_pPsiInp,
        m_pApsiInp
      );
#endif

/* Call the CUDA kernel */
#if defined( __TEST_NATIVE_CUDA__ ) && !defined( __DRY_RUN__ )
      // Synchronize the device
      cudaDeviceSynchronize();
#endif

/* Call the OPEN_ACC kernel */
#if defined( __TEST_OPEN_ACC__ ) && !defined( __DRY_RUN__ )
      // Synchronize the device
      cudaDeviceSynchronize();
#endif

    /* Call the OPEN_MP kernel */
#if  defined( __TEST_OPEN_MP__ ) && !defined( __DRY_RUN__ )

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

    void
    writeOctave()
    {
      FILE* fid;
      /* Save  the permuted vectors */
      fid = fopen( "OctaveOwner.dat", "w" );
      for ( idx_k i=0; i<m_nInternalFacesInp_CPU; ++i )
        {
          fprintf( fid, "%d\n", m_ownerInp_CPU[i] );
        };
      fclose(fid);

      /* Save  the permuted vectors */
      fid = fopen( "OctaveNeigh.dat", "w" );
      for ( idx_k i=0; i<m_nInternalFacesInp_CPU; ++i )
        {
          fprintf( fid, "%d\n", m_neighInp_CPU[i] );
        };
      fclose(fid);

      /* Save  the permuted vectors */
      fid = fopen( "OctavePermOwner.dat", "w" );
      for ( idx_k i=0; i<m_nInternalFacesInp_CPU; ++i )
        {
          fprintf( fid, "%d\n", m_pOwnerInp[i] );
        };
      fclose(fid);

      /* Save  the permuted vectors */
      fid = fopen( "OctavePermNeigh.dat", "w" );
      for ( idx_k i=0; i<m_nInternalFacesInp_CPU; ++i )
        {
          fprintf( fid, "%d\n", m_pNeighInp[i] );
        };
      fclose(fid);


      /* Save  the permuted vectors */
      fid = fopen( "OctaveLower.dat", "w" );
      for ( idx_k i=0; i<m_nInternalFacesInp_CPU; ++i )
        {
          fprintf( fid, "%lf\n", m_lowerInp_CPU[i] );
        };
      fclose(fid);

      /* Save  the permuted vectors */
      fid = fopen( "OctavePlower.dat", "w" );
      for ( idx_k i=0; i<m_nInternalFacesInp_CPU; ++i )
        {
          fprintf( fid, "%lf\n", m_pLowerInp[i] );
        };
      fclose(fid);

      /* Save  the permuted vectors */
      fid = fopen( "OctaveUpper.dat", "w" );
      for ( idx_k i=0; i<m_nInternalFacesInp_CPU; ++i )
        {
          fprintf( fid, "%lf\n", m_upperInp_CPU[i] );
        };
      fclose(fid);

      /* Save  the permuted vectors */
      fid = fopen( "OctavePupper.dat", "w" );
      for ( idx_k i=0; i<m_nInternalFacesInp_CPU; ++i )
        {
          fprintf( fid, "%lf\n", m_pUpperInp[i] );
        };
      fclose(fid);

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
__unitTest__( BlockRowPerm_IO );
#endif
