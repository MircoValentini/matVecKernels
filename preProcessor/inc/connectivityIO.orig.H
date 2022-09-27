/*
 * connectivityIO.H
 *
 *  Created on: May 18, 2022
 *      Author: valentini
 */

#ifndef INC_CONNECTIVITYIO_H_
#define INC_CONNECTIVITYIO_H_

#include <hdf5.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../inc/IO.H"


#define __MIN__( A, B ) A<B?A:B

class connectivityIO
{
protected:
  const char* m_ownerFname;
  const char* m_neighFname;
  int m_compressionBlks;
  unsigned int  m_matSize;
  unsigned int  m_matNNZ;

  unsigned int   m_nkRows;
  unsigned int*  m_rowsSignature;

  unsigned int* __restrict__ m_owner;
  unsigned int* __restrict__ m_neigh;
  bool m_allocated;

  void
  computeSignature()
  {
    unsigned int* tmp = (unsigned int*) malloc( m_matSize*sizeof(unsigned int) );

    for ( int i=0; i<m_matSize; ++i )
      {
        tmp[i] = 1;
      };
    for ( int i=0; i<m_matNNZ; ++i )
      {
            tmp[m_neigh[i]] += 1;
            tmp[m_owner[i]] += 1;
      };
    unsigned int big=0;
    for ( int i=0; i<m_matSize; ++i )
      {
        big = (tmp[i]<big) ? big : tmp[i];
      };
    big++;
    unsigned int* rowSize = (unsigned int*)malloc( big*sizeof(unsigned int) );
    for ( int j=0; j<big; ++j )
      {
        rowSize[j] = 0;
      };
    for ( int i=0; i<m_matSize; ++i )
      {
        for ( int j=0; j<big; ++j )
          {
            if ( tmp[i] == j ) (rowSize[j])++;
          };
      };
    int cnt = 0;
    for ( int j=0; j<big; ++j )
      {
        if ( rowSize[j]>0 ) cnt++;
        // printf( "rowSize( %d) = %d\n", j, rowSize[j] );
      };
    m_nkRows=cnt;
    if ( m_nkRows>0 )
      {
        m_rowsSignature = (unsigned int*)malloc( 2*m_nkRows*sizeof(unsigned int) );

        cnt = 0;
        for ( int i=0; i<big; ++i)
          {
            if ( rowSize[i]>0 )
              {
                // printf( " -- rowSize( %d) = %d\n", i, rowSize[i] );
                m_rowsSignature[2*cnt+0] = i;
                m_rowsSignature[2*cnt+1] = rowSize[i];
                cnt++;
              };
          };
      };
    // Free memory
    free( rowSize );
    free( tmp );
    // Exit point
    return;
  };

public:
  connectivityIO( const char* ownerFname, const char* neighFname, int compressionBlks  ):
    m_nkRows(0),
    m_rowsSignature(nullptr),
    m_matSize(0),
    m_matNNZ(0),
    m_ownerFname(ownerFname),
    m_neighFname(neighFname),
    m_owner(nullptr),
    m_neigh(nullptr),
    m_allocated(false),
    m_compressionBlks(compressionBlks)
  {
    // Exit point
    return;
  };

  void readTXT()
  {

    FOAMdata dataOwner;
    FOAMdata dataNeigh;

    readFile( m_ownerFname, &dataOwner, false );
    readFile( m_neighFname, &dataNeigh, false );

    m_owner = dataOwner.m_data;
    m_neigh = dataNeigh.m_data;

    m_matSize = dataOwner.m_nCells;
    m_matNNZ  = dataOwner.m_nInternalFaces;

    computeSignature();

    return;

  };


  void writeHDF5( const char* fname )
  {

    hid_t file_id;
    hid_t dataset_id;
    hid_t dataspace_id;
    hid_t plist_id;
    hid_t dataspace;
    hid_t datatype;
    hid_t group_id;
    hsize_t dimsf[2];

    size_t       nelmts;
    unsigned int flags;
    unsigned int filter;
    unsigned int _info;
    H5Z_filter_t filter_type;

    herr_t  status;
    hsize_t dims[2];
    hsize_t cdims[2];

    int i, j, numfilt;

    /* Create a file.  */
    file_id = H5Fcreate( fname, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );

    group_id = H5Gcreate2(file_id, "/info", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    dimsf[0] = 1; dimsf[1]=0;
    dataspace  = H5Screate_simple(1, dimsf, NULL);
    datatype   = H5Tcopy(H5T_NATIVE_INT);
    status     = H5Tset_order(datatype, H5T_ORDER_LE);
    dataset_id = H5Dcreate2(group_id, "rowKind", datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status     = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &m_nkRows );
    H5Sclose(dataspace);
    H5Tclose(datatype);
    H5Dclose(dataset_id);

    dims[0]      = 2*m_nkRows;
    dims[1]      = 1;
    dataspace_id = H5Screate_simple(2, dims, NULL);
    dataset_id = H5Dcreate2(group_id, "signature", H5T_STD_I32LE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, m_rowsSignature );
    status = H5Gclose(group_id);
    status = H5Sclose(dataspace_id);
    status = H5Dclose(dataset_id);


    group_id = H5Gcreate2(file_id, "/dimensions", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    dimsf[0] = 1; dimsf[1]=0;
    dataspace  = H5Screate_simple(1, dimsf, NULL);
    datatype   = H5Tcopy(H5T_NATIVE_INT);
    status     = H5Tset_order(datatype, H5T_ORDER_LE);
    dataset_id = H5Dcreate2(group_id, "matSize", datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status     = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &m_matSize );
    H5Sclose(dataspace);
    H5Tclose(datatype);
    H5Dclose(dataset_id);

    dimsf[0] = 1; dimsf[1]=0;
    dataspace  = H5Screate_simple(1, dimsf, NULL);
    datatype   = H5Tcopy(H5T_NATIVE_INT);
    status     = H5Tset_order(datatype, H5T_ORDER_LE);
    dataset_id = H5Dcreate2(group_id, "matNNZ", datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status     = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &m_matNNZ );
    H5Sclose(dataspace);
    H5Tclose(datatype);
    H5Dclose(dataset_id);

    status = H5Gclose(group_id);


    group_id = H5Gcreate2(file_id, "/connectivity", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    dims[0]      = m_matNNZ;
    dims[1]      = 1;
    dataspace_id = H5Screate_simple(2, dims, NULL);

    plist_id = H5Pcreate(H5P_DATASET_CREATE);

    /* Dataset must be chunked for compression */
    cdims[0] = __MIN__( m_compressionBlks, dims[0]/10 );
    cdims[1] = 1;
    status   = H5Pset_chunk(plist_id, 2, cdims);

    /* Set ZLIB / DEFLATE Compression using compression level 6.
     * To use SZIP Compression comment out these lines.
     */
    status = H5Pset_deflate(plist_id, 6);

    dataset_id = H5Dcreate2(group_id, "owner", H5T_STD_I32LE, dataspace_id, H5P_DEFAULT, plist_id, H5P_DEFAULT);

    status = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, m_owner );

    // printf( "Qui arrivo 4\n" );


    /* Create a file.  */
    dims[0]      = m_matNNZ;
    dims[1]      = 1;
    dataspace_id = H5Screate_simple(2, dims, NULL);

    plist_id = H5Pcreate(H5P_DATASET_CREATE);

    /* Dataset must be chunked for compression */
    cdims[0] = __MIN__( m_compressionBlks, dims[0]/10 );
    cdims[1] = 1;
    status   = H5Pset_chunk(plist_id, 2, cdims);

    /* Set ZLIB / DEFLATE Compression using compression level 6.
     * To use SZIP Compression comment out these lines.
     */
    status = H5Pset_deflate(plist_id, 6);

    dataset_id = H5Dcreate2(group_id, "neigh", H5T_STD_I32LE, dataspace_id, H5P_DEFAULT, plist_id, H5P_DEFAULT);

    status = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, m_neigh );


    status = H5Sclose(dataspace_id);
    status = H5Dclose(dataset_id);
    status = H5Pclose(plist_id);
    status = H5Gclose(group_id);
    status = H5Fclose(file_id);

    computeSignature();

    return;

  };


  void readHDF5( const char* fname )
  {

    H5Z_filter_t filter_type;
    hid_t file_id, group_id, dataset_id, dataspace_id; /* identifiers */
    hid_t plist_id;
    size_t       nelmts;
    unsigned     flags, filter_info;
    herr_t  status;

    // printf( "Entro nella funzione di lettura\n" );

    /* Now reopen the file and dataset in the file. */
    file_id  = H5Fopen( fname, H5F_ACC_RDONLY, H5P_DEFAULT);

    group_id = H5Gopen2(file_id, "/dimensions", H5P_DEFAULT );


    dataset_id = H5Dopen2(group_id, "matSize", H5P_DEFAULT);
    status = H5Dread(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &m_matSize);

    dataset_id = H5Dopen2(group_id, "matNNZ", H5P_DEFAULT);
    status = H5Dread(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &m_matNNZ);

    status = H5Gclose(group_id);

    m_owner     = (unsigned int*) malloc( m_matNNZ*sizeof(int) );
    m_neigh     = (unsigned int*) malloc( m_matNNZ*sizeof(int) );

    group_id   = H5Gopen2(file_id, "/connectivity", H5P_DEFAULT );
    dataset_id = H5Dopen2(group_id, "owner", H5P_DEFAULT);
    status     = H5Dread(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, m_owner);
    status     = H5Dclose(dataset_id);
    dataset_id = H5Dopen2(group_id, "neigh", H5P_DEFAULT);
    status     = H5Dread(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, m_neigh);
    status     = H5Dclose(dataset_id);
    status     = H5Gclose(group_id);

    return;
  };


  void
  writeOctave( )
  {
    FILE* fid;

    fid = fopen( "OctaveSize.dat", "w" );
    fprintf( fid, "%d\n", m_matSize );
    fprintf( fid, "%d\n", m_matNNZ );
    fclose(fid);

    fid = fopen( "OctaveOwner.dat", "w" );
    for ( int i=0; i<m_matNNZ; ++i )
      {
        fprintf( fid, "%d\n", m_owner[i] );
      };
    fclose(fid);

    fid = fopen( "OctaveNeigh.dat", "w" );
    for ( int i=0; i<m_matNNZ; ++i )
      {
        fprintf( fid, "%d\n", m_neigh[i] );
      };
    fclose(fid);


    return;
  };


  ~connectivityIO()
  {
    m_matSize = 0;
    m_matNNZ = 0;

    if ( m_nkRows>0 )
      {
        free( m_rowsSignature );
      };

    if ( m_allocated )
      {
        free( m_owner );
        free( m_neigh );
        m_allocated = false;
      };
    return;
  };

  void loadTXT ( const char* fname ){ return; };
  void loadhdf5( const char* fname ){ return; };
  const unsigned int* owner() const { return( m_owner ); };
  const unsigned int* neigh() const { return( m_neigh ); };
  unsigned int matSize() const { return( m_matSize ); };
  unsigned int matNNZ()  const { return( m_matNNZ ); };

};


#endif /* INC_CONNECTIVITYIO_H_ */
