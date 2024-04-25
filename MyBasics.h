#ifndef _INC_MYBASICS
#define _INC_MYBASICS

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "MyBasics.h"
#include "randomgen/randomgen.h"

#define ERRCODE_SUCCESS (0x0) /* Denotes a successful execution. */
#define ERRCODE_GENERAL \
  (0x1) /* An undefined status code; consult the function's documentation. */
#define ERRCODE_BAD_PTR (0x2) /* Denotes a bad pointer argument. */
#define ERRCODE_BAD_FILE \
  (0x3) /* Denotes a bad or `NULL` pointer to a `FILE` argument. */
#define ERRCODE_FILE_AT_EOF \
  (0x4) /* The passed `FILE` is at `EOF` *before* any reading occurred. */
#define ERRCODE_BAD_INPUT                                                      \
  (0x5) /* Denotes a bad string (eg. passing alphabetical chars to strToInt()) \
         */
#define ERRCODE_FILE_REACHED_EOF \
  (0x6) /* The function reached `EOF` during execution. */

/*
 * Writes to the first argument, str, from stream until reading the delimiter
 * character, or upon the string reaching the specified length.
 *
 * \returns Returns the length of the string discounting the null terminator, or
 * 0 if nothing was written.
 *
 * \note If this function returns 0, the string will be left unmodified (that
 * is, nothing will be written).
 *
 * \note A null terminator will replace the delimiter within the string.
 *
 * \note Otherwise, if 'length' characters were written before the delimiter was
 * read, a null terminator will be appended to the end of the string if no
 * delimiter was read or EOF was not met.
 *
 * \note If this occurs, the returned value will be length + 1.
 *
 * \param str The pointer to string that will be written to
 * \param delim The delimiter character
 * \param max_length The maximum size of the string
 * \param stream The input stream to read from
 */
size_t getStr(char *str, const char delim, const size_t max_length,
              FILE *const stream);

/*
 * Writes to the first argument, `str`, from `stdin` until reading a newline
 * character, or upon the string reaching the specified length.
 *
 * The difference between `getStr()` and this function is that this function has
 * an implicit delimiter, `\\n`, an implicit stream, `stdin`, and will
 * automatically flush any unused input from stdin.
 *
 * This function has the same semantics as `getStr()` otherwise.
 */
size_t getStrStdin(char *str, const size_t length);

/*
 * Returns `true` if the passed char represents an alphabetical letter.
 *
 * Otherwise returns `false`.
 */
inline bool isAlphabetical(const char letter) {
  return ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'));
}

/*
 * Returns `true` if the passed char represents a number.
 *
 * Otherwise returns `false`.
 */
inline bool isNumerical(const char number) {
  return (number >= '0' && number <= '9');
}

/*
 * Returns `true` if the passed char represents either a number or an
 * alphabetical letter.
 *
 * Otherwise returns `false`.
 */
inline bool isAlphaNumerical(const char item) {
  return (isAlphabetical(item) || isNumerical(item));
}

/*
 * Returns the passed character as an integer singleton (0-9), if possible.
 *
 * If the passed character does not represent an integer, this function returns
 * `-1`.
 */
inline int charToInt(const char num) {
  if (isNumerical(num)) return num - '0';
  return -1;
}

int readInt(int *const num, FILE *stream);
#endif
