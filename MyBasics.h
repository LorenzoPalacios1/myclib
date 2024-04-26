#ifndef _INC_MYBASICS
#define _INC_MYBASICS

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "MyBasics.h"

/* clang-format off */
#define SUCCESS          (0) /* Denotes a successful execution. */
#define GENERIC_FAILURE  (SUCCESS + 1) /* An undefined status code; consult the function's documentation. */
#define BAD_PTR          (GENERIC_FAILURE + 1) /* Denotes a bad pointer argument. */
#define BAD_FILE         (BAD_PTR + 1) /* Denotes a bad or `NULL` pointer to a `FILE` argument. */
#define FILE_AT_EOF      (BAD_FILE + 1) /* The passed `FILE` is at `EOF` *before* any reading occurred. */
#define FILE_REACHED_EOF (FILE_AT_EOF + 1) /* The function reached `EOF` during execution. */
#define MALLOC_FAILURE   (FILE_REACHED_EOF + 1) /* A call to `malloc()` failed to return a valid pointer. */
/* clang-format on */
#endif
