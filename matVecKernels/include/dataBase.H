/*
 * @file sql.H
 *
 * @brief Definition of the routines to be used to write timing data into the 
 * sqlite database
 *
 *  @date   27/Giu/2022
 *  @author Mirco Valentini
 */


#ifndef __SQL_H__
#define __SQL_H__

/**
 * System include
 */
#include <cstdio>
#include <unistd.h>
#include <string.h>

/**
 * Local include
 */
#include "dataTypes.H"
#include "timing.H"
#include "sqlite3.h"

/**
 * Local defines
 */
#define __str__(a) \
#a

#define __xstr__(a) \
__str__(a)

/**
 * @class datatype used to handle sql database
 */
class sql
{
  private:

    /**
     * Buffer used to generate sql commands
     */
          char  m_buffer[4096];

    /**
     * Buffer used to store errors
     */
          char* m_errmsg;

    /**
     * Date of hte run
     */
    const char* m_date;

    /**
     * Machine used to run the test
     */
    const char* m_machine;

    /**
     * Compiler used to build the test
     */
    const char* m_compiler;

    /**
     * Build flavour
     */
    const char* m_flavour;

    /**
     * Prefix of the test case
     */
    const char* m_prefix;

    /**
     * KErenl name
     */
    const char* m_kernel;

    /**
     * Index of the test
     */
          int   m_index;

    const unitTestBase& m_uT;

    /**
     * Pointer tot he sqlite db
     */
    sqlite3* m_db;


bool
isDigit( char a )
{
  bool tmp=false;
  if ( strncmp( &a, "0",1 ) == 0 ){ tmp = true; };
  if ( strncmp( &a, "1",1 ) == 0 ){ tmp = true; };
  if ( strncmp( &a, "2",1 ) == 0 ){ tmp = true; };
  if ( strncmp( &a, "3",1 ) == 0 ){ tmp = true; };
  if ( strncmp( &a, "4",1 ) == 0 ){ tmp = true; };
  if ( strncmp( &a, "5",1 ) == 0 ){ tmp = true; };
  if ( strncmp( &a, "6",1 ) == 0 ){ tmp = true; };
  if ( strncmp( &a, "7",1 ) == 0 ){ tmp = true; };
  if ( strncmp( &a, "8",1 ) == 0 ){ tmp = true; };
  if ( strncmp( &a, "9",1 ) == 0 ){ tmp = true; };
  return(tmp);
};


void
parse
(
  const char*  prefix,
        char** name,
        int*   RCM,
        int*   nrows,
        int*   pid
)
{

  int len = strlen( prefix );
  *RCM = 0;
  // printf( "Name\n" );
  bool loop = true;
  int i=0;
  while( loop )
  {
    if ( strncmp( &prefix[i], "_",1) == 0 )
    {
      *name = (char*) malloc( i+1 );
      memset( *name, 0, i+1 );
      strncpy( *name, prefix, i );
      loop = false;
      for (  int s=0; s<i+1; s++ )
      {
        if ( strncmp( &(*name)[s], "-", 1 ) == 0 )
        {
          if ( strncmp( &(*name)[s+1], "RCM", 3 ) == 0 )
          {
            *RCM=1;
          }
        }
	if ( *RCM == 1 )
	{
	  (*name)[s] = 0;	
	}
      }
    }
    i++;
    // printf( " - Name  :: %c, %d\n", prefix[i], strncmp( &prefix[i], "_",1) );
    if ( i==len ){ loop = false; };
  };

  // printf( "NRows\n" );
  loop = true;
  int start=i;
  while( loop )
  {
    if ( strncmp( &prefix[i], "_",1) == 0 )
    {
      char* tmp = (char*) malloc( len );
      memset( tmp, 0, len );
      strcpy( tmp, &prefix[start] );
      loop = false;
      for ( int j=0; j<i+1; j++ )
      {
        if ( !isDigit( tmp[j] ) ) { tmp[j] = 0; }
      }
      *nrows = atoi( tmp )*1000;
      free(tmp);
      loop = false;
    };
    // printf( " - NRows :: %c\n", prefix[i] );
    i++;
    if ( i==len ){ loop = false; };
  };

  // printf( "Pid\n" );
  loop = true;
  start=i;
  while( loop )
  {
    if ( strncmp( &prefix[i], "_",1) == 0 || i==len-1 )
    {
      char* tmp = (char*) malloc( len );
      memset( tmp, 0, len );
      strcpy( tmp, &prefix[start] );
      loop = false;
      for ( int j=0; j<i+1; j++ )
      {
        if ( !isDigit( tmp[j] ) ) { tmp[j] = 0; }
      }
      char* tmp1 = tmp;
      while ( tmp1[0] == 0 ) { tmp1++; };
      // printf( "%s\n", tmp1 );
      *pid = atoi( tmp1 );
      free(tmp);
      loop = false;
    }
    // printf( " - Pid :: %c\n", prefix[i] );
    i++;
    if ( i==len ){ loop = false; };
  };

  /* Exit point */
  return;
};


  public:

    /**
     * @brief Constructor for the class
     */
    sql
    (
      unitTestBase& uT
    ):
      m_date(__xstr__(__DATE__))
     ,m_machine(__xstr__(__MACHINE__))
     ,m_compiler(__xstr__(__COMPILER__))
     ,m_flavour(__xstr__(__FLAVOUR__))
     ,m_uT(uT)
    {
      return;
    };


    /**
     * @brief Open the sqlite database
     *
     * @param[in] db_name name of hte database to be opened
     */
    void
    open()
    {
      sqlite3_initialize();
      int rc = 
      sqlite3_open
      (
         m_uT.outputFile(),
         &m_db
      );
      if (rc != SQLITE_OK) {
          fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(m_db));
          sqlite3_close(m_db);
      }
      // Exit point
      return;
    };


    /**
     * @brief Create the table into the db file. If the table already exists
     *        then the error will be ignored
     */
    void
    createTable()
    {
      // Create the command
      memset( m_buffer, 0, 4096 );
      sprintf
      (
        m_buffer, "CREATE TABLE IF NOT EXISTS profiling (date text, machine text, compiler text, flags text, hostname text, prefix text, isRCM int, nRows int, procID int, kernel text, kernelFlavour text, caseIndex int, mpiIndex int, mpiSize int, nRepetitions int, bandWidth double, jumpParam double, warmupElapsedTime double, warmupClockTime double, warmupCudaTime double, raceElapsedTime double, raceClockTime double, raceCudaTime double)"
      );
      // Try to create the table
      int rc = 
      sqlite3_exec
      (
        m_db,
        m_buffer,
        NULL,
        NULL,
        &m_errmsg
      );
      // Error handling
      if (rc != SQLITE_OK )
      {
        fprintf(stderr, "SQL warning: %s\n", m_errmsg);
        sqlite3_free(m_errmsg);
        sqlite3_close(m_db);
        abort();
      };
      // Reset buffer
      memset( m_buffer, 0, 4096 );
      // Exit point
      return;
    };


    /**
     * @brief Create the table into the db file. If the table already exists
     *        then the error will be ignored
     */
    void
    write
    (
      idx_k        mpiIndex,
      idx_k        mpiSize,
      idx_k        nRepetitions,
      double       warmup_executionTime,
      double       warmup_clockTime,
      double       warmup_cudaTime,
      double       race_executionTime,
      double       race_clockTime,
      double       race_cudaTime
    )
    {
      // Extract info from the prefix
      char* pName;
      int nRows;
      int pid;
      int isRCM;
      // Get hostname 
      char hostname[1024];
      memset( hostname, 0, 1024 );
      gethostname(hostname, 1024);
      // Parse prefix
      parse( m_uT.prefix(), &pName, &isRCM, &nRows, &pid );
      // Reset the command buffer
      memset( m_buffer, 0, 4096 );
      double bandWidth = m_uT.bandWidth();
      double jumpParam = m_uT.jumpParam();
      // Crete the command
      sprintf
      (
        m_buffer, "INSERT INTO profiling VALUES (\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%6.6d,%6.6d,%6.6d,\'%s\',\'%s\',%6.6d,%6.6d,%6.6d,%6.6d,%30.20lf,%30.20lf,%30.20lf,%30.20lf,%30.20lf,%30.20lf,%30.20lf,%30.20lf)",
        m_date,
        m_machine,
        m_compiler,
        m_flavour,
        hostname,
        pName,
        isRCM,
        nRows,
        pid,
        m_uT.kernel(),
        m_uT.kernelFlavour(),
        m_uT.index(),
        mpiIndex,
        mpiSize,
        nRepetitions,
        bandWidth,
        jumpParam,
        warmup_executionTime,
        warmup_clockTime,
        warmup_cudaTime,
        race_executionTime,
        race_clockTime,
        race_cudaTime
      );
      // Run the insertion command
      int rc = 
      sqlite3_exec
      (
        m_db,
        m_buffer,
        NULL,
        NULL,
        &m_errmsg
      );
      // Error handling
      if (rc != SQLITE_OK )
      {
        fprintf(stderr, "SQL error: %s\n", m_errmsg);
        sqlite3_free(m_errmsg);
        sqlite3_close(m_db);
      };
      // Reset buffer
      memset( m_buffer, 0, 4096 );
      free( pName );
      // Exit point
      return;
    };


    /**
     * @brief Close the database
     */
    void
    close()
    {
      // Free all the memory allocated inside the sqlite library
      sqlite3_shutdown();
      // Close the database
      int err = sqlite3_close_v2( m_db );
      // Exit point
      return;
    };

};

#endif

