#ifndef __UNIT_TESTS_IO_MANAGER_H__
#define __UNIT_TESTS_IO_MANAGER_H__
#include "hdf5.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <deque>



// Kind of data in the dataset
enum class flavour_e
{
  input,
  output,
  unknown
};


// Class used to manage Unit Tests data
class HDF5kernels
{
  private:
    char m_buffer[4096];
    char m_cid[8];
    const char* m_dir;
    const char* m_prefix;
    const char* m_fname;
    int m_idx;
    hid_t m_fid;
    hid_t m_input;
    hid_t m_result;
    herr_t m_status;
    void CreateDir( const char* dirName )
    {
      struct stat st = {0};
      if (stat(dirName, &st) == -1) {
        mkdir(dirName, 0700);
      }
      return;
    };
  public:
    HDF5kernels( const char* dir, const char* prefix, const char* fname, int idx ):
      m_fname(fname),m_idx(idx),m_dir(dir),m_prefix(prefix)
    { 
       // printf( "Inizializzo :: %s\n", m_fname );
       memset( m_cid, 0, 8 );
       sprintf( m_cid, "%6.6d", idx );
       return;
    };


    void Create()
    {
      memset( m_buffer, 0, 4096 );
      hsize_t l = strlen( m_fname );
      assert( l<512 );
      strcpy( m_buffer, m_dir );
      strcat( m_buffer, "/" );
      strcat( m_buffer, m_fname );
      CreateDir( m_buffer );
      strcat( m_buffer, "/" );
      strcat( m_buffer, m_prefix );
      strcat( m_buffer, "_" );
      strcat( m_buffer, m_fname );
      strcat( m_buffer, "_" );
      strncat( m_buffer, m_cid, 6 );
      strcat( m_buffer, ".h5" );
      // printf( "Provo a creare %s\n", m_buffer );
      m_fid = H5Fcreate( m_buffer, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
      hid_t input  = H5Gcreate( m_fid, "/input",  H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      hid_t result = H5Gcreate( m_fid, "/output", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      m_status = H5Gclose( input );
      m_status = H5Gclose( result );
      memset( m_buffer, 0, 512 );
      return;
    };


    void Open()
    {
      memset( m_buffer, 0, 4096 );
      hsize_t l = strlen( m_fname );
      assert( l<512 );
      strcpy( m_buffer, m_dir );
      strcat( m_buffer, "/" );
      strcat( m_buffer, m_fname );
      strcat( m_buffer, "/" );
      strcat( m_buffer, m_prefix );
      strcat( m_buffer, "_" );
      strcat( m_buffer, m_fname );
      strcat( m_buffer, "_" );
      strncat( m_buffer, m_cid, 6 );
      strcat( m_buffer, ".h5" );
      // printf( "Provo ad aprire %s\n", m_buffer );
      m_fid    = H5Fopen( m_buffer, H5F_ACC_RDONLY, H5P_DEFAULT );
      memset( m_buffer, 0, 512 );
      return;
    };


    void Close()
    {
      if ( m_fid >= 0 )
      {
        m_status = H5Fclose ( m_fid );
        m_fid = -1;
      };
      return;
    };


    ~HDF5kernels()
    {
      // Exit point 
      return;
    };

    void addDataset( const char* dsetName, flavour_e flavour, hsize_t dsetSize, double const * const dsetValue )
    {

      /*
       * 
       */
      assert( dsetSize > 0 );
      assert( dsetValue != nullptr );
      memset( m_buffer, 0, 512 );
      hsize_t l = strlen( dsetName );
      assert( l<64 );

      switch ( flavour )
      {
        case flavour_e::input:
        {
          sprintf( m_buffer, "/input/%s", dsetName );
          break;
        }
        case flavour_e::output:
        {
          sprintf( m_buffer, "/output/%s", dsetName );
          break;
        }
        default:
        {
          assert( false );
          break;
        }
      };

      /*
       * Create dataspace.  Setting maximum size to NULL sets the maximum
       * size to be the current size.
       */
      hid_t space = H5Screate_simple (1, &dsetSize, NULL);
  
      /*
       * Create the dataset.  We will use all default properties for this
       * example.
       */
      hid_t dset = H5Dcreate( m_fid, m_buffer, H5T_IEEE_F64LE, space, H5P_DEFAULT,
                  H5P_DEFAULT, H5P_DEFAULT);
  
      /*
       * Write the data to the dataset.
       */
      m_status = H5Dwrite (dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                  dsetValue);
  
      /*
       * Close and release resources.
       */
      m_status = H5Dclose (dset);
      m_status = H5Sclose (space);

      /*
       * reset the buffer
       */
      memset( m_buffer, 0, 512 );

      /*
       * Exit point
       */
      return;
    };


    void addDataset( const char* dsetName, flavour_e flavour, hsize_t dsetSize, float const * const dsetValue )
    {

      /*
       * 
       */
      assert( dsetSize > 0 );
      assert( dsetValue != nullptr );
      memset( m_buffer, 0, 512 );
      hsize_t l = strlen( dsetName );
      assert( l<64 );

      switch ( flavour )
      {
        case flavour_e::input:
        {
          sprintf( m_buffer, "/input/%s", dsetName );
          break;
        }
        case flavour_e::output:
        {
          sprintf( m_buffer, "/output/%s", dsetName );
          break;
        }
        default:
        {
          assert( false );
          break;
        }
      };


      /*
       * Create dataspace.  Setting maximum size to NULL sets the maximum
       * size to be the current size.
       */
      hid_t space = H5Screate_simple (1, &dsetSize, NULL);
  
      /*
       * Create the dataset.  We will use all default properties for this
       * example.
       */
      hid_t dset = H5Dcreate( m_fid, m_buffer, H5T_IEEE_F32LE, space, H5P_DEFAULT,
                  H5P_DEFAULT, H5P_DEFAULT);
  
      /*
       * Write the data to the dataset.
       */
      m_status = H5Dwrite (dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                  dsetValue);
  
      /*
       * Close and release resources.
       */
      m_status = H5Dclose (dset);
      m_status = H5Sclose (space);

      /*
       * reset the buffer
       */
      memset( m_buffer, 0, 512 );

      /*
       * Exit point
       */
      return;
    };


    void addDataset( const char* dsetName, flavour_e flavour, hsize_t dsetSize, unsigned short const * const dsetValue )
    {

      /*
       * 
       */
      assert( dsetSize > 0 );
      assert( dsetValue != nullptr );
      memset( m_buffer, 0, 512 );
      hsize_t l = strlen( dsetName );
      assert( l<64 );

      switch ( flavour )
      {
        case flavour_e::input:
        {
          sprintf( m_buffer, "/input/%s", dsetName );
          break;
        }
        case flavour_e::output:
        {
          sprintf( m_buffer, "/output/%s", dsetName );
          break;
        }
        default:
        {
          assert( false );
          break;
        }
      };


      /*
       * Create dataspace.  Setting maximum size to NULL sets the maximum
       * size to be the current size.
       */
      hid_t space = H5Screate_simple (1, &dsetSize, NULL);
  
      /*
       * Create the dataset.  We will use all default properties for this
       * example.
       */
      hid_t dset = H5Dcreate( m_fid, m_buffer, H5T_STD_U16LE, space, H5P_DEFAULT,
                  H5P_DEFAULT, H5P_DEFAULT);
  
      /*
       * Write the data to the dataset.
       */
      m_status = H5Dwrite (dset, H5T_NATIVE_USHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                  dsetValue);
  
      /*
       * Close and release resources.
       */
      m_status = H5Dclose (dset);
      m_status = H5Sclose (space);

      /*
       * reset the buffer
       */
      memset( m_buffer, 0, 512 );

      /*
       * Exit point
       */
      return;
    };



    void addDataset( const char* dsetName, flavour_e flavour, hsize_t dsetSize, unsigned int const * const dsetValue )
    {

      /*
       * 
       */
      assert( dsetSize > 0 );
      assert( dsetValue != nullptr );
      memset( m_buffer, 0, 512 );
      hsize_t l = strlen( dsetName );
      assert( l<64 );

      switch ( flavour )
      {
        case flavour_e::input:
        {
          sprintf( m_buffer, "/input/%s", dsetName );
          break;
        }
        case flavour_e::output:
        {
          sprintf( m_buffer, "/output/%s", dsetName );
          break;
        }
        default:
        {
          assert( false );
          break;
        }
      };


      /*
       * Create dataspace.  Setting maximum size to NULL sets the maximum
       * size to be the current size.
       */
      hid_t space = H5Screate_simple (1, &dsetSize, NULL);
  
      /*
       * Create the dataset.  We will use all default properties for this
       * example.
       */
      hid_t dset = H5Dcreate( m_fid, m_buffer, H5T_STD_U32LE, space, H5P_DEFAULT,
                  H5P_DEFAULT, H5P_DEFAULT);
  
      /*
       * Write the data to the dataset.
       */
      m_status = H5Dwrite (dset, H5T_NATIVE_UINT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                  dsetValue);
  
      /*
       * Close and release resources.
       */
      m_status = H5Dclose (dset);
      m_status = H5Sclose (space);

      /*
       * reset the buffer
       */
      memset( m_buffer, 0, 512 );

      /*
       * Exit point
       */
      return;
    };



    void addDataset( const char* dsetName, flavour_e flavour, hsize_t dsetSize, unsigned long const * const dsetValue )
    {

      /*
       * 
       */
      assert( dsetSize > 0 );
      assert( dsetValue != nullptr );
      memset( m_buffer, 0, 512 );
      hsize_t l = strlen( dsetName );
      assert( l<64 );

      switch ( flavour )
      {
        case flavour_e::input:
        {
          sprintf( m_buffer, "/input/%s", dsetName );
          break;
        }
        case flavour_e::output:
        {
          sprintf( m_buffer, "/output/%s", dsetName );
          break;
        }
        default:
        {
          assert( false );
          break;
        }
      };


      /*
       * Create dataspace.  Setting maximum size to NULL sets the maximum
       * size to be the current size.
       */
      hid_t space = H5Screate_simple (1, &dsetSize, NULL);
  
      /*
       * Create the dataset.  We will use all default properties for this
       * example.
       */
      hid_t dset = H5Dcreate( m_fid, m_buffer, H5T_STD_U64BE, space, H5P_DEFAULT,
                  H5P_DEFAULT, H5P_DEFAULT);
  
      /*
       * Write the data to the dataset.
       */
      m_status = H5Dwrite (dset, H5T_NATIVE_ULONG, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                  dsetValue);
  
      /*
       * Close and release resources.
       */
      m_status = H5Dclose (dset);
      m_status = H5Sclose (space);

      /*
       * reset the buffer
       */
      memset( m_buffer, 0, 512 );

      /*
       * Exit point
       */
      return;
    };


    void addDataset( const char* dsetName, flavour_e flavour, hsize_t dsetSize, short const * const dsetValue )
    {

      /*
       * 
       */
      assert( dsetSize > 0 );
      assert( dsetValue != nullptr );
      memset( m_buffer, 0, 512 );
      hsize_t l = strlen( dsetName );
      assert( l<64 );

      switch ( flavour )
      {
        case flavour_e::input:
        {
          sprintf( m_buffer, "/input/%s", dsetName );
          break;
        }
        case flavour_e::output:
        {
          sprintf( m_buffer, "/output/%s", dsetName );
          break;
        }
        default:
        {
          assert( false );
          break;
        }
      };


      /*
       * Create dataspace.  Setting maximum size to NULL sets the maximum
       * size to be the current size.
       */
      hid_t space = H5Screate_simple (1, &dsetSize, NULL);
  
      /*
       * Create the dataset.  We will use all default properties for this
       * example.
       */
      hid_t dset = H5Dcreate( m_fid, m_buffer, H5T_STD_I16LE, space, H5P_DEFAULT,
                  H5P_DEFAULT, H5P_DEFAULT);
  
      /*
       * Write the data to the dataset.
       */
      m_status = H5Dwrite (dset, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                  dsetValue);
  
      /*
       * Close and release resources.
       */
      m_status = H5Dclose (dset);
      m_status = H5Sclose (space);

      /*
       * reset the buffer
       */
      memset( m_buffer, 0, 512 );

      /*
       * Exit point
       */
      return;
    };



    void addDataset( const char* dsetName, flavour_e flavour, hsize_t dsetSize, int const * const dsetValue )
    {

      /*
       * 
       */
      assert( dsetSize > 0 );
      assert( dsetValue != nullptr );
      memset( m_buffer, 0, 512 );
      hsize_t l = strlen( dsetName );
      assert( l<64 );

      switch ( flavour )
      {
        case flavour_e::input:
        {
          sprintf( m_buffer, "/input/%s", dsetName );
          break;
        }
        case flavour_e::output:
        {
          sprintf( m_buffer, "/output/%s", dsetName );
          break;
        }
        default:
        {
          assert( false );
          break;
        }
      };


      /*
       * Create dataspace.  Setting maximum size to NULL sets the maximum
       * size to be the current size.
       */
      hid_t space = H5Screate_simple (1, &dsetSize, NULL);
  
      /*
       * Create the dataset.  We will use all default properties for this
       * example.
       */
      hid_t dset = H5Dcreate( m_fid, m_buffer, H5T_STD_I32LE, space, H5P_DEFAULT,
                  H5P_DEFAULT, H5P_DEFAULT);
  
      /*
       * Write the data to the dataset.
       */
      m_status = H5Dwrite (dset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                  dsetValue);
  
      /*
       * Close and release resources.
       */
      m_status = H5Dclose (dset);
      m_status = H5Sclose (space);

      /*
       * reset the buffer
       */
      memset( m_buffer, 0, 512 );

      /*
       * Exit point
       */
      return;
    };



    void addDataset( const char* dsetName, flavour_e flavour, hsize_t dsetSize, long const * const dsetValue )
    {

      /*
       * 
       */
      assert( dsetSize > 0 );
      assert( dsetValue != nullptr );
      memset( m_buffer, 0, 512 );
      hsize_t l = strlen( dsetName );
      assert( l<64 );

      switch ( flavour )
      {
        case flavour_e::input:
        {
          sprintf( m_buffer, "/input/%s", dsetName );
          break;
        }
        case flavour_e::output:
        {
          sprintf( m_buffer, "/output/%s", dsetName );
          break;
        }
        default:
        {
          assert( false );
          break;
        }
      };


      /*
       * Create dataspace.  Setting maximum size to NULL sets the maximum
       * size to be the current size.
       */
      hid_t space = H5Screate_simple (1, &dsetSize, NULL);
  
      /*
       * Create the dataset.  We will use all default properties for this
       * example.
       */
      hid_t dset = H5Dcreate( m_fid, m_buffer, H5T_STD_I64BE, space, H5P_DEFAULT,
                  H5P_DEFAULT, H5P_DEFAULT);
  
      /*
       * Write the data to the dataset.
       */
      m_status = H5Dwrite (dset, H5T_NATIVE_LONG, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                  dsetValue);
  
      /*
       * Close and release resources.
       */
      m_status = H5Dclose (dset);
      m_status = H5Sclose (space);

      /*
       * reset the buffer
       */
      memset( m_buffer, 0, 512 );

      /*
       * Exit point
       */
      return;
    };


    void getDataset( const char* dsetName, flavour_e flavour, double* dsetValue )
    {

      /*
       * 
       */
      assert( dsetValue != nullptr );
      memset( m_buffer, 0, 512 );
      hsize_t l = strlen( dsetName );
      assert( l<64 );

      switch ( flavour )
      {
        case flavour_e::input:
        {
          sprintf( m_buffer, "/input/%s", dsetName );
          break;
        }
        case flavour_e::output:
        {
          sprintf( m_buffer, "/output/%s", dsetName );
          break;
        }
        default:
        {
          assert( false );
          break;
        }
      };

      /*
       * Create the dataset.  We will use all default properties for this
       * example.
       */
      hid_t dset = H5Dopen ( m_fid, m_buffer, H5P_DEFAULT);
      /*
       * Write the data to the dataset.
       */
       m_status = H5Dread (dset, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                  dsetValue);
      /*
       * Close and release resources.
       */
      m_status = H5Dclose (dset);

      /*
       * reset the buffer
       */
      memset( m_buffer, 0, 512 );

      /*
       * Exit point
       */
      return;
    };



    void getDataset( const char* dsetName, flavour_e flavour, float* dsetValue )
    {

      /*
       * 
       */
      assert( dsetValue != nullptr );
      memset( m_buffer, 0, 512 );
      hsize_t l = strlen( dsetName );
      assert( l<64 );

      switch ( flavour )
      {
        case flavour_e::input:
        {
          sprintf( m_buffer, "/input/%s", dsetName );
          break;
        }
        case flavour_e::output:
        {
          sprintf( m_buffer, "/output/%s", dsetName );
          break;
        }
        default:
        {
          assert( false );
          break;
        }
      };

      /*
       * Create the dataset.  We will use all default properties for this
       * example.
       */
      hid_t dset = H5Dopen ( m_fid, m_buffer, H5P_DEFAULT);
      /*
       * Write the data to the dataset.
       */
       m_status = H5Dread (dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                  dsetValue);
      /*
       * Close and release resources.
       */
      m_status = H5Dclose (dset);

      /*
       * reset the buffer
       */
      memset( m_buffer, 0, 512 );

      /*
       * Exit point
       */
      return;
    };



    void getDataset( const char* dsetName, flavour_e flavour, unsigned short* dsetValue )
    {

      /*
       * 
       */
      assert( dsetValue != nullptr );
      memset( m_buffer, 0, 512 );
      hsize_t l = strlen( dsetName );
      assert( l<64 );

      switch ( flavour )
      {
        case flavour_e::input:
        {
          sprintf( m_buffer, "/input/%s", dsetName );
          break;
        }
        case flavour_e::output:
        {
          sprintf( m_buffer, "/output/%s", dsetName );
          break;
        }
        default:
        {
          assert( false );
          break;
        }
      };

      /*
       * Create the dataset.  We will use all default properties for this
       * example.
       */
      hid_t dset = H5Dopen ( m_fid, m_buffer, H5P_DEFAULT);
      /*
       * Write the data to the dataset.
       */
       m_status = H5Dread (dset, H5T_NATIVE_USHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                  dsetValue);
      /*
       * Close and release resources.
       */
      m_status = H5Dclose (dset);

      /*
       * reset the buffer
       */
      memset( m_buffer, 0, 512 );

      /*
       * Exit point
       */
      return;
    };



    void getDataset( const char* dsetName, flavour_e flavour, unsigned int* dsetValue )
    {

      /*
       * 
       */
      assert( dsetValue != nullptr );
      memset( m_buffer, 0, 512 );
      hsize_t l = strlen( dsetName );
      assert( l<64 );

      switch ( flavour )
      {
        case flavour_e::input:
        {
          sprintf( m_buffer, "/input/%s", dsetName );
          break;
        }
        case flavour_e::output:
        {
          sprintf( m_buffer, "/output/%s", dsetName );
          break;
        }
        default:
        {
          assert( false );
          break;
        }
      };

      /*
       * Create the dataset.  We will use all default properties for this
       * example.
       */
      hid_t dset = H5Dopen ( m_fid, m_buffer, H5P_DEFAULT);
  
      /*
       * Write the data to the dataset.
       */
       m_status = H5Dread (dset, H5T_NATIVE_UINT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                  dsetValue);
      /*
       * Close and release resources.
       */
      m_status = H5Dclose (dset);

      /*
       * reset the buffer
       */
      memset( m_buffer, 0, 512 );

      /*
       * Exit point
       */
      return;
    };



    void getDataset( const char* dsetName, flavour_e flavour, unsigned long* dsetValue )
    {

      /*
       * 
       */
      assert( dsetValue != nullptr );
      memset( m_buffer, 0, 512 );
      hsize_t l = strlen( dsetName );
      assert( l<64 );

      switch ( flavour )
      {
        case flavour_e::input:
        {
          sprintf( m_buffer, "/input/%s", dsetName );
          break;
        }
        case flavour_e::output:
        {
          sprintf( m_buffer, "/output/%s", dsetName );
          break;
        }
        default:
        {
          assert( false );
          break;
        }
      };

      /*
       * Create the dataset.  We will use all default properties for this
       * example.
       */
      hid_t dset = H5Dopen ( m_fid, m_buffer, H5P_DEFAULT);

      /*
       * Write the data to the dataset.
       */
       m_status = H5Dread (dset, H5T_NATIVE_ULONG, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                  dsetValue);
      /*
       * Close and release resources.
       */
      m_status = H5Dclose (dset);

      /*
       * reset the buffer
       */
      memset( m_buffer, 0, 512 );

      /*
       * Exit point
       */
      return;
    };

};

#endif
