/*
 * benchmarkOptions.H
 *
 *  Created on: May 19, 2022
 *      Author: valentini
 */

#ifndef INC_BENCHMARKOPTIONS_H_
#define INC_BENCHMARKOPTIONS_H_

#include "options.H"

class benchmarkOptions
{
private:
 options m_opt;
public:
  benchmarkOptions( int argc, char* argv[] )
  {
    // Parse command line arguments
    bool loop=true;
    int iarg=1;
    init_options const* optionsCallbacks = callbacks();
    if ( argc<1 ) loop=false;
    while ( loop )
      {
        bool  check    = false;
        bool  found    = false;
	char* key      = argv[iarg];
	int   nNeg     = 0;
	// Remove leading and trailing white spaces
        while ( (strncmp( key, " ", 1 )==0) || (strncmp( key, "-", 1 )==0) )
          {
            if ( strncmp( key, "-", 1 )==0 ) nNeg++;
            key++;
          }

        if ( nNeg == 0 )
          {
            printf( "ERRROR :: \"-\" or \"--\" expected: %s\n", argv[iarg] );
            exit(1);
          }

	//
        // printf( "search parameter %d of %d :: %s\n", iarg, argc, argv[iarg] );
        for ( int i=0; i<nOptions(); i++ )
          {

            // Local variables
            const char* name     = optionsCallbacks[i].oname;
            int         nChecks  = optionsCallbacks[i].nCheck;
            argHandler  callback = optionsCallbacks[i].callback;
            int         kind     = optionsCallbacks[i].kind;

            // Read keyword
            if ( kind==1 && (strncmp( name, key, nChecks ) == 0) )
              {
        	// printf( "Parameters\n" );
                // printf( "kind: %d\n", kind );
                // printf( "name: %s\n", name );
        	check = (callback)( &iarg, argv, 1, nNeg, &m_opt );
        	found = true;
              };
          };

        // printf( "Checks :: %d, %d\n", check, found );
        // Error handling
        if ( !check || !found )
          {
    	    printf( "ERROR :: bad command line parameter: \"%s\"\n", argv[iarg] );
    	    exit(1);
          };

        iarg++;

        // Loop exit condition
        if ( iarg >= argc ) loop=false;

      };
    // Exit point
    return;
  };


  void help()
  {


  };

};



#endif /* INC_BENCHMARKOPTIONS_H_ */
