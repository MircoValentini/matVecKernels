#include "../inc/IO.H"

#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

/**
 * Checks if a folder exists
 * @param foldername path to the folder to check.
 * @return true if the folder exists, false otherwise.
 */
bool
folder_exists
(
  const char* foldername
)
{
    struct stat st;
    stat(foldername, &st);
    return st.st_mode & S_IFDIR;
};




void
printSize
(
  int matSize,
  int matNNZ,
  int nBlks,
  int nWrks
)
{

  // 
  printf( " + matSize :: %d\n", matSize );
  printf( " + matNNZ  :: %d\n", matNNZ );
  printf( " + nBlks   :: %d\n", nBlks );
  printf( " + nWrks   :: %d\n", nWrks );
  // Exit point
  return;

};

void
readSize
(
  const char* dname,
  int*        matSize,
  int*        matNNZ,
  int*        nBlks,
  int*        nWrks,
  int*        nTests,
  int         verbose
)
{
  int err;
  char buf[1024];
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/size.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "r" );
  err = fscanf( fd, "%d\n", matSize );
  err = fscanf( fd, "%d\n", matNNZ );
  err = fscanf( fd, "%d\n", nBlks );
  err = fscanf( fd, "%d\n", nWrks );
  err = fscanf( fd, "%d\n", nTests );
  fclose(fd );
  if ( verbose != 0 ) printSize( *matSize, *matNNZ, *nBlks, *nWrks );
  // Exit point
  return;
};


void
writeSize
(
  const char* dname,
  int         matSize,
  int         matNNZ,
  int         nBlks,
  int         nWrks,
  int         nTests,
  int         verbose
)
{
  int err;
  char buf[1024];
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/size.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "w" );
  err = fprintf( fd, "%d\n", matSize );
  err = fprintf( fd, "%d\n", matNNZ );
  err = fprintf( fd, "%d\n", nBlks );
  err = fprintf( fd, "%d\n", nWrks );
  err = fprintf( fd, "%d\n", nTests );

  fclose(fd );
  // Exit point
  return;
};



void
readInfo
(
  const char* dname,
  int*        algType,
  int*        algPar
)
{
  int err;
  char buf[1024];
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/info.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "r" );
  err = fscanf( fd, "%d\n", algType );
  if ( *algType == 1 )
    {
      err = fscanf( fd, "%d\n", &algPar[0] );
      algPar[1] = 0;
      algPar[2] = 0;
      algPar[3] = 0;
      algPar[4] = 0;
    }
  else if ( *algType == 2 )
    {
      err = fscanf( fd, "%d\n", &algPar[0] );
      err = fscanf( fd, "%d\n", &algPar[1] );
      err = fscanf( fd, "%d\n", &algPar[2] );
      algPar[3] = 0;
      algPar[4] = 0;
    }
  else
    {
      printf( "unknown algorithm type\n" );
      abort();
    };
  fclose(fd );
  // Exit point
  return;
};


void
writeInfo
(
  const char* dname,
  int         algType,
  int*        algPar
)
{
  int err;
  char buf[1024];
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/info.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "w" );
  err = fprintf( fd, "%d\n", algType );
  if ( algType == 1 )
    {
      err = fprintf( fd, "%d\n", algPar[0] );
    }
  else if ( algType == 2 )
    {
      err = fprintf( fd, "%d\n", algPar[0] );
      err = fprintf( fd, "%d\n", algPar[1] );
      err = fprintf( fd, "%d\n", algPar[2] );
    }
  else
    {
      printf( "unknown algorithm type\n" );
      abort();
    };
  fclose(fd );
  // Exit point
  return;
};




void
readBlocks
(
  const char* dname,
  int         nBlks,
  int*        blocks
)
{
  int err;
  int i;
  int tmp0;
  int tmp1;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/blocks.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "r" );
  for ( i=0; i<nBlks; ++i )
  {
    err = fscanf( fd, "%d,   %d\n", &tmp0, &tmp1 );
    blocks[2*i+0] = tmp0 - 1;
    blocks[2*i+1] = tmp1 - 1;
  };
  fclose(fd);
  // Exit point
  return;
};

void
writeBlocks
(
  const char* dname,
  int         nBlks,
  int*        blocks
)
{
  int err;
  int i;
  int tmp0;
  int tmp1;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/blocks.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "w" );
  for ( i=0; i<nBlks; ++i )
  {
      tmp0 = blocks[2*i+0] + 1;
      tmp1 = blocks[2*i+1] + 1;
      err = fprintf( fd, "%d,   %d\n", tmp0, tmp1 );
  };
  fclose(fd);
  // Exit point
  return;
};




void
readBlkMap
(
  const char* dname,
  int         nBlks,
  int*        blkMap
)
{
  int err;
  int i;
  int tmp0;
  int tmp1;
  int tmp2;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/blkMap.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "r" );
  for ( i=0; i<nBlks; ++i )
  {
    err = fscanf( fd, "%d,   %d,   %d\n", &tmp0, &tmp1, &tmp2 );
    blkMap[3*i+0] = tmp0 - 1;
    blkMap[3*i+1] = tmp1 - 1;
    blkMap[3*i+2] = tmp2;
  };
  fclose(fd );
  // Exit point
  return;
};

void
writeBlkMap
(
  const char* dname,
  int         nBlks,
  int*        blkMap
)
{
  int err;
  int i;
  int tmp0;
  int tmp1;
  int tmp2;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/blkMap.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "w" );
  for ( i=0; i<nBlks; ++i )
  {
      tmp0 = blkMap[3*i+0] + 1;
      tmp1 = blkMap[3*i+1] + 1;
      tmp2 = blkMap[3*i+2];
      err = fprintf( fd, "%d,   %d,   %d\n", tmp0, tmp1, tmp2 );
  };
  fclose(fd );
  // Exit point
  return;
};



void
readBlkDsc
(
  const char* dname,
  int         nWrks,
  int*        blkDsc
)
{
  int err;
  int i;
  int tmp0;
  int tmp1;
  int tmp2;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/blkDsc.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "r" );
  for ( i=0; i<nWrks; ++i )
  {
    err = fscanf( fd, "%d,   %d,   %d\n", &tmp0, &tmp1, &tmp2 );
    blkDsc[3*i+0] = tmp0 - 1;
    blkDsc[3*i+1] = tmp1 - 1;
    blkDsc[3*i+2] = tmp2;
  };
  fclose(fd );
  // Exit point
  return;
};

void
writeBlkDsc
(
  const char* dname,
  int         nWrks,
  int*        blkDsc
)
{
  int err;
  int i;
  int tmp0;
  int tmp1;
  int tmp2;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/blkDsc.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "w" );
  for ( i=0; i<nWrks; ++i )
  {
      tmp0 = blkDsc[3*i+0] + 1;
      tmp1 = blkDsc[3*i+1] + 1;
      tmp2 = blkDsc[3*i+2];
      err = fprintf( fd, "%d,   %d,   %d\n", tmp0, tmp1, tmp2 );
  };
  fclose(fd );
  // Exit point
  return;
};





void
readOwner
(
  const char* dname,
  int         matNNZ,
  int*        owner
)
{
  int err;
  int i;
  int tmp;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/owner.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "r" );
  for ( i=0; i<matNNZ; ++i )
  {
    err = fscanf( fd, "%d\n", &tmp );
    owner[i] = tmp - 1;
  };
  fclose(fd );
  // Exit point
  return;
};

void
writeOwner
(
  const char* dname,
  int         matNNZ,
  int*        owner
)
{
  int err;
  int i;
  int tmp;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/owner.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "w" );
  for ( i=0; i<matNNZ; ++i )
  {
      tmp = owner[i] + 1;
      err = fprintf( fd, "%d\n", tmp );

  };
  fclose(fd );
  // Exit point
  return;
};



void
readNeigh
(
  const char* dname,
  int         matNNZ,
  int*        neigh
)
{
  int err;
  int i;
  int tmp;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/neighbour.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "r" );
  for ( i=0; i<matNNZ; ++i )
  {
    err = fscanf( fd, "%d\n", &tmp );
    neigh[i] = tmp - 1;
  };
  fclose(fd );
  // Exit point
  return;
};

void
writeNeigh
(
  const char* dname,
  int         matNNZ,
  int*        neigh
)
{
  int err;
  int i;
  int tmp;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/neighbour.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "w" );
  for ( i=0; i<matNNZ; ++i )
  {
      tmp = neigh[i] + 1;
      err = fprintf( fd, "%d\n", tmp );
  };
  fclose(fd );
  // Exit point
  return;
};






void
readRowStart
(
  const char* dname,
  int         m_matSize,
  int*        rowStart
)
{
  int err;
  int i;
  int tmp;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/rowStart.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "r" );
  for ( i=0; i<m_matSize+1; ++i )
  {
    err = fscanf( fd, "%d\n", &tmp );
    rowStart[i] = tmp-1;
  };
  fclose(fd );
  // Exit point
  return;
};

void
writeRowStart
(
  const char* dname,
  int         m_matSize,
  int*        rowStart
)
{
  int err;
  int i;
  int tmp;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/rowStart.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "w" );
  for ( i=0; i<m_matSize+1; ++i )
  {
      tmp = rowStart[i] + 1;
      err = fprintf( fd, "%d\n", tmp );
  };
  fclose(fd );
  // Exit point
  return;
};




void
readDiag
(
  const char* dname,
  int         matSize,
  double*     diag
)
{
  int err;
  int i;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/diag.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "r" );
  for ( i=0; i<matSize; ++i )
  {
    err = fscanf( fd, "%lf\n", &diag[i] );
  };
  fclose(fd );
  // Exit point
  return;
};

void
writeDiag
(
  const char* dname,
  int         matSize,
  double*     diag
)
{
  int err;
  int i;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/diag.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "w" );
  for ( i=0; i<matSize; ++i )
  {
    err = fprintf( fd, "%lf\n", diag[i] );
  };
  fclose(fd );
  // Exit point
  return;
};





void
readLower
(
  const char* dname,
  int         matNNZ,
  double*     lower
)
{
  int err;
  int i;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/lower.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "r" );
  for ( i=0; i<matNNZ; ++i )
  {
    err = fscanf( fd, "%lf\n", &lower[i] );
  };
  fclose(fd );
  // Exit point
  return;
};

void
writeLower
(
  const char* dname,
  int         matNNZ,
  double*     lower
)
{
  int err;
  int i;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/lower.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "w" );
  for ( i=0; i<matNNZ; ++i )
  {
    err = fprintf( fd, "%lf\n", lower[i] );
  };
  fclose(fd );
  // Exit point
  return;
};



void
readUpper
(
  const char* dname,
  int         matNNZ,
  double*     upper
)
{
  int err;
  int i;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/upper.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "r" );
  for ( i=0; i<matNNZ; ++i )
  {
    err = fscanf( fd, "%lf\n", &upper[i] );
  };
  fclose(fd );
  // Exit point
  return;
};

void
writeUpper
(
  const char* dname,
  int         matNNZ,
  double*     upper
)
{
  int err;
  int i;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/upper.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "w" );
  for ( i=0; i<matNNZ; ++i )
  {
    err = fprintf( fd, "%lf\n", upper[i] );
  };
  fclose(fd );
  // Exit point
  return;
};



void
readX
(
  const char* dname,
  int         matSize,
  int         nTests,
  double*     x
)
{
  int err;
  int i;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/x.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "r" );
  for ( i=0; i<matSize*nTests; ++i )
  {
    err = fscanf( fd, "%lf\n", &x[i] );
  };
  fclose(fd );
  // Exit point
  return;
};

void
writeX
(
  const char* dname,
  int         matSize,
  int         nTests,
  double*     x
)
{
  int err;
  int i;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/x.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "w" );
  for ( i=0; i<matSize*nTests; ++i )
  {
    err = fprintf( fd, "%lf\n", x[i] );
  };
  fclose(fd );
  // Exit point
  return;
};



void
readAx
(
  const char* dname,
  int         matSize,
  int         nTests,
  double*     Ax
)
{
  int err;
  int i;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/Ax.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "r" );
  for ( i=0; i<matSize*nTests; ++i )
  {
    err = fscanf( fd, "%lf\n", &Ax[i] );
  };
  fclose(fd );
  // Exit point
  return;
};

void
writeAx
(
  const char* dname,
  int         matSize,
  int         nTests,
  double*     Ax
)
{
  int err;
  int i;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/Ax.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "w" );
  for ( i=0; i<matSize*nTests; ++i )
  {
    err = fprintf( fd, "%lf\n", Ax[i] );
  };
  fclose(fd );
  // Exit point
  return;
};



void
readPerm
(
  const char* dname,
  int         matSize,
  int*        perm
)
{
  int err;
  int i;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/perm.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "r" );
  for ( i=0; i<matSize; ++i )
  {
    err = fscanf( fd, "%d\n", &perm[i] );
  };
  fclose(fd );
  // Exit point
  return;
};

void
writePerm
(
  const char* dname,
  int         matSize,
  int*        perm
)
{
  int err;
  int i;
  char buf[1024];
  memset( buf, 0, 1024 );
  strcpy( buf, "./" );
  strcat( buf, dname );
  strcat( buf, "/perm.dat" );
  //
  // Read the input file
  FILE* fd = fopen( buf, "w" );
  for ( i=0; i<matSize; ++i )
  {
    err = fprintf( fd, "%d\n", perm[i] );
  };
  fclose(fd );
  // Exit point
  return;
};


bool isDigit( const char* token )
{
  bool check = true;
  for ( int i=0; i<strlen(token); i++ )
    {
      int cnt = 0;
      if ( strncmp( &token[i], "0", 1 ) == 0 ) cnt++;
      if ( strncmp( &token[i], "1", 1 ) == 0 ) cnt++;
      if ( strncmp( &token[i], "2", 1 ) == 0 ) cnt++;
      if ( strncmp( &token[i], "3", 1 ) == 0 ) cnt++;
      if ( strncmp( &token[i], "4", 1 ) == 0 ) cnt++;
      if ( strncmp( &token[i], "5", 1 ) == 0 ) cnt++;
      if ( strncmp( &token[i], "6", 1 ) == 0 ) cnt++;
      if ( strncmp( &token[i], "7", 1 ) == 0 ) cnt++;
      if ( strncmp( &token[i], "8", 1 ) == 0 ) cnt++;
      if ( strncmp( &token[i], "9", 1 ) == 0 ) cnt++;
      if ( cnt == 0 ) check=false;
    }

  // Exit point
  return( check );

};


bool isInteger( const char* token )
{
  bool check = true;
  int state = 0;
  for ( int i=0; i<strlen(token); i++ )
    {
      int cnt = 0;
      switch (state)
      {
	case 0:
	  {
            if ( strncmp( &token[i], " ", 1 ) == 0 ) { cnt++; state=0; };
            if ( strncmp( &token[i], "-", 1 ) == 0 ) { cnt++; state=1; };
            if ( strncmp( &token[i], "0", 1 ) == 0 ) { cnt++; state=1; };
            if ( strncmp( &token[i], "1", 1 ) == 0 ) { cnt++; state=1; };
            if ( strncmp( &token[i], "2", 1 ) == 0 ) { cnt++; state=1; };
            if ( strncmp( &token[i], "3", 1 ) == 0 ) { cnt++; state=1; };
            if ( strncmp( &token[i], "4", 1 ) == 0 ) { cnt++; state=1; };
            if ( strncmp( &token[i], "5", 1 ) == 0 ) { cnt++; state=1; };
            if ( strncmp( &token[i], "6", 1 ) == 0 ) { cnt++; state=1; };
            if ( strncmp( &token[i], "7", 1 ) == 0 ) { cnt++; state=1; };
            if ( strncmp( &token[i], "8", 1 ) == 0 ) { cnt++; state=1; };
            if ( strncmp( &token[i], "9", 1 ) == 0 ) { cnt++; state=1; };
            if ( cnt == 0 ) check=false;
            break;
	  }
	case 1:
	  {
            if ( strncmp( &token[i], "0", 1 ) == 0 ) cnt++;
            if ( strncmp( &token[i], "1", 1 ) == 0 ) cnt++;
            if ( strncmp( &token[i], "2", 1 ) == 0 ) cnt++;
            if ( strncmp( &token[i], "3", 1 ) == 0 ) cnt++;
            if ( strncmp( &token[i], "4", 1 ) == 0 ) cnt++;
            if ( strncmp( &token[i], "5", 1 ) == 0 ) cnt++;
            if ( strncmp( &token[i], "6", 1 ) == 0 ) cnt++;
            if ( strncmp( &token[i], "7", 1 ) == 0 ) cnt++;
            if ( strncmp( &token[i], "8", 1 ) == 0 ) cnt++;
            if ( strncmp( &token[i], "9", 1 ) == 0 ) cnt++;
            if ( cnt == 0 ) check=false;
            break;
	  }
	default:
	  {
	    check = false;
	  }
      }
    }
  if (state==0) check=false;
  // Exit point
  return( check );

};


void readVersion( char* str, FOAMdata* data, bool verbose )
{
  if ( verbose ) printf( "version\n" );
  char* rest = str;
  while ( strncmp( rest, " ", 1 ) == 0 ) rest++;
  char* token = strtok_r(rest, ";", &rest);
  data->m_version = atof( rest );
  return;

};

void readFormat( char* str, FOAMdata* data, bool verbose )
{

  if ( verbose ) printf( "format\n" );
  char* rest = str;
  while ( strncmp( rest, " ", 1 ) == 0 ) rest++;
  char* token = strtok_r(rest, ";", &rest);
  size_t l = strlen( token );
  data->m_format = (char*)malloc( l+1 );
  strncpy( data->m_format, token, l );
  return;

};

void readClass( char* str, FOAMdata* data, bool verbose )
{

  if ( verbose ) printf( "class\n" );
  char* rest = str;
  while ( strncmp( rest, " ", 1 ) == 0 ) rest++;
  char* token = strtok_r(rest, ";", &rest);
  size_t l = strlen( token );
  data->m_class = (char*)malloc( l+1 );
  strncpy( data->m_class, token, l );
  return;

};

void readNote( char* str, FOAMdata* data, bool verbose )
{
  if ( verbose ) printf( "note\n" );

  // Remove special characters
  int l = strlen( str );
  for ( int i=0; i<l; ++i )
    {
      if ( strncmp( &str[i], "\"", 1 ) == 0 ) str[i]=' ';
      if ( strncmp( &str[i], ";",  1 ) == 0 ) str[i]=' ';
    };

  char* rest = str;
  while ( strlen(rest) > 0 )
    {
      // extract
      while ( strncmp( rest, " ", 1 ) == 0 ) rest++;
      char* token = strtok_r(rest, " ", &rest);

      // printf( "%s\n", rest );

      char* value=token;
      char* key = strtok_r( value, ":", &value );
      while ( strncmp( key, " ", 1 ) == 0 ) key++;

      if ( strncmp( key, "nPoints", 7 ) == 0 )
	{
          // printf( "%s = %d\n", key, atoi(value) );
          data->m_nPoints = atoi(value);
	};

      if ( strncmp( key, "nCells", 6 ) == 0 )
	{
	  // printf( "%s = %d\n", key, atoi(value) );
          data->m_nCells = atoi(value);
	};

      if ( strncmp( key, "nFaces", 6 ) == 0 )
	{
	  // printf( "%s = %d\n", key, atoi(value) );
          data->m_nFaces = atoi(value);
	};

      if ( strncmp( key, "nInternalFaces", 14 ) == 0 )
	{
	  // printf( "%s = %d\n", key, atoi(value) );
          data->m_nInternalFaces = atoi(value);
	};

      while ( strncmp( rest, " ", 1 ) == 0 )
	{
	  rest++;
	};

    };

  // Exit point
  return;
};

void readObject( char* str, FOAMdata* data, bool verbose )
{

  if ( verbose ) printf( "object\n" );
  char* rest = str;
  while ( strncmp( rest, " ", 1 ) == 0 ) rest++;
  char* token = strtok_r(rest, ";", &rest);
  size_t l = strlen( token );
  data->m_object = (char*)malloc( l+1 );
  strncpy( data->m_object, token, l );
  return;


};

void readLocation( char* str, FOAMdata* data, bool verbose )
{

  if ( verbose ) printf( "location\n" );
  char* rest = str;
  while ( strncmp( rest, " ", 1 ) == 0 ) rest++;
  if ( strncmp( rest, "\"", 1 ) == 0 ) rest++;
  char* token = strtok_r(rest, "\"", &rest);
  size_t l = strlen( token );
  data->m_location = (char*)malloc( l+1 );
  strncpy( data->m_location, token, l );
  return;

};



int readFile( const char* fname, FOAMdata* data, bool verbose )
{
  /* Open the file for reading */
  char *line_buf = NULL;
  size_t line_buf_size = 0;
  int line_count = 0;
  size_t line_size;
  FILE *fp = fopen(fname, "r");
  if (!fp)
  {
    fprintf(stderr, "Error opening file '%s'\n", fname);
    return( EXIT_FAILURE );
  }

  /* initialize the state of the reader */
  unsigned char state=0;

  /* Get the first line of the file. */
  line_size = getline(&line_buf, &line_buf_size, fp);

  /* Index in the buffer */
  unsigned int idx;

  /* Loop through until we are done with the file. */
  while ( line_size >= 0 && state<6 )
  {
    /* Increment our line count */
    line_count++;

    /* Show the line details */
    if ( verbose ) printf(" --> LOG :: line[%06d]: chars=%06zd, buf size=%06zu, contents: %s\n", line_count, line_size, line_buf_size, line_buf);

    /* gettoken */
    if ( line_size != 1 )
      {
        char* rest = line_buf;
        while ( strncmp( rest, " ", 1 ) == 0 ) rest++;
        char* token = strtok_r(rest, " ", &rest);

        switch (state)
        {
          case 0:
    	    {
              if ( strncmp( token, "FoamFile", 8 ) == 0 ) state = 1;
              break;
    	    }
          case 1:
    	    {
              if ( strncmp( token, "{", 1 ) == 0 ) state = 2;
              break;
    	    }
          case 2:
    	    {
              if ( strncmp( token, "version",  7 ) == 0 ) readVersion ( rest, data, verbose );
              if ( strncmp( token, "format",   6 ) == 0 ) readFormat  ( rest, data, verbose );
              if ( strncmp( token, "class",    5 ) == 0 ) readClass   ( rest, data, verbose );
              if ( strncmp( token, "note",     4 ) == 0 ) readNote    ( rest, data, verbose );
              if ( strncmp( token, "location", 8 ) == 0 ) readObject  ( rest, data, verbose );
              if ( strncmp( token, "object",   6 ) == 0 ) readLocation( rest, data, verbose );
              if ( strncmp( token, "}",        1 ) == 0  ) state = 3;
              break;
    	    }
          case 3:
            {
              if (  isDigit( token ) )
        	{
        	  data->m_dataSize = atoi(token);
        	  data->m_data = (unsigned int*)malloc( data->m_dataSize*sizeof(unsigned int) );
        	  state = 4;
        	};
              break;
            }
          case 4:
    	    {
              if ( strncmp( token, "(", 1 ) == 0 ) state = 5;
              idx = 0;
              break;
     	    }
          case 5:
    	    {
    	  if ( strncmp( token, ")", 1 ) == 0 )
    	    {
    	      idx = 0;
    	      state = 6;
    	    }
    	  else
    	    {

    	      if ( idx < data->m_dataSize )
    		{
    	          data->m_data[idx] = atoi( token );
    	          idx++;
    		}
    	      else
    		{
    		  state = 7;
    		}
    	    }
              break;
    	    }
          case 6:
    	    {
    	      printf( "Fine\n" );
    	      break;
    	    }
          case 7:
            {
              printf( "ERROR: Buffer overflow\n" );
              abort();
              break;
            }
          default:
            {
              printf( "ERROR :: Unknown state\n" );
              abort();
            }
        };
      }
    /* Get the next line */
    line_size = getline(&line_buf, &line_buf_size, fp);

    /* Remove the trailing newline*/
    line_buf[line_size-1] = 0;
  }

  /* Free the allocated line buffer */
  free(line_buf);
  line_buf = NULL;

  /* Close the file now that we are done with it */
  fclose(fp);

  return( EXIT_SUCCESS );
}

