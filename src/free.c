#include "include/malloc.h"

// check's of there is only one zone with the passed in type
static bool is_only_zone_with_type(e_zone type) {
  u_int64_t count = 0;
  t_zone_info * zone = g_mem_pool.pool;
  while (zone && zone->next) {
    if (zone->alloc_type == type) {
      count++;
    }
    zone = zone->next;
  }
  return count > 1;
}

void free(void *ptr) {

  if(ptr == NULL) return;

  if(0 != pthread_mutex_lock(&g_mutex_lock)) {
    return;
  }

  t_zone_info * zone = _find_ptr_mem_zone(g_mem_pool.pool, ptr);
  t_alloc_info *ptr_alloc = _find_ptr_mem_alloc(zone, ptr);

  if (!ptr_alloc) {
    pthread_mutex_unlock(&g_mutex_lock);
    return;
  }

  u_int64_t zone_size = _get_zone_mem_size(zone->alloc_pool->size, zone->alloc_type);

  if (ptr_alloc->prev) {
    ptr_alloc->prev->next = ptr_alloc->next;
    ptr_alloc->next->prev = ptr_alloc->prev;
  } else {
    zone->alloc_pool = ptr_alloc->next;
  }
  _update_free_mem_size(zone_size, zone);

  if (zone->free_mem_size == zone_size - sizeof(t_zone_info) && is_only_zone_with_type(zone->alloc_type)) {
    if (zone->prev) {
      zone->prev->next = zone->next;
    } else {
      g_mem_pool.pool = zone->next;
      g_mem_pool.pool->prev = NULL;
    }
    if (zone->next) {
      zone->next->prev = zone->prev;
    }
    if (0 > munmap((void *)zone, zone_size)) {
      INFO("failed to unmap adder = %p\n", zone);
      abort();
    }
  }
  pthread_mutex_unlock(&g_mutex_lock);
  return;
}
