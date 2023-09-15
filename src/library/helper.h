#ifndef HELPER_H
#define HELPER_H

#include <stdlib.h>
#include <stdio.h>

// Handle errors
#define CHECK(condition, errorMessage, handle)  { \
  if(condition){ \
    fprintf(stderr, "Runtime error: %s at %s:%d.\n", errorMessage, __FILE__, __LINE__); \
    handle; \
  } \
}

#endif // !HELPER_H
