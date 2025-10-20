/*execute.c*/

//
// << executes nuPython programs >>
//
// << Joseph Zhao >>
// << Northwestern >>
// << CS211 >>
// << Winter >>
//
// Starter code: Prof. Joe Hummel
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // true, false
#include <string.h>
#include <assert.h>

#include "programgraph.h"
#include "ram.h"
#include "execute.h"

#include <math.h>

//
// Public functions:
//

// RAM_VALUE
// Given a UNARY_EXPR lhs/rhs
// returns a pointer to the associated RAM_VALUE struct
// returns NULL if an identifier is passed in and has no associated RAM_VALUE struct
struct RAM_VALUE* RAM_VALUE(struct RAM* memory, struct UNARY_EXPR* side)
{
  // declare a new RAM_VALUE* structure
  struct RAM_VALUE* ramval = malloc(sizeof(struct RAM_VALUE));
  // if statement
  // if the value is an identifier
  if (side->element->element_type == ELEMENT_IDENTIFIER)
  {
    // if the identifier is NULL
    if (ram_read_cell_by_id(memory, side->element->element_value) == NULL)
    {
      return NULL;
    }
    
    struct RAM_VALUE *access_val = ram_read_cell_by_id(memory, side->element->element_value);
    
    if (access_val->value_type == RAM_TYPE_INT)
    {
      ramval->value_type = RAM_TYPE_INT;
      ramval->types.i = access_val->types.i;
    }
    // implementation for other types tbd
    else 
    {
      printf("WARNING: identifier did not store integer\n");
    }
  }
  else if (side->element->element_type == ELEMENT_INT_LITERAL)
  {
    ramval->value_type = RAM_TYPE_INT;
    ramval->types.i = atoi(side->element->element_value);
  }
  return ramval;
}

//
// execute_function_call
//
// helper function
// Given a nuPython program graph and a memory,
// executes the function statements
// If a semantic error occurs
// and error message is output, execution stops,
// and the function returns false
//
bool execute_function_call(struct STMT *stmt, struct RAM *memory)
{
  if (strcmp(stmt->types.function_call->function_name, "print") == 0)
  {
    if (stmt->types.function_call->parameter == NULL)
    {
      printf("\n");
    }
    else if (stmt->types.function_call->parameter->element_type == ELEMENT_STR_LITERAL)
    {
      printf("%s\n", stmt->types.function_call->parameter->element_value);
    }
    else if (stmt->types.function_call->parameter->element_type == ELEMENT_INT_LITERAL)
    {
      int int_value = atoi(stmt->types.function_call->parameter->element_value);
      printf("%d\n", int_value);
    }
    else if (stmt->types.function_call->parameter->element_type == ELEMENT_IDENTIFIER)
    {
      char *identifier = stmt->types.function_call->parameter->element_value;
      struct RAM_VALUE *ident_value = ram_read_cell_by_id(memory, identifier);
      if (ident_value == NULL)
      {
        printf("**SEMANTIC ERROR: name '%s' is not defined (line %d)\n", identifier, stmt->line);
        return false;
      }
      else
      {
        printf("%d\n", ident_value->types.i);
      }
    }
    return true;
  }
  else
  {
    return false;
  }
}

//
// execute_assignment
// 
// helper function
// Given a nuPython program graph and a memory,
// executes the assignment statements
// If a semantic error occurs
// and error message is output, execution stops,
// and the function returns false
//
bool execute_assignment(struct STMT *stmt, struct RAM *memory)
{
  char *var_name = stmt->types.assignment->var_name;
  struct VALUE *value = stmt->types.assignment->rhs;
  struct UNARY_EXPR *unary_expr = value->types.expr->lhs;
  
  if (value->types.expr->isBinaryExpr) 
  {
    int op = value->types.expr->operator;

    struct RAM_VALUE* left = RAM_VALUE(memory, value->types.expr->lhs);
    struct RAM_VALUE* right = RAM_VALUE(memory, value->types.expr->rhs);

    if (left == NULL)
    {
      printf("**SEMANTIC ERROR: name '%s' is not defined (line %d)\n", value->types.expr->lhs->element->element_value, stmt->line);
      return false;
    }
    else if (right == NULL)
    {
      printf("**SEMANTIC ERROR: name '%s' is not defined (line %d)\n", value->types.expr->rhs->element->element_value, stmt->line);
      return false;
    }

    if (op == OPERATOR_PLUS)
    {
      left->types.i += right->types.i;
      ram_write_cell_by_id(memory, *left, var_name);
    }
    else if (op == OPERATOR_MINUS)
    {
      left->types.i -= right->types.i;
      ram_write_cell_by_id(memory, *left, var_name);
    }
    else if (op == OPERATOR_ASTERICK)
    {
      left->types.i *= right->types.i;
      ram_write_cell_by_id(memory, *left, var_name);
    }
    else if (op == OPERATOR_DIV)
    {
      left->types.i /= right->types.i;
      ram_write_cell_by_id(memory, *left, var_name);
    }
    else if (op == OPERATOR_MOD)
    {
      left->types.i = left->types.i % right->types.i;
      ram_write_cell_by_id(memory, *left, var_name);
    }
    else if (op == OPERATOR_POWER)
    {
      left->types.i = (int)pow((double)left->types.i, (double)right->types.i);
      ram_write_cell_by_id(memory, *left, var_name);
    }
    else {
      return false;
    }
    ram_free_value(left);
    ram_free_value(right);
    return true;
  }

  if (value->value_type == ELEMENT_INT_LITERAL)
  {
    struct RAM_VALUE ram_value;
    ram_value.value_type = RAM_TYPE_INT;
    ram_value.types.i = atoi(unary_expr->element->element_value);
    ram_write_cell_by_id(memory, ram_value, var_name);
  }

  return true;
}

//
// execute
//
// Given a nuPython program graph and a memory,
// executes the statements in the program graph.
// If a semantic error occurs (e.g. type error),
// and error message is output, execution stops,
// and the function returns.
//
void execute(struct STMT *program, struct RAM *memory)
{
  struct STMT *stmt = program;
  while (stmt != NULL)
  {
    if (stmt->stmt_type == STMT_ASSIGNMENT)
    {
      int line = stmt->line;
      char *var_name = stmt->types.assignment->var_name;

      bool success = execute_assignment(stmt, memory);
      if(!success)
      {
        return;
      }

      stmt = stmt->types.assignment->next_stmt;
    }
    else if (stmt->stmt_type == STMT_FUNCTION_CALL)
    {
      int line = stmt->line;
      char *function_name = stmt->types.function_call->function_name;

      bool successful = execute_function_call(stmt, memory);
      if(!successful)
      {
        return;
      }

      stmt = stmt->types.function_call->next_stmt;
    }
    else
    {
      assert(stmt->stmt_type == STMT_PASS);
      stmt = stmt->types.pass->next_stmt;
    }
  }
}
