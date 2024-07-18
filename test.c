#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MyBasics.h"
#include "array/array.h"
#include "randomgen/randomgen.h"
#include "strext/strextensions.h"

/* The total number of tests that can be run. */
#define NUM_TESTS (unsigned long long)(sizeof(tests) / sizeof(*tests))

/*
 * Replace the zero with your desired seed.
 * Otherwise, leave this at zero for a pseudo-random one.
 */
#define MANUAL_SEED_SET (0)

/* The number of passes each test function should run. */
#define DEFAULT_NUM_PASSES (5)

/* Allows the user to select all available tests at once. */
#define RUN_ALL_TESTS_KEYWORD "ALL"

#define IS_RUN_ALL_TESTS_KEYWORD_CASE_SENSITIVE (false)

#if (!IS_RUN_ALL_TESTS_KEYWORD_CASE_SENSITIVE)
static char _RUN_ALL_TESTS_KEYWORD[] = RUN_ALL_TESTS_KEYWORD;
#undef RUN_ALL_TESTS_KEYWORD
static char *const RUN_ALL_TESTS_KEYWORD = _RUN_ALL_TESTS_KEYWORD;
#endif

/*
 * - Utility Functions -
 */
// Generates and applies a seed for `rand()`.
// \returns The applied seed.
static unsigned random_seeder(void) {
#if (MANUAL_SEED_SET == 0)
  const unsigned NEW_SEED = (unsigned)time(NULL);
  srand(NEW_SEED);
  return NEW_SEED;
#else
  srand(MANUAL_SEED_SET);
  return MANUAL_SEED_SET;
#endif
}

/*
 * - Test Functions -
 */

// MAKE SURE TO UPDATE BOTH ARRAYS
static clock_t (*const tests[])(void) = {NULL};
static const char *test_names[NUM_TESTS] = {"string_from_chars"};

static void prompt_user(void) {
  puts("Your test choices are:");
  for (size_t i = 0; i < NUM_TESTS; i++) printf("%zu. %s\n", i, test_names[i]);
  printf(
      "\nEnter the test number(s) you want to run, or \"%s\" to run all "
      "tests.\n"
      "Press the enter/return key to complete your selection and begin running "
      "the selected tests.\n",
      RUN_ALL_TESTS_KEYWORD);
}

/*
 * Asks the user to choose from a list of available tests as defined by `tests`
 * and `test_names`.
 * \returns An array of `size_t` whose elements each correspond to a test.
 */
static array_t *get_user_test_selection(void) {
  size_t selected_tests[NUM_TESTS];

  size_t current_test_num = 0;
  for (; current_test_num < NUM_TESTS; current_test_num++) {
    /*
     * If the user enters only a newline, assume the user is finished making
     * choices and exit the input loop.
     */
    {
      const int first_char = getchar();
      if (first_char == '\n') break;
      ungetc(first_char, stdin);
    }
    string_t *input = string_from_stream(stdin);

#if (!IS_RUN_ALL_TESTS_KEYWORD_CASE_SENSITIVE)
    char *const str = input->data;
    for (size_t i = 0; str[i] != '\0'; i++) str[i] = tolower(str[i]);
#endif
    /* Overwrites any previous choices for consistent behavior. */
    if (strcmp(input->data, RUN_ALL_TESTS_KEYWORD) == 0) {
      for (size_t test_num = 0; test_num < NUM_TESTS; test_num++)
        selected_tests[test_num] = test_num;
      break;
    }
  }
  array_t *const selected_tests_actual =
      new_array(&selected_tests, sizeof(int), current_test_num);
  return selected_tests_actual;
}

static size_t run_tests(const array_t *const test_selection) {
  size_t total_time_taken = 0;
  const int *const test_indexes = test_selection->data;
  for (size_t i = 0; i < test_selection->num_elems; i++) {
    printf("\nTest #%d (%s)\n", test_indexes[i], test_names[test_indexes[i]]);
    const size_t test_elapsed_time = tests[test_indexes[i]]();
    printf("\nTest #%d complete (%s)\nTime taken: %lldms\n", test_indexes[i],
           test_names[test_indexes[i]], test_elapsed_time);
    total_time_taken += test_elapsed_time;
  }
  return total_time_taken;
}

void greet_and_init(void) {
  puts("\n - MyBasics Testing Suite - \n");
  printf("rand() Seed: %u\nrand() Sample: %d\n", random_seeder(), rand());
#if (!RUN_ALL_TESTS_KEYWORD_IS_CASE_SENSITIVE)
  for (size_t i = 0; RUN_ALL_TESTS_KEYWORD[i] != '\0'; i++)
    RUN_ALL_TESTS_KEYWORD[i] = tolower(RUN_ALL_TESTS_KEYWORD[i]);
#endif
}

int main(void) {
  greet_and_init();
  prompt_user();
  array_t *const user_choices = get_user_test_selection();
  const size_t total_time_taken = run_tests(user_choices);
  delete_array(user_choices);
  printf("Total elapsed time: %zu ms\n", total_time_taken);

  return 0;
}
