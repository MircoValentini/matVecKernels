#ifndef __BLOCK_COMPRESSOR_SELL_H__
#define __BLOCK_COMPRESSOR_SELL_H__


/**
 * Local include
 */
#include "BlkRowPerm_sELL.H"
#include "BlockRowPerm_sELL_kernels.H"

class blockCompressor
{
  private:

    /* Pointer to the blockng object geenrated by the partitioning*/
    const blockRowPerm_sELL& m_blocks;

    /* Arrays used to fully describe the batrix blocking */
    idx_k* __restrict__ m_subblkid;
    idx_k* __restrict__ m_blkid;
    idx_k* __restrict__ m_lo;
    idx_k* __restrict__ m_hi;
    idx_k* __restrict__ m_sz;
    idx_k* __restrict__ m_nb;
    idx_k* __restrict__ m_nnz;
    idx_k* __restrict__ m_avx;

    fcn_k* __restrict__ m_productKernels;

    idx_k m_nBlocks;
    idx_k m_blockDescriptorSize;
    bool m_allocated;

    void
    partition
    (
      const blockRowPerm_sELL& tmp,
      idx_k  bid,
      idx_k  sid,
      idx_k  AVXsize,
      idx_k  BlockNRows,
      idx_k  BlockNNZ,
      idx_k* first,
      idx_k* cnt
    )
    {
      idx_k normalizedSize = BlockNRows/AVXsize;
      if ( normalizedSize > 0 )
      {
        /* Compute the lo and hi boundaries */
        m_lo[*cnt]  = (*first);
        m_hi[*cnt] = (*first)+normalizedSize*AVXsize-1;
        m_sz[*cnt]  = normalizedSize*AVXsize;
        m_nb[*cnt]  = normalizedSize;
        m_nnz[*cnt] = BlockNNZ;
        m_blkid[*cnt]    = bid;
        m_subblkid[*cnt] = sid;
        switch (AVXsize)
        {
          case (8):
          {
            m_avx[*cnt] = 3;
            break;
          }
          case (4):
          {
            m_avx[*cnt] = 2;
            break;
          }
          case (2):
          { 
            m_avx[*cnt] = 1;
            break;
          }
          case (1):
          {
            m_avx[*cnt] = 0;
            break;
          }
        }
        *first += normalizedSize*AVXsize;
        if ( BlockNNZ > 0 )
        {
          (*cnt)++;
        };
      }
      if ( AVXsize>1 )
      {
        idx_k reminder = BlockNRows-normalizedSize*AVXsize;
        partition( tmp, bid, sid, AVXsize/2, reminder, BlockNNZ, first, cnt );
      };
      /* Exit point */
      return;
    };


    void
    printBlocking()
    {
      printf( "|------------|-------------------------------------------------------------------------------------------------------|\n" );
      printf( "|         id |        bid |        sid |         lo |         hi |         sz |        blk |        nnz |        avx |\n" );
      printf( "|------------|-------------------------------------------------------------------------------------------------------|\n" );
      idx_k tmp=m_blocks.AVXsize();
      for ( idx_k i=0; i<m_nBlocks; ++i )
      {
        printf
        (
          "| %10u | %10u | %10u | %10u | %10u | %10u | %10u | %10u | %10u |\n", 
          i,
          m_blkid[i],
          m_subblkid[i],
          m_lo[i],
          m_hi[i],
          m_sz[i],
          m_nb[i],
          m_nnz[i],
          m_avx[i]
        );
      }
      printf( "|------------|-------------------------------------------------------------------------------------------------------|\n" );
      /* Exit point */
      return;
    };

    idx_k
    computeNAVX
    (
      idx_k nAVX
    )
    {
      idx_k cnt=1;
      idx_k val=1;
      while ( val < nAVX )
      {
        val *= 2;
        cnt++;
      };
      /* Exit point */
      return( cnt );
    };

    void
    allocateCompressor()
    {
      /* Allocate memory */
      m_subblkid       = static_cast<idx_k*>( malloc( m_blockDescriptorSize*sizeof(idx_k) ) );
      m_blkid          = static_cast<idx_k*>( malloc( m_blockDescriptorSize*sizeof(idx_k) ) );
      m_lo             = static_cast<idx_k*>( malloc( m_blockDescriptorSize*sizeof(idx_k) ) );
      m_hi             = static_cast<idx_k*>( malloc( m_blockDescriptorSize*sizeof(idx_k) ) );
      m_sz             = static_cast<idx_k*>( malloc( m_blockDescriptorSize*sizeof(idx_k) ) );
      m_nb             = static_cast<idx_k*>( malloc( m_blockDescriptorSize*sizeof(idx_k) ) );
      m_nnz            = static_cast<idx_k*>( malloc( m_blockDescriptorSize*sizeof(idx_k) ) );
      m_avx            = static_cast<idx_k*>( malloc( m_blockDescriptorSize*sizeof(idx_k) ) );
      m_productKernels = static_cast<fcn_k*>( malloc( m_blockDescriptorSize*sizeof(fcn_k) ) );
      m_allocated = true;

      if ( m_subblkid       == nullptr ) { printf("ERROR:: m_subblkid       not allocated\n"); abort(); };
      if ( m_blkid          == nullptr ) { printf("ERROR:: m_blkid          not allocated\n"); abort(); };
      if ( m_lo             == nullptr ) { printf("ERROR:: m_lo             not allocated\n"); abort(); };
      if ( m_hi             == nullptr ) { printf("ERROR:: m_hi             not allocated\n"); abort(); };
      if ( m_sz             == nullptr ) { printf("ERROR:: m_sz             not allocated\n"); abort(); };
      if ( m_nb             == nullptr ) { printf("ERROR:: m_nb             not allocated\n"); abort(); };
      if ( m_nnz            == nullptr ) { printf("ERROR:: m_nnz            not allocated\n"); abort(); };
      if ( m_avx            == nullptr ) { printf("ERROR:: m_avx            not allocated\n"); abort(); };
      if ( m_productKernels == nullptr ) { printf("ERROR:: m_productKernels not allocated\n"); abort(); };

      /* Exit point */
      return;
    };

    void
    initializeCompressor()
    {
      /* Allocate memory */
      if ( m_allocated )
      {
        for ( idx_k i=0; i<m_blockDescriptorSize; ++i ){ m_subblkid[i] = 0; };
        for ( idx_k i=0; i<m_blockDescriptorSize; ++i ){ m_blkid[i] = 0; };
        for ( idx_k i=0; i<m_blockDescriptorSize; ++i ){ m_lo[i] = 0; };
        for ( idx_k i=0; i<m_blockDescriptorSize; ++i ){ m_hi[i] = 0; };
        for ( idx_k i=0; i<m_blockDescriptorSize; ++i ){ m_sz[i] = 0; };
        for ( idx_k i=0; i<m_blockDescriptorSize; ++i ){ m_nb[i] = 0; };
        for ( idx_k i=0; i<m_blockDescriptorSize; ++i ){ m_nnz[i] = 0; };
        for ( idx_k i=0; i<m_blockDescriptorSize; ++i ){ m_avx[i] = 0; };
        for ( idx_k i=0; i<m_blockDescriptorSize; ++i ){ m_productKernels[i] = nullptr; };
      }
      else
      {
        printf( "ERROR:: l'oggetto non è allocato\n" );
        abort();
      };
      /* Exit point */
      return;
    };

    void
    freeCompressor()
    {
      /* Allocate memory */
      free( m_subblkid );
      free( m_blkid    );
      free( m_lo       );
      free( m_hi       );
      free( m_sz       );
      free( m_nb       );
      free( m_nnz      );
      free( m_avx      );
      free( m_productKernels );
      /* Exit point */
      return;
    };

  public:

    blockCompressor
    (
      const blockRowPerm_sELL& blocks
    ):
      m_blocks(blocks),
      m_subblkid(nullptr),
      m_blkid(nullptr),
      m_lo(nullptr),
      m_hi(nullptr),
      m_sz(nullptr),
      m_nb(nullptr),
      m_nnz(nullptr),
      m_avx(nullptr),
      m_productKernels(nullptr),
      m_nBlocks(0),
      m_blockDescriptorSize(0),
      m_allocated(false)
    {

      /* Upperbound for the number of subBlocks descriptors */
      idx_k nAVX = computeNAVX( blocks.AVXsize() );
      m_blockDescriptorSize=blocks.nSubBlocks()*nAVX;
      m_nBlocks = m_blockDescriptorSize;

      /* Allocate memory for the object*/
      allocateCompressor();
      initializeCompressor();

      /*
       * Construct the subblock mapping:
       * - Split the subblocks accordingly to the AVX size
       * - Remove the subblocks with 0 nnz
       */
      idx_k first=0;
      idx_k cnt=0;
      const auto& blks = blocks.blocks();
      for ( idx_k i=0; i<blks.size(); ++i )
      {
        const auto& block=blks[i];
        for ( idx_k j=0; j<block.nSubBlocks(); ++j )
        {
          const auto& subBlock=(block.subBlocks())[j];
          partition
          (
            blocks,
            block.id(),
            subBlock.id(),
            blocks.AVXsize(),
            subBlock.sz(),
            subBlock.nnz(),
            &first,
            &cnt
          );
        };
      };
      m_nBlocks = cnt;

      /*
       * Associating the kernels to the blocks. 
       * Kernels are parametrized with the number of non-zero
       * and the AVX kind
       */
      switch ( m_blocks.getMemorySortingStrategy() )
      {
        case ( memorySortingStrategy::ell ):
        {
          for ( idx_k i=0; i<m_nBlocks; ++i )
          {
            idx_k id = m_nnz[i]-1;
            switch ( m_avx[i] )
            {
              case (0): // AVX064 -> 2^0 double
              {
                m_productKernels[i] = kernelList[id];
                break;
              }
              case (1): // AVX0128 -> 2^1 double
              {
                m_productKernels[i] = kernelList[id+128];
                break;
              }
              case (2): // AVX256 -> 2^2 double
              {
                m_productKernels[i] = kernelList[id+256];
                break;
              }
              case (3): // AVX512 -> 2^3 double
              {
                m_productKernels[i] = kernelList[id+384];
                break;
              }
              default:
              {
                printf( "ERROR: wrong AVX size" );
                abort();
              }
            }
          }
          break;
        }
        default:
        {
          printf( "ERROR: not implemented\n" );
          abort();
          break;
        }
      }
      // Print the blockng
      // printBlocking();

      /* Exit point */
      return;
    };


    ~blockCompressor()
    {
      if ( m_allocated )
      {
        freeCompressor();
      };
      return;
    };

          idx_k  nBlocks()        const { return( m_nBlocks ); };
    const fcn_k* productKernels() const { return( m_productKernels ); };
    const idx_k* lo()             const { return( m_lo ); };
    const idx_k* hi()             const { return( m_hi ); };
};

#endif
