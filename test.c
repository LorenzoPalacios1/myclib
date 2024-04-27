#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MyBasics.h"
#include "bpio/boilerplateio.h"
#include "randomgen/randomgen.h"

/*
 * Small note:
 * This test suite is quite worthy of a remake.
 * Expect the majority of the below macros and function bodies to be changed.
 * What I have done in this file (so far) is fix major errors that spawned as a
 * result of the library's restructuring.
 *
 * Once I get around to rewriting things, this note will be removed.
 */

/* The total number of tests that can be run. */
#define NUM_TESTS (int)(sizeof(test_names) / sizeof(*test_names))

/*
 * Set to non-zero to use the specified seed.
 * Otherwise, set to zero for a pseudo-random one.
 */
#define MANUAL_SEED_SET (0)

/* The number of passes each test function should run. */
#define DEFAULT_NUM_PASSES (5u)

/*
 * The maximum number of characters to be read when prompting the user for test
 * choices.
 */
#define MAX_INPUT_PER_LINE (10u)

/*
 * The terminating value to be used by tests_prompt() to signify the end of its
 * returned int array. Preferably keep this value either greater than or less
 * than NUM_TESTS.
 */
#define USR_CHOICES_TERMINATOR (-1)

/*
 * Keyword which allows the user to select all available tests at once. String
 * length should be no longer than MAX_INPUT_PER_LINE. This is CASE-SENSITIVE.
 */
#define RUN_ALL_TESTS_KEYWORD "ALL"

/* The maximum number of characters the user can input when selecting tests. */
#define MAX_INPUT_CHARS (NUM_TESTS / 10 + 1)

/*
 * - Utility Functions -
 */

// Generates a new seed for rand()
// Returns the generated seed
static inline unsigned random_seeder(void) {
  if (MANUAL_SEED_SET == 0) {
    const time_t CUR_SEED = time(NULL) << 4;
    srand(CUR_SEED);
    return CUR_SEED;
  } else {
    srand(MANUAL_SEED_SET);
    return MANUAL_SEED_SET;
  }
}

/*
 * - Test Functions -
 */

static clock_t charToIntTests(void) {
  static const size_t MIN_RAND_STR_LEN = 10;
  static const size_t MAX_RAND_STR_LEN = 30;
  const char *rand_strings[DEFAULT_NUM_PASSES];

  // Generating the random strings and printing them to the user
  for (unsigned i = 0; i < DEFAULT_NUM_PASSES; i++) {
    rand_strings[i] =
        randomString(VIS_CHAR_START, VIS_CHAR_END,
                     randomInt(MIN_RAND_STR_LEN, MAX_RAND_STR_LEN));
    puts(rand_strings[i]);
  }

  clock_t total_time = 0;
  for (unsigned i = 0; i < DEFAULT_NUM_PASSES; i++) {
    char output[32];
    unsigned j = 0;
    for (; rand_strings[i][j] != '\0'; j++) {
      const clock_t run_time = clock();
      output[j] = atoi(&rand_strings[i][j]);
      total_time += clock() - run_time;
    }
    output[j] = '\0';
    puts(output);
  }
  return total_time;
}

// MAKE SURE TO UPDATE BOTH ARRAYS
static const char *const test_names[] = {"charToIntTests"};
static clock_t (*const tests[NUM_TESTS])(void) = {charToIntTests};

/*
 * Asks the user to choose from a list of available tests as defined above.
 * Returns an array of ints that correspond to the tests have been chosen,
 * terminated by USR_CHOICES_TERMINATOR.
 */
static int *tests_prompt(void) {
  puts("Your test choices are:");
  for (unsigned i = 0; i < NUM_TESTS; i++)
    printf_s("%d. %s\n", i, test_names[i]);

  puts("\nEnter the test number(s) you want to run, or \"" RUN_ALL_TESTS_KEYWORD
       "\" (case sensitive) to run all of the tests.\n"
       "Press the enter/return key to start running the selected tests.");

  /*
   * This array will hold the indexes of the tests the user selects, and will
   * use the value defined by USR_CHOICES_TERMINATOR to terminate the array,
   * hence "(NUM_TESTS + 1)"
   */
  static int choices[NUM_TESTS + 1];

  /*
   * Using a static array allows for this function to be run again without
   * having to incur reallocation. The user should only be entering as many
   * characters as there are digits in NUM_TESTS or characters in
   * RUN_ALL_TESTS_KEYWORD.
   *
   * This is a pointer-to-array to satisfy the first parameter of getStrStdin().
   *
   * "MAX_INPUT_CHARS + 1" to account for a null terminator.
   */
  static char input[MAX_INPUT_CHARS < sizeof(RUN_ALL_TESTS_KEYWORD)
                         ? sizeof(RUN_ALL_TESTS_KEYWORD)
                         : (MAX_INPUT_CHARS + 1)];

  unsigned i = 0;
  for (; i < NUM_TESTS; i++) {
    // - NEWLINE CHECK -

    /*
     * If the user enters only a newline, assume the user is finished making
     * choices and exit the input loop
     */
    if (getStrStdin(input, MAX_INPUT_PER_LINE) == 0) {
      if (i == 0) return NULL;
      break;
    }

    // - RUN ALL TESTS KEYWORD CHECK -

    /*
     * Overwrites any previous choices to give this keyword a consistent
     * behavior
     */
    if (strcmp(input, RUN_ALL_TESTS_KEYWORD) == 0) {
      for (int j = 0; j < NUM_TESTS; j++) choices[j] = j;
      i = NUM_TESTS;
      break;
    }

    // - INTEGER PARSING -

    if ((choices[i] = atoi(input)) == SUCCESS && choices[i] < NUM_TESTS &&
        choices[i] >= 0) {
      printf_s("Successfully added \"%s\" to the queue.\n",
               test_names[choices[i]]);
    } else {
      // "NUM_TESTS - 1" since 'NUM_TESTS' does not describe an array index, but
      // the literal number of tests available
      printf_s(
          "Invalid choice\nYour choice must be a whole number between 0-%d "
          "(inclusive)\n",
          NUM_TESTS - 1);
      i--;
    }
  }

  i != NUM_TESTS ? (choices[i] = USR_CHOICES_TERMINATOR)
                 : (choices[NUM_TESTS] = USR_CHOICES_TERMINATOR);

  return choices;
}

int main(void) {
  clock_t global_timer = clock();
  puts("\n - MyBasics Testing Suite - \n");

  printf_s("rand() Seed: %u\nrand() Sample: %d\n", random_seeder(), rand());

  const clock_t user_input_timer = clock();
  const int *user_choices = tests_prompt();

  // Omit the amount of time the user took to input their choices from
  // 'global_timer'
  global_timer += clock() - user_input_timer;

  if (user_choices == NULL) return BAD_PTR;

  for (int i = 0; i < NUM_TESTS; i++) {
    if (user_choices[i] != USR_CHOICES_TERMINATOR) {
      printf_s("\nNow running %d - %s\n", user_choices[i],
               test_names[user_choices[i]]);
      const clock_t time_taken = tests[user_choices[i]]();
      printf_s("\nFinished running %d - %s\nElapsed time: %ldms\n",
               user_choices[i], test_names[user_choices[i]], time_taken);
    } else
      break;
  }
  printf_s("\nTotal elapsed time: %ldms", clock() - global_timer);

  return SUCCESS;
}
