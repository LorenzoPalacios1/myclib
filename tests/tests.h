#ifndef _MYBASICS_TESTS
#define _MYBASICS_TESTS

/* The total number of tests that can be run. */
#define NUM_TESTS (sizeof(test_functions) / sizeof(*test_functions))

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

#endif
