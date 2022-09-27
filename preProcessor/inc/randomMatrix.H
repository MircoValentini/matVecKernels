/*
 * randomMatrix.H
 *
 *  Created on: May 16, 2022
 *      Author: valentini
 */

#ifndef INC_RANDOMMATRIX_H_
#define INC_RANDOMMATRIX_H_

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<time.h>

#include "connectivityIO.H"

unsigned int
initSeed()
{
  return( (unsigned int)time(0) );
};

class randomMatrix
{
private:
  // Connectivity IO
  const connectivityIO& m_IO;
  // Dimension
  unsigned int m_seed;
  // Data
  double* __restrict__ m_Ax;
  double* __restrict__ m_x;
  double* __restrict__ m_diag;
  double* __restrict__ m_upper;
  double* __restrict__ m_lower;
  // Random number generator
  double m_rand( int min, int max )
  {
      double range = (max - min);
      double div = RAND_MAX / range;
      return( min + (rand() / div) );
  };
  
void
LDUmatVecCompCOO_DiagStageOneOutput
(
    const unsigned int                         nCells,
    const unsigned int                         nFaces,
    const unsigned int                         nInternalFaces,
    const unsigned int                         cellsStride,
    const unsigned int                         facesStride,
    const unsigned int                         iComp,
    const double* const  diag,
    const double* const  psi,
          double* const  Apsi
)
{
  // Local variables
  unsigned int iCell;
  unsigned int idx;

  // relax the linear system
  for ( iCell=0; iCell<nCells; iCell++ )
  {
    idx = iComp*cellsStride+iCell;
    Apsi[iCell] = diag[iCell]*psi[idx];
  };
  // Exit point
  return;
};

void
LDUmatVecCompCOO_offDiagStageOneOutput
(
    const unsigned int         nCells,
    const unsigned int         nFaces,
    const unsigned int         nInternalFaces,
    const unsigned int         cellsStride,
    const unsigned int         facesStride,
    const unsigned int         iComp,
    const unsigned int* const  owner,
    const unsigned int* const  neigh,
    const double*       const  upper,
    const double*       const  lower,
    const double*       const  psi,
          double*       const  Apsi
)
{
  // Local variables
  unsigned int iFace;
  unsigned int iOwn;
  unsigned int iNei;
  unsigned int iPrev;
  unsigned int iNext;
  // relax the linear system
  for ( iFace=0; iFace<nInternalFaces; iFace++ )
  {
    iOwn = owner[iFace];
    iNei = neigh[iFace];
    iPrev = iComp*cellsStride+iOwn;
    iNext = iComp*cellsStride+iNei;
    Apsi[iOwn] += upper[iFace]*psi[iNext];
    Apsi[iNei] += lower[iFace]*psi[iPrev];
  };
  // Exit point
  return;
};
  
public:

  randomMatrix
  (
      unsigned int          seed,
      const connectivityIO& IO
  ):
    m_seed(seed),
    m_IO(IO)
  {
    // allocate memory
    m_x     = static_cast<double*>( malloc( m_IO.nCells()*sizeof(double)) );
    m_diag  = static_cast<double*>( malloc( m_IO.nCells()*sizeof(double)) );
    m_Ax    = static_cast<double*>( malloc( m_IO.nCells()*sizeof(double)) );
    m_lower = static_cast<double*>( malloc( m_IO.nInternalFaces()*sizeof(double))  );
    m_upper = static_cast<double*>( malloc( m_IO.nInternalFaces()*sizeof(double))  );
    // Initialize the random matrices
    resetRandomMatrix();
    // Initialize the random matrix
    srand( m_seed );
    // Exit point
    return;
  };


  ~randomMatrix()
  {
    free(m_diag);
    free(m_upper);
    free(m_lower);
    free(m_x);
    return;
  };


  void
  resetRandomMatrix( )
  {
    // Update the random matrix
    unsigned int i;
    for ( i=0; i<m_IO.nCells(); ++i )
      {
        m_diag[i] = 0.0;
        m_x[i] = 0.0;
        m_Ax[i] = 0.0;
      };
    for ( i=0; i<m_IO.nInternalFaces(); ++i )
      {
        m_lower[i] = 0.0;
        m_upper[i] = 0.0;
      };
    // Exit point
    return;
  };


  void
  initApsi( )
  {
    // Update the random matrix
    unsigned int i;
    for ( i=0; i<m_IO.nCells(); ++i )
      {
        m_Ax[i] = 0.0;
      };
    return;
  };


  void
  initPsi( int minVal, int maxVal, int nDecimals )
  {
    // Update the random matrix
    unsigned int i;
    unsigned int r = pow(10,nDecimals);
    double tmp;
    for ( i=0; i<m_IO.nCells(); ++i )
      {
        tmp = static_cast<double>( round( m_rand(minVal,maxVal)*r) );
        m_x[i] = tmp/r;
      };
    return;
  };


  void
  computeSpMv()
  {

    LDUmatVecCompCOO_offDiagStageOneOutput
    (
      m_IO.nCells(),
      m_IO.nFaces(),
      m_IO.nInternalFaces(),
      m_IO.nCells(),
      m_IO.nFaces(),
      0,
      m_IO.owner(),
      m_IO.neigh(),
      m_upper,
      m_lower,
      m_x,
      m_Ax
    );

    return;
  };


  void
  computeRandomMatrix( int minVal, int maxVal, int nDecimals )
  {
    // Update the random matrix
    unsigned int i;
    unsigned int r = pow(10,nDecimals);
    double tmp;
    for ( i=0; i<m_IO.nCells(); ++i )
      {
        tmp = static_cast<double>( round( m_rand(minVal,maxVal)*r ) );
        m_diag[i] = tmp/r;

      };
    for ( i=0; i<m_IO.nInternalFaces(); ++i )
      {
        tmp = static_cast<double>( round( m_rand(minVal,maxVal)*r ) );
        m_lower[i] = tmp/r;
        tmp = static_cast<double>( round( m_rand(minVal,maxVal)*r) );
        m_upper[i] = tmp/r;
      };
    // Exit point
    return;
  };


  double* __restrict__ diag()  const { return( m_diag  ); };
  double* __restrict__ upper() const { return( m_upper ); };
  double* __restrict__ lower() const { return( m_lower ); };
  double* __restrict__ psi()   const { return( m_x     ); };
  double* __restrict__ Apsi()  const { return( m_Ax    ); };

};


#endif /* INC_RANDOMMATRIX_H_ */
