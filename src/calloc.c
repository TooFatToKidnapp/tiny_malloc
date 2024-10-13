#include "include/malloc.h"

void* calloc(size_t count, size_t size) {
  size_t total_size = count * size;
  if (count && size && (total_size / count == size)) {
    void *ptr = malloc(total_size);
    pthread_mutex_lock(&_mutex_lock);
    if (ptr != NULL) {
      return ft_memset(ptr, 0, total_size);
    }
    pthread_mutex_unlock(&_mutex_lock);
  }
  return NULL;
}
