#include "include/malloc.h"

void* calloc(size_t count, size_t size) {
  size_t total_size = count * size;
  if ((total_size / count) == size) {
    void *ptr = malloc(total_size);
    if (ptr != NULL) {
      return ft_memset(ptr, 0, total_size);
    }
  }
  return NULL;
}
