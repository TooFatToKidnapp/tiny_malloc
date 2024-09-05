#include "include/malloc.h"
#include <stdio.h>

void *malloc(size_t size)
{
  if (size == 0)
    return NULL;

  // init the global mem pool on the first user allocation
  if (g_mem_pool.small_zone_max_size == 0 || g_mem_pool.tiny_zone_max_size == 0)
    _init_global_mem_pool();
  printf("small size = %ld bytes = %ld Mib\n", g_mem_pool.small_zone_max_size, g_mem_pool.small_zone_max_size / 1024);
  printf("tiny size = %ld bytes = %ld Mib\n", g_mem_pool.tiny_zone_max_size, g_mem_pool.tiny_zone_max_size / 1024);
  e_zone zone_type = _set_zone_type(size);
  printf("zone type %d\n", zone_type);

  t_zone_info *zone = _get_alloc_zone(size, zone_type);
  if (zone == NULL)
  {
    t_zone_info * new_zone = _create_alloc_zone(size, zone_type);
    if (new_zone == NULL) return NULL;
    INFO("created new zone of type = %d and size = %ld\n", new_zone->alloc_type, new_zone->free_mem_size);
    _push_back_new_zone(new_zone);
  }
  

  return NULL;
}
