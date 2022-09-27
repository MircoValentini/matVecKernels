#if 0
char *readFile(char *fileName) {
    FILE *file = fopen(fileName, "r");
    char *code;
    size_t n = 0;
    int c;

    if (file == NULL) return NULL; //could not open file
    fseek(file, 0, SEEK_END);
    long f_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    code = malloc(f_size);

    while ((c = fgetc(file)) != EOF) {
        code[n++] = (char)c;
    }

    code[n] = '\0';        

    return code;
}
#endif

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <vector>


enum class cellSortingStrategy
{
  bandwidth,
  antiBandwidth,
  sortingIndex,
  last
};


/**
 * @brief enumerator for memory sorting algorithms
 *
 */
enum class memorySortingStrategy
{
  standard,
  ell,
  last
};



    int
    isletter
    (
      char c
    )
    {
      int cnt=0;
      int check=0;
      if ( c == 'a' ) { cnt++; };
      if ( c == 'b' ) { cnt++; };
      if ( c == 'c' ) { cnt++; };
      if ( c == 'd' ) { cnt++; };
      if ( c == 'e' ) { cnt++; };
      if ( c == 'f' ) { cnt++; };
      if ( c == 'g' ) { cnt++; };
      if ( c == 'h' ) { cnt++; };
      if ( c == 'i' ) { cnt++; };
      if ( c == 'j' ) { cnt++; };
      if ( c == 'k' ) { cnt++; };
      if ( c == 'l' ) { cnt++; };
      if ( c == 'm' ) { cnt++; };
      if ( c == 'n' ) { cnt++; };
      if ( c == 'o' ) { cnt++; };
      if ( c == 'p' ) { cnt++; };
      if ( c == 'q' ) { cnt++; };
      if ( c == 'r' ) { cnt++; };
      if ( c == 's' ) { cnt++; };
      if ( c == 't' ) { cnt++; };
      if ( c == 'u' ) { cnt++; };
      if ( c == 'v' ) { cnt++; };
      if ( c == 'w' ) { cnt++; };
      if ( c == 'x' ) { cnt++; };
      if ( c == 'y' ) { cnt++; };
      if ( c == 'z' ) { cnt++; };
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      return(check);
    };


    int
    isLetter
    (
      char c
    )
    {
      int cnt=0;
      int check=0;
      if ( c == 'A' ) { cnt++; };
      if ( c == 'B' ) { cnt++; };
      if ( c == 'C' ) { cnt++; };
      if ( c == 'D' ) { cnt++; };
      if ( c == 'E' ) { cnt++; };
      if ( c == 'F' ) { cnt++; };
      if ( c == 'G' ) { cnt++; };
      if ( c == 'H' ) { cnt++; };
      if ( c == 'I' ) { cnt++; };
      if ( c == 'J' ) { cnt++; };
      if ( c == 'K' ) { cnt++; };
      if ( c == 'L' ) { cnt++; };
      if ( c == 'M' ) { cnt++; };
      if ( c == 'N' ) { cnt++; };
      if ( c == 'O' ) { cnt++; };
      if ( c == 'P' ) { cnt++; };
      if ( c == 'Q' ) { cnt++; };
      if ( c == 'R' ) { cnt++; };
      if ( c == 'S' ) { cnt++; };
      if ( c == 'T' ) { cnt++; };
      if ( c == 'U' ) { cnt++; };
      if ( c == 'V' ) { cnt++; };
      if ( c == 'W' ) { cnt++; };
      if ( c == 'X' ) { cnt++; };
      if ( c == 'Y' ) { cnt++; };
      if ( c == 'Z' ) { cnt++; };
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      return(check);
    };

int
isDigit( char c )
{
  int check;
  int cnt=0;
  if ( c == '0' ) { cnt++; }
  if ( c == '1' ) { cnt++; }
  if ( c == '2' ) { cnt++; }
  if ( c == '3' ) { cnt++; }
  if ( c == '4' ) { cnt++; }
  if ( c == '5' ) { cnt++; }
  if ( c == '6' ) { cnt++; }
  if ( c == '7' ) { cnt++; }
  if ( c == '8' ) { cnt++; }
  if ( c == '9' ) { cnt++; }

  if ( cnt > 0 ){ check = 0; }
  else{ check = 1; };

  return( check ); 
  
};


int
char2int( char c )
{
  // Can be (int)c-48 but this 
  // implememtnation is safer
  // and more portable

  int Int;
  if ( c == '0' ) { Int = 0; }
  if ( c == '1' ) { Int = 1; }
  if ( c == '2' ) { Int = 2; }
  if ( c == '3' ) { Int = 3; }
  if ( c == '4' ) { Int = 4; }
  if ( c == '5' ) { Int = 5; }
  if ( c == '6' ) { Int = 6; }
  if ( c == '7' ) { Int = 7; }
  if ( c == '8' ) { Int = 8; }
  if ( c == '9' ) { Int = 9; }

  return( Int ); 
  
};




    int
    isStringDelim
    (
      char c
    )
    {
      int check;
      int cnt=0;
      if ( c == '"' ) { cnt++; }
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      return( check ); 
    };

int
isSeparator( char c )
{
  int check;

  if ( c == '.' ){ check=0; }
  else { check = 1; };

  return( check );

};

int
isPlus( char c )
{

  int check;
  int cnt=0;

  if ( c == '+' ) { cnt++; }

  if ( cnt > 0 ){ check = 0; }
  else{ check = 1; };

  return( check ); 

};



int
isMinus( char c )
{

  int check;
  int cnt=0;

  if ( c == '-' ) { cnt++; }

  if ( cnt > 0 ){ check = 0; }
  else{ check = 1; };

  return( check ); 

};


int
isSpace( char c )
{

  int check;
  int cnt=0;

  if ( c == ' ' ) { cnt++; }

  if ( cnt > 0 ){ check = 0; }
  else{ check = 1; };

  return( check ); 

};

int
isE( char c )
{

  int check;
  int cnt=0;

  if ( c == 'E' ) { cnt++; }
  if ( c == 'e' ) { cnt++; }

  if ( cnt > 0 ){ check = 0; }
  else{ check = 1; };

  return( check ); 

};


    int
    isStringTerminator
    (
      char c
    )
    {
      int check;
      int cnt=0;
      if ( c == 0 ) { cnt++; }
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      return( check ); 
    };




    memorySortingStrategy
    readMemorySortingStrategy
    (
      int*        index,
      const char* var,
            char* errMsg,
             int* errStatus
    )
    {

      int sign = 0;
      int expSign = 0;

      unsigned long nB = 0;
      unsigned long nF = 0;
      unsigned long nE = 0;

      unsigned long base = 0;
      unsigned long frac = 0;
      unsigned long exp = 0;
      unsigned long div = 0;

      double dnum;
      int c;

      char state = 0;
      char old_state = 0;
      int loop = 0;
      memorySortingStrategy retValue=memorySortingStrategy::last;
      std::vector<char> data;
      while( loop == 0 )
      {
          c = var[*index];
          printf( " - Character: %c\n", c );
          printf( " - State:     %d\n", state );
          printf( " - Index:     %d\n", *index );
          printf( "---------------------------\n" );
          (*index)++;
          if
          (
            isStringTerminator(c) == 0 &&
            state                 != 2 &&
            state                 != 3
          )
          {
            printf( "ERROR:: not valid \"memorySortingStrategy\"\n" );
            abort();
            // printf( "Esco dio can\n" );
            // loop = 1;
            // continue;
          }
          else
          {
            switch (state)
            {
              // Loop through spaces
              case (0):
              {
                if ( isStringDelim(c) == 0 )
                {
                  old_state=state;
                  state = 1;
                }
                else
                {
                  old_state = state;
                  state=3;
                }
                break;
              }
              case (1):
              {
                if ( isStringDelim(c) == 0 )
                {
                  (*index)--;
                  data.push_back(0);
                  old_state=state;
                  state = 2;
                }
                else
                {
                  data.push_back(c);
                }
                break;
              }
              case (2):
              {
                bool found=false;
                if (strcmp( &data[0], "ell" )==0  ){ found=true; retValue=memorySortingStrategy::ell; };
                if (strcmp( &data[0], "standard" )==0  ){ found=true; retValue=memorySortingStrategy::standard; };
                data.clear();
                if ( found==false )
                {
                  (*index)--;
                  old_state = state;
                  state=3;
                }
                else
                {
                  loop=1;
                }
                break;
              }
              case (3):
              {
                printf( "ERROR:: not valid \"memorySortingStrategy\"\n" );
                abort();
                break;
              }
            }
          }
      }
      return(retValue);
    };




    cellSortingStrategy
    readCellSortingStrategy
    (
      int*        index,
      const char* var,
            char* errMsg,
             int* errStatus
    )
    {

      int sign = 0;
      int expSign = 0;

      unsigned long nB = 0;
      unsigned long nF = 0;
      unsigned long nE = 0;

      unsigned long base = 0;
      unsigned long frac = 0;
      unsigned long exp = 0;
      unsigned long div = 0;

      double dnum;
      int c;

      char state = 0;
      char old_state = 0;
      int loop = 0;
      cellSortingStrategy retValue=cellSortingStrategy::last;
      std::vector<char> data;
      while( loop == 0 )
      {
          c = var[*index];
          printf( " - Character: %c\n", c );
          printf( " - State:     %d\n", state );
          printf( " - Index:     %d\n", *index );
          printf( "---------------------------\n" );
          (*index)++;
          if
          (
            isStringTerminator(c) == 0 &&
            state                 != 2 &&
            state                 != 3
          )
          {
            printf( "ERROR:: not valid \"memorySortingStrategy\"\n" );
            abort();
            // printf( "Esco dio can\n" );
            // loop = 1;
            // continue;
          }
          else
          {
            switch (state)
            {
              // Loop through spaces
              case (0):
              {
                if ( isStringDelim(c) == 0 )
                {
                  old_state=state;
                  state = 1;
                }
                else
                {
                  old_state = state;
                  state=3;
                }
                break;
              }
              case (1):
              {
                if ( isStringDelim(c) == 0 )
                {
                  (*index)--;
                  data.push_back(0);
                  old_state=state;
                  state = 2;
                }
                else
                {
                  data.push_back(c);
                }
                break;
              }
              case (2):
              {
                bool found=false;
                if (strcmp( &data[0], "bandwidth" )==0     ){ found=true; retValue=cellSortingStrategy::bandwidth; };
                if (strcmp( &data[0], "antiBandwidth" )==0 ){ found=true; retValue=cellSortingStrategy::antiBandwidth; };
                if (strcmp( &data[0], "sortingIndex" )==0  ){ found=true; retValue=cellSortingStrategy::sortingIndex; };
                data.clear();
                if ( found==false )
                {
                  (*index)--;
                  old_state = state;
                  state=3;
                }
                else
                {
                  loop=1;
                }
                break;
              }
              case (3):
              {
                printf( "ERROR:: not valid \"memorySortingStrategy\"\n" );
                abort();
                break;
              }
            }
          }
      }
      return(retValue);
    };



    double
    readDouble
    (
      int*        index,
      const char* var,
            char* errMsg,
             int* errStatus
    )
    {

      int sign = 0;
      int expSign = 0;

      unsigned long nB = 0;
      unsigned long nF = 0;
      unsigned long nE = 0;

      unsigned long base = 0;
      unsigned long frac = 0;
      unsigned long exp = 0;
      unsigned long div = 0;

      double dnum;
      int c;

      char state = 0;
      int loop = 0;
      while( loop == 0 )
      {
          c = var[*index];
          printf( " - Character: %c\n", c );
          printf( " - State:     %d\n", state );
          printf( " - Index:     %d\n", *index );
          printf( "---------------------------\n" );
          (*index)++;
          if
          (
            isStringTerminator(c) == 0 &&
            state                 != 3 &&
            state                 != 5 &&
            state                 != 6
          )
          {
            loop = 1;
            continue;
          }
          else
          {
            switch (state)
            {
              // Loop through spaces
              case (0):
              { 
                if ( isSpace(c) != 0 )
                {
                  if ( isPlus(c) == 0 )
                  {
                    sign  = +1;
                    nB    = 0;
                    nF    = 0;
                    base  = 0;
                    frac  = 0;
                    div   = 0;
                    state = 1;
                  }
                  else if ( isMinus(c) == 0 ) 
                  {
                    sign  = -1;
                    nB    = 0;
                    nF    = 0;
                    base  = 0;
                    frac  = 0;
                    div   = 0;
                    state = 1;
                  }
                  else if ( isDigit(c) == 0 )
                  {
                    sign  = +1;
                    nB    = 0;
                    nF    = 0;
                    base  = 0;
                    frac  = 0;
                    div   = 0;
                    (*index)--;
                    state = 1;
                  }
                  else if ( isSeparator(c) == 0 )
                  {
                    sign  = +1;
                    nB    = 1;
                    nF    = 0;
                    base  = 0;
                    frac  = 0;
                    div   = 1;
                    state = 3;
                  }
                  else if ( isE(c) == 0 )
                  {
                    sign    = +1;
                    nB      = 1;
                    nF      = 1;
                    base    = 1;
                    frac    = 0;
                    div     = 1;
                    state   = 4;
                  }
                  else 
                  {
                    printf( "3\n" );
                    state = 254;
                  }; 
                }
                else
                {
                  state = 0;
                };
                break;
              };
              // Read positive number
              case (1):
              {
                if ( isDigit(c) == 0 )
                {
                  nB++;
                  base  = base*10+char2int(c);
                  state = 1;
                }
                else if ( isSeparator(c) == 0 )
                {
                  nF    = 0;
                  frac  = 0;
                  div   = 1;
                  state = 3;
                }
                else if ( isE(c) == 0 )
                {
                  nF    = 1;
                  frac  = 0;
                  div   = 1;
                  state = 4;
                }
                else
                {
                  // printf( "2\n" );
                  state = 254;
                };
                break;
              };
              case (3):
              {
                if ( isDigit(c) == 0 )
                {
                  nF++;
                  frac  = frac*10+char2int(c);
                  div  *= 10;
                  state = 3;
                }
                else if ( isE(c) == 0 )
                {           
                  nE      = 0;
                  expSign = +1;
                  exp     = 0;
                  state   = 4;
                }
                else
                {
                  (*index)--;
                  state = 5;
                };
                break;
              };
              case (4):
              {
                if ( isPlus( c ) == 0 )
                {
                  expSign = +1;
                  state   = 5;
                }
                else if ( isMinus( c ) == 0 )
                {
                  expSign = -1;
                  state = 5;
                }
                else if ( isDigit( c ) == 0 )
                {
                  expSign = +1;
                  nE  = 0;
                  exp = 0;
                  (*index)--;
                  state = 5;
                }
                else
                {
                  // printf( "1\n" );
                  state = 254;
                };
                break;
              };
              case (5):
              {
                if ( isDigit(c) == 0 )
                {
                  nE++;
                  exp   = exp*10+char2int(c);
                  state = 5;
                }
                else
                {
                  (*index)--;
                  state = 6;
                };
                break;
              };
              case (6):
              {
                (*index)--;
                loop  = 1;
                unsigned long scal = pow( 10, exp );
                dnum = sign*( (double)base + ((double)frac)/div );
                if ( expSign > 0 )
                {
                  dnum *= scal;
                }
                else
                {
                  dnum /= scal;
                }
                loop = 1;
                // printf( "dnum: %lf\n", dnum );
                break;
              };
              default:
              {
                printf( "ERROR:: not a float\n" );
                abort();
              };
            };              
          };
      };
      return(dnum);
    };



    int
    readInteger
    (
      int*        index,
      const char* var,
            char* errMsg,
             int* errStatus
    )
    {

      int sign = 0;
      int expSign = 0;

      unsigned long nB = 0;
      unsigned long nF = 0;
      unsigned long nE = 0;

      unsigned long base = 0;
      unsigned long frac = 0;
      unsigned long exp = 0;
      unsigned long div = 0;

      int dnum;
      int c;

      char state = 0;
      int loop = 0;
      while( loop == 0 )
      {
          c = var[*index];
          // printf( " - Character: %c\n", c );
          // printf( " - State:     %d\n", state );
          // printf( " - Index:     %d\n", *index );
          // printf( "---------------------------\n" );
          (*index)++;
          if
          (
            isStringTerminator(c) == 0 &&
            state                 != 5 &&
            state                 != 6
          )
          {
            loop = 1;
            continue;
          }
          else
          {
            switch (state)
            {
              // Loop through spaces
              case (0):
              { 
                if ( isSpace(c) != 0 )
                {
                  if ( isPlus(c) == 0 )
                  {
                    sign  = +1;
                    nB    = 0;
                    nF    = 0;
                    base  = 0;
                    frac  = 0;
                    div   = 0;
                    state = 1;
                  }
                  else if ( isMinus(c) == 0 ) 
                  {
                    sign  = -1;
                    nB    = 0;
                    nF    = 0;
                    base  = 0;
                    frac  = 0;
                    div   = 0;
                    state = 1;
                  }
                  else if ( isDigit(c) == 0 )
                  {
                    sign  = +1;
                    nB    = 0;
                    nF    = 0;
                    base  = 0;
                    frac  = 0;
                    div   = 0;
                    (*index)--;
                    state = 1;
                  }
                  //  else if ( isSeparator(c) == 0 )
                  //  {
                  //    sign  = +1;
                  //    nB    = 1;
                  //    nF    = 0;
                  //    base  = 0;
                  //    frac  = 0;
                  //    div   = 1;
                  //    state = 3;
                  //  }
                  else if ( isE(c) == 0 )
                  {
                    sign    = +1;
                    nB      = 1;
                    nF      = 1;
                    base    = 1;
                    frac    = 0;
                    div     = 1;
                    state   = 4;
                  }
                  else 
                  {
                    state = 254;
                  }; 
                }
                else
                {
                  state = 0;
                };
                break;
              };
              // Read positive number
              case (1):
              {
                if ( isDigit(c) == 0 )
                {
                  nB++;
                  base  = base*10+char2int(c);
                  state = 1;
                }
                // else if ( isSeparator(c) == 0 )
                // {
                //  nF    = 0;
                //  frac  = 0;
                //  div   = 1;
                //  state = 3;
                //}
                else if ( isE(c) == 0 )
                {
                  nF    = 1;
                  frac  = 0;
                  div   = 1;
                  state = 4;
                }
                else
                {
                  state = 254;
                };
                break;
              };
              case (3):
              {
                if ( isDigit(c) == 0 )
                {
                  nF++;
                  frac  = frac*10+char2int(c);
                  div  *= 10;
                  state = 3;
                }
                else if ( isE(c) == 0 )
                {           
                  nE      = 0;
                  expSign = +1;
                  exp     = 0;    
                  state   = 4;
                }
                else
                {
                  state = 254;
                };
                break;
              };
              case (4):
              {
                if ( isPlus( c ) == 0 )
                {
                  expSign = +1;
                  state   = 5;
                }
                //else if ( isMinus( c ) == 0 )
                //{
                //  expSign = -1;
                //  state = 5;
                //}
                else if ( isDigit( c ) == 0 )
                {
                  expSign = +1;
                  nE  = 0;
                  exp = 0;
                  (*index)--;
                  state = 5;
                }
                else
                {
                  state = 254;
                };
                break;
              };
              case (5):
              {
                if ( isDigit(c) == 0 )
                {
                  nE++;
                  exp   = exp*10+char2int(c);
                  state = 5;
                }
                else
                {
                  (*index)--;
                  state = 6;
                };
                break;
              };
              case (6):
              {
                (*index)--;
                loop  = 1;
                unsigned long scal = pow( 10, exp );
                dnum = sign*base;
                dnum *= scal;
                loop=1;
                break;
              };
              default:
              {
                printf( "ERROR:: not an integer\n" );
                abort();
              };
            };
          };
      };
      return(dnum);
    };




int
main()
{

   const char* name="\"sortingIndex\" 0.123456";
   char errMsg[1024];
   int  errStat;
   int state = 0;
   int loop = 0;
   int index=0;
   char c;
   while (loop == 0)
   {
        c = name[index];
        index++;
        // printf( "Sto leggendo %c, %c \n", c, state );
        if ( isStringTerminator(c) == 0 )
        {
          loop = 1;
          continue;
        }
        else
        {
          switch (state)
          {
            // Loop through spaces
            case (0):
            {
              if ( isDigit(c)       == 0 ||
                   isPlus(c)        == 0 ||
                   isMinus(c)       == 0 ||
                   isSeparator(c)   == 0 )
              {
                index--;
                state = 2; 
              }
              else if ( isStringDelim(c) == 0 )
              {
                index--;
                state = 1; 
              }
              break;
            }
            case (1):
            {
              printf( "%s\n", name );
              printf( "%d\n", index );
              index--;
              // double tmp = readDouble( &index, name, &errMsg[0], &errStat );
              // int tmp = readInteger( &index, name, &errMsg[0], &errStat );
              cellSortingStrategy tmp = readCellSortingStrategy(&index, name, &errMsg[0], &errStat );

              printf( "%10d\n", tmp );
              state = 0;
              break;
            }
            case (2):
            {
              index--;
              printf( "name: %c\n", name[index] );
              double tmp = readDouble( &index, name, &errMsg[0], &errStat );
              // int tmp = readInteger( &index, name, &errMsg[0], &errStat );
              // cellSortingStrategy tmp = readCellSortingStrategy(&index, name, &errMsg[0], &errStat );
              printf( "Il risultato è: %10lf\n", tmp );
              state = 0;
              break;
            };
            
          };
        }; 
   };

   return(0);
}
