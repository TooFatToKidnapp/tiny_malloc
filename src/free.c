#include "include/malloc.h"

/*
 * check's of there is only one zone with the passed in type
 * only for small and tiny types
 */
static bool is_only_zone_with_type(e_zone type)
{
  u_int64_t count = 0;
  zone_info_t *zone = _mem_pool.pool;
  while (zone && zone->next)
  {
    if (zone->alloc_type == type)
    {
      count++;
    }
    zone = zone->next;
  }
  return count <= 1;
}

void free(void *ptr)
{
  if (ptr == NULL)
    return;

  if (0 != pthread_mutex_lock(&_mutex_lock))
  {
    _abort_program("Failed to lock Allocation Mutex");
    return;
  }

  zone_info_t *zone = _find_ptr_mem_zone(_mem_pool.pool, ptr);
  alloc_info_t *ptr_alloc = _find_ptr_mem_alloc(zone, ptr);

  if (!ptr_alloc)
  {
    if (0 != pthread_mutex_unlock(&_mutex_lock))
    {
      _abort_program("Failed to unlock Allocation Mutex");
    }
    _abort_program("Free: the passed in pointer was not allocated");
    return;
  }

  u_int64_t zone_size = _get_zone_mem_size(zone->alloc_pool->size, zone->alloc_type);

  if (ptr_alloc->prev)
  {
    ptr_alloc->prev->next = ptr_alloc->next;
    if (ptr_alloc->next)
    {
      ptr_alloc->next->prev = ptr_alloc->prev;
    }
  }
  else
  {
    zone->alloc_pool = ptr_alloc->next;
    if (zone->alloc_pool)
    {
      zone->alloc_pool->prev = NULL;
    }
  }
  _update_free_mem_size(zone_size, zone);

  if (zone->free_mem_size == zone_size - sizeof(zone_info_t) && (zone->alloc_type == large || !is_only_zone_with_type(zone->alloc_type)))
  {
    if (zone->prev)
    {
      zone->prev->next = zone->next;
    }
    else
    {
      _mem_pool.pool = zone->next;
      if (_mem_pool.pool)
      {
        _mem_pool.pool->prev = NULL;
      }
    }
    if (zone->next)
    {
      zone->next->prev = zone->prev;
    }
    if (0 > munmap((void *)zone, zone_size))
    {
      _abort_program("Failed to unmap memory");
    }
  }
  if (0 != pthread_mutex_unlock(&_mutex_lock))
  {
    _abort_program("Failed to unlock Allocation Mutex");
  }
  return;
}
