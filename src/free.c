#include "include/malloc.h"

void free(void *ptr) {

  if(ptr == NULL) return;

  if(0 != pthread_mutex_lock(&g_mutex_lock)) {
    return;
  }

  t_zone_info * zone = _find_ptr_mem_zone(g_mem_pool.pool, ptr);
  (void)zone;


  if(0 != pthread_mutex_unlock(&g_mutex_lock)) {
    return;
  }
}
