#include "tests.h"

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../array/array.h"
#include "../randomgen/randomgen.h"
#include "../strext/strextensions.h"

typedef clock_t (*test_func_t)(void);

typedef struct test_entry {
  test_func_t func;
  clock_t time_taken;
} test_entry;

/*
 * Generates and applies a seed for `rand()`.
 * \returns The applied seed.
 */
static inline unsigned apply_seed_to_srand(void) {
#if (MANUAL_SEED_SET == 0)
  const unsigned NEW_SEED = (unsigned)time(NULL);
  srand(NEW_SEED);
  return NEW_SEED;
#else
  srand(MANUAL_SEED_SET);
  return MANUAL_SEED_SET;
#endif
}

/* MAKE SURE TO UPDATE BOTH ARRAYS */
static clock_t (*const test_functions[])(void) = {NULL};
static const char *const test_names[NUM_TESTS] = {""};

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
 * Asks the user to choose from a list of available tests as defined by
 * `test_functions` and `test_names`.
 *
 * \returns An array of function pointers where each element corresponds to a
 * given test function.
 *
 * \note As it stands, this function was written with the intent of getting it
 * out of the way so the rest of this test suite can be worked on.
 * Bearing this in mind, the quality is not up to my usual standards, and I
 * intend to refactor this function later.
 */
static test_entry *get_user_test_selection(void) {
  test_entry *const selected_tests = malloc(sizeof(test_entry) * NUM_TESTS);

  /* 
   * I'm trying to puzzle out a compile-time method to allocate exactly enough
   * memory to store the digits of a `size_t` in the below buffer.
   */
  char buf[32];
  size_t test_index;
  size_t i = 0;
  for (; i < NUM_TESTS; i++) {
    if (fgets(buf, sizeof(buf) - 1, stdin) == NULL) break;
    const int sscanf_status = sscanf(buf, "%zu", &test_index);
    if (sscanf_status == 1 && test_index < NUM_TESTS) {
      selected_tests[i].func = test_functions[test_index];
      selected_tests[i].time_taken = 0;
    } else if (sscanf_status == EOF) { /* Exit on single newline. */
      break;
    } else { /* Redo iteration on invalid entry. */
      i--;
    }
  }
  selected_tests[i].func = NULL; /* Terminator element for `selected_tests`. */
  return selected_tests;
}

static clock_t run_tests(test_entry *const test_selection) {
  clock_t total_time = 0;
  for (size_t i = 0; test_selection[i].func != NULL; i++) {
    const clock_t time_taken = test_selection[i].func();
    test_selection[i].time_taken = time_taken;
    total_time += time_taken;
  }
  return total_time;
}

void greet_and_init(void) {
  puts("\n - MyBasics Testing Suite - \n");
  printf("rand() Seed: %u\nrand() Sample: %d\n", apply_seed_to_srand(), rand());
#if (!RUN_ALL_TESTS_KEYWORD_IS_CASE_SENSITIVE)
  for (size_t i = 0; RUN_ALL_TESTS_KEYWORD[i] != '\0'; i++)
    RUN_ALL_TESTS_KEYWORD[i] = tolower(RUN_ALL_TESTS_KEYWORD[i]);
#endif
}

int main(void) {
  greet_and_init();
  prompt_user();
  test_entry *const selected_tests = get_user_test_selection();
  const size_t total_time_taken = run_tests(selected_tests);
  free(selected_tests);
  printf("Total elapsed time: %zu ms\n", total_time_taken);

  return 0;
}
