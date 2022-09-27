/*
 * computeBlkPerm.C
 *
 *  Created on: May 12, 2022
 *      Author: valentini
 */

#ifndef INC_PERMUTATIONS_BLOCKROWPERMCC_H_
#define INC_PERMUTATIONS_BLOCKROWPERMCC_H_

#include <memory.h>
#include <stdlib.h>
#include <math.h>

/**
 * Algoritmo:
 *
 * 1) Compute the line-NNZ of the input,
 * 2) Divide in blocks
 * 3) Sort each block
 * 4) Compute the permutation (from RCM) and permute the lower upper vectors
 */




class blockRowPermCC
{
private:

  struct sortItem
  {
    idx_k* m_NNZ;
    idx_k* m_idx;
  };


  struct sortItemMem
  {
    idx_k* m_owner;
    idx_k* m_neigh;
    idx_k* m_idx;
  };

  static int
  comparatorMemCC(const void *p, const void *q)
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

  static int
  comparatorCC(const void *p, const void *q)
  {
    int check;
    idx_k l = *(((struct sortItem *)p)->m_NNZ);
    idx_k r = *(((struct sortItem *)q)->m_NNZ);
    if ( r > l ) check = -1;
    if ( l > r ) check = +1;
    if ( l == r ) check = 0;
    return( check );
  }

  const idx_k m_nCells;
  const idx_k m_nInternalFaces;
  idx_k* m_owner;
  idx_k* m_neigh;

  idx_k m_nBlocks;
  idx_k m_nSubBlocks;

  sortItem*    m_sortItems;
  sortItemMem* m_sortItemsMem;
  idx_k* m_temporaryArray;

  idx_k* __restrict__ m_lineNNZ;
  idx_k* __restrict__ m_Perm;
  idx_k* __restrict__ m_invPerm;
  idx_k* __restrict__ m_PermMem;
  idx_k* __restrict__ m_invPermMem;
  idx_k* __restrict__ m_blocks;
  idx_k* __restrict__ m_blkDsc;
  idx_k* __restrict__ m_subBlocks;
//  idx_k* __restrict__ m_pOwner;
//  idx_k* __restrict__ m_pNeigh;


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
      }
    // Exit point
    return;

  };

  void
  computeBlockSize()
  {
    idx_k blockSize         = m_nCells/m_nBlocks;
    idx_k blockSizeReminder = m_nCells%m_nBlocks;
    idx_k i;
    idx_k cnt;

    cnt = 0;
    for ( i=0; i<blockSizeReminder; ++i )
      {
        m_blocks[3*i+0] = cnt;
        m_blocks[3*i+1] = cnt+blockSize;
        m_blocks[3*i+2] = blockSize + 1;
        cnt += blockSize+1;
      };

    for ( i=blockSizeReminder; i<m_nBlocks; ++i )
      {
        m_blocks[3*i+0] = cnt;
        m_blocks[3*i+1] = cnt + blockSize - 1;
        m_blocks[3*i+2] = blockSize;
        cnt += blockSize;
      };

    // Exit point
    return;
  };

  void
  computeBlocking()
  {
    // Local variables
    idx_k i;
    idx_k j;
    idx_k B;
    idx_k cnt;
    // Extract
    cnt = 0;
    for ( j=0; j<m_nBlocks; ++j )
      {
        // printf( "Qui arrivo 60\n" );
        B = m_blocks[3*j+2]; // Dimensions del blocco
        // Prepare the data structure to run the sorting algorithm
        for ( i=0; i<B; ++i )
          {
            idx_k idx = m_blocks[3*j+0]+i;
            m_temporaryArray[i] = idx;
            m_sortItems[i].m_NNZ = &m_lineNNZ[idx];
            m_sortItems[i].m_idx = &m_temporaryArray[i];
          };
        // Sort data
        qsort( (void*)m_sortItems, B, sizeof(sortItem), &comparatorCC );
        // Fill the permutation vector
        for ( i=0; i<B; ++i )
          {
            m_Perm[m_blocks[3*j+0]+i] = *(m_sortItems[i].m_idx);
            m_invPerm[*(m_sortItems[i].m_idx)] = m_blocks[3*j+0]+i;
          };
        // Sub block descriptor
        m_blkDsc[j*3+0] = cnt;
        m_subBlocks[4*cnt+0] = 0;
        idx_k NNZprev=*(m_sortItems[0].m_NNZ);
        for ( i=1; i<B; i++ )
          {
            if ( NNZprev != *(m_sortItems[i].m_NNZ) )
              {
                m_subBlocks[4*cnt+1] = i-1;
                m_subBlocks[4*cnt+2] = m_subBlocks[4*cnt+1] - m_subBlocks[4*cnt+0] + 1;
                m_subBlocks[4*cnt+3] = *(m_sortItems[i-1].m_NNZ);
                cnt++;
                m_subBlocks[4*cnt+0] = i;
              };
              NNZprev = *(m_sortItems[i].m_NNZ);
          };
        m_subBlocks[4*cnt+1] = B-1;
        m_subBlocks[4*cnt+2] = m_subBlocks[4*cnt+1] - m_subBlocks[4*cnt+0] + 1;
        m_subBlocks[4*cnt+3] = *(m_sortItems[B-1].m_NNZ);
        m_blkDsc[j*3+1] = cnt;
        m_blkDsc[j*3+2] = m_blkDsc[j*3+1] - m_blkDsc[j*3+0]  +1;
        cnt++;
    };
    m_nSubBlocks = cnt;
    // Exit point
    return;
  };

  void
  computeMemoryPermutation()
  {
    idx_k i;
    // Prepare for sorting
    for ( i=0; i<m_nInternalFaces; ++i)
      {
        m_temporaryArray[i] = i;
        m_sortItemsMem[i].m_owner = &m_invPerm[(m_owner)[i]];
        m_sortItemsMem[i].m_neigh = &m_invPerm[(m_neigh)[i]];
        m_sortItemsMem[i].m_idx   = &m_temporaryArray[i];
      };
    // Sort the owner
    qsort( static_cast<void*>(m_sortItemsMem), m_nInternalFaces, sizeof(sortItemMem), &comparatorMemCC );
    // Extract the memory permutation
    for ( i=0; i<m_nInternalFaces; ++i)
      {
        m_PermMem[i]               = *(m_sortItemsMem[i].m_idx);
        m_invPermMem[m_PermMem[i]] = i;
      };
    // Exit point
    return;
  };


public:


  blockRowPermCC
  (
    const idx_k  nCells,
    const idx_k  nInternalFaces,
          idx_k* owner,
          idx_k* neigh
  ):
      m_nCells(nCells),
      m_nInternalFaces(nInternalFaces),
      m_owner(owner),
      m_neigh(neigh),
      m_nBlocks(0),
      m_nSubBlocks(0)
    {

      // Allocate memory
      m_lineNNZ        = static_cast<idx_k*>( malloc( 1*m_nCells*sizeof(idx_k) ) );
      m_Perm           = static_cast<idx_k*>( malloc( 1*m_nCells*sizeof(idx_k) ) );
      m_invPerm        = static_cast<idx_k*>( malloc( 1*m_nCells*sizeof(idx_k) ) );
      m_PermMem        = static_cast<idx_k*>( malloc( 1*m_nInternalFaces* sizeof(idx_k) ));
      m_invPermMem     = static_cast<idx_k*>( malloc( 1*m_nInternalFaces* sizeof(idx_k) ));
      m_blocks         = static_cast<idx_k*>( malloc( 3*m_nCells*sizeof(idx_k) ) );
      m_blkDsc         = static_cast<idx_k*>( malloc( 3*m_nCells*sizeof(idx_k) ) );
      m_subBlocks      = static_cast<idx_k*>( malloc( 4*m_nCells*sizeof(idx_k) ) );
      m_temporaryArray = static_cast<idx_k*>( malloc( 1*m_nInternalFaces* sizeof(idx_k) ) );
      m_sortItems      = static_cast<sortItem*>    ( malloc( 1*m_nInternalFaces* sizeof(sortItem) ));
      m_sortItemsMem   = static_cast<sortItemMem*> ( malloc( 1*m_nInternalFaces* sizeof(sortItemMem) ));

      // Exit point
      return;
    };


  ~blockRowPermCC()
  {
    free( m_lineNNZ );
    free( m_Perm );
    free( m_invPerm );
    free( m_PermMem );
    free( m_invPermMem );
    // free( m_pOwner );
    // free( m_pNeigh );
    free( m_blocks );
    free( m_blkDsc );
    free( m_subBlocks );
    free( m_temporaryArray );
    free( m_sortItems );
    free( m_sortItemsMem );
    // Exit point
    return;
  };


  idx_k
  nBlocks() const
  {
    // Exit point
    return( m_nBlocks );
  };

  idx_k
  nSubBlocks() const
  {
    // Exit point
    return( m_nSubBlocks );
  }

  void
  resetMemory()
  {
    for ( idx_k i=0; i<1*m_nCells; ++i ) { m_lineNNZ[i] = 0; };
    for ( idx_k i=0; i<1*m_nCells; ++i ) { m_Perm[i] = 0; };
    for ( idx_k i=0; i<1*m_nCells; ++i ) { m_invPerm[i] = 0; };

    for ( idx_k i=0; i<1*m_nInternalFaces; ++i ) { m_PermMem[i] = 0; };
    for ( idx_k i=0; i<1*m_nInternalFaces; ++i ) { m_invPermMem[i] = 0; };
    // for ( idx_k i=0; i<1*m_nInternalFaces; ++i ) { m_pOwner[i] = 0; };
    // for ( idx_k i=0; i<1*m_nInternalFaces; ++i ) { m_pNeigh[i] = 0; };

    for ( idx_k i=0; i<3*m_nCells; ++i ) { m_blocks[i] = 0; };
    for ( idx_k i=0; i<3*m_nCells; ++i ) { m_blkDsc[i] = 0; };
    for ( idx_k i=0; i<4*m_nCells; ++i ) { m_subBlocks[i] = 0; };

    for ( idx_k i=0; i<1*m_nInternalFaces; ++i ) { m_temporaryArray[i] = 0; };
    for ( idx_k i=0; i<1*m_nInternalFaces; ++i )
      {
        m_sortItems[i].m_NNZ = nullptr;
        m_sortItems[i].m_idx = nullptr;
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

  void
  computePermutation( idx_k nBlocks )
  {
    // Update the number of blocks
    m_nBlocks = nBlocks;
    // Reset memory
    resetMemory();
    // recompute blocks
    computeLineNNZ();
    computeBlockSize();
    computeBlocking();
    computeMemoryPermutation();
    // Exit point
    return;
  };

  void
  print() const
  {
    printf( "Number of blocks: %6d\n", m_nBlocks );
    printf( "Number of sub-blocks: %6d\n", m_nSubBlocks );
    printf( "\n\n\nBLOCKS:\n" );
    for ( idx_k i=0; i<m_nBlocks; ++i )
      {
        printf( "%6d, %6d, %6d, %6d\n", i, m_blkDsc[3*i+0], m_blkDsc[3*i+1], m_blkDsc[3*i+2]  );
      }
    printf( " - - - - - - - - - - - - - - - - - - - - - - - \n\n\n" );
    printf( "\n\n\nSUBBLOCKS:\n" );

    int cnt=0;
    for ( idx_k j=0; j<m_nBlocks; ++j )
      {
        printf( " - -Block: %6d - - - - - - - - - - - - - \n", j );
        for ( idx_k i=m_blkDsc[3*j+0]; i<m_blkDsc[3*j+1]; ++i )
          {
            printf( "%6d, %6d, %6d, %6d, %6d\n", i, m_subBlocks[4*i+0], m_subBlocks[4*i+1], m_subBlocks[4*i+2], m_subBlocks[4*i+3]  );
          };
      };
    printf( " - - - - - - - - - - - - - - - - - - - - - - - \n" );
    printf( "\n\n\n" );
    // Exit point
    return;
  };

  void
  writeOctave( )
  {
/*
  const idx_k m_nCells;
  const idx_k m_nInternalFaces;
  idx_k* m_owner;
  idx_k* m_neigh;

  idx_k m_nBlocks;
  idx_k m_nSubBlocks;

  sortItem* m_sortItems;
  idx_k* m_temporaryArray;

  idx_k* __restrict__ m_lineNNZ;
  idx_k* __restrict__ m_Perm;
  idx_k* __restrict__ m_invPerm;
  idx_k* __restrict__ m_PermMem;
  idx_k* __restrict__ m_invPermMem;
  idx_k* __restrict__ m_blocks;
  idx_k* __restrict__ m_blkDsc;
  idx_k* __restrict__ m_subBlocks;
  idx_k* __restrict__ m_pOwner;
  idx_k* __restrict__ m_pNeigh;
*/


    FILE* fid;

    fid = fopen( "OctavePermSize.dat", "w" );
    fprintf( fid, "%d\n", m_nCells );
    fprintf( fid, "%d\n", m_nInternalFaces );
    fclose(fid);

/*
    fid = fopen( "OctavePermOwner.dat", "w" );
    for ( idx_k i=0; i<m_nInternalFaces; ++i )
      {
        fprintf( fid, "%d\n", m_pOwner[i] );
      };
    fclose(fid);

    fid = fopen( "OctavePermNeigh.dat", "w" );
    for ( idx_k i=0; i<m_nInternalFaces; ++i )
      {
        fprintf( fid, "%d\n", m_pNeigh[i] );
      };
    fclose(fid);

    fid = fopen( "OctaveOwner.dat", "w" );
    for ( idx_k i=0; i<m_nInternalFaces; ++i )
      {
        fprintf( fid, "%d\n", m_owner[i] );
      };
    fclose(fid);

    fid = fopen( "OctaveNeigh.dat", "w" );
    for ( idx_k i=0; i<m_nInternalFaces; ++i )
      {
        fprintf( fid, "%d\n", m_neigh[i] );
      };
    fclose(fid);
*/


    fid = fopen( "OctaveXPerm.dat", "w" );
    for ( idx_k i=0; i<m_nCells; ++i )
      {
        fprintf( fid, "%d\n", m_Perm[i] );
      };
    fclose(fid);

    fid = fopen( "OctaveXInvPerm.dat", "w" );
    for ( idx_k i=0; i<m_nCells; ++i )
      {
        fprintf( fid, "%d\n", m_invPerm[i] );
      };
    fclose(fid);

    fid = fopen( "OctavePermMem.dat", "w" );
    for ( idx_k i=0; i<m_nInternalFaces; ++i )
      {
        fprintf( fid, "%d\n", m_PermMem[i] );
      };
    fclose(fid);

    fid = fopen( "OctaveInvPermMem.dat", "w" );
    for ( idx_k i=0; i<m_nInternalFaces; ++i )
      {
        fprintf( fid, "%d\n", m_invPermMem[i] );
      };
    fclose(fid);

    return;
  };

  void
  pOwner( const idx_k* owner, idx_k* pOwner ) const
  {
    for ( idx_k i=0; i<m_nInternalFaces; ++i )
      {
        pOwner[i] = m_invPerm[(owner)[m_PermMem[i]]];
      };
    /* Exit point*/
    return;
  };

  void
  pNeigh( const idx_k* neigh, idx_k* pNeigh ) const
  {
    for ( idx_k i=0; i<m_nInternalFaces; ++i )
      {
        pNeigh[i] = m_invPerm[(neigh)[m_PermMem[i]]];
      };
    /* Exit point*/
    return;
  };

  idx_k*
  blocks() const
  {
    return( m_blkDsc );
  };

  idx_k*
  subBlocks() const
  {
    return( m_subBlocks);
  };

  void
  pDiag( const LDUdiag_k* diag, LDUdiag_k* pDiag ) const
  {
    for ( idx_k i=0; i<m_nCells; ++i )
      {
        pDiag[i] = diag[m_Perm[i]];
      };
    // Exit point
    return;
  };


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

  void
  pX( const fld_k* x, fld_k* pX ) const
  {
    for ( idx_k i=0; i<m_nCells; ++i )
      {
        pX[i] = x[m_Perm[i]];
      };
    // Exit point
    return;
  };


  void
  pAx( const fld_k* Ax, fld_k* pAx ) const
  {
    for ( idx_k i=0; i<m_nCells; ++i )
      {
        pAx[i] = Ax[m_Perm[i]];
      };
    // Exit point
    return;
  };


  void
  Ax( const fld_k* pAx, fld_k* Ax ) const
  {
    for ( idx_k i=0; i<m_nCells; ++i )
      {
        Ax[i] = pAx[m_invPerm[i]];
      };
    // Exit point
    return;
  };

};


#endif
