#ifndef BOILERPLATEIO_H
#define BOILERPLATEIO_H

#include <stdio.h>

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
 * \note A null terminator will replace the delimiter where it would have
 * occurred within the string.
 *
 * \note If 'length' characters were written before the delimiter was
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
string_t *getStr(const char delim, const size_t max_length,
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
string_t *getStrStdin(const size_t length);
#endif
