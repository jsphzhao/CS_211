/*tests.c*/

//
// << Google tests for checking behavior of functions in ram.c >>
//
// << Joseph Zhao >>
//
// Initial template: Prof. Joe Hummel
// Northwestern University
// CS 211
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ram.h"
#include "gtest/gtest.h"

//
// private helper functions:
//

//
// Test case: writing one integer value
//
TEST(memory_module, write_one_int) 
{
  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE i;

  i.value_type = RAM_TYPE_INT;
  i.types.i = 123;

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);

  //
  // check the memory, does it contain x = 123?
  //
  ASSERT_TRUE(memory->num_values == 1);
  ASSERT_TRUE(memory->cells[0].value.value_type == RAM_TYPE_INT);
  ASSERT_TRUE(memory->cells[0].value.types.i == 123);
  ASSERT_TRUE(strcmp(memory->cells[0].identifier, "x") == 0);

  //
  // done test, free memory
  //
  ram_destroy(memory);
}


//
// TODO: add many more unit tests
//
TEST(memory_module, GetAddrBoundaryCheck) {
    struct RAM* memory = ram_init();
    for (int i = 0; i < 10; i++) {
        struct RAM_VALUE value = {RAM_TYPE_INT, {.i = i}};
        char identifier[20];
        sprintf(identifier, "var%d", i);
        ram_write_cell_by_id(memory, value, identifier);
    }
    int lastAddr = ram_get_addr(memory, "var9");
    EXPECT_EQ(lastAddr, 9);
    ram_destroy(memory);
}

TEST(memory_module, ReadCellByAddrBoundary) {
    struct RAM* memory = ram_init();
    for (int i = 0; i < memory->capacity; i++) {
        struct RAM_VALUE value = {RAM_TYPE_INT, {.i = i}};
        char identifier[20];
        sprintf(identifier, "var%d", i);
        ram_write_cell_by_id(memory, value, identifier);
    }

    struct RAM_VALUE* lastValid = ram_read_cell_by_addr(memory, memory->capacity - 1);
    ASSERT_NE(lastValid, nullptr);
    EXPECT_EQ(lastValid->value_type, RAM_TYPE_INT);
    ram_free_value(lastValid);

    struct RAM_VALUE* pastEnd = ram_read_cell_by_addr(memory, memory->capacity);
    EXPECT_EQ(pastEnd, nullptr);
    ram_destroy(memory);
}

TEST(memory_module, CapacityExpansion) {
    struct RAM* memory = ram_init();

    for (int i = 0; i < 4; ++i) {
        struct RAM_VALUE value = {RAM_TYPE_INT, {.i = i}};
        char identifier[10];
        sprintf(identifier, "var%d", i);
        ram_write_cell_by_id(memory, value, identifier);
    }

    struct RAM_VALUE extraValue = {RAM_TYPE_INT, {.i = 5}};
    ram_write_cell_by_id(memory, extraValue, "var5");

    EXPECT_EQ(memory->capacity, 8);
    ram_destroy(memory);
}

TEST(memory_module, StringReallocationBoundary) {
    struct RAM* memory = ram_init();
    struct RAM_VALUE firstValue = {RAM_TYPE_STR, {.s = strdup("Hello")}};
    ram_write_cell_by_id(memory, firstValue, "greeting");

    struct RAM_VALUE secondValue = {RAM_TYPE_STR, {.s = strdup("Hello, World!")}};
    ram_write_cell_by_id(memory, secondValue, "greeting");

    struct RAM_VALUE* output = ram_read_cell_by_id(memory, "greeting");
    ASSERT_NE(output, nullptr);
    EXPECT_EQ(output->value_type, RAM_TYPE_STR);
    EXPECT_STREQ(output->types.s, "Hello, World!");
    ram_free_value(output);
    ram_destroy(memory);
}

TEST(memory_module, WriteAndReadString) {
    struct RAM* memory = ram_init();
    const char* testString = "Test String";
    struct RAM_VALUE value = {RAM_TYPE_STR, {.s = (char*)testString}};

    ram_write_cell_by_id(memory, value, "strVar");
    struct RAM_VALUE* readValue = ram_read_cell_by_id(memory, "strVar");

    ASSERT_NE(readValue, nullptr);
    EXPECT_EQ(readValue->value_type, RAM_TYPE_STR);
    EXPECT_STREQ(readValue->types.s, testString);

    ram_free_value(readValue);
    ram_destroy(memory);
}

TEST(memory_module, UpdateString) {
    struct RAM* memory = ram_init();
    struct RAM_VALUE firstValue = {RAM_TYPE_STR, {.s = strdup("First String")}};
    ram_write_cell_by_id(memory, firstValue, "updateStr");

    struct RAM_VALUE secondValue = {RAM_TYPE_STR, {.s = strdup("Second String")}};
    ram_write_cell_by_id(memory, secondValue, "updateStr");

    struct RAM_VALUE* output = ram_read_cell_by_id(memory, "updateStr");
    ASSERT_NE(output, nullptr);
    EXPECT_EQ(output->value_type, RAM_TYPE_STR);
    EXPECT_STREQ(output->types.s, "Second String");

    ram_free_value(output);
    ram_destroy(memory);
}

TEST(memory_module, FreeStringValue) {
    struct RAM_VALUE* value = (struct RAM_VALUE*)malloc(sizeof(struct RAM_VALUE));
    value->value_type = RAM_TYPE_STR;
    value->types.s = strdup("Dynamic String");

    ram_free_value(value);
}

TEST(memory_module, StringReallocationOnCapacity) {
    struct RAM* memory = ram_init();
    for (int i = 0; i < memory->capacity; i++) {
        char identifier[20];
        sprintf(identifier, "str%d", i);
        char* dynamicStr = (char*)malloc(20);
        sprintf(dynamicStr, "String %d", i);
        struct RAM_VALUE value = {RAM_TYPE_STR, {.s = dynamicStr}};

        ram_write_cell_by_id(memory, value, identifier);
    }

    struct RAM_VALUE extraValue = {RAM_TYPE_STR, {.s = strdup("Extra String")}};
    ram_write_cell_by_id(memory, extraValue, "extraStr");

    struct RAM_VALUE* output = ram_read_cell_by_id(memory, "extraStr");
    ASSERT_NE(output, nullptr);
    EXPECT_EQ(output->value_type, RAM_TYPE_STR);
    EXPECT_STREQ(output->types.s, "Extra String");

    ram_free_value(output);
    ram_destroy(memory);
}

TEST(memory_module, AccessLastMemoryCell) {
    struct RAM* memory = ram_init();
    assert(memory != NULL);

    for (int i = 0; i < memory->capacity - 1; i++) {
        struct RAM_VALUE value = {RAM_TYPE_INT, {.i = i}};
        char identifier[20];
        sprintf(identifier, "var%d", i);
        ram_write_cell_by_id(memory, value, identifier);
    }

    struct RAM_VALUE lastValue = {RAM_TYPE_INT, {.i = 999}};
    ram_write_cell_by_id(memory, lastValue, "lastVar");

    int lastAddr = ram_get_addr(memory, "lastVar");
    assert(lastAddr == memory->capacity - 1);

    struct RAM_VALUE* readBack = ram_read_cell_by_addr(memory, lastAddr);
    assert(readBack != NULL);
    assert(readBack->value_type == RAM_TYPE_INT);
    assert(readBack->types.i == 999);

    ram_free_value(readBack);
    ram_destroy(memory);
}

TEST(memory_module, WriteToLastValidAddress) {
    struct RAM* memory = ram_init();

    char identifier[20];
    for (int i = 0; i < memory->capacity; i++) {
        struct RAM_VALUE val;
        val.value_type = RAM_TYPE_INT;
        val.types.i = i;
        sprintf(identifier, "var%d", i);
        bool success = ram_write_cell_by_id(memory, val, identifier);
        ASSERT_TRUE(success);
    }

    ASSERT_EQ(memory->num_values, memory->capacity);
    struct RAM_VALUE lastVal;
    lastVal.value_type = RAM_TYPE_INT;
    lastVal.types.i = 999;
    ASSERT_TRUE(ram_write_cell_by_addr(memory, lastVal, memory->capacity - 1));

    struct RAM_VALUE* readVal = ram_read_cell_by_addr(memory, memory->capacity - 1);
    ASSERT_NE(readVal, nullptr);
    ASSERT_EQ(readVal->value_type, RAM_TYPE_INT);
    ASSERT_EQ(readVal->types.i, 999);

    ram_free_value(readVal);
    ram_destroy(memory);
}

TEST(memory_module, StringDuplication) {
  struct RAM* memory = ram_init();
  ASSERT_TRUE(memory != NULL);

  struct RAM_VALUE str_value;
  str_value.value_type = RAM_TYPE_STR;
  char* original = (char*)malloc(sizeof("Hello") + 1);
  strcpy(original, "Hello");
  str_value.types.s = original;

  bool success_str = ram_write_cell_by_id(memory, str_value, "str_var");

  ASSERT_TRUE(success_str);

  strcpy(original, "World");

  RAM_VALUE* ramvalue = ram_read_cell_by_addr(memory, 0);

  ASSERT_TRUE(strcmp(ramvalue->types.s, "Hello") == 0);
  ram_free_value(ramvalue);
  free(original);
  ram_destroy(memory);
}

