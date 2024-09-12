#include "include/malloc.h"

void *realloc(void *ptr, size_t size)
{
  if (ptr == NULL) {
    return malloc(size);
  } else if (size == 0) {
    free(ptr);
    return NULL;
  }

  if (0 != pthread_mutex_lock(&g_mutex_lock)) {
    return NULL;
  }
  t_zone_info *zone = _find_ptr_mem_zone(g_mem_pool.pool, ptr);
  t_alloc_info *ptr_alloc = _find_ptr_mem_alloc(zone, ptr);

  if (!ptr_alloc)
  {
    pthread_mutex_unlock(&g_mutex_lock);
    INFO("PTR = [%p] WAS NOT ALLOCATED\n", ptr);
    pthread_mutex_unlock(&g_mutex_lock);
    abort();
    return NULL;
  } else if (size == ptr_alloc->capacity) {
    pthread_mutex_unlock(&g_mutex_lock);
    return ptr;
  }



  pthread_mutex_unlock(&g_mutex_lock);

  return NULL;
}
