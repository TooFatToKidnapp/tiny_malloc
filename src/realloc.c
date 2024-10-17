#include "include/malloc.h"

static void *_find_and_set_new_allocation(size_t size, size_t zone_size, zone_info_t *zone, alloc_info_t *alloc)
{

  char *next = NULL;

  if (_set_zone_type(alloc->capacity) != _set_zone_type(size))
  {
    return NULL;
  }

  if (size >= alloc->capacity)
  {
    next = (char *)(((uint64_t)alloc->next * (alloc->next != NULL)) * (((uint64_t)(char *)zone + zone_size) * (alloc->next == NULL)));
    if ((uint64_t)next <= (uint64_t)alloc->chunk + size)
      return NULL;
  }
  alloc->size = size;
  _update_free_mem_size(zone_size, zone);
  if (0 != pthread_mutex_unlock(&_mutex_lock))
  {
    _abort_program("Failed to unlock Allocation Mutex");
  }
  return alloc->chunk;
}

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

  size = (size + 15) & ~15;
  if (0 != pthread_mutex_lock(&_mutex_lock))
  {
    _abort_program("Failed to lock Allocation Mutex");
    return NULL;
  }
  zone_info_t *zone = _find_ptr_mem_zone(_mem_pool.pool, ptr);
  alloc_info_t *ptr_alloc = _find_ptr_mem_alloc(zone, ptr);
  if (!ptr_alloc)
  {
    if (0 != pthread_mutex_unlock(&_mutex_lock))
    {
      _abort_program("Failed to unlock Allocation Mutex");
      return NULL;
    }
    _abort_program("Realloc: the passed in pointer was not allocated");
    return NULL;
  }
  else if (size == ptr_alloc->capacity)
  {
    if (0 != pthread_mutex_unlock(&_mutex_lock))
    {
      _abort_program("Failed to unlock Allocation Mutex");
    }
    return ptr;
  }

  uint64_t zone_size = _get_zone_mem_size(size, zone->alloc_type);

  if (_mem_pool.small_zone_max_size == 0 || _mem_pool.tiny_zone_max_size == 0)
    _init_global_mem_pool();

  void *client_ptr = _find_and_set_new_allocation(size, zone_size, zone, ptr_alloc);

  if (client_ptr == NULL)
  {
    if (0 != pthread_mutex_unlock(&_mutex_lock))
    {
      _abort_program("Failed to unlock Allocation Mutex");
      return NULL;
    }
    client_ptr = malloc(size);
    if (client_ptr == NULL)
      return NULL;
  }

  if (0 != pthread_mutex_lock(&_mutex_lock))
  {
    _abort_program("Failed to lock Allocation Mutex");
    return NULL;
  }

  size_t data_size = size > ptr_alloc->capacity ? ptr_alloc->capacity : size;

  ft_memmove(client_ptr, ptr_alloc->chunk, data_size);

  if (0 != pthread_mutex_unlock(&_mutex_lock))
  {
    _abort_program("Failed to unlock Allocation Mutex");
    return NULL;
  }

  free(ptr_alloc->chunk);
  return client_ptr;
}
