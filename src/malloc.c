#include "include/malloc.h"
#include <stdio.h>

void *malloc(size_t size)
{
  if (size == 0)
    return NULL;

  // init the global mem pool on the first user allocation
  if (g_mem_pool.small_zone_max_size == 0 || g_mem_pool.tiny_zone_max_size == 0)
    _init_global_mem_pool();
  printf("small size = %ld, tiny size = %ld\n", g_mem_pool.small_zone_max_size / 1024, g_mem_pool.tiny_zone_max_size  / 1024);
  e_zone zone_type = _set_zone_type(size);
  printf("zone type %d\n", zone_type);
  return NULL;
}
