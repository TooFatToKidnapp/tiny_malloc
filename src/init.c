#include "include/malloc.h"

void _init_global_mem_pool()
{
  int mem_page_size = getpagesize();

  g_mem_pool.small_zone_max_size =
      (TINY_ZONE * mem_page_size - sizeof(t_zone_info) / 100) - sizeof(t_alloc_info);
  g_mem_pool.tiny_zone_max_size =
      (SMALL_ZONE * mem_page_size - sizeof(t_zone_info) / 100) - sizeof(t_alloc_info);
}
