/*execute.c*/

//
// Executes nuPython program, given as a Program Graph.
//
// Joseph Zhao
// Project 03
// Prof. Joe Hummel
// Northwestern University
// CS 211
//
// Starter code: Prof. Joe Hummel
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // true, false
#include <string.h>
#include <assert.h>
#include <math.h>
#include "programgraph.h"
#include "ram.h"
#include "execute.h"

//
// compare_values
//
// Compare two RAM_VALUES in a binary expression.
// Assumes that the operator is a comparison. 
//
bool perform_comparison_operation(struct RAM_VALUE *val1, struct RAM_VALUE *val2, int operator, bool *outcome)
{
    bool result;
    if (val1->value_type != val2->value_type)
    {
        return false;
    }
    switch (operator)
    {
    case OPERATOR_EQUAL:
    {
        if (val1->value_type == RAM_TYPE_INT)
            result = (val1->types.i == val2->types.i);
        else if (val1->value_type == RAM_TYPE_REAL)
            result = (fabs(val1->types.d - val2->types.d) < 0.0000001);
        else if (val1->value_type == RAM_TYPE_STR)
            result = (strcmp(val1->types.s, val2->types.s) == 0);
        else
            return false;
        break;
    }
    case OPERATOR_NOT_EQUAL:
    {
        if (val1->value_type == RAM_TYPE_INT)
            result = (val1->types.i != val2->types.i);
        else if (val1->value_type == RAM_TYPE_REAL)
            result = (fabs(val1->types.d - val2->types.d) > 0.0000001);
        else if (val1->value_type == RAM_TYPE_STR)
            result = (strcmp(val1->types.s, val2->types.s) != 0);
        else
            return false;
        break;
    }
    case OPERATOR_LT:
    {
        if (val1->value_type == RAM_TYPE_INT)
            result = (val1->types.i < val2->types.i);
        else if (val1->value_type == RAM_TYPE_REAL)
            result = (val1->types.d < val2->types.d);
        else if (val1->value_type == RAM_TYPE_STR)
            result = (strcmp(val1->types.s, val2->types.s) < 0);
        else
            return false;
        break;
    }
    case OPERATOR_LTE:
    {
        if (val1->value_type == RAM_TYPE_INT)
            result = (val1->types.i <= val2->types.i);
        else if (val1->value_type == RAM_TYPE_REAL)
            result = (val1->types.d <= val2->types.d);
        else if (val1->value_type == RAM_TYPE_STR)
            result = (strcmp(val1->types.s, val2->types.s) <= 0);
        else
            return false;
        break;
    }
    case OPERATOR_GT:
    {
        if (val1->value_type == RAM_TYPE_INT)
            result = (val1->types.i > val2->types.i);
        else if (val1->value_type == RAM_TYPE_REAL)
            result = (val1->types.d > val2->types.d);
        else if (val1->value_type == RAM_TYPE_STR)
            result = (strcmp(val1->types.s, val2->types.s) > 0);
        else
            return false;
        break;
    }
    case OPERATOR_GTE:
    {
        if (val1->value_type == RAM_TYPE_INT)
            result = (val1->types.i >= val2->types.i);
        else if (val1->value_type == RAM_TYPE_REAL)
            result = (val1->types.d >= val2->types.d);
        else if (val1->value_type == RAM_TYPE_STR)
            result = (strcmp(val1->types.s, val2->types.s) >= 0);
        else
            return false;
        break;
    }
    }
    *outcome = result;
    return true;
}

//
// get_RAM_VALUE
//
// Take in a UNARY_EXPR and 
// update the given RAM_VALUE pointer.
//
bool get_RAM_VALUE(struct RAM *memory, struct UNARY_EXPR *expr, struct RAM_VALUE *update)
{
    int unary_type = expr->element->element_type;
    switch (unary_type)
    {
    case ELEMENT_IDENTIFIER:;
        struct RAM_VALUE *accessed_value = malloc(sizeof(struct RAM_VALUE));
        accessed_value = ram_read_cell_by_id(memory, expr->element->element_value);
        if (accessed_value == NULL)
            return false;
        int accessed_type = accessed_value->value_type;
        switch (accessed_type)
        {
        case RAM_TYPE_INT:
            update->value_type = RAM_TYPE_INT;
            update->types.i = accessed_value->types.i;
            break;
        case RAM_TYPE_REAL:
            update->value_type = RAM_TYPE_REAL;
            update->types.d = accessed_value->types.d;
            break;
        case RAM_TYPE_STR:
            update->value_type = RAM_TYPE_STR;
            update->types.d = accessed_value->types.d;
            break;
        case RAM_TYPE_BOOLEAN:
            update->value_type = RAM_TYPE_BOOLEAN;
            update->types.i = accessed_value->types.i;
            break;
        default:
            return false;
        }
        free(accessed_value);
        break;
    case ELEMENT_INT_LITERAL:
        update->value_type = RAM_TYPE_INT;
        update->types.i = atoi(expr->element->element_value);
        break;
    case ELEMENT_REAL_LITERAL:
        update->value_type = RAM_TYPE_REAL;
        update->types.d = atof(expr->element->element_value);
        break;
    case ELEMENT_STR_LITERAL:
        update->value_type = RAM_TYPE_STR;
        update->types.s = expr->element->element_value;
        break;
    case ELEMENT_TRUE:
        update->value_type = RAM_TYPE_BOOLEAN;
        update->types.i = 1;
        break;
    case ELEMENT_FALSE:
        update->value_type = RAM_TYPE_BOOLEAN;
        update->types.i = 0;
        break;
  }
  return true;
}

//
// zero_str
//
// Test if a string has only 0's.
// Return true if yes, otherwise, no.
//
bool zero_str(char *s)
{
    for (int i = 0; i < strlen(s); i++)
        if (s[i] != '0')
            return false;
    return true;
}

//
// execute_assignment_function_call
//
// Helper function to execute assignment statements which include
// a function call, returning true for successes and false for failures.
//
static bool execute_assignment_function_call(struct STMT *stmt, struct RAM *memory)
{
    assert(stmt->stmt_type == STMT_ASSIGNMENT);
    assert(stmt->types.assignment->rhs->value_type == VALUE_FUNCTION_CALL);
    char *var_name = stmt->types.assignment->var_name;
    char *func_name = stmt->types.assignment->rhs->types.function_call->function_name;
    struct ELEMENT *param = stmt->types.assignment->rhs->types.function_call->parameter;
    struct RAM_VALUE temp;
    if (strcmp(func_name, "input") == 0)
    {
        if (param->element_type == ELEMENT_STR_LITERAL)
            printf("%s", param->element_value);

        else
        {
            printf("**WARNING: invalid input() parameter\n");
            return false;
        }
        char line[256];
        fgets(line, sizeof(line), stdin);
        line[strcspn(line, "\r\n")] = '\0';
        temp.value_type = RAM_TYPE_STR;
        temp.types.s = line;
        ram_write_cell_by_id(memory, temp, var_name);
    }
    else if (strcmp(func_name, "int") == 0)
    {
        if (param->element_type == ELEMENT_IDENTIFIER)
        {
            struct RAM_VALUE *val = ram_read_cell_by_id(memory, param->element_value);
            if (val == NULL)
            {
                printf("**SEMANTIC ERROR: name '%s' is not defined (line %d)\n", param->element_value, stmt->line);
                return false;
            }
            if (val->value_type != RAM_TYPE_STR)
            {
                printf("**WARNING: invalid int() call\n");
                return false;
            }
            int value = atoi(val->types.s);
            if (value == 0)
            {
                if (zero_str(val->types.s))
                {
                }
                else
                {
                    printf("**SEMANTIC ERROR: invalid string for int() (line %d)\n", stmt->line);
                    return false;
                }
            }
            temp.value_type = RAM_TYPE_INT;
            temp.types.i = value;
            ram_write_cell_by_id(memory, temp, var_name);
        }
        else
        {
            printf("**WARNING: invalid int() parameter\n");
            return false;
        }
    }
    else if (strcmp(func_name, "float") == 0)
    {
        if (param->element_type == ELEMENT_IDENTIFIER)
        {
            struct RAM_VALUE *val = ram_read_cell_by_id(memory, param->element_value);
            if (val == NULL)
            {
                printf("**SEMANTIC ERROR: name '%s' is not defined (line %d)\n", param->element_value, stmt->line);
                return false;
            }
            if (val->value_type != RAM_TYPE_STR)
            {
                printf("**WARNING: invalid float() call\n");
                return false;
            }
            char *endptr;
            double value = strtod(val->types.s, &endptr);
            if (*endptr != '\0')
            {
                printf("**SEMANTIC ERROR: invalid string for float() (line %d)\n", stmt->line);
                return false;
            }
            temp.value_type = RAM_TYPE_REAL;
            temp.types.d = value;
            ram_write_cell_by_id(memory, temp, var_name);
        }
        else
        {
            printf("**WARNING: invalid float() parameter\n");
            return false;
        }
    }
    else
        return false;
    return true;
}

//
// execute_function_call
//
// Given a nuPython program graph and a memory,
// executes function calls (for now only print() functions)
// returns true if the statement executes succesfully,
// and false if not.
//
bool execute_function_call(struct STMT *stmt, struct RAM *memory)
{
    char *func_name = stmt->types.function_call->function_name;
    if (strcmp(func_name, "print") == 0 && stmt->types.function_call->parameter == NULL)
    {
        printf("\n");
        return true;
    }
    char *param = stmt->types.function_call->parameter->element_value;
    int elem_type = stmt->types.function_call->parameter->element_type;
    if (strcmp(func_name, "print") == 0)
    {
        if (elem_type == ELEMENT_INT_LITERAL)
            printf("%d\n", atoi(param));
        else if (elem_type == ELEMENT_REAL_LITERAL)
            printf("%lf\n", atof(param));
        else if (elem_type == ELEMENT_STR_LITERAL)
            printf("%s\n", param);
        else if (elem_type == ELEMENT_TRUE)
            printf("True\n");
        else if (elem_type == ELEMENT_FALSE)
            printf("False\n");
        else if (elem_type == ELEMENT_IDENTIFIER)
        {
            if (ram_read_cell_by_id(memory, param) == NULL)
            {
                printf("**SEMANTIC ERROR: name '%s' is not defined (line %d)\n", param, stmt->line);
                return false;
            }
            struct RAM_VALUE *accessed_value = ram_read_cell_by_id(memory, param);
            switch (accessed_value->value_type)
            {
            case RAM_TYPE_INT:
                printf("%d\n", accessed_value->types.i);
                break;
            case RAM_TYPE_REAL:
                printf("%lf\n", accessed_value->types.d);
                break;
            case RAM_TYPE_STR:
                printf("%s\n", accessed_value->types.s);
                break;
            case RAM_TYPE_BOOLEAN:
                if (accessed_value->types.i == 0)
                    printf("False\n");
                else
                    printf("True\n");
                break;
            }
        }
        return true;
    }
    else
    {
        printf("not a print function\n");
        return false;
    }
}

//
// invalid_operand
//
// Return an error message for having an invalid operand
// on the right hand side of an assignment statement, including line number.
// Always returns false for easy return calls.
//
bool invalid_operand(struct STMT *stmt)
{
    printf("**SEMANTIC ERROR: invalid operand types (line %d)\n", stmt->line);
    return false;
}

//
// convert_to_real
//
// Convert both operands (for binary expressions) to real literals.
// Returns false if one of the two operands are not integer literals
// or they are not matching and non integer literals.
//
bool convert_to_real(struct RAM_VALUE *left, struct RAM_VALUE *right)
{

    if (left->value_type == RAM_TYPE_INT)
    {
        if (right->value_type != RAM_TYPE_REAL)
            return false;
        left->types.d = (double)left->types.i;
        left->value_type = RAM_TYPE_REAL;
        return true;
    }
    if (right->value_type == RAM_TYPE_INT)
    {
        if (left->value_type != RAM_TYPE_REAL)
            return false;
        right->types.d = (double)right->types.i;
        right->value_type = RAM_TYPE_REAL;
        return true;
    }
    return false;
}

//
// execute_assignment
//
// Given a nuPython program statement and a memory,
// executes assignment statements, storing data into memory.
// Also returns semantic errors if an identifier is used without a valid definition.
//
bool execute_assignment(struct STMT *stmt, struct RAM *memory)
{
  assert(stmt->stmt_type == STMT_ASSIGNMENT);
  char *variable_name = stmt->types.assignment->var_name;
  int value_type = stmt->types.assignment->rhs->value_type;
  if (value_type == VALUE_EXPR)
  {
    struct VALUE_EXPR *value = stmt->types.assignment->rhs->types.expr;
    struct RAM_VALUE *left = malloc(sizeof(struct RAM_VALUE));
    if (!get_RAM_VALUE(memory, value->lhs, left))
    {
      printf("**SEMANTIC ERROR: name '%s' is not defined (line %d)\n", value->lhs->element->element_value, stmt->line);
      return false;
    }

    if (value->isBinaryExpr)
    {
      struct RAM_VALUE *right = malloc(sizeof(struct RAM_VALUE));
      if (!get_RAM_VALUE(memory, value->rhs, right))
      {
        printf("**SEMANTIC ERROR: name '%s' is not defined (line %d)\n", value->rhs->element->element_value, stmt->line);
        return false;
      }

      int op = value->operator;
      assert(op != OPERATOR_NO_OP);

      if (left->value_type != right->value_type)
        if (!convert_to_real(left, right))
          return invalid_operand(stmt);

      switch (op)
      {
      case OPERATOR_PLUS:
        if (left->value_type == RAM_TYPE_INT)
          left->types.i += right->types.i;
        else if (left->value_type == RAM_TYPE_REAL)
          left->types.d += right->types.d;
        // string concatenation handling
        else if (left->value_type == RAM_TYPE_STR && right->value_type == RAM_TYPE_STR)
        {
          size_t newSize = strlen(left->types.s) + strlen(right->types.s) + 1;
          char *newStr = malloc(newSize);
          if (newStr == NULL)
          {
            printf("**ERROR: Memory allocation failed for string concatenation\n");
            return false;
          }
          strcpy(newStr, left->types.s);
          strcat(newStr, right->types.s);
          free(left->types.s);
          left->types.s = newStr;
        }
        else
          return invalid_operand(stmt);
        break;

      case OPERATOR_MINUS:
        if (left->value_type == RAM_TYPE_INT)
          left->types.i -= right->types.i;
        else if (left->value_type == RAM_TYPE_REAL)
          left->types.d -= right->types.d;
        else
          return invalid_operand(stmt);
        break;

      case OPERATOR_ASTERISK:
        if (left->value_type == RAM_TYPE_INT)
          left->types.i *= right->types.i;
        else if (left->value_type == RAM_TYPE_REAL)
          left->types.d *= right->types.d;
        else
          return invalid_operand(stmt);
        break;

      case OPERATOR_POWER:
        if (left->value_type == RAM_TYPE_INT)
          left->types.i = (int)pow((double)left->types.i, (double)right->types.i);
        else if (left->value_type == RAM_TYPE_REAL)
          left->types.d = pow(left->types.d, right->types.d);
        else
          return invalid_operand(stmt);
        break;

      case OPERATOR_MOD:
        if (left->value_type == RAM_TYPE_INT)
          left->types.i = (int)fmod((double)left->types.i, (double)right->types.i);
        else if (left->value_type == RAM_TYPE_REAL)
          left->types.d = fmod(left->types.d, right->types.d);
        else
          return invalid_operand(stmt);
        break;

      case OPERATOR_DIV:
        if (left->value_type == RAM_TYPE_INT)
          left->types.i /= right->types.i;
        else if (left->value_type == RAM_TYPE_REAL)
          left->types.d /= right->types.d;
        else
          return invalid_operand(stmt);
        break;
      case OPERATOR_EQUAL:
      case OPERATOR_NOT_EQUAL:
      case OPERATOR_LT:
      case OPERATOR_LTE:
      case OPERATOR_GT:
      case OPERATOR_GTE:
      {
        bool value;
        // value is what will be adjusted to become left->types.i,
        // the function returns false if failed.
        if (!perform_comparison_operation(left, right, op, &value))
          return invalid_operand(stmt);
        left->value_type = RAM_TYPE_BOOLEAN;
        left->types.i = value ? 1 : 0;
        break;
      }
      default:
        printf("**EXECUTION ERROR: unexpected operator (%d) in execute_binary_expr\n", op);
        return false;
      }
      free(right);
    }
    ram_write_cell_by_id(memory, *left, variable_name);
    free(left);
    return true;
  }
  // this must be an assignment function call
  else
  {
    return (execute_assignment_function_call(stmt, memory));
  }
  return true;
}

bool execute_while_loop(struct STMT *stmt, struct RAM *memory)
{
    struct VALUE_EXPR *condition = stmt->types.while_loop->condition;
    struct STMT *loop_start = stmt->types.while_loop->loop_body;

    bool boolean;
    while (true)
    {
        struct RAM_VALUE left, right;

        if (!get_RAM_VALUE(memory, condition->lhs, &left))
            return false;

        if (!get_RAM_VALUE(memory, condition->rhs, &right))
            return false;

        if (!perform_comparison_operation(&left, &right, condition->operator, & boolean))
            return invalid_operand(stmt);

        if (!boolean)
            break;

        struct STMT *curr = loop_start;
        while (curr != stmt)
        {
            if (curr->stmt_type == STMT_ASSIGNMENT)
            {
                if (!execute_assignment(curr, memory))
                    return false;
                curr = curr->types.assignment->next_stmt;
            }
            else if (curr->stmt_type == STMT_FUNCTION_CALL)
            {
                if (!execute_function_call(curr, memory))
                    return false;
                curr = curr->types.function_call->next_stmt;
            }
            else if (curr->stmt_type == STMT_PASS)
                curr = curr->types.pass->next_stmt;
            else if (curr->stmt_type == STMT_WHILE_LOOP)
            {
                if (!execute_while_loop(curr, memory))
                    return false;
                curr = curr->types.while_loop->next_stmt;
            }
            else if (curr->stmt_type == STMT_IF_THEN_ELSE)
            {
                return false;
            }
        }
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
            if (!execute_assignment(stmt, memory))
                return;
            stmt = stmt->types.assignment->next_stmt;
        }
        else if (stmt->stmt_type == STMT_FUNCTION_CALL)
        {
            if (!execute_function_call(stmt, memory))
                return;
            stmt = stmt->types.function_call->next_stmt;
        }
        else if (stmt->stmt_type == STMT_PASS)
            stmt = stmt->types.pass->next_stmt;
        else if (stmt->stmt_type == STMT_WHILE_LOOP)
        {
            if (!execute_while_loop(stmt, memory))
                return;
            stmt = stmt->types.while_loop->next_stmt;
        }
        else if (stmt->stmt_type == STMT_IF_THEN_ELSE)
            return;
    }
}
