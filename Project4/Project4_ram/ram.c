/*ram.c*/

//
// << Implementation of random access memory (RAM) functions for nuPython >>
//
// << Joseph Zhao >>
//
// Template: Prof. Joe Hummel
// Northwestern University
// CS 211
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // true, false
#include <string.h>
#include <assert.h>

#include "ram.h"


//
// Public functions:
//

//
// ram_init
//
// Returns a pointer to a dynamically-allocated memory
// for storing nuPython variables and their values. All
// memory cells are initialized to the value None.
//
struct RAM* ram_init(void) {
    int initial_capacity = 4;
    
    struct RAM* ram = (struct RAM*)malloc(sizeof(struct RAM));
    if (ram == NULL) {
        return NULL;
    }

    ram->cells = (struct RAM_CELL*)malloc(initial_capacity * sizeof(struct RAM_CELL));
    if (ram->cells == NULL) {
        free(ram);
        return NULL;
    }

    ram->num_values = 0;
    ram->capacity = initial_capacity;

    for (int i = 0; i < ram->capacity; i++) {
        ram->cells[i].identifier = NULL;
        ram->cells[i].value.value_type = RAM_TYPE_NONE;
    }

    return ram;
}


//
// ram_destroy
//
// Frees the dynamically-allocated memory associated with
// the given memory. After the call returns, you cannot
// use the memory.
//
void ram_destroy(struct RAM* memory) {
    if (memory != NULL) {
        for (int i = 0; i < memory->num_values; i++) {
            free(memory->cells[i].identifier);
            if (memory->cells[i].value.value_type == RAM_TYPE_STR) {
                free(memory->cells[i].value.types.s);
            }
        }
        free(memory->cells);
        free(memory);
    }
}


//
// ram_get_addr
// 
// If the given identifier (e.g. "x") has been written to 
// memory, returns the address of this value --- an integer
// in the range 0..N-1 where N is the number of values currently 
// stored in memory. Returns -1 if no such identifier exists 
// in memory. 
// 
// NOTE: a variable has to be written to memory before you can
// get its address. Once a variable is written to memory, its
// address never changes.
//
int ram_get_addr(struct RAM* memory, char* identifier) {
    if (memory == NULL || identifier == NULL) {
        return -1;
    }

    for (int i = 0; i < memory->num_values; i++) {
        if (memory->cells[i].identifier != NULL && strcmp(memory->cells[i].identifier, identifier) == 0) {
            return i;
        }
    }

    return -1;
}


//
// ram_read_cell_by_addr
//
// Given a memory address (an integer in the range 0..N-1), 
// returns a COPY of the value contained in that memory cell.
// Returns NULL if the address is not valid.
//
// NOTE: this function allocates memory for the value that
// is returned. The caller takes ownership of the copy and 
// must eventually free this memory via ram_free_value().
//
// NOTE: a variable has to be written to memory before its
// address becomes valid. Once a variable is written to memory,
// its address never changes.
//
struct RAM_VALUE* ram_read_cell_by_addr(struct RAM* memory, int address) {
    if (memory == NULL || address < 0 || address >= memory->num_values) {
        return NULL;
    }

    struct RAM_VALUE* value_copy = (struct RAM_VALUE*)malloc(sizeof(struct RAM_VALUE));
    if (value_copy == NULL) {
        return NULL;
    }

    *value_copy = memory->cells[address].value;

    if (value_copy->value_type == RAM_TYPE_STR) {
        value_copy->types.s = strdup(memory->cells[address].value.types.s);
        if (value_copy->types.s == NULL) {
            free(value_copy);
            return NULL;
        }
    }

    return value_copy;
}


// 
// ram_read_cell_by_id
//
// If the given identifier (e.g. "x") has been written to 
// memory, returns a COPY of the value contained in memory.
// Returns NULL if no such identifier exists in memory.
//
// NOTE: this function allocates memory for the value that
// is returned. The caller takes ownership of the copy and 
// must eventually free this memory via ram_free_value().
//
struct RAM_VALUE* ram_read_cell_by_id(struct RAM* memory, char* identifier) {
    int address = ram_get_addr(memory, identifier);
    if (address == -1) {
        return NULL;
    }

    return ram_read_cell_by_addr(memory, address);
}


//
// ram_free_value
//
// Frees the memory value returned by ram_read_cell_by_id and
// ram_read_cell_by_addr.
//
void ram_free_value(struct RAM_VALUE* value) {
    if (value == NULL) {
        return;
    }

    if (value->value_type == RAM_TYPE_STR) {
        free(value->types.s);
    }

    free(value);
}


//
// ram_write_cell_by_addr
//
// Writes the given value to the memory cell at the given 
// address. If a value already exists at this address, that
// value is overwritten by this new value. Returns true if 
// the value was successfully written, false if not (which 
// implies the memory address is invalid).
// 
// NOTE: if the value being written is a string, it will
// be duplicated and stored.
// 
// NOTE: a variable has to be written to memory before its
// address becomes valid. Once a variable is written to memory,
// its address never changes.
//
bool ram_write_cell_by_addr(struct RAM* memory, struct RAM_VALUE value, int address) {
    if (address < 0 || address >= memory->num_values) {
        return false;
    }

    if (memory->cells[address].value.value_type == RAM_TYPE_STR) {
        free(memory->cells[address].value.types.s); 
        memory->cells[address].value.types.s = NULL;
    }

    memory->cells[address].value.value_type = value.value_type;

    switch (value.value_type) {
        case RAM_TYPE_INT:
        case RAM_TYPE_PTR:
        case RAM_TYPE_BOOLEAN:
            memory->cells[address].value.types.i = value.types.i;
            break;
        case RAM_TYPE_REAL:
            memory->cells[address].value.types.d = value.types.d;
            break;
        case RAM_TYPE_STR:
            memory->cells[address].value.types.s = strdup(value.types.s);
            if (memory->cells[address].value.types.s == NULL) {
                return false;
            }
            break;
        case RAM_TYPE_NONE:
            break;
        default:
            break;
    }

    return true;
}


//
// ram_write_cell_by_id
//
// Writes the given value to a memory cell named by the given
// identifier. If a memory cell already exists with this name,
// the existing value is overwritten by the given value. Returns
// true since this operation always succeeds.
// 
// NOTE: if the value being written is a string, it will
// be duplicated and stored.
// 
// NOTE: a variable has to be written to memory before its
// address becomes valid. Once a variable is written to memory,
// its address never changes.
//
bool ram_write_cell_by_id(struct RAM* memory, struct RAM_VALUE value, char* identifier) {
    if (memory == NULL || identifier == NULL) {
        return false;
    }

    int address = ram_get_addr(memory, identifier);
    if (address == -1) {
        if (memory->num_values == memory->capacity) {
            int newCapacity = memory->capacity * 2;
            struct RAM_CELL* newCells = (struct RAM_CELL*)realloc(memory->cells, newCapacity * sizeof(struct RAM_CELL));
            if (newCells == NULL) {
                return false;
            }
            memory->cells = newCells;
            memory->capacity = newCapacity;
        }

        address = memory->num_values++;
        memory->cells[address].identifier = strdup(identifier);
        if (memory->cells[address].identifier == NULL) {
            return false;
        }
    } else {
        if (memory->cells[address].value.value_type == RAM_TYPE_STR) {
            free(memory->cells[address].value.types.s);
        }
    }

    memory->cells[address].value = value;
    if (value.value_type == RAM_TYPE_STR) {
        memory->cells[address].value.types.s = strdup(value.types.s);
        if (memory->cells[address].value.types.s == NULL) {
            return false;
        }
    }

    return true;
}


//
// ram_print
//
// Prints the contents of memory to the console.
//
void ram_print(struct RAM* memory)
{
    printf("**MEMORY PRINT**\n");

    printf("Capacity: %d\n", memory->capacity);
    printf("Num values: %d\n", memory->num_values);
    printf("Contents:\n");

    for (int i = 0; i < memory->num_values; i++)
    {
        printf(" %d: %s, ", i, memory->cells[i].identifier);

        switch (memory->cells[i].value.value_type)
        {
        case RAM_TYPE_INT:
            printf("int, %d", memory->cells[i].value.types.i);
            break;
        case RAM_TYPE_REAL:
            printf("real, %lf", memory->cells[i].value.types.d);
            break;
        case RAM_TYPE_STR:
            printf("str, '%s'", memory->cells[i].value.types.s);
            break;
        case RAM_TYPE_PTR:
            printf("ptr, %d", memory->cells[i].value.types.i);
            break;
        case RAM_TYPE_BOOLEAN:
            printf("boolean, %s", memory->cells[i].value.types.i ? "True" : "False");
            break;
        case RAM_TYPE_NONE:
            printf("none, None");
            break;
        default:
            printf("unknown");
        }
        printf("\n");
    }
    printf("**END PRINT**\n");
}
