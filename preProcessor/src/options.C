/*
 * options.C
 *
 *  Created on: May 19, 2022
 *      Author: valentini
 */
#include "options.H"
#include "IO.H"

static const int nOpts=18;

init_options const optionsCallbacks[] =
    {
	"help",          1, 4,  &readHelp,
	"h",             1, 1,  &readHelp,
	"?",             1, 1,  &readHelp,

	"ownerStd",      2, 8,  &readOwnerStdFile,
	"neighStd",      2, 8,  &readNeighStdFile,
	"ownerRcm",      2, 8,  &readOwnerRcmFile,
	"neighRcm",      2, 8,  &readNeighRcmFile,

	"nBlocks",       1, 7,  &readNBlocks,

	"seed",          1, 4,  &readSeed,

	"nRepetitions",  1, 12, &readNRepetitions,

	"RandMin",       1, 7,  &readRandMin,
	"RandMax",       1, 7,  &readRandMax,
	"RandNDecimals", 1, 13, &readRandNDecimals,

	"inputDir",      1, 8,  &readInputDir,
	"outputDir",     1, 9,  &readOutputDir,
	"outputFile",    1,10,  &readOutputFile,
	"inputFile",     1, 9,  &readInputFile,
	"outputHDF5",    1,10,  &readOutputHDF5
    };


int nOptions()
{
  return(nOpts);
};


init_options const* callbacks()
{
   return( optionsCallbacks );
};

bool
readHelp
( int* iarg, char** argv, int kind, int nNeg, options* opt )
{
  printf( "Sono qui read Help\n" );
  return( true );
};

bool
readOwnerStdFile
( int* iarg, char** argv, int kind, int nNeg, options* opt )
{
  (*iarg)++;
  opt->m_owner = argv[*iarg];
  return( true );
};


bool
readNeighStdFile
( int* iarg, char** argv, int kind, int nNeg, options* opt )
{
  (*iarg)++;
  opt->m_neigh = argv[*iarg];
  return( true );
};

bool
readOwnerRcmFile
( int* iarg, char** argv, int kind, int nNeg, options* opt )
{
  (*iarg)++;
  opt->m_owner = argv[*iarg];
  return( true );
};


bool
readNeighRcmFile
( int* iarg, char** argv, int kind, int nNeg, options* opt )
{
  (*iarg)++;
  opt->m_neigh = argv[*iarg];
  return( true );
};


bool
readNBlocks
( int* iarg, char** argv, int kind, int nNeg, options* opt )
{
  (*iarg)++;
  if ( isDigit( argv[*iarg] ) )
    {
      opt->m_nBlocks = atoi( argv[*iarg] );
      return( true );
    }
  else
    {
      return( false );
    }
};


bool
readSeed
( int* iarg, char** argv, int kind, int nNeg, options* opt )
{
  (*iarg)++;
  if ( isDigit( argv[*iarg] ) )
    {
      opt->m_RandSeed = atoi( argv[*iarg] );
      return( true );
    }
  else
    {
      return( false );
    }
};


bool
readNRepetitions
( int* iarg, char** argv, int kind, int nNeg, options* opt )
{
  (*iarg)++;
  if ( isDigit( argv[*iarg] ) )
    {
      opt->m_nRepetitions = atoi( argv[*iarg] );
      return( true );
    }
  else
    {
      return( false );
    }
};


bool
readRandMin
( int* iarg, char** argv, int kind, int nNeg, options* opt )
{
  (*iarg)++;
  if ( isInteger( argv[*iarg] ) )
    {
      opt->m_loRand = atoi( argv[*iarg] );
      return( true );
    }
  else
    {
      return( false );
    }
};


bool
readRandMax
( int* iarg, char** argv, int kind, int nNeg, options* opt )
{
  (*iarg)++;
  if ( isInteger( argv[*iarg] ) )
    {
      opt->m_hiRand = atoi( argv[*iarg] );
      return( true );
    }
  else
    {
      return( false );
    }
};


bool
readRandNDecimals
( int* iarg, char** argv, int kind, int nNeg, options* opt )
{
  (*iarg)++;
  if ( isInteger( argv[*iarg] ) )
    {
      opt->m_nDecimalsRand = atoi( argv[*iarg] );
      return( true );
    }
  else
    {
      return( false );
    }
};


bool
readOutputFile
( int* iarg, char** argv, int kind, int nNeg, options* opt )
{
  (*iarg)++;
  opt->m_outputFile = argv[*iarg];
  return( true );
};


bool
readInputDir
( int* iarg, char** argv, int kind, int nNeg, options* opt )
{
  (*iarg)++;
  opt->m_inputDir = argv[*iarg];
  return( true );
};


bool
readOutputDir
( int* iarg, char** argv, int kind, int nNeg, options* opt )
{
  (*iarg)++;
  opt->m_outputDir = argv[*iarg];
  return( true );
};


bool readInputFile
( int* iarg, char** argv, int kind, int nNeg, options* opt )
{
  (*iarg)++;
  opt->m_inputFile = argv[*iarg];
  return( true );
};


bool
readOutputHDF5
( int* iarg, char** argv, int kind, int nNeg, options* opt )
{
  (*iarg)++;
  opt->m_outputHDF5 = argv[*iarg];
  return( true );
};





