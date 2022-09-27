#ifndef __IO_H__
#define __IO_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

bool
isDigit
(
    const char* token
);

bool
isInteger
(
    const char* token
);

struct FOAMdata
{
  float         m_version;
  char*         m_format;
  char*         m_class;
  char*         m_location;
  char*         m_object;
  unsigned int  m_nPoints;
  unsigned int  m_nCells;
  unsigned int  m_nFaces;
  unsigned int  m_nInternalFaces;
  unsigned int  m_dataSize;
  unsigned int* m_data;
};

bool
folder_exists
(
    const char* foldername
);

void
printSize
(
  int matSize,
  int matNNZ,
  int nBlks,
  int nWrks
);

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
);

void
readInfo
(
  const char* dname,
  int*        algType,
  int*        algPar
);

void
readBlocks
(
  const char* dname,
  int         nBlks,
  int*        blocks
);

void
readBlkMap
(
  const char* dname,
  int         nBlks,
  int*        blkMap
);

void
readBlkDsc
(
  const char* dname,
  int         nWrks,
  int*        blkDsc
);

void
readOwner
(
  const char* dname,
  int         matNNZ,
  int*        owner
);

void
readNeigh
(
  const char* dname,
  int         matNNZ,
  int*        neigh
);

void
readRowStart
(
  const char* dname,
  int         matNNZ,
  int*        rowStart
);

void
readDiag
(
  const char* dname,
  int         matSize,
  double*     diag
);

void
readLower
(
  const char* dname,
  int         matNNZ,
  double*     lower
);

void
readUpper
(
  const char* dname,
  int         matNNZ,
  double*     upper
);

void
readX
(
  const char* dname,
  int         matSize,
  int         nTests,
  double*     x
);

void
readAx
(
  const char* dname,
  int         matSize,
  int         nTests,
  double*     Ax
);


void
readPerm
(
  const char* dname,
  int         matSize,
  int*        perm
);







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
);

void
writeInfo
(
  const char* dname,
  int         algType,
  int*        algPar
);

void
writeBlocks
(
  const char* dname,
  int         nBlks,
  int*        blocks
);

void
writeBlkMap
(
  const char* dname,
  int         nBlks,
  int*        blkMap
);

void
writeBlkDsc
(
  const char* dname,
  int         nWrks,
  int*        blkDsc
);

void
writeOwner
(
  const char* dname,
  int         matNNZ,
  int*        owner
);

void
writeNeigh
(
  const char* dname,
  int         matNNZ,
  int*        neigh
);

void
writeRowStart
(
  const char* dname,
  int         matNNZ,
  int*        rowStart
);

void
writeDiag
(
  const char* dname,
  int         matSize,
  double*     diag
);

void
writeLower
(
  const char* dname,
  int         matNNZ,
  double*     lower
);

void
writeUpper
(
  const char* dname,
  int         matNNZ,
  double*     upper
);

void
writeX
(
  const char* dname,
  int         matSize,
  int         matnTests,
  double*     x
);

void
writeAx
(
  const char* dname,
  int         matSize,
  int         matnTests,
  double*     Ax
);

void
writePerm
(
  const char* dname,
  int         matSize,
  int*        perm
);

int
readFile
(
    const char* fname,
    FOAMdata*   data,
    bool        verbose
);

#endif
