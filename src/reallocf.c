#include "include/malloc.h"


void * reallocf(void *ptr, size_t size) {
  void * user_ptr = realloc(ptr, size);
  if (user_ptr == NULL) {
    free(ptr);
    return NULL;
  }
  return user_ptr;
}
