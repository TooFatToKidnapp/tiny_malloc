#include "include/malloc.h"

void *realloc(void *ptr, size_t size)
{
  if (ptr == NULL)
  {
    return malloc(size);
  }
  else if (size == 0)
  {
    free(ptr);
    return NULL;
  }

  if (0 != pthread_mutex_lock(&_mutex_lock))
  {
    return NULL;
  }
  zone_info_t *zone = _find_ptr_mem_zone(_mem_pool.pool, ptr);
  alloc_info_t *ptr_alloc = _find_ptr_mem_alloc(zone, ptr);

  if (!ptr_alloc)
  {
    pthread_mutex_unlock(&_mutex_lock);
    INFO("PTR = [%p] WAS NOT ALLOCATED\n", ptr);
    pthread_mutex_unlock(&_mutex_lock);
    abort();
    return NULL;
  }
  else if (size == ptr_alloc->capacity)
  {
    pthread_mutex_unlock(&_mutex_lock);
    return ptr;
  }

  pthread_mutex_unlock(&_mutex_lock);

  return NULL;
}
