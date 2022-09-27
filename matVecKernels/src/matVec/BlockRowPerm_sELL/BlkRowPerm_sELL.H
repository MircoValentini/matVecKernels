/*
 * @file In this file different strategies for the computation of hte BlockRowPerm 
 *       permutation are computed.
 *       Given the owner and neighbour of the original RCM matrix, and some comfiguration parameters
 *       PermCell and PermMem are computed. It is then possible to renumber the matrix and 
 *       test the permformances.
 *
 *  Created on: May 12, 2022
 *      Author: valentini
 */

#ifndef __INC_PERMUTATIONS_BLOCKROWPERMSELL_H__
#define __INC_PERMUTATIONS_BLOCKROWPERMSELL_H__

#include <memory.h>
#include <stdlib.h>
#include <math.h>

#include <vector>


/**
 * @class class used to handle a sub-block in the matrix
 *
 */
class subBlock
{

  private:
    const idx_k m_blockID;
    const idx_k m_id;
    const idx_k m_lo;
    const idx_k m_hi;
    const idx_k m_sz;
    const idx_k m_nnz;

  public:

    subBlock
    (
      idx_k blkID,
      idx_k id,
      idx_k lo,
      idx_k hi,
      idx_k sz,
      idx_k nnz
    ):
      m_blockID(blkID),
      m_id(id),
      m_lo(lo),
      m_hi(hi),
      m_sz(sz),
      m_nnz(nnz)
    {};

    idx_k blockID()  const { return( m_blockID ); };
    idx_k id()       const { return( m_id );      };
    idx_k lo()       const { return( m_lo );      };
    idx_k hi()       const { return( m_hi );      };
    idx_k sz()       const { return( m_sz );      };
    idx_k nnz()      const { return( m_nnz );     };
};



/**
 * @class class used to handle a block in the matrix
 *
 */
class block
{
  private:
    /* Indices used to map the block*/
    const idx_k m_id;
    const idx_k m_lo;
    const idx_k m_hi;
    const idx_k m_sz;

    idx_k m_subBlock_lo;
    idx_k m_subBlock_hi;
    idx_k m_subBlock_sz;


    /* Sub-blocks ordered by nnz */
    std::vector<subBlock> m_subBlocks;

  public:

    block
    (
      idx_k id,
      idx_k lo,
      idx_k hi,
      idx_k sz
    ):
      m_id(id),
      m_lo(lo),
      m_hi(hi),
      m_sz(sz),
      m_subBlocks()
    {};

    ~block()
    {
      m_subBlocks.clear();
      return;
    };

    std::vector<subBlock>& subBlocks(){ return(m_subBlocks); };
    const std::vector<subBlock>& subBlocks() const { return(m_subBlocks); };

    void
    subBlocksStats( idx_k lo, idx_k hi, idx_k sz )
    {
      m_subBlock_lo = lo;
      m_subBlock_hi = hi;
      m_subBlock_sz = sz;
      return;
    };

    idx_k nSubBlocks() const { return( m_subBlocks.size()  ); };
    idx_k id() const { return( m_id ); };
    idx_k lo() const { return( m_lo ); };
    idx_k hi() const { return( m_hi ); };
    idx_k sz() const { return( m_sz ); };

    idx_k subBlocks_lo() const { return( m_subBlock_lo ); };
    idx_k subBlocks_hi() const { return( m_subBlock_hi ); };
    idx_k subBlocks_sz() const { return( m_subBlock_sz ); };

};


/**
 * @brief enumerator for cell sorting algorithms
 *
 */
enum class blockConstructionKind
{
  oneParameter,
  twoParameters,
  last
};


/**
 * @brief enumerator for cell sorting algorithms
 *
 */
enum class cellSortingStrategy
{
  bandwidth,
  antiBandwidth,
  sortingIndex,
  last
};


/**
 * @brief enumerator for memory sorting algorithms
 *
 */
enum class memorySortingStrategy
{
  standard,
  ell,
  last
};




/**
 * @class class used to compute the a reorder of the matrix.
 *        Blocks representes the subdivision of the matrix in blocks and sub-blocks
 *
 */
class blockRowPerm_sELL
{
private:


  /**
   * Private datatypes
   *
   */

  /**
   * @brief structure used to sort the cells
   *
   */
  struct sortItem
  {
    idx_k* m_NNZ;  // Number of nonzero
    idx_k* m_BND;  // Bandwidth 
    idx_k* m_ABND; // antiBandwidth
    idx_k* m_idx;  // index in the original ordering
  };


  /**
   * @brief structure used to sort the memory
   *
   */
  struct sortItemMem
  {
    idx_k* m_owner;
    idx_k* m_neigh;
    idx_k* m_idx;
  };


  /**
   * Private data
   *
   */
  const idx_k m_nCells;
  const idx_k m_nInternalFaces;

  idx_k m_maxBandWidth_val;
  idx_k m_maxBandWidth_idx;

  const idx_k* m_owner;
  const idx_k* m_neigh;

  const idx_k m_AVXsize;

  idx_k m_nBlocks;
  idx_k m_nSubBlocks;

  sortItem*    m_sortItems;
  sortItemMem* m_sortItemsMem;
  idx_k*       m_temporaryArray;

  idx_k* __restrict__   m_lineNNZ;
  idx_k* __restrict__   m_lineBandWidth;
  idx_k* __restrict__   m_lineAntiBandWidth;
  idx_k* __restrict__   m_lineUpperBandWidth;

  idx_k* __restrict__   m_PermCell;
  idx_k* __restrict__   m_invPermCell;
  idx_k* __restrict__   m_PermMem;
  idx_k* __restrict__   m_invPermMem;
  idx_k* __restrict__   m_ELLpermutation;

  std::vector<block>    m_blocks;
  std::vector<block>    m_outBlocks;
  cellSortingStrategy   m_cellSortingStrategy;
  memorySortingStrategy m_memorySortingStrategy;
  blockConstructionKind m_blockConstructionKind;


  /**
   * PRivate functions
   */

  /**
   * @brief comparator used in the sort operations for the memory
   *
   * @param [in] p left  value to be compared
   * @param [in] q right value to be compared
   *
   */
  static int
  comparatorMem_sELL(const void *p, const void *q)
  {
    int check;
    idx_k lo = *(((struct sortItemMem *)p)->m_owner);
    idx_k ro = *(((struct sortItemMem *)q)->m_owner);
    //
    idx_k ln = *(((struct sortItemMem *)p)->m_neigh);
    idx_k rn = *(((struct sortItemMem *)q)->m_neigh);
    if ( ro > lo ) check = -1;
    if ( lo > ro ) check = +1;
    if ( lo == ro )
    {
      if ( rn > ln )  check = -1;
      if ( ln > rn )  check = +1;
      if ( ln == rn ) check = 0;
      // check = 0;
    }
    return( check );
  }


  /**
   * @brief comparator used in the sort operations for the cells
   *
   * @param [in] p left  value to be compared
   * @param [in] q right value to be compared
   *
   */
  static int
  comparatorBND_sELL(const void *p, const void *q)
  {
    int check;
    idx_k l  = *(((struct sortItem *)p)->m_NNZ);
    idx_k r  = *(((struct sortItem *)q)->m_NNZ);
    idx_k bl = *(((struct sortItem *)p)->m_BND);
    idx_k br = *(((struct sortItem *)q)->m_BND);
    if ( r >  l ) check = -1;
    if ( l >  r ) check = +1;
    if ( l == r )
    {
      if ( br >  bl ) check = -1;
      if ( bl >  br ) check = +1;
      if ( bl == br ) check = 0;
      // check = 0;
    }
    return( check );
  }


  /**
   * @brief comparator used in the sort operations for the cells
   *
   * @param [in] p left  value to be compared
   * @param [in] q right value to be compared
   *
   */
  static int
  comparatorABND_sELL(const void *p, const void *q)
  {
    int check;
    idx_k l  = *(((struct sortItem *)p)->m_NNZ);
    idx_k r  = *(((struct sortItem *)q)->m_NNZ);
    idx_k bl = *(((struct sortItem *)p)->m_ABND);
    idx_k br = *(((struct sortItem *)q)->m_ABND);
    if ( r >  l ) check = -1;
    if ( l >  r ) check = +1;
    if ( l == r )
    {
      if ( br >  bl ) check = -1;
      if ( bl >  br ) check = +1;
      if ( bl == br ) check = 0;
      // check = 0;
    }
    return( check );
  }


  /**
   * @brief comparator used in the sort operations for the cells
   *
   * @param [in] p left  value to be compared
   * @param [in] q right value to be compared
   *
   */
  static int
  comparatorIDX_sELL(const void *p, const void *q)
  {
    int check;
    idx_k l  = *(((struct sortItem *)p)->m_NNZ);
    idx_k r  = *(((struct sortItem *)q)->m_NNZ);
    idx_k bl = *(((struct sortItem *)p)->m_idx);
    idx_k br = *(((struct sortItem *)q)->m_idx);
    if ( r >  l ) check = -1;
    if ( l >  r ) check = +1;
    if ( l == r )
    {
      if ( br >  bl ) check = -1;
      if ( bl >  br ) check = +1;
      if ( bl == br ) check = 0;
      // check = 0;
    }
    return( check );
  }


  /**
   * @brief compute the number of nonzero of the upper part of the matrix line by line
   *
   */
  void
  computeLineNNZ()
  {
    idx_k i;
    for ( i=0; i<m_nCells; ++i )
      {
        m_lineNNZ[i] = 0;
      };
    for ( i=0; i<m_nInternalFaces; ++i )
      {
        m_lineNNZ[(m_owner)[i]]++;
      };
    // Exit point
    return;
  };


  /**
   * @brief compute the maximum column index (relative to the diagonal) line by line
   *
   */
  void
  computeLineBandWidth()
  {
    idx_k i;
    for ( i=0; i<m_nCells; ++i )
    {
      m_lineBandWidth[i] = 0;
    };
    for ( i=0; i<m_nInternalFaces; ++i )
    {
      idx_k idx = m_owner[i];
      idx_k col = m_neigh[i] -idx;
      if ( m_lineBandWidth[idx] < col )
      {
        m_lineBandWidth[idx] = col;
      }
    }
    // Exit point
    return;
  };


  /**
   * @brief compute the minimum column index (relative to the diagonal) line by line
   *
   */
  void
  computeLineAntiBandWidth()
  {
    idx_k i;
    for ( i=0; i<m_nCells; ++i )
    {
      m_lineAntiBandWidth[i] = 99999999;
    };
    for ( i=0; i<m_nInternalFaces; ++i )
    {
      idx_k id  = m_owner[i];
      idx_k col = m_neigh[i]-id;
      if ( m_lineAntiBandWidth[id] > col )
      {
        m_lineAntiBandWidth[id] = col;
      }
    };
    for ( i=0; i<m_nCells; ++i )
    {
      if ( m_lineAntiBandWidth[i] == 99999999 )
      {
        m_lineAntiBandWidth[i] = 0;
      }
    };
    // Exit point
    return;
  };


  /**
   * @brief compute the maximum bandWidth of the matrix
   *
   */
  void
  computeMaxBandwidth()
  {
    idx_k i;
    m_maxBandWidth_val = 0;
    m_maxBandWidth_idx = 0;
    for ( i=0; i<m_nCells; ++i )
    {
      if ( m_maxBandWidth_val < m_lineBandWidth[i] )
      {
        m_maxBandWidth_val = m_lineBandWidth[i];
        m_maxBandWidth_idx = i;
      }
    }
    /* Exit point */
    return;
  }


  /**
   * @brief compute the upper bandwidth.
   * Given the bandwidth as a function of the row index i i.e. BDW[i]. The 
   * upper bandwidth is computed from the bandwidh by imposing that 
   * starting from the i-th coordinate of the maximum bendwidth and proceeding 
   * to the index 0 and to the index Nrows the upper bandwidth is monotone
   *
   */
  void
  computeUpperBandwidth()
  {
    /*
     * Local variables
     */
    idx_k max = 0;
    /*
     * Compute the blocking profile
     */
    for ( idx_k j=0; j<m_nCells; ++j )
    {
      m_lineUpperBandWidth[j] = 0;
    };
    for ( int i=m_maxBandWidth_idx; i>=0; i-- )
    {
      max = 0;
      for ( int j=i; j>=0; j-- )
      {
        if (max<m_lineBandWidth[j])
        {
          max=m_lineBandWidth[j];
        }
      }
      m_lineUpperBandWidth[i] = max;
    }
    for ( int i=m_maxBandWidth_idx+1; i<m_nCells; i++ )
    {
      max = 0;
      for ( int j=i; j<m_nCells; j++ )
      {
        if (max<m_lineBandWidth[j])
        {
          max=m_lineBandWidth[j];
        }
      }
      m_lineUpperBandWidth[i] = max;
    }
    /* Exit point */
    return;
  }


  /*
   *
   */
  void
  computeBandwidthBLocking
  (
    double scale,
    double limitScale,
    bool   verbose
  )
  {


#define __MAX__( A, B ) \
(((A)>(B)) ? (A) : (B));

#define __MIN__( A, B ) \
(((A)<(B)) ? (A) : (B));

    int tmp;
    int cnt;
    // double scale=1.5;
    // double limitScale=0.05;
    idx_k limit = ceil( m_maxBandWidth_val*limitScale*scale );

    /*
     * Compute the total number of blocks
     */
    idx_k lower_count = 0;
    tmp = m_maxBandWidth_idx;
    while ( tmp > 0 )
    {
      idx_k blkDim = __MAX__( ceil(m_lineUpperBandWidth[tmp]*scale), limit );
      tmp -= blkDim-1;
      tmp--;
      lower_count++;
    }

    idx_k upper_count = 0;
    tmp = m_maxBandWidth_idx+1;
    while ( tmp < m_nCells )
    {
      idx_k blkDim = __MAX__( ceil(m_lineUpperBandWidth[tmp]*scale), limit );
      tmp += blkDim-1;
      tmp++;
      upper_count++;
    }
    idx_k numberOfBlocks = upper_count + lower_count;

    /*
     * Allocate memory for the blocks
     */
    idx_k* lowerBlocksLo = static_cast<idx_k*>( malloc( lower_count*sizeof(idx_k) ) );
    idx_k* lowerBlocksHi = static_cast<idx_k*>( malloc( lower_count*sizeof(idx_k) ) );
    idx_k* upperBlocksLo = static_cast<idx_k*>( malloc( upper_count*sizeof(idx_k) ) );
    idx_k* upperBlocksHi = static_cast<idx_k*>( malloc( upper_count*sizeof(idx_k) ) );


    /*
     * Construc the blocks boundaries
     */
    tmp = m_maxBandWidth_idx;
    cnt=0;
    while ( tmp > 0 )
    {
      lowerBlocksLo[cnt] = tmp;
      idx_k blkDim = __MAX__( ceil(m_lineUpperBandWidth[tmp]*scale), limit );
      tmp -= blkDim-1;
      lowerBlocksHi[cnt] = __MAX__( tmp, 0 );
      tmp--;
      cnt++;
    };
    tmp = m_maxBandWidth_idx+1;
    cnt=0;
    while ( tmp < m_nCells )
    {
      upperBlocksLo[cnt] = tmp;
      idx_k blkDim = __MAX__( ceil(m_lineUpperBandWidth[tmp]*scale), limit );
      tmp += blkDim-1;
      upperBlocksHi[cnt] = __MIN__( tmp, m_nCells-1 );
      tmp++;
      cnt++;
    };


    /**
     * @brief Construct the blocking
     */
    if ( verbose )
    {
      /*
       * Allocate memory for the blocks
       */
      idx_k* blocksLo = static_cast<idx_k*>( malloc( numberOfBlocks*sizeof(idx_k) ) );
      idx_k* blocksHi = static_cast<idx_k*>( malloc( numberOfBlocks*sizeof(idx_k) ) );
      idx_k* blocksSz = static_cast<idx_k*>( malloc( numberOfBlocks*sizeof(idx_k) ) );


      cnt = 0;
      for ( idx_k i=0; i<lower_count; ++i )
      {
        blocksLo[cnt] = lowerBlocksHi[lower_count-i-1];
        blocksHi[cnt] = lowerBlocksLo[lower_count-i-1];
        blocksSz[cnt] = blocksHi[cnt] - blocksLo[cnt] + 1;
        cnt++;
      };
      for ( idx_k i=0; i<upper_count; ++i )
      {
        blocksLo[cnt] = upperBlocksLo[i];
        blocksHi[cnt] = upperBlocksHi[i];
        blocksSz[cnt] = blocksHi[cnt] - blocksLo[cnt] + 1;
        cnt++;
      };

      /*
       * Print blocking
       */
      FILE* fid = fopen( "BandWidthBLocking.dat", "w" );
      for ( idx_k i=0; i<numberOfBlocks; ++i )
      {
        fprintf( fid, "%10lu, %10lu, %10lu\n", blocksLo[i], blocksHi[i], blocksSz[i] );
      }
      fclose(fid);

      /**
       * Free memory
       */
      free(blocksLo);
      free(blocksHi);
      free(blocksSz);
    }
    else
    {
      cnt = 0;
      for ( idx_k i=0; i<lower_count; ++i )
      {
        idx_k lo = lowerBlocksHi[lower_count-i-1];
        idx_k hi = lowerBlocksLo[lower_count-i-1];
        idx_k sz = hi - lo + 1;
        m_blocks.push_back( block(cnt, lo, hi, sz) );
        cnt++;
      };
      for ( idx_k i=0; i<upper_count; ++i )
      {
        idx_k lo = upperBlocksLo[i];
        idx_k hi = upperBlocksHi[i];
        idx_k sz = hi - lo + 1;
        m_blocks.push_back( block(cnt, lo, hi, sz) );
        cnt++;
      };
      m_nBlocks=m_blocks.size();
    }

    /*
     * Free unused memory
     */
    free(lowerBlocksLo);
    free(lowerBlocksHi);
    free(upperBlocksLo);
    free(upperBlocksHi);

    /* Exit point */
    return;
  };


  /**
   * @brief Function used for debug purposes, it allows to print the bandwidth
   *        of the matrix.
   *
   * // Script for plotting the file generated in octave
   * B = load( "Bandwidth.dat" );
   * plot(B(:,1), B(:,3), '.r');
   * hold on;
   * plot(B(:,1), B(:,2), '.g');
   * plot(B(:,1), B(:,4), '-b', 'linewidth', 2);
   */
  void
  printBandwidth()
  {
    FILE* fid;
    fid = fopen( "sparsityPattern.dat", "w" );
    for ( idx_k i=0; i<m_nInternalFaces; i++ )
    {
      fprintf( fid, "%10lu, %10lu, %10lu, %10lu\n", i, m_owner[i], m_neigh[i], 1 );
    }
    fclose(fid);

    fid = fopen( "Bandwidth.dat", "w" );
    for ( idx_k i=0; i<m_nCells; i++ )
    {
      fprintf( fid, "%10lu, %10lu, %10lu, %10lu\n", i, m_lineBandWidth[i], m_lineAntiBandWidth[i], m_lineUpperBandWidth[i] );
    }
    fclose(fid);
  };


  /**
   * @brief compute the ELL permutation inside the block partitioning. 
   *        The size is assumed to be the width of the AVX unit
   *
   * @param [in]      N number of rows in the sub-block (width of the AVX unit)
   * @param [in]      M number of non-zeroes per row
   * @param [inout] cnt global counter for the position in the permutation vector
   * @param [inout] permutation permutation vector to be filled by the routine
   */
  void
  reshape
  (
    idx_k  N,            // Numero di righe (non faccio nbessun check!!!)
    idx_k  M,            // Numero di nonzero per linea
    idx_k* cnt,          // Contatore da usare anceh come ofset iniziale
    idx_k* permutation   // Vettore di permutazione
  )
  {
    idx_k offset = *cnt;
    /* Permutation vector*/
    for ( int j=0; j<M; ++j )
    {
      for ( int i=0; i<N; ++i )
      {
        // permutation[*cnt] = offset + ((j*N)+i);
        permutation[*cnt] = offset + ((i*M)+j);
        (*cnt)++;
      };
    };
    /* Exit point */
    return;
  };


  /**
   * @brief compute the ELL permutation inside the block partitioning. 
   *        The permutation is performed recursively in order to handle the reminders
   *
   * @param [in]      N number of rows in the sub-block
   * @param [in]      M number of non-zeroes per row
   * @param [in]      S AVX size (in double) i.e. 1,2,4,8...
   * @param [inout] cnt global counter for the position in the permutation vector
   * @param [inout] permutation permutation vector to be filled by the routine
   */
  void
  ELL
  (
    idx_k  N,          // Number of rows
    idx_k  M,          // Number of non zeroes per line
    idx_k  S,          // AVX size (in number of double precision numbers)
    idx_k* cnt,        // counter for the permutation
    idx_k* permutation // permutation vector
  )
  {
    idx_k upperLimit = N/S;
    for ( int i=0; i<upperLimit; ++i )
    {
      reshape( S, M, cnt, permutation );
    }
    if ( S>1 )
    {
      ELL( N-upperLimit*S, M, S/2, cnt, permutation );
    }
    /* Exit point */
    return;
  };


  /**
   * @brief initialize block dimensions
   *
   */
  void
  computeBlockSize
  (
    idx_k nBlocks
  )
  {
    m_nBlocks               = nBlocks;
    idx_k blockSize         = m_nCells/nBlocks;
    idx_k blockSizeReminder = m_nCells%nBlocks;
    idx_k i;
    idx_k cnt;

    cnt = 0;
    for ( i=0; i<blockSizeReminder; ++i )
      {
        idx_k lo = cnt;
        idx_k hi = cnt+blockSize;
        idx_k sz = blockSize + 1;
        cnt += blockSize+1;
        m_blocks.push_back( block(i, lo, hi, sz) );
      };

    for ( i=blockSizeReminder; i<nBlocks; ++i )
      {
        idx_k lo = cnt;
        idx_k hi = cnt + blockSize - 1;
        idx_k sz = blockSize;
        cnt += blockSize;
        m_blocks.push_back( block(i, lo, hi, sz) );
      };

    // Exit point
    return;
  };


  /**
   * @brief compute the sub-blocks
   *
   */
  void
  computeBlocking()
  {
    // Local variables
    idx_k i;
    idx_k j;
    idx_k cnt;
    // Extract
    cnt = 0;
    for ( j=0; j<m_blocks.size(); ++j )
      {
        // Prepare the data structure to run the sorting algorithm
        for ( i=0; i<m_blocks[j].sz(); ++i )
          {
            idx_k idx = m_blocks[j].lo()+i;
            m_temporaryArray[i] = idx;
            m_sortItems[i].m_NNZ  = &m_lineNNZ[idx];
            m_sortItems[i].m_BND  = &m_lineBandWidth[idx];
            m_sortItems[i].m_ABND = &m_lineAntiBandWidth[idx];
            m_sortItems[i].m_idx  = &m_temporaryArray[i];
          };
        /*
         * Sort data by number of nonzero. The switch is used to select the strategy used 
         * when two lines have the sam enumber of non-sero
         * 
         */
        switch (m_cellSortingStrategy)
        {
          case (cellSortingStrategy::bandwidth):
          {
            qsort( (void*)m_sortItems, m_blocks[j].sz(), sizeof(sortItem), &comparatorBND_sELL );
            break;
          }
          case (cellSortingStrategy::antiBandwidth):
          {
            qsort( (void*)m_sortItems, m_blocks[j].sz(), sizeof(sortItem), &comparatorABND_sELL );
            break;
          }
          case (cellSortingStrategy::sortingIndex):
          {
            qsort( (void*)m_sortItems, m_blocks[j].sz(), sizeof(sortItem), &comparatorIDX_sELL );
            break;
          }
          default:
          {
            printf( "ERROR:: wrong sort parameter\n" );
            abort();
          }
        };
        // Fill the permutation vector for the matrix lines
        for ( i=0; i<m_blocks[j].sz(); ++i )
          {
            m_PermCell[m_blocks[j].lo()+i] = *(m_sortItems[i].m_idx);
            m_invPermCell[*(m_sortItems[i].m_idx)] = m_blocks[j].lo()+i;
          };
        // Sub block descriptor. Search the indices where the NNZ change and mark them as 
        // sub-block boundary
        idx_k s_lo = cnt;
        idx_k lo  = 0;
        idx_k hi  = 0;
        idx_k sz  = 0;
        idx_k nnz = 0;
        idx_k NNZprev=*(m_sortItems[0].m_NNZ);
        for ( i=1; i<m_blocks[j].sz(); i++ )
          {
            if ( NNZprev != *(m_sortItems[i].m_NNZ) )
              {
                hi = i-1;
                sz = hi - lo + 1;
                nnz = *(m_sortItems[i-1].m_NNZ);
                m_blocks[j].subBlocks().push_back( subBlock( j, cnt, lo, hi, sz, nnz ) );
                cnt++;
                lo = i;
              };
              NNZprev = *(m_sortItems[i].m_NNZ);
          };
        hi  = m_blocks[j].sz()-1;
        sz  = hi - lo + 1;
        nnz = *(m_sortItems[m_blocks[j].sz()-1].m_NNZ);
        m_blocks[j].subBlocks().push_back( subBlock( j, cnt, lo, hi, sz, nnz ) );
        idx_k s_hi = cnt;
        idx_k s_sz = s_hi - s_lo + 1;
        m_blocks[j].subBlocksStats( s_lo, s_hi, s_sz );
        cnt++;
    };
    m_nSubBlocks = cnt;
    // Exit point
    return;
  };


  /**
   * @brief compute the memory permutation of the matrix
   *
   */
  void
  computeMemoryPermutation()
  {
    // Local variables
    idx_k i;
    idx_k j;
    idx_k cnt=0;
    switch (m_memorySortingStrategy)
    {
      case (memorySortingStrategy::ell):
      {
        // Prepare the ELL permutation vector
        for ( i=0; i<m_blocks.size(); i++ )
        {
          for ( j=0; j<m_blocks[i].subBlocks().size(); j++ )
          {
            ELL( m_blocks[i].subBlocks()[j].sz(), m_blocks[i].subBlocks()[j].nnz(), m_AVXsize, &cnt, m_ELLpermutation );
          }
        }
        // Prepare for sorting
        for ( i=0; i<m_nInternalFaces; ++i)
        {
          m_temporaryArray[i] = i;
          m_sortItemsMem[i].m_owner = &m_invPermCell[(m_owner)[i]];
          m_sortItemsMem[i].m_neigh = &m_invPermCell[(m_neigh)[i]];
          m_sortItemsMem[i].m_idx   = &m_temporaryArray[i];
        };
        // Sort the owner
        qsort( static_cast<void*>(m_sortItemsMem), m_nInternalFaces, sizeof(sortItemMem), &comparatorMem_sELL );
        // Extract the memory permutation
        for ( i=0; i<m_nInternalFaces; ++i)
        {
          m_PermMem[i]               = *(m_sortItemsMem[m_ELLpermutation[i]].m_idx);
          m_invPermMem[m_PermMem[i]] = i;
        };
        break;
      }
      case (memorySortingStrategy::standard):
      {
        // Prepare for sorting
        for ( i=0; i<m_nInternalFaces; ++i)
        {
          m_temporaryArray[i] = i;
          m_sortItemsMem[i].m_owner = &m_invPermCell[(m_owner)[i]];
          m_sortItemsMem[i].m_neigh = &m_invPermCell[(m_neigh)[i]];
          m_sortItemsMem[i].m_idx   = &m_temporaryArray[i];
        };
        // Sort the owner
        qsort( static_cast<void*>(m_sortItemsMem), m_nInternalFaces, sizeof(sortItemMem), &comparatorMem_sELL );
        // Extract the memory permutation
        for ( i=0; i<m_nInternalFaces; ++i)
        {
          m_PermMem[i]               = *(m_sortItemsMem[i].m_idx);
          m_invPermMem[m_PermMem[i]] = i;
        };
        break;
      }
      default:
      {
        printf( "ERROR:: wrong memory sort parameter\n" );
        abort();
      }
    };
    // Exit point
    return;
  };


  /**
   * @brief reset the memory of hte class
   *
   */
  void
  resetMemory()
  {
    for ( idx_k i=0; i<1*m_nCells; ++i ) { m_lineNNZ[i] = 0; };
    for ( idx_k i=0; i<1*m_nCells; ++i ) { m_lineBandWidth[i] = 0; };
    for ( idx_k i=0; i<1*m_nCells; ++i ) { m_lineAntiBandWidth[i] = 0; };
    for ( idx_k i=0; i<1*m_nCells; ++i ) { m_lineUpperBandWidth[i] = 0; };

    for ( idx_k i=0; i<1*m_nCells; ++i ) { m_PermCell[i] = 0; };
    for ( idx_k i=0; i<1*m_nCells; ++i ) { m_invPermCell[i] = 0; };

    for ( idx_k i=0; i<1*m_nInternalFaces; ++i ) { m_PermMem[i] = 0; };
    for ( idx_k i=0; i<1*m_nInternalFaces; ++i ) { m_invPermMem[i] = 0; };
    for ( idx_k i=0; i<1*m_nInternalFaces; ++i ) { m_ELLpermutation[i] = 0; };

    for ( idx_k i=0; i<1*m_nInternalFaces; ++i ) { m_temporaryArray[i] = 0; };
    for ( idx_k i=0; i<1*m_nInternalFaces; ++i )
      {
        m_sortItems[i].m_NNZ  = nullptr;
        m_sortItems[i].m_BND  = nullptr;
        m_sortItems[i].m_ABND = nullptr;
        m_sortItems[i].m_idx  = nullptr;
      };
    for ( idx_k i=0; i<1*m_nInternalFaces; ++i )
      {
        m_sortItemsMem[i].m_owner = nullptr;
        m_sortItemsMem[i].m_neigh = nullptr;
        m_sortItemsMem[i].m_idx   = nullptr;
      };
    // Exit point
    return;
  };

public:

  /**
   * @brief Constructor for the class. This class computes a BlockRowPerm permutation of 
   *        a matrix in which the data in memory are organized as sorted ELL
   * 
   * @param [in] nCells         number of rows of the matrix
   * @param [in] nInternalFaces number of nonzeros in the upper/lower part of the matrix
   * @param [in] AVXsize        maximum AVX size (in double) i.e. 1,2,4,8 for: noAVX, AVX128, AVX256, AVX512
   * @param [in] owner          owner of the matrix (openFOAM format)
   * @param [in] neigh          neigh of the matrix (openFOAM format)
   *
   */
  blockRowPerm_sELL
  (
    const idx_k  nCells,
    const idx_k  nInternalFaces,
    const idx_k  AVXsize,
          idx_k* owner,
          idx_k* neigh
  ):
   m_nCells(nCells)
  ,m_nInternalFaces(nInternalFaces)
  ,m_maxBandWidth_val(0)
  ,m_maxBandWidth_idx(0)
  ,m_owner(owner)
  ,m_neigh(neigh)
  ,m_AVXsize(AVXsize)
  ,m_nBlocks(0)
  ,m_nSubBlocks(0)
  ,m_sortItems(nullptr)
  ,m_sortItemsMem(nullptr)
  ,m_temporaryArray(nullptr)
  ,m_lineNNZ(nullptr)
  ,m_lineBandWidth(nullptr)
  ,m_lineAntiBandWidth(nullptr)
  ,m_lineUpperBandWidth(nullptr)
  ,m_PermCell(nullptr)
  ,m_invPermCell(nullptr)
  ,m_PermMem(nullptr)
  ,m_invPermMem(nullptr)
  ,m_ELLpermutation(nullptr)
  ,m_blocks()
  ,m_outBlocks()
  ,m_cellSortingStrategy(cellSortingStrategy::last)
  ,m_memorySortingStrategy(memorySortingStrategy::last)
  ,m_blockConstructionKind(blockConstructionKind::last)
  {


    // Allocate memory
    m_lineNNZ            = static_cast<idx_k*>( malloc( 1*m_nCells*sizeof(idx_k) ) );
    m_lineBandWidth      = static_cast<idx_k*>( malloc( 1*m_nCells*sizeof(idx_k) ) );
    m_lineAntiBandWidth  = static_cast<idx_k*>( malloc( 1*m_nCells*sizeof(idx_k) ) );
    m_lineUpperBandWidth = static_cast<idx_k*>( malloc( 1*m_nCells*sizeof(idx_k) ) );
    m_PermCell           = static_cast<idx_k*>( malloc( 1*m_nCells*sizeof(idx_k) ) );
    m_invPermCell        = static_cast<idx_k*>( malloc( 1*m_nCells*sizeof(idx_k) ) );
    m_PermMem            = static_cast<idx_k*>( malloc( 1*m_nInternalFaces* sizeof(idx_k) ));
    m_invPermMem         = static_cast<idx_k*>( malloc( 1*m_nInternalFaces* sizeof(idx_k) ));
    m_ELLpermutation     = static_cast<idx_k*>( malloc( 1*m_nInternalFaces* sizeof(idx_k) ));
    m_temporaryArray     = static_cast<idx_k*>( malloc( 1*m_nInternalFaces* sizeof(idx_k) ) );
    m_sortItems          = static_cast<sortItem*>    ( malloc( 1*m_nInternalFaces* sizeof(sortItem) ));
    m_sortItemsMem       = static_cast<sortItemMem*> ( malloc( 1*m_nInternalFaces* sizeof(sortItemMem) ));
    // Initialize local data
    resetMemory();
    computeLineNNZ();
    computeLineBandWidth();
    computeMaxBandwidth();
    computeLineAntiBandWidth();
    computeUpperBandwidth();
    printBandwidth();
    // computeBandwidthBLocking( 2.0, 0.2, true );
    // Exit point
    return;
  };


  /**
   * @brief Destructor for the class
   */
  ~blockRowPerm_sELL()
  {

    /*
     * Free all thememory in the heap
     */
    free( m_lineNNZ );
    free( m_lineBandWidth );
    free( m_lineAntiBandWidth );
    free( m_lineUpperBandWidth );
    free( m_PermCell );
    free( m_invPermCell );
    free( m_PermMem );
    free( m_invPermMem );
    free( m_ELLpermutation );
    free( m_temporaryArray );
    free( m_sortItems );
    free( m_sortItemsMem );

    /*
     * Reset al the pointer tothe heap
     */
    m_lineNNZ            = nullptr;
    m_lineBandWidth      = nullptr;
    m_lineAntiBandWidth  = nullptr;
    m_lineUpperBandWidth = nullptr;
    m_PermCell           = nullptr;
    m_invPermCell        = nullptr;
    m_PermMem            = nullptr;
    m_invPermMem         = nullptr;
    m_ELLpermutation     = nullptr;
    m_temporaryArray     = nullptr;
    m_sortItems          = nullptr;
    m_sortItemsMem       = nullptr;

    // Exit point
    return;
  };



  /**
   * @brief Get the elements of the ELL permutation
   * 
   * @return i-th element of the ELL permutation
   */
  idx_k ELLperm( idx_k i ) const { return( m_ELLpermutation[i] ); };


  /**
   * @brief return the AVX size used to construct the matrix
   * 
   * @return size of thte AVX operations
   */
  idx_k
  AVXsize() const
  {
    // Exit point
    return( m_AVXsize );
  };


  /**
   * @brief return the number of blocks of the matrix
   * 
   * @return number of blocks in the permutation
   */
  idx_k
  nBlocks() const
  {
    // Exit point
    return( m_nBlocks );
  };


  /**
   * @brief return the number of sub-blocks of the matrix
   * 
   * @return number of sub-blocks in the permutation
   */
  idx_k
  nSubBlocks() const
  {
    // Exit point
    return( m_nSubBlocks );
  }


  /**
   * @brief return the sorting strategy used as a secondary criterion for cell reordering
   * 
   * @return sorting strategy used for the cells
   */
  cellSortingStrategy
  getCellSortingStrategy() const { return( m_cellSortingStrategy ); };

  /**
   * @brief return the sorting strategy for the memory organization
   * 
   * @return sorting strategy used for the memory
   */
  memorySortingStrategy
  getMemorySortingStrategy() const { return( m_memorySortingStrategy ); };


  /**
   * @brief return the sorting strategy for the memory organization
   * 
   * @return sorting strategy used for the memory
   */
  blockConstructionKind
  getBlockConstructionKind() const { return( m_blockConstructionKind ); };


  /**
   * @brief return the block hierarchy
   * 
   * @return blocks hierarchy
   */
  std::vector<block>& blocks() { return( m_blocks ); };
  const std::vector<block>& blocks() const { return( m_blocks ); };


  /**
   * @brief return the number of blocks of the matrix
   * 
   * @param [in] nBlocks number of blocks in the permutations
   */
  void
  computePermutation
  (
    idx_k nBlocks
  )
  {

    /* Configuration of the algorithm to be used*/
    m_blockConstructionKind = blockConstructionKind::oneParameter;
    m_cellSortingStrategy   = cellSortingStrategy::bandwidth;
    m_memorySortingStrategy = memorySortingStrategy::ell;

    /* Compute the blocking and the permutations */
    computeBlockSize( nBlocks );
    computeBlocking();
    computeMemoryPermutation();

    // Exit point
    return;
  };




  /**
   * @brief return the number of blocks of the matrix
   * 
   * @param [in] nBlocks number of blocks in the permutations
   * @param [in] cellSortingStrategy   during the computation of the cell sorting, it is necessary to select a strategy
   *                                   for handling the rows with the same number of nonZero; possible strategies are:
   *                                    - bandwidth     : sorting done accordingly with the maximum local (line) bandwidth
   *                                    - antiBandwidth : sorting done accordingly with the minimum local (line) bandwidth
   *                                    - sortingIndex  : done accordingly with the index in the original RCM ordering
   * @param [in] memorySortingStrategy  during the computation of the memory permutation it is necessary to select a strategy
   *                                    for the neighbour numbering:
   *                                    - standard : standard numbering row by row
   *                                    - ell      : ell numbering organized as block of rows
   */
  void
  computePermutation
  (
    idx_k nBlocks,
    cellSortingStrategy    cellSortingStrategy,
    memorySortingStrategy  memorySortingStrategy
  )
  {

    /* Configuration of the algorithm to be used*/
    m_blockConstructionKind = blockConstructionKind::oneParameter;
    m_cellSortingStrategy   = cellSortingStrategy;
    m_memorySortingStrategy = memorySortingStrategy;

    /* Compute the blocking and the permutations */
    computeBlockSize( nBlocks );
    computeBlocking();
    computeMemoryPermutation();

    // Exit point
    return;
  };


  /**
   * @brief return the number of blocks of the matrix
   * 
   * @param [in]      scale dimension of the blocks with respect to the maximal bandwidth
   * @param [in] limitScale dimension of the blocks at the boundaries of the matrix. 
   *                        It is used to set a lower bound to the boundary blocsk in the matrix.
   *                        If this parameter is set to 0, the boundary blocks will be small,
   *                        If this parameter is 1 the boundary blocks will be the same as the center
   * @param [in] cellSortingStrategy   during the computation of the cell sorting, it is necessary to select a strategy
   *                                   for handling the rows with the same number of nonZero; possible strategies are:
   *                                    - bandwidth     : sorting done accordingly with the maximum local (line) bandwidth
   *                                    - antiBandwidth : sorting done accordingly with the minimum local (line) bandwidth
   *                                    - sortingIndex  : done accordingly with the index in the original RCM ordering
   * @param [in] memorySortingStrategy  during the computation of the memory permutation it is necessary to select a strategy
   *                                    for the neighbour numbering:
   *                                    - standard : standard numbering row by row
   *                                    - ell      : ell numbering organized as block of rows
   */
  void
  computePermutation
  (
    double                 scale,
    double                 limitScale
  )
  {

    /* Configuration of the algorithm to be used*/
    m_blockConstructionKind = blockConstructionKind::twoParameters;
    m_cellSortingStrategy   = cellSortingStrategy::bandwidth;
    m_memorySortingStrategy = memorySortingStrategy::ell;

    /* Compute the permutations */
    computeBandwidthBLocking( scale, limitScale, false );
    computeBlocking();
    computeMemoryPermutation();

    // Exit point
    return;
  };


  /**
   * @brief return the number of blocks of the matrix
   * 
   * @param [in]      scale dimension of the blocks with respect to the maximal bandwidth
   * @param [in] limitScale dimension of the blocks at the boundaries of the matrix. 
   *                        It is used to set a lower bound to the boundary blocsk in the matrix.
   *                        If this parameter is set to 0, the boundary blocks will be small,
   *                        If this parameter is 1 the boundary blocks will be the same as the center
   * @param [in] cellSortingStrategy   during the computation of the cell sorting, it is necessary to select a strategy
   *                                   for handling the rows with the same number of nonZero; possible strategies are:
   *                                    - bandwidth     : sorting done accordingly with the maximum local (line) bandwidth
   *                                    - antiBandwidth : sorting done accordingly with the minimum local (line) bandwidth
   *                                    - sortingIndex  : done accordingly with the index in the original RCM ordering
   * @param [in] memorySortingStrategy  during the computation of the memory permutation it is necessary to select a strategy
   *                                    for the neighbour numbering:
   *                                    - standard : standard numbering row by row
   *                                    - ell      : ell numbering organized as block of rows
   */
  void
  computePermutation
  (
    double                 scale,
    double                 limitScale,
    cellSortingStrategy    cellSortingStrategy,
    memorySortingStrategy  memorySortingStrategy
  )
  {

    /* Configuration of the algorithm to be used*/
    m_blockConstructionKind = blockConstructionKind::twoParameters;
    m_cellSortingStrategy   = cellSortingStrategy;
    m_memorySortingStrategy = memorySortingStrategy;

    /* Compute the permutations */
    computeBandwidthBLocking( scale, limitScale, false );
    computeBlocking();
    computeMemoryPermutation();

    // Exit point
    return;
  };


  /**
   * @brief print the block permutation
   */
  void
  print() const
  {
    // Exit point
    return;
  };


  /**
   * @brief write the permutation to a set of files readable from octave
   * 
   */
  void
  writeOctave( )
  {
    // Exit point
    return;
  };


  /**
   * @brief compute the permuted owner given the unpermuted one
   * 
   * @param [in]  owner  original owner of the matrix
   * @param [out] pOwner permuted owner of the new matrix
   * A = EI^T * M * EJ
   * where EI is owner inflated, EJ is neighbour inflated, M is diagonal
   * A_blk = P_c^T * A * P_c = P_c^T EI^T * M * EJ * P_c   (sparsity pattern is changed with the cell permutation P_c)
   * A_blk = P_c^T EI^T * P_m * P_m^T * M * P_m * P_m^T * EJ * P_c (triplets of the matrix are changed in memory with the memory permuatation P_m, sparsity pattern is the same as before)
   * EI_blk^T = P_c^T * EI^T * P_m = P_c^-1 * EI^T * P_m
   */
  void
  pOwner( const idx_k* owner, idx_k* pOwner ) const
  {
    for ( idx_k i=0; i<m_nInternalFaces; ++i )
      {
        pOwner[i] = m_invPermCell[(owner)[m_PermMem[i]]];
      };
    /* Exit point*/
    return;
  };


  /**
   * @brief compute the owner given the permuted one
   * 
   * @param [in]  pOwner permuted owner of the new matrix
   * @param [out] owner  original owner of the matrix
   * A = EI^T * M * EJ
   * where EI is owner inflated, EJ is neighbour inflated, M is diagonal
   * A_blk = P_c^T * A * P_c = P_c^T EI^T * M * EJ * P_c   (sparsity pattern is changed with the cell permutation P_c)
   * A_blk = P_c^T EI^T * P_m * P_m^T * M * P_m * P_m^T * EJ * P_c (triplets of the matrix are changed in memory with the memory permuatation P_m, sparsity pattern is the same as before)
   * EI_blk^T = P_c^T * EI^T * P_m = P_c^-1 * EI^T * P_m
   */
  void
  owner( const idx_k* pOwner, idx_k* owner ) const
  {
    for ( idx_k i=0; i<m_nInternalFaces; ++i )
      {
        owner[i] = m_PermCell[(pOwner)[m_invPermMem[i]]];
      };
    /* Exit point*/
    return;
  };


  /**
   * @brief compute the permuted neighbour given the unpermuted one
   * 
   * @param [in]  neigh  original neighbour of the matrix
   * @param [out] pNeigh permuted neighbour of the new matrix
   * EJ_blk^T = (P_m^T * EJ * P_c)^T = P_c^T * EJ^T * P_m
   */
  void
  pNeigh( const idx_k* neigh, idx_k* pNeigh ) const
  {
    for ( idx_k i=0; i<m_nInternalFaces; ++i )
      {
        pNeigh[i] = m_invPermCell[(neigh)[m_PermMem[i]]];
      };
    /* Exit point*/
    return;
  };


  /**
   * @brief compute the neighbour given the permuted one
   * 
   * @param [in]  pNeigh permuted neighbour of the new matrix
   * @param [out] neigh  original neighbour of the matrix
   * EJ_blk^T = (P_m^T * EJ * P_c)^T = P_c^T * EJ^T * P_m
   */
  void
  neigh( const idx_k* pNeigh, idx_k* neigh ) const
  {
    for ( idx_k i=0; i<m_nInternalFaces; ++i )
      {
        neigh[i] = m_PermCell[(pNeigh)[m_invPermMem[i]]];
      };
    /* Exit point*/
    return;
  };


  /**
   * @brief compute the permuted diagonal vector given the unpermuted one
   * 
   * @param [in]  diag  original diagonal of the matrix
   * @param [out] pDiag permuted diagonal of the new matrix
   */
  void
  pDiag( const LDUdiag_k* diag, LDUdiag_k* pDiag ) const
  {
    for ( idx_k i=0; i<m_nCells; ++i )
      {
        pDiag[i] = diag[m_PermCell[i]];
      };
    // Exit point
    return;
  };


  /**
   * @brief compute the diagonal vector given the permuted one
   * 
   * @param [in]  pDiag permuted diagonal of the new matrix
   * @param [out] diag  original diagonal of the matrix
   */
  void
  diag( const LDUdiag_k* pDiag, LDUdiag_k* diag ) const
  {
    for ( idx_k i=0; i<m_nCells; ++i )
      {
        diag[i] = pDiag[m_invPermCell[i]];
      };
    // Exit point
    return;
  };


  /**
   * @brief compute the permuted lower values vector
   * 
   * @param [in]  lower  original lower values of the matrix
   * @param [out] pLower permuted lower values of the new matrix
   */
  void
  pLower( const LDUoffDiag_k* lower, LDUoffDiag_k* pLower ) const
  {
    for ( idx_k i=0; i<m_nInternalFaces; ++i )
      {
        pLower[i] = lower[m_PermMem[i]];
      };
    // Exit point
    return;
  };


  /**
   * @brief compute the permuted lower values vector given the unpermuted one
   * 
   * @param [in]  lower permuted lower values of the new matrix
   * @param [out] pLower  original lower values of the matrix
   */
  void
  lower( const LDUoffDiag_k* pLower, LDUoffDiag_k* lower ) const
  {
    for ( idx_k i=0; i<m_nInternalFaces; ++i )
      {
        lower[i] = pLower[m_invPermMem[i]];
      };
    // Exit point
    return;
  };


  /**
   * @brief compute the permuted upper values vector given the unpermuted one
   * 
   * @param [in]  upper  original upper values of the matrix
   * @param [out] pUpper permuted upper values of the new matrix
   */
  void
  pUpper( const LDUoffDiag_k* upper, LDUoffDiag_k* pUpper ) const
  {
    for ( idx_k i=0; i<m_nInternalFaces; ++i )
      {
        pUpper[i] = upper[m_PermMem[i]];
      };
    // Exit point
    return;
  };


  /**
   * @brief compute the permuted upper values vector given the permuted one
   * 
   * @param [in]  pUpper permuted upper values of the new matrix
   * @param [out] upper  original upper values of the matrix
   */
  void
  upper( const LDUoffDiag_k* pUpper, LDUoffDiag_k* upper ) const
  {
    for ( idx_k i=0; i<m_nInternalFaces; ++i )
      {
        upper[i] = pUpper[m_invPermMem[i]];
      };
    // Exit point
    return;
  };


  /**
   * @brief compute the permuted x vector
   * 
   * @param [in]  x  original x vector
   * @param [out] pX permuted x vector
   */
  void
  pX( const fld_k* x, fld_k* pX ) const
  {
    for ( idx_k i=0; i<m_nCells; ++i )
      {
        pX[i] = x[m_PermCell[i]];
      };
    // Exit point
    return;
  };



  /**
   * @brief compute the x vector given the permuted one
   * 
   * @param [in]  pX permuted x vector
   * @param [out] x  original x vector
   */
  void
  x( const fld_k* pX, fld_k* x ) const
  {
    for ( idx_k i=0; i<m_nCells; ++i )
      {
        x[i] = pX[m_invPermCell[i]];
      };
    // Exit point
    return;
  };


  /**
   * @brief compute the permuted result vector from the original result vector
   * 
   * @param [in]  Ax  original result vector
   * @param [out] pAX permuted result vector
   */
  void
  pAx( const fld_k* Ax, fld_k* pAx ) const
  {
    for ( idx_k i=0; i<m_nCells; ++i )
      {
        pAx[i] = Ax[m_PermCell[i]];
      };
    // Exit point
    return;
  };


  /**
   * @brief compute the original result vector from the permuted result vector
   * 
   * @param [in]  pAx permuted result vector
   * @param [out] AX  original result vector
   */
  void
  Ax( const fld_k* pAx, fld_k* Ax ) const
  {
    for ( idx_k i=0; i<m_nCells; ++i )
      {
        Ax[i] = pAx[m_invPermCell[i]];
      };
    // Exit point
    return;
  };

};


#endif
