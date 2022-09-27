#include <stdio.h>

typedef unsigned int idx_k;

void
partition
(
  unsigned int  bid,
  unsigned int  sid,
  unsigned int  AVXsize,
  unsigned int  BlockNRows,
  unsigned int  BlockNNZ,
  unsigned int* first,
  unsigned int* lo,
  unsigned int* hi,
  unsigned int* sz,
  unsigned int* nb,
  unsigned int* nnz,
  unsigned int* avx,
  unsigned int* blockId,
  unsigned int* subBlockId,
  unsigned int* cnt
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


int
main()
{

  /*  */
  idx_k subblkid[100];
  idx_k blkid[100];
  idx_k lo[100];
  idx_k hi[100];
  idx_k sz[100];
  idx_k nb[100];
  idx_k nnz[100];
  idx_k avx[100];

  idx_k cnt=0;

  idx_k AVXsize=8;
  idx_k BlockNRows=71;
  idx_k BlockNNZ=3;
  idx_k first=0;
  idx_k bid=0;
  idx_k sid=0;
  partition( bid, sid, AVXsize, BlockNRows, BlockNNZ, &first, lo, hi, sz, nb, nnz, avx, blkid, subblkid, &cnt );
  BlockNRows=15;
  BlockNNZ=0;
  sid++;
  partition( bid, sid, AVXsize, BlockNRows, BlockNNZ, &first, lo, hi, sz, nb, nnz, avx, blkid, subblkid, &cnt );
  BlockNRows=379;
  BlockNNZ=2;
  bid++;
  sid=0;
  partition( bid, sid, AVXsize, BlockNRows, BlockNNZ, &first, lo, hi, sz, nb, nnz, avx, blkid, subblkid, &cnt );
  BlockNRows=71;

  printf( "|------|-------------------------------------------------------|\n" );
  printf( "|   id |  bid |  sid |   lo |   hi |   sz |  blk |  nnz |  avx |\n" );
  printf( "|------|-------------------------------------------------------|\n" );
  idx_k tmp=AVXsize;
  for ( idx_k i=0; i<cnt; ++i )
  {
    printf( "| %4u | %4u | %4u | %4u | %4u | %4u | %4u | %4u | %4u |\n", i, blkid[i], subblkid[i], lo[i], hi[i], sz[i], nb[i], nnz[i], avx[i] );
    tmp /= 2;
  }
  printf( "|------|-------------------------------------------------------|\n" );
  printf( "Number of blocks %d\n", cnt );

  printf( "%d\n", computeNAVX( 8 ) );
  printf( "%d\n", computeNAVX( 4 ) );
  printf( "%d\n", computeNAVX( 2 ) );
  printf( "%d\n", computeNAVX( 1 ) );

  /* Exit point */
  return(0);
};

