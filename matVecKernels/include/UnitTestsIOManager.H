/*
 * @file UnitTestsIOManager.H
 *
 * @brief Definiton of a class which is a wrapper to the hdf5 library.
 * The class provides an high level interface to be used to write/read
 * unitTests data
 *
 *  @date   18/Giu/2022
 *  @author Mirco Valentini
 */

#ifndef __UNIT_TESTS_IO_MANAGER_H__
#define __UNIT_TESTS_IO_MANAGER_H__


/**
 * System include
 */
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



/**
 * @brief enumerators used to differentiate between input and output 
 *        of a kernels during the write/read pahse
 */
enum class flavour_e
{
  input,
  output,
  unknown
};


/**
 * @class Class used as a wrapper for the hdf5 library. 
 *        This class is meant to write/read input-output parameters 
 *        for kernels in order to perform unit tests
 */
class HDF5kernels
{
  private:

    /**
     * Buffer used for the names creation
     */
    char m_buffer[4096];


    /**
     * Buffer used for conversion of the index to string
     */
    char m_cid[8];


    /**
     * Directory name
     */
    const char* m_dir;


    /**
     * Prefix name
     */
    const char* m_prefix;


    /**
     * Name of the testcase
     */
    const char* m_fname;


    /**
     * Variable used to store the index of hte testcase
     */
    int m_idx;


    /**
     * File identifier
     */
    hid_t m_fid;


    /**
     * Pointer to the input group inside the hdf5 file
     */
    hid_t m_input;


    /**
     * Pointer to the result/output group inside the hdf5 file
     */
    hid_t m_result;


    /**
     * Return values fo rhte hdf5 calls
     */
    herr_t m_status;


    /**
     * @brief Create a directory with exist checks
     *
     * @param[in] dirNme Name of hte directory to be created
     */
    void
    CreateDir
    (
      const char* dirName
    )
    {
      /*
       * Create a new directory if not exist
       */
      struct stat st = {0};
      if (stat(dirName, &st) == -1)
      {
        mkdir(dirName, 0700);
      };
      /* Exit point */
      return;
    };


    /**
     * @brief Check if a file exist
     *
     * @param[in] name Name of hte file to be checked
     */
    bool
    fileExist
    (
      const char* name
    )
    {
      struct stat buffer;
      /* Exit point */
      return( stat(m_buffer, &buffer) == 0 ? true : false );
    };


    /**
     * @brief Create the dataset name accodine to the requiretd flavour
     *
     * @param[in] dsetName name of the dataset to be written
     * @param[in] flavour  flavour of the dataset (input/output)
     */
    void
    datasetName
    (
      const char* dsetName,
      flavour_e   flavour
    )
    {

      /* Checks on the name */
      memset( m_buffer, 0, 512 );
      hsize_t l = strlen( dsetName );
      assert( l<64 );

      /* Create the name accordingly to the flavour */
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

      /* Exit point*/
      return;
    };



  public:

    /*
     * @brief Constructor for the class
     *
     * Name will be generated as:
     * >>> <dir>/<prefix>_<name>_<%6.6d - index>.h5
     *
     * @param[in] dir    name of the directory of the unit tests
     * @param[in] prefix name of the prefix of the unit tests
     * @param[in] fname  name of the unit tests
     * @param[in] idx    name of the index of the unit tests
     *
     */
    HDF5kernels
    (
       const char* dir,
       const char* prefix,
       const char* fname,
             int   idx
    ):
      m_dir(dir),
      m_prefix(prefix),
      m_fname(fname),
      m_idx(idx),
      m_fid(0),
      m_input(0),
      m_result(0),
      m_status(0)
    {
       /* Convert the index to string */
       memset( m_cid, 0, 8 );
       sprintf( m_cid, "%6.6d", idx );
       /* Exit pooint */
       return;
    };


    /**
     * @brief Method used to create an hdf5 file to store unitTest data.
     *        N.B. If the file exist the old file will be deleted.
     *        The routine also creates two folder inside the file:
     *        - input
     *        - outpt
     *
     * @see Open
     * @see Close
     */
    void Create()
    {
      /* Create the name of the file */
      memset( m_buffer, 0, 4096 );
      hsize_t l = strlen( m_fname );
      assert( l<512 );
      strcpy( m_buffer, m_dir );
      // strcat( m_buffer, "/" );
      // strcat( m_buffer, m_fname );
      CreateDir( m_buffer );
      strcat( m_buffer, "/" );
      strcat( m_buffer, m_prefix );
      strcat( m_buffer, "_" );
      strcat( m_buffer, m_fname );
      strcat( m_buffer, "_" );
      strncat( m_buffer, m_cid, 6 );
      strcat( m_buffer, ".h5" );
      /* Log the creation */
       printf( "Create :: %s\n", m_fname );
      /* Create the hdf5 file */
      m_fid = H5Fcreate( m_buffer, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
      /* Create the groups inside the file */
      hid_t input  = H5Gcreate( m_fid, "/input",  H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      hid_t result = H5Gcreate( m_fid, "/output", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
      /* Close the groups after creation */
      m_status = H5Gclose( input );
      m_status = H5Gclose( result );
      /* Reset memory buffers used for names */
      memset( m_buffer, 0, 512 );
      return;
    };


    /**
     * @brief Method used to open an hdf5 file to store unitTest data.
     *
     * @see Create
     * @see Close
     */
    void Open()
    {
      /* Create the name of the file */
      memset( m_buffer, 0, 4096 );
      hsize_t l = strlen( m_fname );
      assert( l<512 );
      strcpy( m_buffer, m_dir );
      // strcat( m_buffer, "/" );
      // strcat( m_buffer, m_fname );
      strcat( m_buffer, "/" );
      strcat( m_buffer, m_prefix );
      strcat( m_buffer, "_" );
      strcat( m_buffer, m_fname );
      strcat( m_buffer, "_" );
      strncat( m_buffer, m_cid, 6 );
      strcat( m_buffer, ".h5" );
      /* Throw an error if the file does not exist */
      if ( !fileExist( m_buffer ) )
      {
        printf( "ERROR : file: %s, does not exist. function: %s, file: %s, line: %d\n", m_buffer, __FUNCTION__, __FILE__, __LINE__ );
        abort();
      }
      //else
      //{
      //	      printf( "Opening : %s file\n", m_buffer );
      //};
      /* Open the name of the file */
      m_fid    = H5Fopen( m_buffer, H5F_ACC_RDONLY, H5P_DEFAULT );
      /* Reset memory buffers used for names */
      memset( m_buffer, 0, 512 );
      /* Exit point */
      return;
    };


    /**
     * @brief Method used to close the hdf5 file
     *
     * @see Create
     * @see Open
     */
    void Close()
    {
      /* Close only if hte file is already opened */
      if ( m_fid >= 0 )
      {
        m_status = H5Fclose ( m_fid );
        m_fid = -1;
      };
      /* Exit point */
      return;
    };


    /**
     * @brief Destructor of the class
     */
    ~HDF5kernels()
    {
      /* REst internal variables */
      m_idx = 0;
      m_fid = 0;
      m_input = 0;
      m_result = 0;
      m_status = 0;
      /* Exit point */
      return;
    };


    /**
     * @brief add <double> Dataset 
     *
     * @param[in] dsetName  name of the dataset to be wirtten
     * @param[in] flavour   flavour of hte dataset (input/output)
     * @param[in] dsetSize  size of the dataset
     * @param[in] dsetValue values in the dataset
     *
     * @see getDataset
     */
    void
    addDataset
    (
      const char*          dsetName,
      flavour_e            flavour,
      hsize_t              dsetSize,
      double const * const dsetValue
    )
    {

      /*
       * Checks on the input data
       */
      assert( dsetSize > 0 );
      assert( dsetValue != nullptr );

      /*
       * Createthe dataset name
       */
      datasetName( dsetName, flavour );

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

      /* Exit point */
      return;
    };


    /**
     * @brief add <float> Dataset 
     *
     * @param[in] dsetName  name of the dataset to be wirtten
     * @param[in] flavour   flavour of hte dataset (input/output)
     * @param[in] dsetSize  size of the dataset
     * @param[in] dsetValue values in the dataset
     *
     * @see getDataset
     */
    void
    addDataset
    (
      const char*         dsetName,
      flavour_e           flavour,
      hsize_t             dsetSize,
      float const * const dsetValue
    )
    {

      /*
       * Checks on the input data
       */
      assert( dsetSize > 0 );
      assert( dsetValue != nullptr );

      /*
       * Createthe dataset name
       */
      datasetName( dsetName, flavour );

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


    /**
     * @brief add <unsigned short> Dataset 
     *
     * @param[in] dsetName  name of the dataset to be wirtten
     * @param[in] flavour   flavour of hte dataset (input/output)
     * @param[in] dsetSize  size of the dataset
     * @param[in] dsetValue values in the dataset
     *
     * @see getDataset
     */
    void
    addDataset
    (
      const char*                  dsetName,
      flavour_e                    flavour,
      hsize_t                      dsetSize,
      unsigned short const * const dsetValue
    )
    {

      /*
       * Checks on the input data
       */
      assert( dsetSize > 0 );
      assert( dsetValue != nullptr );

      /*
       * Createthe dataset name
       */
      datasetName( dsetName, flavour );

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


    /**
     * @brief add <unsigned int> Dataset 
     *
     * @param[in] dsetName  name of the dataset to be wirtten
     * @param[in] flavour   flavour of hte dataset (input/output)
     * @param[in] dsetSize  size of the dataset
     * @param[in] dsetValue values in the dataset
     *
     * @see getDataset
     */
    void
    addDataset
    (
      const char*                dsetName,
      flavour_e                  flavour,
      hsize_t                    dsetSize,
      unsigned int const * const dsetValue
    )
    {

      /*
       * Checks on the input data
       */
      assert( dsetSize > 0 );
      assert( dsetValue != nullptr );

      /*
       * Createthe dataset name
       */
      datasetName( dsetName, flavour );

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


    /**
     * @brief add <unsigned long> Dataset 
     *
     * @param[in] dsetName  name of the dataset to be wirtten
     * @param[in] flavour   flavour of hte dataset (input/output)
     * @param[in] dsetSize  size of the dataset
     * @param[in] dsetValue values in the dataset
     *
     * @see getDataset
     */
    void
    addDataset
    (
      const char*                 dsetName,
      flavour_e                   flavour,
      hsize_t                     dsetSize,
      unsigned long const * const dsetValue
    )
    {

      /*
       * Checks on the input data
       */
      assert( dsetSize > 0 );
      assert( dsetValue != nullptr );

      /*
       * Createthe dataset name
       */
      datasetName( dsetName, flavour );

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


    /**
     * @brief add <short> Dataset 
     *
     * @param[in] dsetName  name of the dataset to be wirtten
     * @param[in] flavour   flavour of hte dataset (input/output)
     * @param[in] dsetSize  size of the dataset
     * @param[in] dsetValue values in the dataset
     *
     * @see getDataset
     */
    void
    addDataset
    (
      const char*         dsetName,
      flavour_e           flavour,
      hsize_t             dsetSize,
      short const * const dsetValue
    )
    {

      /*
       * Checks on the input data
       */
      assert( dsetSize > 0 );
      assert( dsetValue != nullptr );

      /*
       * Createthe dataset name
       */
      datasetName( dsetName, flavour );

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


    /**
     * @brief add <int> Dataset 
     *
     * @param[in] dsetName  name of the dataset to be wirtten
     * @param[in] flavour   flavour of hte dataset (input/output)
     * @param[in] dsetSize  size of the dataset
     * @param[in] dsetValue values in the dataset
     *
     * @see getDataset
     */
    void
    addDataset
    (
      const char*       dsetName,
      flavour_e         flavour,
      hsize_t           dsetSize,
      int const * const dsetValue
    )
    {

      /*
       * Checks on the input data
       */
      assert( dsetSize > 0 );
      assert( dsetValue != nullptr );

      /*
       * Createthe dataset name
       */
      datasetName( dsetName, flavour );

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


    /**
     * @brief add <long> Dataset 
     *
     * @param[in] dsetName  name of the dataset to be wirtten
     * @param[in] flavour   flavour of hte dataset (input/output)
     * @param[in] dsetSize  size of the dataset
     * @param[in] dsetValue values in the dataset
     *
     * @see getDataset
     */
    void
    addDataset
    (
      const char*        dsetName,
      flavour_e          flavour,
      hsize_t            dsetSize,
      long const * const dsetValue
    )
    {

      /*
       * Checks on the input data
       */
      assert( dsetSize > 0 );
      assert( dsetValue != nullptr );

      /*
       * Createthe dataset name
       */
      datasetName( dsetName, flavour );

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



    /**
     * @brief read <double> Dataset 
     *
     * @param[in]  dsetName  name of the dataset to be wirtten
     * @param[in]  flavour   flavour of hte dataset (input/output)
     * @param[out] dsetValue values in the dataset
     *
     * @see addDataset
     */
    void
    getDataset
    (
      const char* dsetName,
      flavour_e   flavour,
      double*     dsetValue
    )
    {

      /*
       * Checks on the input data
       */
      assert( dsetValue != nullptr );

      /*
       * Createthe dataset name
       */
      datasetName( dsetName, flavour );

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


    /**
     * @brief read <float> Dataset 
     *
     * @param[in]  dsetName  name of the dataset to be wirtten
     * @param[in]  flavour   flavour of hte dataset (input/output)
     * @param[out] dsetValue values in the dataset
     *
     * @see addDataset
     */
    void
    getDataset
    (
      const char* dsetName,
      flavour_e   flavour,
      float*      dsetValue
    )
    {

      /*
       * Checks on the input data
       */
      assert( dsetValue != nullptr );

      /*
       * Createthe dataset name
       */
      datasetName( dsetName, flavour );

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


    /**
     * @brief read <unsigned short> Dataset 
     *
     * @param[in]  dsetName  name of the dataset to be wirtten
     * @param[in]  flavour   flavour of hte dataset (input/output)
     * @param[out] dsetValue values in the dataset
     *
     * @see addDataset
     */
    void
    getDataset
    (
      const char*     dsetName,
      flavour_e       flavour,
      unsigned short* dsetValue
    )
    {

      /*
       * Checks on the input data
       */
      assert( dsetValue != nullptr );

      /*
       * Createthe dataset name
       */
      datasetName( dsetName, flavour );

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


    /**
     * @brief read <unsigned int> Dataset 
     *
     * @param[in]  dsetName  name of the dataset to be wirtten
     * @param[in]  flavour   flavour of hte dataset (input/output)
     * @param[out] dsetValue values in the dataset
     *
     * @see addDataset
     */
    void
    getDataset
    (
      const char*   dsetName,
      flavour_e     flavour,
      unsigned int* dsetValue
    )
    {

      /*
       * Checks on the input data
       */
      assert( dsetValue != nullptr );

      /*
       * Createthe dataset name
       */
      datasetName( dsetName, flavour );

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


    /**
     * @brief read <unsigned long> Dataset 
     *
     * @param[in]  dsetName  name of the dataset to be wirtten
     * @param[in]  flavour   flavour of hte dataset (input/output)
     * @param[out] dsetValue values in the dataset
     *
     * @see addDataset
     */
    void
    getDataset
    (
      const char*    dsetName,
      flavour_e      flavour,
      unsigned long* dsetValue
    )
    {

      /*
       * Checks on the input data
       */
      assert( dsetValue != nullptr );

      /*
       * Createthe dataset name
       */
      datasetName( dsetName, flavour );

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


    /**
     * @brief read <short> Dataset 
     *
     * @param[in]  dsetName  name of the dataset to be wirtten
     * @param[in]  flavour   flavour of hte dataset (input/output)
     * @param[out] dsetValue values in the dataset
     *
     * @see addDataset
     */
    void
    getDataset
    (
      const char* dsetName,
      flavour_e   flavour,
      short*      dsetValue
    )
    {

      /*
       * Checks on the input data
       */
      assert( dsetValue != nullptr );

      /*
       * Createthe dataset name
       */
      datasetName( dsetName, flavour );

      /*
       * Create the dataset.  We will use all default properties for this
       * example.
       */
      hid_t dset = H5Dopen ( m_fid, m_buffer, H5P_DEFAULT);

      /*
       * Write the data to the dataset.
       */
       m_status = H5Dread (dset, H5T_NATIVE_SHORT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
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


    /**
     * @brief read <int> Dataset 
     *
     * @param[in]  dsetName  name of the dataset to be wirtten
     * @param[in]  flavour   flavour of hte dataset (input/output)
     * @param[out] dsetValue values in the dataset
     *
     * @see addDataset
     */
    void
    getDataset
    (
      const char* dsetName,
      flavour_e   flavour,
      int*        dsetValue
    )
    {

      /*
       * Checks on the input data
       */
      assert( dsetValue != nullptr );

      /*
       * Createthe dataset name
       */
      datasetName( dsetName, flavour );

      /*
       * Create the dataset.  We will use all default properties for this
       * example.
       */
      hid_t dset = H5Dopen ( m_fid, m_buffer, H5P_DEFAULT);
  
      /*
       * Write the data to the dataset.
       */
       m_status = H5Dread (dset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
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


    /**
     * @brief read <long> Dataset 
     *
     * @param[in]  dsetName  name of the dataset to be wirtten
     * @param[in]  flavour   flavour of hte dataset (input/output)
     * @param[out] dsetValue values in the dataset
     *
     * @see addDataset
     */
    void
    getDataset
    (
      const char* dsetName,
      flavour_e   flavour,
      long*       dsetValue
    )
    {

      /*
       * Checks on the input data
       */
      assert( dsetValue != nullptr );

      /*
       * Createthe dataset name
       */
      datasetName( dsetName, flavour );

      /*
       * Create the dataset.  We will use all default properties for this
       * example.
       */
      hid_t dset = H5Dopen ( m_fid, m_buffer, H5P_DEFAULT);

      /*
       * Write the data to the dataset.
       */
       m_status = H5Dread (dset, H5T_NATIVE_LONG, H5S_ALL, H5S_ALL, H5P_DEFAULT,
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
