#ifndef __BLOCK_COMPRESSOR_SELL_H__
#define __BLOCK_COMPRESSOR_SELL_H__


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

    idx_k m_blockDescriptorSize;
    bool m_allocated;

    void
    partition
    (
      idx_k  bid,
      idx_k  sid,
      idx_k  AVXsize,
      idx_k  BlockNRows,
      idx_k  BlockNNZ,
      idx_k* first,
      idx_k* lo,
      idx_k* hi,
      idx_k* sz,
      idx_k* nb,
      idx_k* nnz,
      idx_k* avx,
      idx_k* blockId,
      idx_k* subBlockId,
      idx_k* cnt
    )
    {
      idx_k normalizedSize = BlockNRows/AVXsize;
      if ( normalizedSize > 0 )
      {
        /* Compute the lo and hi boundaries */
        lo[*cnt]  = (*first);
        hi[*cnt]  = (*first)+normalizedSize*AVXsize-1;
        sz[*cnt]  = normalizedSize*AVXsize;
        nb[*cnt]  = normalizedSize;
        nnz[*cnt] = BlockNNZ;
        blockId[*cnt]    = bid;
        subBlockId[*cnt] = sid;
        switch (AVXsize)
        {
          case (8):
          {
            avx[*cnt] = 3;
            break;
          }
          case (4):
          {
            avx[*cnt] = 2;
            break;
          }
          case (2):
          { 
            avx[*cnt] = 1;
            break;
          }
          case (1):
          {
            avx[*cnt] = 0;
            break;
          }
        }
        *first = hi[(*cnt)]+1;
        if ( BlockNNZ > 0 )
        {
          (*cnt)++;
        };
      }
      if ( AVXsize>1 )
      {
        idx_k reminder = BlockNRows-normalizedSize*AVXsize;
        partition( bid, sid, AVXsize/2, reminder, BlockNNZ, first, lo, hi, sz, nb, nnz, avx, blockId, subBlockId, cnt );
      };
      /* Exit point */
      return;
    };


    void
    printBlocking()
    {
      printf( "|------|-------------------------------------------------------|\n" );
      printf( "|   id |  bid |  sid |   lo |   hi |   sz |  blk |  nnz |  avx |\n" );
      printf( "|------|-------------------------------------------------------|\n" );
      idx_k tmp=AVXsize;
      for ( idx_k i=0; i<cnt; ++i )
      {
        printf
        (
          "| %4u | %4u | %4u | %4u | %4u | %4u | %4u | %4u | %4u |\n", 
          i,
          blkid[i],
          subblkid[i],
          lo[i],
          hi[i],
          sz[i],
          nb[i],
          nnz[i],
          avx[i]
        );
      }
      printf( "|------|-------------------------------------------------------|\n" );
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
      m_subblkid = static_cast<idx_k*>( malloc( m_blockDescriptorSize*sizeof(idx_k) ) );
      m_blkid    = static_cast<idx_k*>( malloc( m_blockDescriptorSize*sizeof(idx_k) ) );
      m_lo       = static_cast<idx_k*>( malloc( m_blockDescriptorSize*sizeof(idx_k) ) );
      m_hi       = static_cast<idx_k*>( malloc( m_blockDescriptorSize*sizeof(idx_k) ) );
      m_sz       = static_cast<idx_k*>( malloc( m_blockDescriptorSize*sizeof(idx_k) ) );
      m_nb       = static_cast<idx_k*>( malloc( m_blockDescriptorSize*sizeof(idx_k) ) );
      m_nnz      = static_cast<idx_k*>( malloc( m_blockDescriptorSize*sizeof(idx_k) ) );
      m_avx      = static_cast<idx_k*>( malloc( m_blockDescriptorSize*sizeof(idx_k) ) );
      m_productKernels = static_cast<fcn_k*>( malloc( m_blockDescriptorSize*sizeof(fcn_k) ) );
      m_allocated = true;
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
      blockRowPerm_sELL blocks
    ):
      m_blocks(blocks)
      m_subblkid(nullptr),
      m_blkid(nullptr),
      m_lo(nullptr),
      m_hi(nullptr),
      m_sz(nullptr),
      m_nb(nullptr),
      m_nnz(nullptr),
      m_avx(nullptr),
      m_productKernels(nullptr),
      m_blkStart(nullptr),
      m_blkDeltaCol(nullptr),
      m_dNeigh(nullptr),
      m_blockDescriptorSize(0),
      m_allocated(false)
    {

      /* Upperbound for the number of subBlocks descriptors */
      idx_k nAVX = computeNAVX( blocks.AVXsize() );
      idx_k m_blockDescriptorSize=blocks.nSubBlocks()*nAVX;

      /* Allocate memory for the object*/
      allocateCompressor();

      /*
       * Construct the subblock mapping:
       * - Split the subblocks accordingly to the AVX size
       * - Remove the subblocks with 0 nnz
       */
      idx_k first=0;
      idx_k cnt=0;
      for ( const auto& block : blocks.blocks() )
      {
        for ( const auto& subBlock : block.subBlocks() )
        {
          partition
          (
            block.id(),
            subBlock.id(),
            blocks.AVXsize(),
            subBlock.sz(),
            subBlock.nnz(),
            &first,
            m_lo,
            m_hi,
            m_sz,
            m_nb,
            m_nnz,
            m_avx,
            m_blkid,
            m_subblkid,
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
      for ( idx_k i=0; i<m_nBlocks; ++i )
      {
        idx_k id = m_nnz[i];
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
        }
      }

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
