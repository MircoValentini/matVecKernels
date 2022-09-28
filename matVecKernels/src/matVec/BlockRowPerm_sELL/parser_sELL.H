#ifndef __PARSE_ELL_H__
#define __PARSE_ELL_H__


/*
 * System include
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>

/*
 * Local includes
 */
#include "BlkRowPerm_sELL.H"


class parseELL
{
  private:

//     idx_k       m_kind;
    idx_k       m_nBlocks;
    idx_k       m_kernelFlavour;
    idx_k       m_AVXsize;
    double      m_scale;
    double      m_limitScale;
    char        m_kernelName[1024];
    bool        m_verbose;
    bool        m_lastParseStatus;

    memorySortingStrategy m_memorySortingStrategy;
    cellSortingStrategy   m_cellSortingStrategy;

    char m_errMsg[4096];
    int  m_errStat;
    int  m_errLine;

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


    int
    isComma
    (
      char c
    )
    {
      
      int check;
      int cnt=0;
      if ( c == ',' ) { cnt++; }
      if ( cnt > 0 ){ check = 0; }
      else{ check = 1; };
      
      return( check ); 
    };



    memorySortingStrategy
    readMemorySortingStrategy
    (
      idx_k*      index,
      const char* var,
            char* errMsg,
             int* errStatus,
             int* errLine,
             bool verbose
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
      *errStatus = 0;
      char state = 0;
      char old_state = 0;
      int loop = 0;
      memorySortingStrategy retValue=memorySortingStrategy::last;
      std::vector<char> data;
      while( loop == 0 )
      {
          c = var[*index];
          if (verbose)
          {
            printf( " - readMemorySortingStrategy: Character: %c\n", c );
            printf( " - readMemorySortingStrategy: State:     %d\n", state );
            printf( " - readMemorySortingStrategy: Index:     %d\n", *index );
            printf( "---------------------------\n" );
          }
          (*index)++;
          if
          (
            isStringTerminator(c) == 0 &&
            state                 != 2 &&
            state                 != 3
          )
          {
            strcpy( errMsg, "ERROR: String terminated before parsing" );
            *errStatus = 401;
            *errLine = __LINE__;
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
                  strcpy( errMsg, "ERROR: Invalid name for cellSorting" );
                  *errStatus = 400;
                  *errLine = __LINE__;
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
                loop=1;
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
      idx_k*      index,
      const char* var,
            char* errMsg,
             int* errStatus,
             int* errLine,
             bool verbose
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
      *errStatus = 0;
      cellSortingStrategy retValue=cellSortingStrategy::last;
      std::vector<char> data;
      while( loop == 0 )
      {
          c = var[*index];
          if ( verbose )
          {
            printf( " - readCellSortingStrategy: Character: %c\n", c );
            printf( " - readCellSortingStrategy: State:     %d\n", state );
            printf( " - readCellSortingStrategy: Index:     %d\n", *index );
            printf( "---------------------------\n" );
          }
          (*index)++;
          if
          (
            isStringTerminator(c) == 0 &&
            state                 != 2 &&
            state                 != 3
          )
          {
            strcpy( errMsg, "ERROR: string terminated before parsing" );
            *errStatus = 301;
            *errLine = __LINE__;
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
                  strcpy( errMsg, "ERROR: Invalid name for cellSorting" );
                  *errStatus = 300;
                  *errLine = __LINE__;
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
                loop=0;
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
      idx_k*      index,
      const char* var,
            char* errMsg,
             int* errStatus,
             int* errLine,
             bool verbose
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

      *errStatus = 0;
      char state = 0;
      int loop = 0;
      while( loop == 0 )
      {
          c = var[*index];
          if (verbose)
          {
            printf( " - readDouble: Character: %c\n", c );
            printf( " - readDouble: State:     %d\n", state );
            printf( " - readDouble: Index:     %d\n", *index );
            printf( "---------------------------\n" );
          }
          (*index)++;
          if
          (
            isStringTerminator(c) == 0 &&
            state                 != 5 &&
            state                 != 6
          )
          {
            strcpy( errMsg, "ERROR: String terminated before end of parsing" );
            *errStatus = 202;
            *errLine = __LINE__;
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
                    strcpy( errMsg, "ERROR: Not valid double start" );
                    *errStatus = 200;
                    *errLine = __LINE__;
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
                  (*index)--;
                  state = 6;
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
                  (*index)--;
                  state = 6;
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
                  strcpy( errMsg, "ERROR: expected \"+\",\"-\" or digit" );
                  *errStatus = 201;
                  *errLine = __LINE__;
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
                if ( div != 0 )
                {
                  dnum = sign*( (double)base + ((double)frac)/div );
                }
                else
                {
                  dnum = sign*( (double)base );
                }
                if ( expSign > 0 )
                {
                  dnum *= scal;
                }
                else
                {
                  dnum /= scal;
                }
                loop = 1;
                break;
              };
              default:
              {
                loop=1;
              };
            };              
          };
      };
      return(dnum);
    };



    int
    readInteger
    (
      idx_k*      index,
      const char* var,
            char* errMsg,
             int* errStatus,
             int* errLine,
             bool verbose
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
      *errStatus = 0;
      *errLine = 0;
      while( loop == 0 )
      {
          c = var[*index];
          if (verbose)
          {
            printf( " - readInteger: Character: %c\n", c );
            printf( " - readInteger: State:     %d\n", state );
            printf( " - readInteger: Index:     %d\n", *index );
            printf( "---------------------------\n" );
          }
          (*index)++;
          if
          (
            isStringTerminator(c) == 0 &&
            state                 != 5 &&
            state                 != 6
          )
          {
            strcpy( errMsg, "ERROR: String terminated before end of parsing" );
            *errStatus = 102;
            *errLine = __LINE__;
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
                    strcpy( errMsg, "ERROR: Not valid integer start" );
                    *errStatus = 100;
                    *errLine = __LINE__;
                    state = 254;
                  }; 
                }
                else
                {
                  state = 0;
                };
                break;
              };
              case (1):
              {
                if ( isDigit(c) == 0 )
                {
                  nB++;
                  base  = base*10+char2int(c);
                  state = 1;
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
                  (*index)--;
                  state = 6;
                };
                break;
              };
#if 0
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
#endif
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
                  strcpy( errMsg, "ERROR: Expecter \"+\" or digit" );
                  *errStatus = 101;
                  *errLine = __LINE__;
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
                loop = 1;
              };
            };
          };
      };
      return(dnum);
    };


  public:

    parseELL
    ( ):
      m_verbose(true)
    {
      memset( &m_kernelName[0],0,1024);
      memset( &m_errMsg[0],    0,4096);
      m_lastParseStatus=false;
      m_errStat;
      m_errLine;
      return;
    };


    bool lastParserStatus() const { return( m_lastParseStatus ); };


    bool
    readConfiguration
    (
      const char* name,
      bool        verbose
    )
    {
      // Local variables
      int idx=0;
      int state = 0;
      int old_state = 0;
      int loop = 0;
      char c;
      int context=0;
      std::vector<char> value;
      memset(&m_errMsg[0],0,4096);
      m_errStat=0;
      m_errLine;
      bool parserSuccess=false;
      idx_k index=0;
      while (loop == 0)
      {
        c = name[index];
        index++;
        if ( verbose )
        {
          printf( "State = |%d|, |%c|, |%d|\n", state, c, idx );
        };
        if
        (
          isStringTerminator(c) == 0  && 
                          state != 50
        )
        {
          sprintf( &m_errMsg[0], "ERROR: string finished before end of parsing" );
          m_errStat=1;
          m_errLine=__LINE__;
          parserSuccess=false;
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
                isLetter(c) == 0 ||
                isletter(c) == 0
              )
              {
                index--;
                value.resize(0);
                old_state = state;
                state = 1;
              }
              else
              {
                sprintf( &m_errMsg[0], "ERROR: expected kernelName valid start character and instead found %c", c );
                m_errStat=2;
                m_errLine=__LINE__;
                old_state = state;
                state = 60;
              }
              break;
            }
            // Read the kernel name
            case (1):
            {
              // Read the test name
              if ( isOpenAngular(c) == 0 )
              {
                value.push_back(0);
                strcpy( m_kernelName, &value[0] );
                if (verbose)
                {
                  printf( "Kernel Name: %s\n", &value[0] );
                }
                index--;
                value.clear();
                old_state = state;
                state = 2;
              }
              else if
              (
                isLetter(c)  == 0 ||
                isletter(c)  == 0 ||
                isDigit(c)   == 0 ||
                isSpecial(c) == 0
              )
              {
                value.push_back(c);
                state = 1;
              }
              else
              {
                sprintf( &m_errMsg[0], "ERROR: expected kernelName valid character and instead found %c", c );
                m_errStat=3;
                m_errLine=__LINE__;
                old_state = state;
                state = 60;
              }
              break;
            }
            /*
             * Read kernel flavour
             */
            case (2):
            {
              if ( isSpace(c) != 0 )
              {
                if ( isOpenAngular(c) == 0 )
                {
                  old_state = state;
                  state = 4;
                }
                else
                {
                  sprintf( &m_errMsg[0], "ERROR: expected \">\" and instead found %c", c );
                  m_errStat=4;
                  m_errLine=__LINE__;
                  old_state = state;
                  state = 60;
                }
              }
              break;
            }
            case (4):
            {
              index--;
              m_kernelFlavour = readInteger( &index, name, &m_errMsg[0], &m_errStat, &m_errLine, verbose );
              if ( verbose )
              {
                printf( "KernelFlavour: %d\n", m_kernelFlavour );
              }
              if ( m_errStat != 0 )
              {
                old_state = state;
                state = 60;
              }
              else
              {
                old_state = state;
                state = 5;
              }
              break;
            }
            /*
             * Read kernel parameters (1 parameters kernel)
             */
            case (5):
            {
              if ( isSpace(c) != 0 )
              {
                if ( isClosedAngular(c) == 0 )
                {
                  old_state = state;
                  state = 6;
                }
                else
                {
                  sprintf( &m_errMsg[0], "ERROR: expected \">\" and instead found %c", c );
                  m_errStat=5;
                  m_errLine=__LINE__;
                  old_state = state;
                  state = 60;
                }
              }
              break;
            }
            // Check start of kernel parameters
            case (6):
            {
              if ( isSpace(c) != 0 )
              {
                if ( isOpenParen(c) == 0 )
                {
                  old_state = state;
                  state = 7;
                }
                else
                {
                  sprintf( &m_errMsg[0], "ERROR: expected \"(\" and instead found %c", c );
                  m_errStat=6;
                  m_errLine=__LINE__;
                  old_state = state;
                  state = 60;
                }
              }
              break;
            }
            // Check start of kernel parameters
            case (7):
            {
              if ( isSpace(c) != 0 )
              {
                if ( isDigit(c) == 0 )
                {
                  index--;
                  old_state = state;
                  state = 8;
                }
                else
                {
                  sprintf( &m_errMsg[0], "ERROR: expected digit and instead found %c", c );
                  m_errStat=7;
                  m_errLine=__LINE__;
                  old_state = state;
                  state = 60;
                }
              }
              break;
            }
            // Check start of kernel parameters
            case (8):
            {
              index--;
              m_AVXsize = readInteger( &index, name, &m_errMsg[0], &m_errStat, &m_errLine, verbose );
              if ( verbose )
              {
                printf( "AVXsize: %d\n", m_AVXsize );
              }
              if ( m_errStat != 0 )
              {
                old_state = state;
                state = 60;
              }
              else
              {
                old_state = state;
                state = 9;
              }
              break;
            }
            case (9):
            {
              if ( isSpace(c) != 0 )
              {
                if ( isComma(c) == 0 )
                {
                  switch ( m_kernelFlavour )
                  {
                    case (1):
                    { 
                      old_state = state;
                      state = 22;
                      break;
                    }
                    case (2):
                    {
                      old_state = state;
                      state = 30;
                      break;
                    }
                    default:
                    {
                      old_state = state;
                      state = 60;
                    }
                  }
                }
                else
                {
                  sprintf( &m_errMsg[0], "ERROR: expected comma and instead found %c", c );
                  m_errStat=8;
                  m_errLine=__LINE__;
                  old_state = state;
                  state = 60;
                }
              }
              break;
            }
            /*
             * Read kernel parameters (1 parameters kernel)
             */
            // Check start of kernel parameters
            case (22):
            {
              if ( isSpace(c) != 0 )
              {
                if 
                (
                  isDigit(c) == 0
                )
                {
                  index--;
                  old_state = state;
                  state = 23;
                }
                else
                {
                  sprintf( &m_errMsg[0], "ERROR: expected digit and instead found %c", c );
                  m_errStat=9;
                  m_errLine=__LINE__;
                  old_state = state;
                  state = 60;
                }
              }
              break;
            }
            // Check start of kernel parameters
            case (23):
            {
              index--;
              m_nBlocks = readInteger( &index, name, &m_errMsg[0], &m_errStat, &m_errLine, verbose );
              if ( verbose )
              {
                printf( "nBlocks: %d\n", m_nBlocks );
              }
              if ( m_errStat != 0 )
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
              if ( isSpace(c) != 0 )
              {
                if ( isComma(c) == 0 )
                {
                  old_state = state;
                  state = 25;
                }
                else
                {
                  sprintf( &m_errMsg[0], "ERROR: expected comma and instead found %c", c );
                  m_errStat=10;
                  m_errLine=__LINE__;
                  old_state = state;
                  state = 60;
                }
              }
              break;
            }
            case (25):
            {
              if ( isSpace(c) != 0 )
              {
                if ( isStringDelim(c) == 0 )
                {
                  index--;
                  old_state = state;
                  state = 40;
                }
                else
                {
                  sprintf( &m_errMsg[0], "ERROR: expected string delim and instead found %c", c );
                  m_errStat=11;
                  m_errLine=__LINE__;
                  old_state = state;
                  state = 60;
                }
              }
              break;
            }
            /*
             * Read kernel parameters (1 parameters kernel)
             */
            // Check start of kernel parameters
            case (30):
            {
              if ( isSpace(c) != 0 )
              {
                if
                (
                  isDigit(c)     == 0 ||
                  isSeparator(c) == 0 ||
                  isPlus(c)      == 0 ||
                  isMinus(c)     == 0
                )
                {
                  index--;
                  old_state = state;
                  state = 31;
                }
                else
                {
                  sprintf( &m_errMsg[0], "ERROR: expected double and instead found %c", c );
                  m_errStat=12;
                  m_errLine=__LINE__;
                  old_state = state;
                  state = 60;
                }
              }
              break;
            }
            // Check start of kernel parameters
            case (31):
            {
              index--;
              m_scale = readDouble( &index, name, &m_errMsg[0], &m_errStat, &m_errLine, verbose );
              if ( verbose )
              {
                printf( "Scale: %lf\n", m_scale );
              }
              if ( m_errStat != 0 )
              {
                old_state = state;
                state = 60;
              }
              else
              {
                old_state = state;
                state = 32;
              }
              break;
            }
            case (32):
            {
              if ( isComma(c) == 0 )
              {
                old_state = state;
                state = 33;
              }
              break;
            }
            case (33):
            {
              if ( isSpace(c) != 0 )
              {
                if
                (
                  isDigit(c)     == 0 ||
                  isSeparator(c) == 0 ||
                  isPlus(c)      == 0 ||
                  isMinus(c)     == 0
                )
                {
                  index--;
                  old_state = state;
                  state = 34;
                }
                else
                {
                  sprintf( &m_errMsg[0], "ERROR: expected double and instead found %c", c );
                  m_errStat=13;
                  m_errLine=__LINE__;
                  old_state = state;
                  state = 60;
                }
              }
              break;
            }
            // Check start of kernel parameters
            case (34):
            {
              index--;
              m_limitScale = readDouble( &index, name, &m_errMsg[0], &m_errStat, &m_errLine, verbose );
              if ( verbose )
              {
                printf( "limitScale: %lf\n", m_limitScale );
              }
              if ( m_errStat != 0 )
              {
                old_state = state;
                state = 60;
              }
              else
              {
                old_state = state;
                state = 35;
              }
              break;
            }
            case (35):
            {
              if ( isSpace(c) != 0 )
              {
                if ( isComma(c) == 0 )
                {
                  old_state = state;
                  state = 36;
                }
                else
                {
                  sprintf( &m_errMsg[0], "ERROR: expected comma and instead found %c", c );
                  m_errStat=14;
                  m_errLine=__LINE__;
                  old_state = state;
                  state = 60;
                }
              }
              break;
            }
            case (36):
            {
              if ( isSpace(c) != 0 )
              {
                if ( isStringDelim(c) == 0 )
                {
                  index--;
                  old_state = state;
                  state = 40;
                }
                else
                {
                  sprintf( &m_errMsg[0], "ERROR: expected string delim and instead found %c", c );
                  m_errStat=15;
                  m_errLine=__LINE__;
                  old_state = state;
                  state = 60;
                }
              }
              break;
            }
            case (40):
            {
              index--;
              m_cellSortingStrategy = 
              readCellSortingStrategy
              (
                  &index,
                  name,
                  m_errMsg,
                  &m_errStat,
                  &m_errLine,
                  verbose
              );
              if ( m_errStat == 0 )
              {
                old_state = state;
                state = 41;
              }
              else
              {
                old_state = state;
                state = 60;
              }
              break;
            }
            case (41):
            {
              if ( isSpace(c) != 0 )
              {
                if ( isComma(c) == 0 )
                {
                  old_state = state;
                  state = 42;
                }
                else
                {
                  sprintf( &m_errMsg[0], "ERROR: expected comma and instead found %c", c );
                  m_errStat=16;
                  m_errLine=__LINE__;
                  old_state = state;
                  state = 60;
                }
              }
              break;
            }
            case (42):
            {
              if ( isSpace(c) != 0 )
              {
                if ( isStringDelim(c) == 0 )
                {
                  index--;
                  old_state = state;
                  state = 43;
                }
                else
                {
                  sprintf( &m_errMsg[0], "ERROR: expected string delim and instead found %c", c );
                  m_errStat=17;
                  m_errLine=__LINE__;
                  old_state = state;
                  state = 60;
                }
              }
              break;
            }
            case (43):
            {
              index--;
              m_memorySortingStrategy = 
              readMemorySortingStrategy
              (
                  &index,
                  name,
                  m_errMsg,
                  &m_errStat,
                  &m_errLine,
                  verbose
              );
              if ( m_errStat == 0 )
              {
                index--;
                old_state = state;
                state = 44;
              }
              else
              {
                old_state = state;
                state = 60;
              }
              break;
            }
            case (44):
            {
              if ( isClosedParen(c) == 0 )
              {
                index--;
                old_state = state;
                state = 50;
              }
              break;
            }
            /*
             * Exit conditions
             */
            case (50):
            {
              /* Exit success*/
              loop = 1;
              parserSuccess=true;
              break;
            }
            case (60):
            {
              /* Exit error */
              if ( verbose )
              {
                printf( "ERROR PARSER\n" );
                printf( "------------\n" );
                printf( "Parser exit (old_stat=%d) at position %d, with status %d and message %s\n", old_state, index, m_errStat, m_errMsg );
              }
              parserSuccess=false;
              loop = 1;
            }
          }
        }
      }
      m_lastParseStatus=parserSuccess;
      // Exit point
      return(parserSuccess);
    };

    idx_k kernelFlavour() const { return(m_kernelFlavour); };

    memorySortingStrategy memorySorting() const { return(m_memorySortingStrategy); };
    cellSortingStrategy   cellSorting()   const { return(m_cellSortingStrategy); };

    idx_k  AVXsize()          const { return(m_AVXsize);    };
    double scaleFactor()      const { return(m_scale);      };
    double limitScaleFactor() const { return(m_limitScale); };
    idx_k  nBlocks()          const { return(m_nBlocks);    };

    idx_k       lastErrorIdx()  const { return(m_errStat); };
    idx_k       lastErrorLine() const { return(m_errLine); };
    const char* lastErrorMsg()  const { return(m_errMsg); };
    const char* kernelName()    const { return(m_kernelName); };

    blockConstructionKind getBlockConstructionKind() const 
    {
      blockConstructionKind tmp;
      switch (m_kernelFlavour)
      {
        case (1):
        {
          tmp = blockConstructionKind::oneParameter;
          break;
        }
        case (2):
        {
          tmp = blockConstructionKind::twoParameters;
          break;
        }
        default:
        {
          printf( "ERROR: wrong kernel flavour\n" );
          abort();
        }
      }
      return(tmp);
    };


    char* configurationString() const
    {
      char sortCellName[1024];
      char sortMemName[1024];
      char* tmp;
      tmp = (char*)malloc(4096);
      memset(sortCellName,0,1024);
      memset(sortMemName,0,1024);
      memset(tmp,0,4096);
      switch (m_cellSortingStrategy)
      {
        case (cellSortingStrategy::bandwidth):
        {
          strcpy( &sortCellName[0], "bandwidth" );
          break;
        }
        case (cellSortingStrategy::antiBandwidth):
        {
          strcpy( &sortCellName[0], "antiBandwidth" );
          break;
        }
        case (cellSortingStrategy::sortingIndex):
        {
          strcpy( &sortCellName[0], "sortingIndex" );
          break;
        }
        default:
        {
          printf( "Invalid enumerator cell\n" );
          abort();
        }
      }
      switch (m_memorySortingStrategy)
      {
        case (memorySortingStrategy::standard):
        {
          strcpy( &sortMemName[0], "standard" );
          break;
        }
        case (memorySortingStrategy::ell):
        {
          strcpy( &sortMemName[0], "ell" );
          break;
        }
        default:
        {
          printf( "Invalid enumerator memory\n" );
          abort();
        }
      }
      switch (m_kernelFlavour)
      {
        case (1):
        {
          sprintf( tmp, "%s<%d>(%d,%d,\"%s\",\"%s\")", m_kernelName, m_kernelFlavour, m_AVXsize,m_nBlocks, sortCellName, sortMemName );
          break;
        }
        case (2):
        {
          sprintf( tmp, "%s<%d>(%d,%f,%f,\"%s\",\"%s\")", m_kernelName, m_kernelFlavour, m_AVXsize, m_scale, m_limitScale, sortCellName, sortMemName );
          break;
        }
        default:
        {
          printf( "Invalid kernel flavour\n" );
          abort();
        }
      }
      return(tmp);
    }

};

#endif
