#include <string.h>

#include "../MyBasics.h"

typedef struct string_t {
  char *data;
  size_t length;
  size_t allocated_bytes;
  double reallocation_multiplier;
} string_t;

string_t *findReplace(string_t *const haystack, const string_t *const needle,
                  const string_t *const replacer);

/* To be added. */
string_t *findReplaceAll(string_t *const haystack, const string_t *const needle,
                         const string_t *const replacement);
