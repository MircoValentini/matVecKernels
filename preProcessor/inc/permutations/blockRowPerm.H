/*
 * computeBlkPerm.C
 *
 *  Created on: May 12, 2022
 *      Author: valentini
 */

#ifndef INC_PERMUTATIONS_BLOCKROWPERM_H_
#define INC_PERMUTATIONS_BLOCKROWPERM_H_

#include "../inc/IO.H"
#include "../inc/connectivityIO.H"

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

struct sortItem
{
  unsigned int* m_NNZ;
  unsigned int* m_idx;
};

int
comparator(const void *p, const void *q)
{
    int check;
    unsigned int l = *(((struct sortItem *)p)->m_NNZ);
    unsigned int r = *(((struct sortItem *)q)->m_NNZ);
    if ( r > l ) check = -1;
    if ( l > r ) check = +1;
    if ( l == r ) check = 0;
    return( check );
}


class blockRowPerm
{
private:

  const connectivityIO& m_conn;

  unsigned int m_nBlocks;
  unsigned int m_nSubBlocks;

  sortItem* m_sortItems;
  unsigned int* m_temporaryArray;

  unsigned int* __restrict__ m_lineNNZ;
  unsigned int* __restrict__ m_Perm;
  unsigned int* __restrict__ m_invPerm;
  unsigned int* __restrict__ m_PermMem;
  unsigned int* __restrict__ m_invPermMem;
  unsigned int* __restrict__ m_blocks;
  unsigned int* __restrict__ m_blkDsc;
  unsigned int* __restrict__ m_subBlocks;
  unsigned int* __restrict__ m_pOwner;
  unsigned int* __restrict__ m_pNeigh;


  void
  computeLineNNZ()
  {

    unsigned int i;
    for ( i=0; i<m_conn.matSize(); ++i )
      {
	m_lineNNZ[i] = 0;
      };

    for ( i=0; i<m_conn.matNNZ(); ++i )
      {
	m_lineNNZ[(m_conn.owner())[i]]++;
      }
/*
    for ( i=0; i<m_conn.matSize(); i++ )
      {
	printf( "line NNZ: %d, %d\n", i, m_lineNNZ[i] );
      }
    printf( "matSize: %d\n", m_conn.matSize() );
*/
    // Exit point
    return;

  };

  void
  computeBlockSize()
  {
    unsigned int blockSize = m_conn.matSize()/m_nBlocks;
    unsigned int blockSizeReminder = m_conn.matSize()%m_nBlocks;
    unsigned int i;
    unsigned int cnt;

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

/*
    for ( i=0; i<m_nBlocks; i++ )
      {
	printf( "Block size: %d, %d, %d\n", m_blocks[3*i+0], m_blocks[3*i+1], m_blocks[3*i+2] );
      }
    printf( "matSize: %d\n", m_conn.matSize() );
*/
    // Exit point
    return;
  };

  void
  computeBlocking()
  {

    unsigned int i;
    unsigned int j;
    unsigned int B;
    unsigned int cnt;
    // Extract

    cnt = 0;
    for ( j=0; j<m_nBlocks; ++j )
      {
	// printf( "Qui arrivo 60\n" );
        B = m_blocks[3*j+2]; // Dimensions del blocco

        // printf( "Qui arrivo 61\n" );
        // Prepare the data structure to run the sorting algorithm
        for ( i=0; i<B; ++i )
          {
            unsigned int idx = m_blocks[3*j+0]+i;
            m_temporaryArray[i] = idx;
            m_sortItems[i].m_NNZ = &m_lineNNZ[idx];
            m_sortItems[i].m_idx = &m_temporaryArray[i];
          };

        /*
        for ( i=0; i<B; ++i )
          {
            printf( "%d, %d, %d, %d\n", i, B, *(m_sortItems[i].m_NNZ), *(m_sortItems[i].m_idx) );
          }
        */

        // Sort data
        /*
        printf( "Qui arrivo 62\n" );
        */
        qsort( (void*)m_sortItems, B, sizeof(sortItem), &comparator );
        /*
        printf( "Qui arrivo 63\n" );

        for ( i=0; i<B; ++i )
          {
            printf( "%d, %d, %d, %d\n", i, B, *(m_sortItems[i].m_NNZ), *(m_sortItems[i].m_idx) );
          }
        */

        // Fill the permutation vector
        for ( i=0; i<B; ++i )
          {
            m_Perm[m_blocks[3*j+0]+i] = *(m_sortItems[i].m_idx);
            m_invPerm[*(m_sortItems[i].m_idx)] = m_blocks[3*j+0]+i;
          };

        // Sub block descriptor
        // printf( "\n\n\n\n\n\n" );
        m_blkDsc[j*3+0] = cnt;
        m_subBlocks[4*cnt+0] = 0;
        unsigned int NNZprev=*(m_sortItems[0].m_NNZ);
        for ( i=1; i<B; i++ )
          {
            // printf( "%d %d %d %d %d\n", j, cnt, matSize(), i, B );
            if ( NNZprev != *(m_sortItems[i].m_NNZ) )
              {
        	m_subBlocks[4*cnt+1] = i-1;
        	m_subBlocks[4*cnt+2] = m_subBlocks[4*cnt+1] - m_subBlocks[4*cnt+0] + 1;
        	m_subBlocks[4*cnt+3] = *(m_sortItems[i-1].m_NNZ);
                // printf( "SubBlocks: %6d, %6d %6d %6d %6d\n", cnt, m_subBlocks[4*cnt+0], m_subBlocks[4*cnt+1], m_subBlocks[4*cnt+2], m_subBlocks[4*cnt+3] );
        	cnt++;
        	m_subBlocks[4*cnt+0] = i;
              };
              NNZprev = *(m_sortItems[i].m_NNZ);
          };
	m_subBlocks[4*cnt+1] = B-1;
        m_subBlocks[4*cnt+2] = m_subBlocks[4*cnt+1] - m_subBlocks[4*cnt+0] + 1;
        m_subBlocks[4*cnt+3] = *(m_sortItems[B-1].m_NNZ);
        // printf( "SubBlocks: %6d, %6d %6d %6d %6d\n", cnt, m_subBlocks[4*cnt+0], m_subBlocks[4*cnt+1], m_subBlocks[4*cnt+2], m_subBlocks[4*cnt+3] );
        m_blkDsc[j*3+1] = cnt;
        m_blkDsc[j*3+2] = m_blkDsc[j*3+1] - m_blkDsc[j*3+0]  +1;
        cnt++;
        //printf( "Block descriptor: %d %d %d | %d %d %d | %d %d %d \n", cnt, B, m_conn.matSize(),
	//	m_blkDsc[j*3+0], m_blkDsc[j*3+1], m_blkDsc[j*3+2],
	//	m_subBlocks[3*cnt+0], m_subBlocks[3*cnt+1], m_subBlocks[3*cnt+2] );
        // printf( "\n\n=====================================================================================================\n" );

    };

    m_nSubBlocks = cnt-1;
/*
    printf( "Permutation\n" );
    for ( i=0; i<m_conn.matSize(); ++i )
      {
        printf( "%5d  ", m_Perm[i] );
        if ( (i+1)%30 == 0 ) printf( "\n" );
      };
    printf( "\n" );
*/
    for ( i=0; i<m_conn.matNNZ(); ++i )
      {
        m_pOwner[i] = m_invPerm[(m_conn.owner())[i]];
        m_pNeigh[i] = m_invPerm[(m_conn.neigh())[i]];
      };
    // Exit point
    return;
  };

  void
  computeMemoryPermutation()
  {
    unsigned int i;
    // Prepare for sorting
    for ( i=0; i<m_conn.matNNZ(); ++i)
      {
	m_temporaryArray[i] = i;
        m_sortItems[i].m_NNZ = &m_invPerm[(m_conn.owner())[i]];
        m_sortItems[i].m_idx = &m_temporaryArray[i];
      };
    // Sort the owner
    qsort( static_cast<void*>(m_sortItems), m_conn.matNNZ(), sizeof(sortItem), &comparator );
    // Extract the memory permutation
    for ( i=0; i<m_conn.matNNZ(); ++i)
      {
        m_PermMem[i]               = *(m_sortItems[i].m_idx);
        m_invPermMem[m_PermMem[i]] = i;
      };
    // Compute the permutation
    for ( i=0; i<m_conn.matNNZ(); ++i )
      {
        m_pOwner[i] = m_invPerm[(m_conn.owner())[m_PermMem[i]]];
        m_pNeigh[i] = m_invPerm[(m_conn.neigh())[m_PermMem[i]]];
      };
/*
    for ( i=0; i<m_conn.matNNZ(); ++i)
      {
	printf( "%5d ", m_pOwner[i] );
        if ( (i+1)%30 == 0 ) printf( "\n" );
      };
    printf( "\n" );
*/
    // Exit point
    return;
  };


public:


  blockRowPerm( const connectivityIO& conn  ):
      m_nBlocks(0),
      m_conn(conn),
      m_nSubBlocks(0)
    {

      // Allocate memory
      m_lineNNZ        = static_cast<unsigned int*>( malloc( 1*m_conn.matSize()*sizeof(unsigned int) ) );
      m_Perm           = static_cast<unsigned int*>( malloc( 1*m_conn.matSize()*sizeof(unsigned int) ) );
      m_invPerm        = static_cast<unsigned int*>( malloc( 1*m_conn.matSize()*sizeof(unsigned int) ) );
      m_PermMem        = static_cast<unsigned int*>( malloc( 1*m_conn.matNNZ()* sizeof(unsigned int) ) );
      m_invPermMem     = static_cast<unsigned int*>( malloc( 1*m_conn.matNNZ()* sizeof(unsigned int) ) );
      m_pOwner         = static_cast<unsigned int*>( malloc( 1*m_conn.matNNZ()* sizeof(unsigned int) ) );
      m_pNeigh         = static_cast<unsigned int*>( malloc( 1*m_conn.matNNZ()* sizeof(unsigned int) ) );
      m_blocks         = static_cast<unsigned int*>( malloc( 3*m_conn.matSize()*sizeof(unsigned int) ) );
      m_blkDsc         = static_cast<unsigned int*>( malloc( 3*m_conn.matSize()*sizeof(unsigned int) ) );
      m_subBlocks      = static_cast<unsigned int*>( malloc( 4*m_conn.matSize()*sizeof(unsigned int) ) );
      m_temporaryArray = static_cast<unsigned int*>( malloc( 1*m_conn.matNNZ()* sizeof(unsigned int) ) );
      m_sortItems      = static_cast<sortItem*>    ( malloc( 1*m_conn.matNNZ()* sizeof(sortItem)     ) );

      // Exit point
      return;
    };


  ~blockRowPerm()
  {
    free( m_lineNNZ );
    free( m_Perm );
    free( m_invPerm );
    free( m_PermMem );
    free( m_invPermMem );
    free( m_pOwner );
    free( m_pNeigh );
    free( m_blocks );
    free( m_blkDsc );
    free( m_subBlocks );
    free( m_temporaryArray );
    free( m_sortItems );
    // Exit point
    return;
  };


  unsigned int
  nBlocks() const
  {
    // Exit point
    return( m_nBlocks );
  };

  unsigned int
  nSubBlocks() const
  {
    // Exit point
    return( m_nSubBlocks );
  }

  void
  resetMemory()
  {
    for ( int i=0; i<1*m_conn.matSize(); ++i ) { m_lineNNZ[i] = 0; };
    for ( int i=0; i<1*m_conn.matSize(); ++i ) { m_Perm[i] = 0; };
    for ( int i=0; i<1*m_conn.matSize(); ++i ) { m_invPerm[i] = 0; };

    for ( int i=0; i<1*m_conn.matNNZ(); ++i ) { m_PermMem[i] = 0; };
    for ( int i=0; i<1*m_conn.matNNZ(); ++i ) { m_invPermMem[i] = 0; };
    for ( int i=0; i<1*m_conn.matNNZ(); ++i ) { m_pOwner[i] = 0; };
    for ( int i=0; i<1*m_conn.matNNZ(); ++i ) { m_pNeigh[i] = 0; };

    for ( int i=0; i<3*m_conn.matSize(); ++i ) { m_blocks[i] = 0; };
    for ( int i=0; i<3*m_conn.matSize(); ++i ) { m_blkDsc[i] = 0; };
    for ( int i=0; i<4*m_conn.matSize(); ++i ) { m_subBlocks[i] = 0; };

    for ( int i=0; i<1*m_conn.matNNZ(); ++i ) { m_temporaryArray[i] = 0; };
    for ( int i=0; i<1*m_conn.matSize(); ++i )
      {
	m_sortItems[i].m_NNZ = nullptr;
	m_sortItems[i].m_idx = nullptr;
      };
    // Exit point
    return;
  };

  void
  computePermutation( int nBlocks )
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
    for ( int i=0; i<m_nBlocks; ++i )
      {
	printf( "%6d, %6d, %6d, %6d\n", i, m_blkDsc[3*i+0], m_blkDsc[3*i+1], m_blkDsc[3*i+2]  );
      }
    printf( " - - - - - - - - - - - - - - - - - - - - - - - \n\n\n" );
    printf( "\n\n\nSUBBLOCKS:\n" );

    int cnt=0;
    for ( int j=0; j<m_nBlocks; ++j )
      {
        printf( " - -Block: %6d - - - - - - - - - - - - - \n", j );
        for ( int i=m_blkDsc[3*j+0]; i<m_blkDsc[3*j+1]; ++i )
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
    FILE* fid;

    fid = fopen( "OctavePermSize.dat", "w" );
    fprintf( fid, "%d\n", m_conn.matSize() );
    fprintf( fid, "%d\n", m_conn.matNNZ() );
    fclose(fid);

    fid = fopen( "OctavePermOwner.dat", "w" );
    for ( int i=0; i<m_conn.matNNZ(); ++i )
      {
	fprintf( fid, "%d\n", m_pOwner[i] );
      };
    fclose(fid);

    fid = fopen( "OctavePermNeigh.dat", "w" );
    for ( int i=0; i<m_conn.matNNZ(); ++i )
      {
	fprintf( fid, "%d\n", m_pNeigh[i] );
      };
    fclose(fid);


    return;
  };

  unsigned int*
  pOwner() const
  {
    return( m_pOwner );
  };

  unsigned int*
  pNeigh() const
  {
    return( m_pNeigh );
  };

  unsigned int*
  blocks() const
  {
    return( m_blkDsc );
  };

  unsigned int*
  subBlocks() const
  {
    return( m_subBlocks);
  };

  const double*
  diag( const double* diag ) const
  {

    // Exit point
    return( diag );
  };


  const double*
  lower( const double* lower ) const
  {

    // Exit point
    return( lower );
  };

  const double*
  upper( const double* upper ) const
  {

    // Exit point
    return( upper );
  };

  const double*
  x( const double* x) const
  {

    // Exit point
    return( x );
  };

};


#endif
