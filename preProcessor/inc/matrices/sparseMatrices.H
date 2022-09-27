/*
 * sparseMAtrices.H
 *
 *  Created on: May 18, 2022
 *      Author: valentini
 */

#ifndef INC_MATRICES_SPARSEMATRICES_H_
#define INC_MATRICES_SPARSEMATRICES_H_

#include <stdio.h>
#include "../sparseMatrixBase.H"
#include "./sparseStdMatrix.H"
#include "./sparseBlkMatrix.H"

class sparseMatrices
{
private:
  sparseStdMatrix m_mat00;
  sparseBlkMatrix m_mat01;


public:
  sparseMatrices( const connectivityIO& IO ):
    m_mat00(IO),
    m_mat01(IO)
  {};

  ~sparseMatrices()
  {
    return;
  };

  unsigned int
  size()
  {
    return(2);
  };

  sparseMatrix&
  operator[]( unsigned int idx )
  {
    switch (idx)
    {
      case 0:
	{
	  return( m_mat00 );
	  break;
	}
      case 1:
	{
	  return( m_mat01 );
	  break;
	}
      default:
	{
	  printf( "ERROR :: unknown case" );
	  abort();
	}
    };
  };

  sparseMatrix&
  operator()( const char* name )
  {

      if ( strncmp( name, "Standard", 8 ) )
	{
	  return( m_mat00 );
	};

      if ( strncmp( name, "blockRowPerm", 8 ) )
	{
	  return( m_mat01 );
	};

      printf( "ERROR :: unknown matrix type\n" );
      abort();
  };


};


#endif /* INC_MATRICES_SPARSEMATRICES_H_ */
