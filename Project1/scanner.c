/*scanner.c*/

//
// << scanner that reads characters from a python program and forms tokens >>
// << Joseph Zhao >>
// << Northwestern >>
// << CS 211 >>
// << Winter quarter >>
// 
// Starter code: Prof. Joe Hummel
//

#include <stdio.h>
#include <stdbool.h>  // true, false
#include <ctype.h>    // isspace, isdigit, isalpha
#include <string.h>   // strcmp
#include <assert.h>   // assert

#include "scanner.h"


//
// collect_identifier
//
// Given the start of an identifier, collects the rest into value
// while advancing the column number.
//
static void collect_identifier(FILE* input, int c, int* colNumber, char* value)
{
  assert(isalpha(c) || c == '_');  // should be start of an identifier

  int i = 0;

  while (isalnum(c) || c == '_')  // letter, digit, or underscore
  {
    value[i] = (char)c;  // store char
    i++;

    (*colNumber)++;  // advance col # past char

    c = fgetc(input);  // get next char
  }

  // at this point we found the end of the identifer, so put
  // that last char back for processing next:
  ungetc(c, input);

  // turn the value into a string:
  value[i] = '\0';  // build C-style string:

  return;
}

static int collect_int_or_real_literal(FILE *input, int c, int *colNumber, char *value, bool shift) 
{
  int i = 0;

  bool realLiteral = false;

  int count = -1;

  while (isdigit(c) || c == '.') 
  {
    if (c == '.') 
    {
      if (realLiteral == true) 
      {
        break;
      }
      realLiteral = true;
    }

    value[i] = (char)c;

    c = fgetc(input);

    (*colNumber)++;

    i++;
    count ++;
  }

  ungetc(c, input);

  value[i] = '\0';
  
  if (realLiteral && count >= 1)
  {
    return nuPy_REAL_LITERAL;
  }
  else if (realLiteral && count < 1) 
  {
    return nuPy_UNKNOWN;
  }
  else
  {
    return nuPy_INT_LITERAL;
  }
}

//id or keyword
//returns token id
static int id_or_keyword (char* value)
{
  char* keywords[] = {"and", "break", "continue", "def", "elif", "else", "False", "for", "if", "in", "is", "None", "not", "or", "pass", "return", "True", "while"};
  
  int N = 18;

  int index = -1;

  for (int i = 0; i < N; i++)
  {
    if (strcmp(value, keywords[i]) == 0)
    {
      index = i;
      break;
    }
  }

  if (index > -1)
  {
   return nuPy_KEYW_AND + index;
  }
  else
  {
    return nuPy_IDENTIFIER;
  }
}

//
// scanner_init
//
// Initializes line number, column number, and value before
// the start of processing the input stream.
//
void scanner_init(int* lineNumber, int* colNumber, char* value)
{
  assert(lineNumber != NULL);
  assert(colNumber != NULL);
  assert(value != NULL);

  *lineNumber = 1;
  *colNumber = 1;
  value[0] = '\0';  // empty string
}


//
// scanner_nextToken
//
// Returns the next token in the given input stream, advancing the line
// number and column number as appropriate. The token's string-based 
// value is returned via the "value" parameter. For example, if the 
// token returned is an integer literal, then the value returned is
// the actual literal in string form, e.g. "456". For an identifer,
// the value is the identifer itself, e.g. "print" or "y". For a 
// string literal such as 'hi class', the value is the contents of the 
// string literal without the quotes.
//
struct Token scanner_nextToken(FILE* input, int* lineNumber, int* colNumber, char* value)
{
  assert(input != NULL);
  assert(lineNumber != NULL);
  assert(colNumber != NULL);
  assert(value != NULL);

  struct Token T;

  //
  // repeatedly input characters one by one until a token is found:
  //
  while (true) {
    //
    // Get the next input character:
    //

    int c = fgetc(input);

    //
    // Let's see what we have...
    //

    if (c == EOF) // no more input, return EOS:
    {
      T.id = nuPy_EOS;
      T.line = *lineNumber;
      T.col = *colNumber;

      value[0] = '$';

      value[1] = '\0';

      return T;

    } 
    else if (c == '$') // this is also EOS
    {
      T.id = nuPy_EOS;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; // advance col # past char

      value[0] = '$';

      value[1] = '\0';

      return T;

    } 
    else if (c == '\n') // end of line, keep going:
    {
      (*lineNumber)++; // next line, restart column:
      *colNumber = 1;
      continue;
    } 
    else if (isspace(c)) // other form of whitespace, skip:
    {
      (*colNumber)++; // advance col # past char
      continue;
    } 
    else if (c == '(') 
    {
      T.id = nuPy_LEFT_PAREN;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; // advance col # past char

      value[0] = (char)c;

      value[1] = '\0';

      return T;
    } 
    else if (c == ')') 
    {
      T.id = nuPy_RIGHT_PAREN;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; // advance col # past char

      value[0] = (char)c;

      value[1] = '\0';

      return T;
    } 
    else if (c == '{') 
    {
      T.id = nuPy_LEFT_BRACE;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; // advance col # past char

      value[0] = (char)c;

      value[1] = '\0';

      return T;
    } 
    else if (c == '}') 
    {
      T.id = nuPy_RIGHT_BRACE;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; // advance col # past char

      value[0] = (char)c;

      value[1] = '\0';

      return T;

    } 
    else if (c == '[') 
    {
      T.id = nuPy_LEFT_BRACKET;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; // advance col # past char

      value[0] = (char)c;

      value[1] = '\0';

      return T;

    } 
    else if (c == ']') 
    {
      T.id = nuPy_RIGHT_BRACKET;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; // advance col # past char

      value[0] = (char)c;

      value[1] = '\0';
 
      return T;
    }
    else if (isalpha(c) || c == '_') 
    {
      //
      // start of identifier or keyword, let's assume identifier for now:
      //
      T.id = nuPy_IDENTIFIER;
      T.line = *lineNumber;
      T.col = *colNumber;

      collect_identifier(input, c, colNumber, value);

      T.id = id_or_keyword(value);
      return T;
    } 
    else if (c == '*') 
    {
      T.id = nuPy_ASTERISK;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; // advance col # past char

      value[0] = '*';

      value[1] = '\0';

      c = fgetc(input);
      if (c == '*') // it's **
      {
        T.id = nuPy_POWER;

        (*colNumber)++; // advance col # past char

        value[1] = '*';
        value[2] = '\0';

        return T;
      }

      ungetc(c, input);

      return T;
    }
    else if (c == '+') 
    {
      T.id = nuPy_PLUS;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; // advance col # past char

      value[0] = (char)c;

      value[1] = '\0';

      return T;
    } 
    else if (c == '-') 
    {
      T.id = nuPy_MINUS;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; // advance col # past char

      value[0] = (char)c;

      value[1] = '\0';

      return T;
    } 
    else if (c == '/') 
    {
      T.id = nuPy_SLASH;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; 

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    } 
    else if (c == '%') 
    {
      T.id = nuPy_PERCENT;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; 

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    } 
    else if (c == '=') 
    {
      T.id = nuPy_EQUAL;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; 

      value[0] = (char)c;
      value[1] = '\0';
      
      c = fgetc(input);

      if (c == '=') 
      {
        T.id = nuPy_EQUALEQUAL;
        (*colNumber)++; 

        value[1] = (char)c;
        value[2] = '\0';

        return T;
      }

      ungetc(c, input);

      return T;
    } 
    else if (c == '!')
    {
      T.id = nuPy_UNKNOWN;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; 

      value[0] = (char)c;
      value[1] = '\0';

      c = fgetc(input);

      if (c == '=') 
      {
        T.id = nuPy_NOTEQUAL;

        (*colNumber)++; // advance col # past char

        value[1] = (char)c;
        value[2] = '\0';

        return T;
      }

      ungetc(c, input);

      return T;
    } 
    else if (c == '<') 
    {
      T.id = nuPy_LT;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; 

      value[0] = (char)c;
      value[1] = '\0';

      c = fgetc(input);

      if (c == '=') 
      {
        T.id = nuPy_LTE;

        (*colNumber)++;

        value[1] = (char)c;
        value[2] = '\0';
        
        return T;
      }

      ungetc(c, input);

      return T;
    } 
    else if (c == '>') 
    {
      T.id = nuPy_GT;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; // advance col # past char

      value[0] = (char)c;
      value[1] = '\0';

      c = fgetc(input);

      if (c == '=') 
      {
        T.id = nuPy_GTE;

        (*colNumber)++;

        value[1] = (char)c;
        value[2] = '\0';

        return T;
      }

      ungetc(c, input);

      return T;
    } 
    else if (c == '&') 
    {
      T.id = nuPy_AMPERSAND;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; // advance col # past char

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    } 
    else if (c == ':') 
    {
      T.id = nuPy_COLON;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; // advance col # past char

      value[0] = (char)c;
      value[1] = '\0';

      return T;
    } 
    else if (c == '#') 
    {
      while (c != '\n' && c != EOF) 
      {
        c = fgetc(input);
      }

      (*lineNumber)++;

      *colNumber = 1;
    } 
    else if (c == '\'') 
    {
      T.id = nuPy_STR_LITERAL;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++;

      c = fgetc(input);

      (*colNumber)++;

      int index = 0;

      if (c != '\'') 
      {
        while (c != '\'') 
        {
          if (c == EOF || c == '\n') 
          {
            printf("**WARNING: string literal @ (%d, %d) not terminated properly\n", *lineNumber, T.col);

            ungetc(c, input);

            break;
          }

          value[index] = (char)c;

          c = fgetc(input);

          (*colNumber)++;

          index++;

        }

      }

      value[index] = '\0';

      return T;

    } 
    else if (c == '\"') 
    {
      T.id = nuPy_STR_LITERAL;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++;

      c = fgetc(input);

      (*colNumber)++;

      int index = 0;

      if (c != '\"') 
      {
        while (c != '\"') 
        {

          if (c == EOF || c == '\n') 
          {
            printf("**WARNING: string literal @ (%d, %d) not terminated properly\n", *lineNumber, T.col);

            ungetc(c, input);

            break;
          }

          value[index] = (char)c;

          c = fgetc(input);

          (*colNumber)++;

          index++;
        }
      }

      value[index] = '\0';
      return T;

    } 
    else if (isdigit(c) || c == '.') 
    {
      T.line = *lineNumber;
      T.col = *colNumber;
      T.id = collect_int_or_real_literal(input, c, colNumber, value, false);

      return T;
    } 
    else 
    {
      //
      // if we get here, then char denotes an UNKNOWN token:
      //
      T.id = nuPy_UNKNOWN;
      T.line = *lineNumber;
      T.col = *colNumber;

      (*colNumber)++; // advance past char
 
      value[0] = (char)c;
      value[1] = '\0';
      return T;
    }

  } // while

  //
  // execution should never get here, return occurs
  // from within loop
  //
}