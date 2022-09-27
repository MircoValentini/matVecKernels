/*
 * options.H
 *
 *  Created on: May 19, 2022
 *      Author: valentini
 */

#ifndef INC_OPTIONS_H_
#define INC_OPTIONS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>



struct options
{
public:
   int  m_loRand;
   int  m_hiRand;
   int  m_nDecimalsRand;
   int  m_RandSeed;
   int  m_nBlocks;
   int  m_nRepetitions;
   bool m_verbose;
   const char* m_owner;
   const char* m_neigh;
   const char* m_inputFile;
   const char* m_inputDir;
   const char* m_outputFile;
   const char* m_outputDir;
   const char* m_outputHDF5;
private:

};



typedef bool (*argHandler)(int*,char**,int,int,options*);
typedef struct init_fnct init_options;

int nOptions();
init_options const* callbacks();

bool readHelp         ( int* iarg, char** argv, int kind, int nNeg, options* opt );
bool readOwnerStdFile ( int* iarg, char** argv, int kind, int nNeg, options* opt );
bool readNeighStdFile ( int* iarg, char** argv, int kind, int nNeg, options* opt );
bool readOwnerRcmFile ( int* iarg, char** argv, int kind, int nNeg, options* opt );
bool readNeighRcmFile ( int* iarg, char** argv, int kind, int nNeg, options* opt );
bool readNBlocks      ( int* iarg, char** argv, int kind, int nNeg, options* opt );
bool readSeed         ( int* iarg, char** argv, int kind, int nNeg, options* opt );
bool readNRepetitions ( int* iarg, char** argv, int kind, int nNeg, options* opt );
bool readRandMin      ( int* iarg, char** argv, int kind, int nNeg, options* opt );
bool readRandMax      ( int* iarg, char** argv, int kind, int nNeg, options* opt );
bool readRandNDecimals( int* iarg, char** argv, int kind, int nNeg, options* opt );
bool readOutputHDF5   ( int* iarg, char** argv, int kind, int nNeg, options* opt );
bool readInputDir     ( int* iarg, char** argv, int kind, int nNeg, options* opt );
bool readOutputDir    ( int* iarg, char** argv, int kind, int nNeg, options* opt );
bool readInputFile    ( int* iarg, char** argv, int kind, int nNeg, options* opt );
bool readOutputFile   ( int* iarg, char** argv, int kind, int nNeg, options* opt );


struct init_fnct
{
    char const* oname;
    int         kind;
    int         nCheck;
    argHandler  callback;
};

#endif /* INC_OPTIONS_H_ */
