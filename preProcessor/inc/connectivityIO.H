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
  bool m_allocated;
  
  FOAMdata m_dataOwner;
  FOAMdata m_dataNeigh;

public:

  connectivityIO( const char* ownerFname, const char* neighFname ):
    m_ownerFname(ownerFname),
    m_neighFname(neighFname),
    m_allocated(false)
  {
    // Exit point
    return;
  };


  void readTXT()
  {

    if ( !m_allocated )
    {
      // Read input files
      readFile( m_ownerFname, &m_dataOwner, false );
      readFile( m_neighFname, &m_dataNeigh, false );
      // Set allocation flag
      m_allocated = true;
    };

    // Exit point
    return;

  };


  ~connectivityIO()
  {
  
    if ( m_allocated == true )
    {
      free(m_dataOwner.m_format);
      free(m_dataOwner.m_class);
      free(m_dataOwner.m_location);
      free(m_dataOwner.m_object);
      free(m_dataOwner.m_data);

      free(m_dataNeigh.m_format);
      free(m_dataNeigh.m_class);
      free(m_dataNeigh.m_location);
      free(m_dataNeigh.m_object);
      free(m_dataNeigh.m_data);
    };
    
    // Exit point
    return;
  };


  const unsigned int* owner() const
  {
    if ( !m_allocated )
    {
      printf( "ERROR: Data not allocated\n" );
      abort();
    }
    return( m_dataOwner.m_data );
  };


  const unsigned int* neigh() const
  {
    if ( !m_allocated )
    {
      printf( "ERROR: Data not allocated\n" );
      abort();
    }
    return( m_dataNeigh.m_data );
  };


  unsigned int nCells() const
  {
    if ( !m_allocated )
    {
      printf( "ERROR: Data not allocated\n" );
      abort();
    }
    if ( m_dataOwner.m_nCells != m_dataNeigh.m_nCells )
    {
      printf( "ERROR: nCells not coherent\n" );
      abort();
    }
    return( m_dataOwner.m_nCells );
  };


  unsigned int nPoints() const
  {
    if ( !m_allocated )
    {
      printf( "ERROR: Data not allocated\n" );
      abort();
    }
    if ( m_dataOwner.m_nPoints != m_dataNeigh.m_nPoints )
    {
      printf( "ERROR: nPoints not coherent\n" );
      abort();
    }
    return( m_dataOwner.m_nPoints );
  };


  unsigned int nFaces() const
  {
    if ( !m_allocated )
    {
      printf( "ERROR: Data not allocated\n" );
      abort();
    }
    if ( m_dataOwner.m_nFaces != m_dataNeigh.m_nFaces )
    {
      printf( "ERROR: nFaces not coherent\n" );
      abort();
    }
    return( m_dataOwner.m_nFaces );
  };


  unsigned int nInternalFaces() const
  {
    if ( !m_allocated )
    {
      printf( "ERROR: Data not allocated\n" );
      abort();
    }
    if ( m_dataOwner.m_nInternalFaces != m_dataNeigh.m_nInternalFaces )
    {
      printf( "ERROR: nInternalFaces not coherent\n" );
      abort();
    }
    return( m_dataOwner.m_nInternalFaces );
  };

};


#endif /* INC_CONNECTIVITYIO_H_ */
