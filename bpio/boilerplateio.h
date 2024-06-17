#ifndef BOILERPLATEIO_H
#define BOILERPLATEIO_H

#include <stdio.h>

#include "../strext/strextensions.h"

/*
 * \returns A string whose content consists of characters within `stream` until
 * either `delim` is read, or the string reaches the specified length.
 *
 * \note A null terminator will replace `delim` where it would have occurred
 * within the string.
 *
 * \param delim The delimiter character
 * \param max_length The maximum size of the string
 * \param stream The input stream to read from
 */
string_t *get_str(const char delim, const size_t max_length,
                  FILE *const stream);

/*
 * Returns a string whose contents consists of characters within `stdin` until
 * reading a newline character, or upon the string reaching `max_length`
 *
 * The difference between `get_str()` and this function is that this function
 * has an implicit delimiter, `\\n`, an implicit stream, `stdin`, and will
 * automatically flush any unused input from `stdin`.
 *
 * Otherwise, this function has the same semantics as `get_str()`.
 */
string_t *get_str_stdin(const size_t max_length);
#endif
