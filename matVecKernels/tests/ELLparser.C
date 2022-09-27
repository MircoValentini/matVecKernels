#include <stdio.h>

#define __enter_function__()
#define __exit_function__()

typedef idx_k unsigned int;

/**
 * @brief enumerator for cell sorting algorithms
 *
 */
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



class parseELL
{
  private:
    const char* m_name

    idx_k       m_kind;
    idx_k       m_nBlocks;
    double      m_scale;
    double      m_limitScale;

    memorySortingStrategy m_memorySortingStrategy;
    cellSortingStrategy   m_cellSortingStrategy;


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
    isDigit
    (
      char c
    )
    {
      
      int cnt=0;
      int check=0;
      if ( c == '0' ) { cnt++; };
      if ( c == '1' ) { cnt++; };
      if ( c == '2' ) { cnt++; };
      if ( c == '3' ) { cnt++; };
      if ( c == '4' ) { cnt++; };
      if ( c == '5' ) { cnt++; };
      if ( c == '6' ) { cnt++; };
      if ( c == '7' ) { cnt++; };
      if ( c == '8' ) { cnt++; };
      if ( c == '9' ) { cnt++; };
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      
      return(check);
    };


    int
    char2int
    (
      char c
    )
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
    isSpecial
    (
      char c
    )
    {
      
      int cnt=0;
      int check=0;
      if ( c == '_' ) { cnt++; };
      if ( c == '-' ) { cnt++; };
      if ( cnt > 0 ) check=0;
      if ( cnt == 0 ) check = 1;
      
      return(check);
    };


    int
    isSeparator
    (
      char c
    )
    {
      
      int check;
      if ( c == '.' ){ check=0; }
      else { check = 1; };
      
      return( check );
    };


    int
    isPlus
    (
      char c
    )
    {
      
      int check;
      int cnt=0;
      if ( c == '+' ) { cnt++; }
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      
      return( check ); 
    };


    int
    isMinus
    (
      char c
    )
    {
      
      int check;
      int cnt=0;
      if ( c == '-' ) { cnt++; }
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      
      return( check ); 
    };


    int
    isSpace
    (
      char c
    )
    {
      
      int check;
      int cnt=0;
      if ( c == ' ' ) { cnt++; }
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      
      return( check ); 
    };


    int
    isE
    (
      char c
    )
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
    isStar
    (
      char c
    )
    {
      
      int check;
      int cnt=0;
      if ( c == '*' ) { cnt++; }
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      
      return( check ); 
    };


    int
    isSlash
    (
      char c
    )
    {
      
      int check;
      int cnt=0;
      if ( c == '/' ) { cnt++; }
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      
      return( check ); 
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
    isOpenParen
    (
      char c
    )
    {
      
      int check;
      int cnt=0;
      if ( c == '(' ) { cnt++; }
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      
      return( check ); 
    };


    int
    isOpenBracket
    (
      char c
    )
    {
      
      int check;
      int cnt=0;
      if ( c == '{' ) { cnt++; }
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      
      return( check ); 
    };


    int
    isOpenAngular
    (
      char c
    )
    {
      
      int check;
      int cnt=0;
      if ( c == '<' ) { cnt++; }
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      
      return( check ); 
    };


    int
    isClosedParen
    (
      char c
    )
    {
      
      int check;
      int cnt=0;
      if ( c == ')' ) { cnt++; }
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      
      return( check ); 
    };


    int
    isClosedBracket
    (
      char c
    )
    {
      
      int check;
      int cnt=0;
      if ( c == '}' ) { cnt++; }
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      
      return( check ); 
    };


    int
    isClosedAngular
    (
      char c
    )
    {
      
      int check;
      int cnt=0;
      if ( c == '>' ) { cnt++; }
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      
      return( check ); 
    };


    int
    isNewLine
    (
      char c
    )
    {
      
      int check;
      int cnt=0;
      if ( c == '\n' ) { cnt++; }
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


    int
    isSemiColon
    (
      char c
    )
    {
      
      int check;
      int cnt=0;
      if ( c == ';' ) { cnt++; }
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      
      return( check ); 
    };


    int
    isColon
    (
      char c
    )
    {
      
      int check;
      int cnt=0;
      if ( c == ':' ) { cnt++; }
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
          //printf( " - Character: %c\n", c );
          //printf( " - State:     %d\n", state );
          //printf( " - Index:     %d\n", *index );
          //printf( "---------------------------\n" );
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
                  // printf( "0 %c\n", c );
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



    void
    readConfiguration
    (
      const char*    name,
      unsigned int** labelList,
      unsigned int*  nLabelList
    )
    {
      
      // Error check
      if (!fid)
      {
         printf( "ERROR: invalid file\n" );
         abort();
      };
      // main loop
      int idx=0;
      int state = 0;
      int old_state = 0;
      int loop = 0;
      char c;
      int context=0;
      std::vector<char> keyword;
      std::vector<char> value;
      idx_k index=0;
      while (loop == 0)
      {
        c = name[index];
        index++;
        if ( m_verbose ){ printf( "State = |%d|, |%c|, |%d|\n", state, c, idx ); };
        if ( cisStringTerminator(c) == 0 ) )
        {
          loop = 1;
          continue;
        }
        else
        {
          switch (state)
          {
            // Search the start of the kernel name
            case (0):
            {
              // Skip all invalid characters at the string begin
              if
              (
                isDigit(c)  == 0 ||
                isLetter(c) == 0 ||
                isletter(c) == 0 ||
              )
              {
                index--;
                old_state = state;
                state = 1;
              };
              break;
            }
            // Read the kernel name
            case (1):
            {
              // Read the test name
              if ( isOpenAngular(c) == 0 )
              {
                value.push_back(0);
                bool check = kernelName2kernelIndex( &value[0], &kernelId );
                if ( check )
                {
                  index--;
                  value.clear();
                  old_state = state;
                  state = 2;
                }
                else
                {
                  index--;
                  value.clear();
                  old_state = state;
                  state = 60;
                }
              }
              else if ( isStringTerminator(c) == 0 )
              {
                value.push_back(0);
                bool check = kernelName2kernelIndex( &value[0], &kernelId );
                if ( check )
                {
                  index--;
                  value.clear();
                  old_state = state;
                  state = 50;
                }
                else
                {
                  index--;
                  value.clear();
                  old_state = state;
                  state = 60;
                }
              }
              else
              {
                value.push_back(c);
                state = 1;
              };
              break;
            }


            /*
             * Read kernel flavour
             */
            case (2):
            {
              if ( isOpenAngular(c) == 0 )
              {
                old_state = state;
                state = 3;
              }
              else
              {
                old_state = state;
                state = 60;
              }
              break;
            }
            // Read the kernel flavour
            case (3):
            {
              if ( isDigit(c) == 0 )
              {
                value.push_back(c);
              }
              else
              {
                if ( isClosedAngular(c) == 0 )
                {
                  index--;
                  old_state = state;
                  state = 4;
                }
                else
                {
                  old_state = state;
                  state = 60;
                }
              }
              break;
            }
            // 
            case (4):
            {
              index--;
              value.push_back(0);
              kernelFlavour = atoi( &value[0] );
              value.clear();
              switch ( kernelFlavour )
              {
                case (0):
                {
                  old_state = state;
                  state = 20;
                  break
                }
                case (1):
                {
                  old_state = state;
                  state = 30;
                  break
                }
                default:
                {
                  old_state = state;
                  state = 60;
                }
              }
              break;
            }



            /*
             * Read kernel parameters (1 parameters kernel)
             */
            case (20):
            {
              if ( isClosedAngular(c) == 0 )
              {
                old_state = state;
                state = 21;
              }
              else
              {
                old_state = state;
                state = 60;
              }
              break;
            }
            // Check start of kernel parameters
            case (21):
            {
              if ( isOpenParen(c) == 0 )
              {
                old_state = state;
                state = 22;
              }
              else
              {
                // ErrorMsg
                // ErrorIdx
                old_state = state;
                state = 60;
              }
              break;
            }
            // Check start of kernel parameters
            case (22):
            {
              if ( isSpace(c) != 0 )
              {
                index--;
                old_state = state;
                state = 23;
              }
              break;
            }
            // Check start of kernel parameters
            case (23):
            {
              m_nBlocks = readInteger( &index, name, &errMsg[0], &errStat );
              if ( errStat == 0 )
              {
                old_state = state;
                state = 60;
              }
              else
              {
                old_state = state;
                state = 24;
              }
              break;
            }
            case (24):
            {
              if ( isComma(c) == 0 )
              {
                old_state = state;
                state = 25;
              }
              break;
            }
            case (25):
            {
              if ( isStringDelim(c) == 0 )
              {
                index--;
                old_state = state;
                state = 40;
              }
              break;
            }













            /*
             * Read kernel parameters (2 parameters kernel)
             */
            case (30):
            {
              if ( isClosedAngular(c) == 0 )
              {
                old_state = state;
                state = 31;
              }
              else
              {
                old_state = state;
                state = 60;
              }
              break;
            }
            // Check start of kernel parameters
            case (31):
            {
              if ( isOpenParen(c) == 0 )
              {
                old_state = state;
                state = 32;
              }
              else
              {
                // ErrorMsg
                // ErrorIdx
                old_state = state;
                state = 60;
              }
              break;
            }




            case (40):
            {
              break;
            }














            /*
             * Exit conditions
             */
            case (50):
            {
              /* Exit success*/
              loop = 1;
            }
            case (60):
            {
              /* Exit error */
              loop = 1;
            }
          }
        }
      }
      
      // Exit point
      return;
    };

  public:

    parseELL
    (
      const char* name
    ):
      m_name(name)
    {
      return;
    };

     

}


int
main()
{

  parseELL par;

  const char* name="BlockRowPerm<1>( 10, \"bandwidth\", \"ell\" )";



  /* Exit point */
  return(0);
};
