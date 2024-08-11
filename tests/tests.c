#include "tests.h"

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vadefs.h>

#include "../array/array.h"
#include "../randomgen/randomgen.h"
#include "../strext/strext.h"

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

/*
 * Operates as if `vprintf()` itself was called directly, but measures the time
 * taken for the function call to return.
 *
 * \return The time elapsed by the function.
 */
static inline clock_t timed_printf(const char *format, ...) {
  const clock_t start_time = clock();
  va_list args;
  _crt_va_start(args, format);
  vprintf(format, args);
  _crt_va_end(args);
  const clock_t end_time = clock() - start_time;
  return end_time;
}

/*
 * Helper function used by test functions to print `int` test data.
 *
 * Returns the amount of time taken to complete the operation.
 */
static clock_t print_data(const int *data, const size_t num_elems) {
  const clock_t START_TIME = clock();
  char buf[512] = "[";
  for (size_t data_i = 0, buf_i = 1; data_i < num_elems; data_i++) {
    if (buf_i >= sizeof(buf) - 1) {
      buf[sizeof(buf) - 1] = '\0';
      printf("%s", buf);
      buf_i = 0;
    }
    if (data_i == num_elems - 1) {
      snprintf(buf + buf_i, sizeof(buf) - buf_i, "%d]", data[data_i]);
      break;
    }
    buf_i += snprintf(buf + buf_i, sizeof(buf) - buf_i, "%d, ", data[data_i]);
  }
  puts(buf);
  const clock_t elapsed_time = clock() - START_TIME;
  return elapsed_time;
}

/* - TEST FUNCTIONS BEGIN - */

static clock_t _test_new_array(void) {
  const int test_data[][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 0}};
  puts("Testing new_array()");
  clock_t start_time = clock();
  for (size_t i = 0; i < SIZEOF_ARR(test_data); i++) {
    clock_t IO_TIME = timed_printf("Test %zu data: ", i) +
                      print_data(test_data[i], SIZEOF_ARR(test_data[i]));
    array_t *arr =
        new_array(test_data[i], sizeof(**test_data), SIZEOF_ARR(*test_data));
    IO_TIME += timed_printf("array_t contents: ") +
               print_data(arr->data, arr->num_elems) +
               timed_printf("Status: (%d)\n",
                            memcmp(test_data[i], arr->data, arr->num_elems));
    delete_array(arr);
    start_time += IO_TIME;
  }
  const clock_t end_time = clock() - start_time;

  puts("new_array() tests complete.");
  return end_time;
}

/* - TEST FUNCTIONS END -*/

/* MAKE SURE TO UPDATE BOTH ARRAYS */
static clock_t (*const test_functions[])(void) = {_test_new_array};
static const char *const test_names[NUM_TESTS] = {"new_array()"};

static void prompt_user(void) {
  puts("Your test choices are:");
  for (size_t i = 0; i < NUM_TESTS; i++) printf("%zu. %s\n", i, test_names[i]);
  printf(
      "\nEnter the number(s) of the tests you want to run, or \"%s\" to run "
      "all tests.\n"
      "Press the enter/return key to complete your selection and begin running "
      "the selected tests.\n",
      RUN_ALL_TESTS_KEYWORD);
}

void str_to_lower(char *str) {
  while (*str != '\0') {
    *str = tolower(*str);
    str++;
  }
}

/*
 * Helper function used by `get_user_test_selection`.
 * `fgets()` retains newlines inside input, thereby making detection of
 * `RUN_ALL_TESTS_KEYWORD` difficult, and `scanf()` is too finicky.
 */
static void get_line_from_stdin(char *const str, const size_t max_len) {
  for (size_t i = 0; i < max_len; i++) {
    const char chr = getchar();
    if (chr == '\n') {
      str[i] = '\0';
      return;
    }
    str[i] = chr;
  }
  /* Discard any unused input. */
  while (getchar() != '\n');
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
  test_entry *const selected_tests =
      malloc(sizeof(test_entry) * (NUM_TESTS + 1));

  char buf[DIGITS_IN_NUM(SIZE_MAX) + 1];
  size_t test_index;
  size_t i = 0;
  for (; i < NUM_TESTS; i++) {
    get_line_from_stdin(buf, sizeof(buf) - 1);
    str_to_lower(buf);
    if (strcmp(RUN_ALL_TESTS_KEYWORD, buf) == 0) {
      for (size_t j = 0; j < NUM_TESTS; j++) {
        selected_tests[i].func = test_functions[i];
        selected_tests[i].time_taken = 0;
      }
      selected_tests[NUM_TESTS].func = NULL;
      return selected_tests;
    }

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
  /* Ensures `RUN_ALL_TESTS_KEYWORD` is lowercase. */
  /* clang-format off */
  #if (!RUN_ALL_TESTS_KEYWORD_IS_CASE_SENSITIVE)
    str_to_lower(RUN_ALL_TESTS_KEYWORD);
  #endif
  /* clang-format on */
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
