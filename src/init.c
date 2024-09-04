#include "include/malloc.h"

t_pool g_mem_pool = {NULL, 0, 0};

/*
 ** init the global mem pool
 ** this function is only called on the first time any allocating function is called. malloc, realloc, calloc ...
 ** sets the zone sizes for the calling program
 ** small alloc size is set to ≈ 500 Mib
 ** tiny alloc size is set to ≈ 50 Mib
*/
void _init_global_mem_pool()
{
    int mem_page_size = getpagesize();
    g_mem_pool.tiny_zone_max_size =
        (TINY_ZONE * mem_page_size - sizeof(t_zone_info) / 100) - sizeof(t_alloc_info);
    g_mem_pool.small_zone_max_size =
        (SMALL_ZONE * mem_page_size - sizeof(t_zone_info) / 100) - sizeof(t_alloc_info);
}

e_zone _set_zone_type(size_t size)
{
    return (
        (0 * (size <= g_mem_pool.tiny_zone_max_size)) +
        (1 * (size > g_mem_pool.tiny_zone_max_size && size <= g_mem_pool.small_zone_max_size)) +
        (2 * (size > g_mem_pool.small_zone_max_size)));
}
