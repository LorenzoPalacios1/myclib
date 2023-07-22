#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MyBasics.h"

// Set to non-zero to tell the program to use the specified seed.
// Otherwise, set to zero to let the program generate a seed.
#define MANUAL_SEED_SET 0

// The total number of tests available.
// (MAKE SURE TO INCREMENT THIS WHEN ADDING A NEW TEST FUNCTION)
#define NUM_TESTS 2

// The maximum number of characters to be read by getStrStdin() when prompting the user
// for their test choices.
#define MAX_INPUT_PER_LINE 10

// The terminator to be used by tests_prompt() to signify the end of the returned
// int array.
// Preferably keep this value greater than or less than NUM_TESTS.
#define USR_CHOICES_TERMINATOR -1

// Keyword for the user to input in order to select all of the available tests at once.
// String length should be no longer than MAX_INPUT_PER_LINE.
#define RUN_ALL_TESTS_KEYWORD "ALL"

// The minimum signed char value for a visible character (inclusive).
#define VIS_CHAR_START ' ' + 1
// The maximum signed char value for a visible character (inclusive).
#define VIS_CHAR_END '' - 1

/*
 * - Utility Functions -
 */

/*
 * - Random Singleton Generators -
 */

// Returns a single int from rand() within the specified range (inclusive).
// Always returns 0 if (max < min)
int randomint(const int min, const int max)
{
    if (max < min)
    {
        fprintf_s(stderr, "randomint(): Invalid values for max and min values (%u is NOT GREATER THAN %u)\n", max, min);
        return 0;
    }
    return rand() % (max - min + 1) + min;
}

// Returns a single visible unsigned char from rand()
unsigned char random_vis_uchar(void)
{
    return randomint(UCHAR_MAX, VIS_CHAR_START);
}

// Returns a single unsigned char from rand() within the min and max values (inclusive)
unsigned char random_uchar_range(const unsigned char min, const unsigned char max)
{
    if (max < min)
    {
        fprintf_s(stderr, "random_uchar_range(): Invalid values for max and min values (%u is NOT GREATER THAN %u)\n", max, min);
        return 0;
    }
    return randomint(min, max);
}

// Generates a new seed for rand()
// Returns the generated seed
unsigned random_seeder(void)
{
    if (MANUAL_SEED_SET == 0)
    {
        const unsigned CUR_SEED = time(NULL) % rand() * (((rand() + 1000) + 1) << 4);
        srand(CUR_SEED);
        return CUR_SEED;
    }
    else
    {
        srand(MANUAL_SEED_SET);
        return MANUAL_SEED_SET;
    }
}

/*
 * - Mass Random Generators -
 */

// Returns a random string of unsigned chars using random_vis_uchar().
unsigned char *generate_test_ustring(unsigned char min, unsigned char max, const size_t length)
{
    if (max < min)
    {
        fprintf_s(stderr, "generate_test_ustring(): Invalid values for max and min values (%u is NOT GREATER THAN %u)\n", max, min);
        return 0;
    }

    if (length == 0)
    {
        fprintf_s(stderr, "generate_test_ustring(): Invalid length: %llu", length);
        return 0;
    }

    // (length + 1) to account for the null terminator
    unsigned char *str = malloc(sizeof(unsigned char) * (length + 1));

    if (str == NULL)
    {
        fputs("generate_test_ustring(): malloc() failure; returning NULL\n", stderr);
        return NULL;
    }

    for (unsigned i = 0; i < length; i++)
    {
        str[i] = random_uchar_range(min, max);
    }
    str[length] = '\0';

    return str;
}

// Returns a random string of signed chars using random_vis_uchar().
char *generate_test_string(char min, char max, const size_t length)
{
    const clock_t timer = clock();
    if (max < min)
    {
        fprintf_s(stderr, "generate_test_ustring(): Invalid values for max and min values (%u is NOT GREATER THAN %u)\n", max, min);
        return 0;
    }

    if (length == 0)
    {
        fprintf_s(stderr, "generate_test_ustring(): Invalid length: %llu", length);
        return 0;
    }

    // (length + 1) to account for the null terminator
    char *str = malloc(sizeof(char) * (length + 1));
    if (str == NULL)
    {
        fputs("generate_test_string(): malloc() failure; returning NULL\n", stderr);
        return NULL;
    }

    for (unsigned i = 0; i < length; i++)
    {
        str[i] = random_uchar_range(min, max);
    }
    str[length] = '\0';
    printf_s("%ldms\n", clock() - timer);

    return str;
}

char *generate_test_string_alphabetic(const size_t length)
{
    if (length == 0)
    {
        fprintf_s(stderr, "generate_test_string_alphabetic(): Invalid length: %llu", length);
        return 0;
    }

    // (length + 1) to account for the null terminator
    char *str = malloc(sizeof(char) * (length + 1));
    if (str == NULL)
    {
        fprintf_s(stderr, "generate_test_string(): malloc() failure; returning NULL\n");
        return NULL;
    }

    for (unsigned i = 0; i < length; i++)
    {
        if (rand() % 2)
        {
            str[i] = random_uchar_range('a', 'z');
        }
        else
        {
            str[i] = random_uchar_range('A', 'Z');
        }
    }
    str[length] = '\0';

    return str;
}

// Returns a pointer to a read-only FILE containing the specified amount of random characters
// This function will overwrite any files with the same name
FILE *generate_test_file(const char *filename, const size_t num_chars)
{
    FILE *NEW_TEST_FILE = NULL;

    const errno_t _fopen_errcode = fopen_s(&NEW_TEST_FILE, filename, "w");

    if (_fopen_errcode != 0)
    {
        fprintf_s(stderr, "generate_test_file(): Failed to open file %s\nError code: %d\n", filename, _fopen_errcode);
        return NULL;
    }

    // Here we fill the file with random gibberish
    for (unsigned i = 0; i < num_chars; i++)
    {
        fputc(random_vis_uchar(), NEW_TEST_FILE);
    }

    freopen_s(&NEW_TEST_FILE, filename, "r", NEW_TEST_FILE);
    return NEW_TEST_FILE;
}

// Returns a pointer to a read-only FILE containing the specified amount of random characters
// with values between min and max (inclusive).
// This function will overwrite any files with the same name.
FILE *generate_test_file_range(const char *filename, const size_t num_chars, const unsigned char min, const unsigned char max)
{
    FILE *NEW_TEST_FILE = NULL;

    const errno_t _fopen_errcode = fopen_s(&NEW_TEST_FILE, filename, "w");

    if (_fopen_errcode != 0)
    {
        fprintf_s(stderr, "generate_test_file_range(): Failed to open file %s\nError code: %d\n", filename, _fopen_errcode);
        return NULL;
    }

    // Here we fill the file with random gibberish
    unsigned char buffer[num_chars + 1];
    for (unsigned i = 0; i < num_chars; i++)
    {
        buffer[i] = random_uchar_range(min, max);
    }
    buffer[num_chars] = '\0';
    fprintf_s(NEW_TEST_FILE, "%s", buffer);

    freopen_s(&NEW_TEST_FILE, filename, "r", NEW_TEST_FILE);
    return NEW_TEST_FILE;
}

/*
 * - Test Functions -
 */

void fDiscardLineTests(void)
{
}

void charToIntTests(void)
{
}

// Array of function pointers for easy user access.
// Will have to be manually updated, however this shouldn't be too difficult to keep up with.
// Make sure to update 'NUM_TESTS' and 'test_names' accordingly.
static void (*const tests[])(void) = {charToIntTests, fDiscardLineTests};
static const char *test_names[] = {"charToIntTests", "fDiscardLineTests"};

// Gives the user a generic prompt allowing them to choose from a list of available tests.
// Returns an array of ints that correspond to the tests they have chosen terminated by USR_CHOICES_TERMINATOR
int *tests_prompt(void)
{
    puts("Your test choices are:");
    for (int i = 0; i < NUM_TESTS; i++)
    {
        printf_s("%d. %s\n", i, test_names[i]);
    }
    printf_s("\nEnter the test number(s) you want to run, or \"%s\" (case insensitive) to run all of the tests.\n", RUN_ALL_TESTS_KEYWORD);
    puts("Press the enter/return key to start running the selected tests.");

    // This array will hold the indexes of the tests the user selects, but will use the value defined by USR_CHOICES_TERMINATOR
    // to define the end of the array, hence "(NUM_TESTS + 1)"
    int *choices = malloc(sizeof(int) * (NUM_TESTS + 1));
    if (choices == NULL)
    {
        fprintf_s(stderr, "tests_prompt(): malloc() failure");
        return NULL;
    }

    char *input = NULL;
    int i = 0;
    for (; i < NUM_TESTS; i++)
    {
        // - NEWLINE CHECK -

        // We exit upon reading a single newline which can be detected if getStrStdin() returns 0
        if (getStrStdin(&input, MAX_INPUT_PER_LINE) == 0)
        {
            // Returning NULL if the user enters a single newline as their first "choice" since
            // there's no point in doing anything else with the memory allocated for 'choices'
            if (i == 0)
            {
                free(choices);
                return NULL;
            }
            break;
        }

        // - RUN ALL TESTS KEYWORD CHECK -

        // We overwrite any previous choices to give this a consistent behavior
        if (strcasecmp(input, RUN_ALL_TESTS_KEYWORD) == 0)
        {
            for (int j = 0; j < NUM_TESTS; j++)
            {
                choices[j] = j;
            }
            i = NUM_TESTS;
            break;
        }

        // - INTEGER PARSING -

        // If the user inputs non-numerical characters, a value >= 'NUM_TESTS', or a value less than 0, we
        // notify them and prevent the increment of 'i'
        if (strToInt(input, choices + i) == ERRCODE_SUCCESS && choices[i] < NUM_TESTS && choices[i] >= 0)
        {
            printf_s("Successfully added \"%s\" to the queue.\n", test_names[choices[i]]);
        }
        else
        {
            // "NUM_TESTS - 1" since 'NUM_TESTS' does not describe an array index, but the literal number of tests available
            printf_s("Invalid choice\nYour choice must be a whole number between 0-%d (inclusive)\n", NUM_TESTS - 1);
            // Negating the loop's incrementation so that the respective index of 'choices' has a chance to get a valid value
            i--;
        }
    }

    // Appending the necessary terminator to the array 'choices'
    if (i != NUM_TESTS)
    {
        choices[i] = USR_CHOICES_TERMINATOR;
    }
    else
    {
        choices[NUM_TESTS] = USR_CHOICES_TERMINATOR;
    }

    return choices;
}
int main(void)
{


    // MYBASICS FUNCTION TESTING
    // DELETE AFTER YOU'RE FINISHED
    int num = -2;
    strToInt("0000000", &num);
    printf_s("num = %d", num);
    return 0;



    clock_t timer = clock();
    puts("\n - MyBasics Testing Suite - \n");

    // Ensuring that the seed for this test run is shown to the user so that ample information
    // is given for any future debugging
    printf_s("rand() Seed: %u\nrand() Sample: %d\n", random_seeder(), rand());

    const clock_t user_input_timer = clock();
    const int *user_choices = tests_prompt();
    timer += clock() - user_input_timer;

    if (!user_choices)
        return ERRCODE_NULL_PTR;

    // Now we run the tests
    for (int i = 0; i < NUM_TESTS; i++)
    {
        if (user_choices[i] != USR_CHOICES_TERMINATOR)
        {
            printf_s("Now running %d - %s\n", user_choices[i], test_names[user_choices[i]]);
            tests[user_choices[i]]();
        }
        else
            break;
    }
    printf("Overall time taken: %ldms", clock() - timer);
    return ERRCODE_SUCCESS;
}
