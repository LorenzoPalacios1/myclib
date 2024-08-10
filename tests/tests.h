#ifndef _MYBASICS_TESTS
#define _MYBASICS_TESTS

#include <stdbool.h>
#include <time.h>

/* Intended to be a helper macro for DIGITS_IN_NUM. */
#define REMOVE_SIGN(n) ((n) < 0 ? -n : n)
/* clang-format off */
#define DIGITS_IN_NUM(n) ( \
    (REMOVE_SIGN(n)) < 10 ? 1 \
  : (REMOVE_SIGN(n)) < 100 ? 2 \
  : (REMOVE_SIGN(n)) < 1000 ? 3 \
  : (REMOVE_SIGN(n)) < 10000 ? 4 \
  : (REMOVE_SIGN(n)) < 100000 ? 5 \
  : (REMOVE_SIGN(n)) < 1000000 ? 6 \
  : (REMOVE_SIGN(n)) < 10000000 ? 7 \
  : (REMOVE_SIGN(n)) < 100000000 ? 8 \
  : (REMOVE_SIGN(n)) < 1000000000 ? 9 \
  : (REMOVE_SIGN(n)) < 10000000000 ? 10 \
  : (REMOVE_SIGN(n)) < 100000000000 ? 11 \
  : (REMOVE_SIGN(n)) < 1000000000000 ? 12 \
  : (REMOVE_SIGN(n)) < 10000000000000 ? 13 \
  : (REMOVE_SIGN(n)) < 100000000000000 ? 14 \
  : (REMOVE_SIGN(n)) < 1000000000000000 ? 15 \
  : (REMOVE_SIGN(n)) < 10000000000000000 ? 16 \
  : (REMOVE_SIGN(n)) < 100000000000000000 ? 17 \
  : (REMOVE_SIGN(n)) < 1000000000000000000 ? 18 \
  : (REMOVE_SIGN(n)) < 10000000000000000000ULL ? 19 : 20)
/* clang-format on */

#define SIZEOF_ARR(arr) (sizeof(arr) / sizeof(*(arr)))

/* The total number of tests that can be run. */
#define NUM_TESTS SIZEOF_ARR(test_functions)

/*
 * Replace the zero with your desired seed.
 * Otherwise, leave this at zero for a pseudo-random one.
 */
#define MANUAL_SEED_SET (0)

/* Allows the user to select all available tests at once. */
#define RUN_ALL_TESTS_KEYWORD "ALL"

#define IS_RUN_ALL_TESTS_KEYWORD_CASE_SENSITIVE (false)

#if (!IS_RUN_ALL_TESTS_KEYWORD_CASE_SENSITIVE)
static char _RUN_ALL_TESTS_KEYWORD[] = RUN_ALL_TESTS_KEYWORD;
#undef RUN_ALL_TESTS_KEYWORD
static char *const RUN_ALL_TESTS_KEYWORD = _RUN_ALL_TESTS_KEYWORD;
#endif

typedef clock_t (*test_func_t)(void);

typedef struct test_entry {
  test_func_t func;
  clock_t time_taken;
} test_entry;

#endif
