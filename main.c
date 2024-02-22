#include <stdio.h>
#include <string.h>
#include "bit_array.h"

#define TEST_BIT_ARRAY_TOTAL 52
BIT_ARRAY_DEFINE(test_bit_array, TEST_BIT_ARRAY_TOTAL);

//
// Tests
//
const char *suite_name;
char suite_pass;
int suites_run = 0, suites_failed = 0, suites_empty = 0;
int tests_in_suite = 0, tests_run = 0, tests_failed = 0;

#define QUOTE(str) #str
#define ASSERT(x)                                                                                  \
    {                                                                                              \
        tests_run++;                                                                               \
        tests_in_suite++;                                                                          \
        if (!(x))                                                                                  \
        {                                                                                          \
            printf("failed assert [%s:%i] %s\n", __FILE__, __LINE__, QUOTE(x));                    \
            suite_pass = 0;                                                                        \
            tests_failed++;                                                                        \
        }                                                                                          \
    }

void SUITE_START(const char *name)
{
    suite_pass = 1;
    suite_name = name;
    suites_run++;
    tests_in_suite = 0;
}

void SUITE_END(void)
{
    printf("Testing %s ", suite_name);
    size_t suite_i;
    for (suite_i = strlen(suite_name); suite_i < 80 - 8 - 5; suite_i++)
        printf(".");
    printf("%s\n", suite_pass ? " pass" : " fail");
    if (!suite_pass)
        suites_failed++;
    if (!tests_in_suite)
        suites_empty++;
}

char test_str_buf[0x1000];

#define TEST_BIT_CHECK(arr, bit_idx, exp_value)                                                    \
    ASSERT(bit_array_get((arr), (bit_idx)) == (exp_value));

#define TEST_ARRAY_CHECK(arr, size, exp_value)                                                     \
    ASSERT(memcmp(bit_array_to_str_8((arr), (size), test_str_buf), (exp_value),                    \
                  strlen(exp_value)) == 0);

#define TEST_ARRAY_PRINT(bit_arr, size)                                                            \
    printf("%s\n", bit_array_to_str_8((bit_arr), (size), test_str_buf));

static void test_assign(void)
{
    SUITE_START("test_assign");

    bit_array_clear_all(test_bit_array, TEST_BIT_ARRAY_TOTAL);

    bit_array_assign(test_bit_array, 1, 1);
    bit_array_assign(test_bit_array, 2, 0);
    bit_array_assign(test_bit_array, 3, 1);
    bit_array_assign(test_bit_array, 8, 1);
    bit_array_assign(test_bit_array, 17, 1);

    // Test second item
    bit_array_assign(test_bit_array, 1 + 32, 1);
    bit_array_assign(test_bit_array, 2 + 32, 0);
    bit_array_assign(test_bit_array, 3 + 32, 1);
    bit_array_assign(test_bit_array, 8 + 32, 1);
    bit_array_assign(test_bit_array, 17 + 32, 1);

    TEST_BIT_CHECK(test_bit_array, 1, 1);
    TEST_BIT_CHECK(test_bit_array, 2, 0);
    TEST_BIT_CHECK(test_bit_array, 3, 1);
    TEST_BIT_CHECK(test_bit_array, 8, 1);
    TEST_BIT_CHECK(test_bit_array, 16, 0);

    // Test second item
    TEST_BIT_CHECK(test_bit_array, 1 + 32, 1);
    TEST_BIT_CHECK(test_bit_array, 2 + 32, 0);
    TEST_BIT_CHECK(test_bit_array, 3 + 32, 1);
    TEST_BIT_CHECK(test_bit_array, 8 + 32, 1);
    TEST_BIT_CHECK(test_bit_array, 16 + 32, 0);

    // value change
    bit_array_assign(test_bit_array, 1, 0);
    TEST_BIT_CHECK(test_bit_array, 1, 0);

    // Test second item
    bit_array_assign(test_bit_array, 1 + 32, 0);
    TEST_BIT_CHECK(test_bit_array, 1 + 32, 0);

    // TEST_ARRAY_PRINT(test_bit_array, TEST_BIT_ARRAY_TOTAL);
    TEST_ARRAY_CHECK(test_bit_array, TEST_BIT_ARRAY_TOTAL,
                     "00010000 10000000 01000000 00000000 00010000 10000000 0100");
    ASSERT(bit_array_num_bits_set(test_bit_array, TEST_BIT_ARRAY_TOTAL) == 6);

    SUITE_END();
}

static void test_set_clear(void)
{
    SUITE_START("test_set_clear");

    bit_array_clear_all(test_bit_array, TEST_BIT_ARRAY_TOTAL);

    bit_array_set(test_bit_array, 1);
    bit_array_clear(test_bit_array, 2);
    bit_array_set(test_bit_array, 3);
    bit_array_set(test_bit_array, 8);
    bit_array_set(test_bit_array, 17);

    // Test second item
    bit_array_set(test_bit_array, 1 + 32);
    bit_array_clear(test_bit_array, 2 + 32);
    bit_array_set(test_bit_array, 3 + 32);
    bit_array_set(test_bit_array, 8 + 32);
    bit_array_set(test_bit_array, 17 + 32);

    TEST_BIT_CHECK(test_bit_array, 1, 1);
    TEST_BIT_CHECK(test_bit_array, 2, 0);
    TEST_BIT_CHECK(test_bit_array, 3, 1);
    TEST_BIT_CHECK(test_bit_array, 8, 1);
    TEST_BIT_CHECK(test_bit_array, 16, 0);

    // Test second item
    TEST_BIT_CHECK(test_bit_array, 1 + 32, 1);
    TEST_BIT_CHECK(test_bit_array, 2 + 32, 0);
    TEST_BIT_CHECK(test_bit_array, 3 + 32, 1);
    TEST_BIT_CHECK(test_bit_array, 8 + 32, 1);
    TEST_BIT_CHECK(test_bit_array, 16 + 32, 0);

    // value change
    bit_array_clear(test_bit_array, 1);
    TEST_BIT_CHECK(test_bit_array, 1, 0);

    // Test second item
    bit_array_clear(test_bit_array, 1 + 32);
    TEST_BIT_CHECK(test_bit_array, 1 + 32, 0);

    bit_array_set(test_bit_array, 1);
    TEST_BIT_CHECK(test_bit_array, 1, 1);

    // Test second item
    bit_array_set(test_bit_array, 1 + 32);
    TEST_BIT_CHECK(test_bit_array, 1 + 32, 1);

    TEST_ARRAY_CHECK(test_bit_array, TEST_BIT_ARRAY_TOTAL,
                     "01010000 10000000 01000000 00000000 01010000 10000000 0100");
    ASSERT(bit_array_num_bits_set(test_bit_array, TEST_BIT_ARRAY_TOTAL) == 8);

    SUITE_END();
}

static void test_toggle(void)
{
    SUITE_START("test_toggle");

    // Toggle all
    bit_array_from_str(test_bit_array,
                       "01010000 10000000 01000000 00000000 01010000 10000000 0101");
    bit_array_toggle_all(test_bit_array, TEST_BIT_ARRAY_TOTAL);
    TEST_ARRAY_CHECK(test_bit_array, TEST_BIT_ARRAY_TOTAL,
                     "10101111 01111111 10111111 11111111 10101111 01111111 1010");

    // Toggle bit
    bit_array_clear_all(test_bit_array, TEST_BIT_ARRAY_TOTAL);

    bit_array_toggle(test_bit_array, 1);
    TEST_BIT_CHECK(test_bit_array, 1, 1);
    bit_array_toggle(test_bit_array, 1);
    TEST_BIT_CHECK(test_bit_array, 1, 0);

    // Test second item
    bit_array_toggle(test_bit_array, 1 + 32);
    TEST_BIT_CHECK(test_bit_array, 1 + 32, 1);
    bit_array_toggle(test_bit_array, 1 + 32);
    TEST_BIT_CHECK(test_bit_array, 1 + 32, 0);

    TEST_ARRAY_CHECK(test_bit_array, TEST_BIT_ARRAY_TOTAL,
                     "00000000 00000000 00000000 00000000 00000000 00000000 0000");

    SUITE_END();
}

static void test_copy(void)
{
    SUITE_START("test_copy");

    bit_array_clear_all(test_bit_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_from_str(test_bit_array,
                       "01010000 10000000 01000000 00000000 01010000 10000000 0101");

    BIT_ARRAY_DEFINE(tmp_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_clear_all(tmp_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_copy_all(tmp_array, test_bit_array, TEST_BIT_ARRAY_TOTAL);
    TEST_ARRAY_CHECK(tmp_array, TEST_BIT_ARRAY_TOTAL,
                     "01010000 10000000 01000000 00000000 01010000 10000000 0101");

    bit_array_clear_all(tmp_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_copy(tmp_array, 0, test_bit_array, 0, TEST_BIT_ARRAY_TOTAL, TEST_BIT_ARRAY_TOTAL,
                   TEST_BIT_ARRAY_TOTAL);
    TEST_ARRAY_CHECK(tmp_array, TEST_BIT_ARRAY_TOTAL,
                     "01010000 10000000 01000000 00000000 01010000 10000000 0101");

    bit_array_clear_all(tmp_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_copy(tmp_array, 4, test_bit_array, 0, TEST_BIT_ARRAY_TOTAL - 4, TEST_BIT_ARRAY_TOTAL,
                   TEST_BIT_ARRAY_TOTAL);
    TEST_ARRAY_CHECK(tmp_array, TEST_BIT_ARRAY_TOTAL,
                     "00000101 00001000 00000100 00000000 00000101 00001000 0000");

    bit_array_clear_all(tmp_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_copy(tmp_array, 32, test_bit_array, 0, TEST_BIT_ARRAY_TOTAL - 32,
                   TEST_BIT_ARRAY_TOTAL, TEST_BIT_ARRAY_TOTAL);
    TEST_ARRAY_CHECK(tmp_array, TEST_BIT_ARRAY_TOTAL,
                     "00000000 00000000 00000000 00000000 01010000 10000000 0100");

    SUITE_END();
}

static void test_logic(void)
{
    SUITE_START("test_logic");

    bit_array_clear_all(test_bit_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_from_str(test_bit_array,
                       "01010000 10000000 01000000 00000000 01010000 10000000 0101");

    BIT_ARRAY_DEFINE(tmp_array, TEST_BIT_ARRAY_TOTAL);

    // Test and
    bit_array_clear_all(tmp_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_from_str(tmp_array, "01011111 10000110 01000000 01101000 00010110 10001100 0100");
    bit_array_and(tmp_array, tmp_array, test_bit_array, TEST_BIT_ARRAY_TOTAL);
    TEST_ARRAY_CHECK(tmp_array, TEST_BIT_ARRAY_TOTAL,
                     "01010000 10000000 01000000 00000000 00010000 10000000 0100");

    // Test or
    bit_array_clear_all(tmp_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_from_str(tmp_array, "01011111 10000110 01000000 01101000 00010110 10001100 0100");
    bit_array_or(tmp_array, tmp_array, test_bit_array, TEST_BIT_ARRAY_TOTAL);
    TEST_ARRAY_CHECK(tmp_array, TEST_BIT_ARRAY_TOTAL,
                     "01011111 10000110 01000000 01101000 01010110 10001100 0101");

    // Test xor
    bit_array_clear_all(tmp_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_from_str(tmp_array, "01011111 10000110 01000000 01101000 00010110 10001100 0100");
    bit_array_xor(tmp_array, tmp_array, test_bit_array, TEST_BIT_ARRAY_TOTAL);
    TEST_ARRAY_CHECK(tmp_array, TEST_BIT_ARRAY_TOTAL,
                     "00001111 00000110 00000000 01101000 01000110 00001100 0001");

    // Test not
    bit_array_clear_all(tmp_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_not(tmp_array, test_bit_array, TEST_BIT_ARRAY_TOTAL);
    TEST_ARRAY_CHECK(tmp_array, TEST_BIT_ARRAY_TOTAL,
                     "10101111 01111111 10111111 11111111 10101111 01111111 1010");

    // TEST_ARRAY_PRINT(tmp_array, TEST_BIT_ARRAY_TOTAL);

    SUITE_END();
}

static void test_shift(void)
{
    SUITE_START("test_shift");

    bit_array_clear_all(test_bit_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_from_str(test_bit_array,
                       "01010000 10000000 01000000 00000000 01010000 10000000 0101");

    BIT_ARRAY_DEFINE(tmp_array, TEST_BIT_ARRAY_TOTAL);

    // Test shift right
    bit_array_copy_all(tmp_array, test_bit_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_shift_right(tmp_array, TEST_BIT_ARRAY_TOTAL, 4, 1);
    TEST_ARRAY_CHECK(tmp_array, TEST_BIT_ARRAY_TOTAL,
                     "00001000 00000100 00000000 00000101 00001000 00000101 1111");

    bit_array_copy_all(tmp_array, test_bit_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_shift_right(tmp_array, TEST_BIT_ARRAY_TOTAL, 12, 0);
    TEST_ARRAY_CHECK(tmp_array, TEST_BIT_ARRAY_TOTAL,
                     "00000100 00000000 00000101 00001000 00000101 00000000 0000");

    // Test shift left
    bit_array_copy_all(tmp_array, test_bit_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_shift_left(tmp_array, TEST_BIT_ARRAY_TOTAL, 4, 1);
    TEST_ARRAY_CHECK(tmp_array, TEST_BIT_ARRAY_TOTAL,
                     "11110101 00001000 00000100 00000000 00000101 00001000 0000");

    bit_array_copy_all(tmp_array, test_bit_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_shift_left(tmp_array, TEST_BIT_ARRAY_TOTAL, 12, 0);
    TEST_ARRAY_CHECK(tmp_array, TEST_BIT_ARRAY_TOTAL,
                     "00000000 00000101 00001000 00000100 00000000 00000101 0000");

    // TEST_ARRAY_PRINT(tmp_array, TEST_BIT_ARRAY_TOTAL);

    SUITE_END();
}

static void test_cmp(void)
{
    SUITE_START("test_cmp");

    bit_array_clear_all(test_bit_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_from_str(test_bit_array,
                       "01010000 10000000 01000000 00000000 01010000 10000000 0101");

    BIT_ARRAY_DEFINE(tmp_array, TEST_BIT_ARRAY_TOTAL);
    bit_array_copy_all(tmp_array, test_bit_array, TEST_BIT_ARRAY_TOTAL);

    ASSERT(bit_array_cmp(test_bit_array, tmp_array, TEST_BIT_ARRAY_TOTAL) == 0);

    bit_array_from_str(test_bit_array,
                       "11010000 10000000 01000000 00000000 01010000 10000000 0101");

    ASSERT(bit_array_cmp(test_bit_array, tmp_array, TEST_BIT_ARRAY_TOTAL) != 0);

    SUITE_END();
}

int main(void)
{
    test_assign();
    test_set_clear();
    test_toggle();
    test_copy();
    test_logic();
    test_shift();
    test_cmp();

    return 0;
}
