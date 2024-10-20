#include "include/malloc.h"

/**
 * iter over the mempool to find the first chunk that fits our required size
 * then set the new alloc and update the previous and next alloc so the linked list of alloc stays consistent
 * returns the newly created alloc_info_t
 */
static alloc_info_t *_get_alloc(uint64_t size, zone_info_t *zone, void *alloc_end)
{
  alloc_info_t *head = zone->alloc_pool;
  alloc_info_t *ptr = NULL;

  if (!head)
  { // pool is empty
    ptr = _set_new_alloc(size, (alloc_info_t *)(zone + 1), NULL, NULL);
    zone->alloc_pool = ptr;
    return ptr;
  }
  else if ((uint8_t *)head > (uint8_t *)(zone + 1) + sizeof(alloc_info_t) + size)
  {
    ptr = _set_new_alloc(size, (alloc_info_t *)(zone + 1), head, NULL);
    zone->alloc_pool = ptr;
    head->prev = ptr;
    return ptr;
  }

  for (; head->next; head = head->next)
  {
    if ((uint8_t *)head->next > (uint8_t *)head->chunk + size + head->size + sizeof(alloc_info_t))
    {
      ptr = _set_new_alloc(size, (alloc_info_t *)(head->chunk + head->size), head->next, head);
      head->next->prev = ptr;
      head->prev = ptr;
      return ptr;
    }
  }

  if ((uint8_t *)alloc_end >= (uint8_t *)head->chunk + size + head->size + sizeof(alloc_info_t))
  {
    ptr = (alloc_info_t *)(head->chunk + head->size);
    head->next = ptr;
    return _set_new_alloc(size, ptr, NULL, head);
  }

  return NULL;
}

static void *_creat_client_mem_ptr(uint64_t size, zone_info_t *zone)
{
  uint64_t zone_size = _get_zone_mem_size(size, zone->alloc_type);
  alloc_info_t *head = _get_alloc(size, zone, (uint8_t *)zone + zone_size);
  if (head == NULL)
  {
    return NULL;
  }
  _update_free_mem_size(zone_size, zone);
  return head->chunk;
}

void *malloc(size_t size)
{
  //  round up the size to the next multiple of 16.
  size = (size + 15) & ~15;
  if (0 == size)
  {
    return NULL;
  }
  if (0 != pthread_mutex_lock(&_mutex_lock))
  {
    _abort_program("Failed to lock Allocation Mutex", NULL);
    return NULL;
  }
  // init the global mem pool on the first user allocation
  if (_mem_pool.small_zone_max_size == 0 || _mem_pool.tiny_zone_max_size == 0)
    _init_global_mem_pool();

  e_zone zone_type = _set_zone_type(size);

  zone_info_t *zone = _get_alloc_zone(size, zone_type);
  if (zone == NULL)
  {
    zone = _create_alloc_zone(size, zone_type);
    if (zone == NULL)
    {
      return NULL;
    }
    _push_back_new_zone(zone);
  }
  void *client_ptr = _creat_client_mem_ptr(size, zone);
  if (0 != pthread_mutex_unlock(&_mutex_lock))
  {
    _abort_program("Failed to unlock Allocation Mutex", NULL);
    return NULL;
  }
  return client_ptr;
}
