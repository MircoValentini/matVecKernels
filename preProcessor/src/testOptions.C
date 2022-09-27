/*
 * testOptions.C
 *
 *  Created on: May 19, 2022
 *      Author: valentini
 */

#include "benchmarkOptions.H"


int main( int argc, char* argv[] )
{

  printf( "conunter :: %d\n", argc );
  benchmarkOptions O( argc, argv );

  return(0);
};


