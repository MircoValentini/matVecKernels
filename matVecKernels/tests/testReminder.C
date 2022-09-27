#include <stdio.h>


#include <functional>


template<const unsigned int N>
void
test
(
  int x
)
{
  printf( "Test %d\n", N );
  for ( int i=0; i<N; i++ )
  {
    printf( "%d, %d\n", i, x );
  }
  printf( "- - - - - - - - - - - - - - - - - \n" );
  return;
};


typedef void (*fcn)( int x);

// template <> using V0()=test<0>();
static auto test10 = test<10>;
static auto test11 = test<11>;
static auto test12 = test<12>;
static auto test13 = test<13>;
static auto test14 = test<14>;
static auto test15 = test<15>;

static fcn ff[6] = 
{ 
test10,
test11,
test12,
test13,
test14,
test15
};

int
main()
{

  fcn f[6];

  f[0] = test10;
  f[1] = test11;
  f[2] = test12;
  f[3] = test13;
  f[4] = test14;
  f[5] = test15;

  int i;
  int N=16;
  for ( i=0; i+7<N; i=i+8 )
  {
    printf("irow 8 :: %d\n",i+0);
    printf("irow 8 :: %d\n",i+1);
    printf("irow 8 :: %d\n",i+2);
    printf("irow 8 :: %d\n",i+3);
    printf("irow 8 :: %d\n",i+4);
    printf("irow 8 :: %d\n",i+5);
    printf("irow 8 :: %d\n",i+6);
    printf("irow 8 :: %d\n",i+7);
    printf("- - - - - - - - - - - - - - \n");
  }
  for ( ; i+3<N; i=i+4 )
  {
    printf("irow 4 :: %d\n",i+0);
    printf("irow 4 :: %d\n",i+1);
    printf("irow 4 :: %d\n",i+2);
    printf("irow 4 :: %d\n",i+3);
    printf("- - - - - - - - - - - - - - \n");
  }
  for ( ; i+1<N; i=i+2 )
  {
    printf("irow 2 :: %d\n",i+0);
    printf("irow 2 :: %d\n",i+1);
    printf("- - - - - - - - - - - - - - \n");
  }
  for ( ; i+0<N; i=i+1 )
  {
    printf("irow 1 :: %d\n",i+0);
    printf("- - - - - - - - - - - - - - \n");
  }
  printf( "irow final :: %d\n", i );

  ff[0]( 1 );
  ff[1]( 2 );
  ff[2]( 3 );
  ff[3]( 4 );
  ff[4]( 5 );
  ff[5]( 6 );

  fcn tmp=ff[0];
  for ( int i=0; i<6; ++i )
  {
//     tmp( i+1 );
//     tmp=ff[i];
      (ff[i])(i+1);
  }

  return(0);
};
