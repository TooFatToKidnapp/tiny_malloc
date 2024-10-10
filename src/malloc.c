#include "include/malloc.h"
#include <stdio.h>

/**
 * iter over the mempool to find the first chunk that fits our required size
 * then set the new alloc and update the previous and next alloc so the linked list of alloc stays consistent
 * returns the newly created alloc_info_t
 */
alloc_info_t *_get_alloc(uint64_t size, zone_info_t *zone, void *alloc_end)
{
  alloc_info_t *head = zone->alloc_pool;
  alloc_info_t *ptr = NULL;

  if (!head)
  { // pool is empty
    ptr = _set_new_alloc(size, (alloc_info_t *)(zone + 1), NULL, NULL);
    zone->alloc_pool = ptr;
    return ptr;
  }
  else if ((char *)head > (char *)(zone + 1) + sizeof(alloc_info_t) + size)
  {
    ptr = _set_new_alloc(size, (alloc_info_t *)(zone + 1), head, NULL);
    zone->alloc_pool = ptr;
    head->prev = ptr;
    return ptr;
  }

  for (; head->next; head = head->next)
  {
    if ((char *)head->next > (char *)head->chunk + size + head->size + sizeof(alloc_info_t))
    {
      ptr = _set_new_alloc(size, (alloc_info_t *)(head->chunk + head->size), head->next, head);
      head->next->prev = ptr;
      head->prev = ptr;
      return ptr;
    }
  }

  if ((char *)alloc_end >= (char *)head->chunk + size + head->size + sizeof(alloc_info_t))
  {
    ptr = (alloc_info_t *)(head->chunk + head->size);
    head->next = ptr;
    return _set_new_alloc(size, ptr, NULL, head);
  }

  return NULL;
}

void *_creat_client_mem_ptr(uint64_t size, zone_info_t *zone)
{
  uint64_t zone_size = _get_zone_mem_size(size, zone->alloc_type);
  alloc_info_t *head = _get_alloc(size, zone, (char *)zone + zone_size);
  if (head == NULL) {
    // INFO("\n\n=============\n\n HEAD == %p \n =================\n\n", head);
    return NULL;
  }
  _update_free_mem_size(zone_size, zone);
  return head->chunk;
}

void *malloc(size_t size)
{
  if (size == 0)
    return NULL;
  // round up the size to the next multiple of 16.
  size = (size + 15) & ~15;
  // INFO("requested allocation size after memory alignment = %zu\n", size);
  if (0 != pthread_mutex_lock(&_mutex_lock))
  {
    fprintf(stderr, "Failed to mutex lock\n");
    abort();
    // return NULL;
  }
  // init the global mem pool on the first user allocation
  if (_mem_pool.small_zone_max_size == 0 || _mem_pool.tiny_zone_max_size == 0)
    _init_global_mem_pool();
  // printf("TINY ZONE SIZE == %d | SMALL ZONE SIZE == %d \n", TINY_ZONE, SMALL_ZONE);
  // printf("small size = %llu bytes = %f Mib\n", _mem_pool.small_zone_max_size, _mem_pool.small_zone_max_size / 1024.f);
  // printf("tiny size = %llu bytes = %f Mib\n", _mem_pool.tiny_zone_max_size, _mem_pool.tiny_zone_max_size / 1024.f);
  e_zone zone_type = _set_zone_type(size);
  // INFO("***Allocation type = [%d]***\n", zone_type);

  zone_info_t *zone = _get_alloc_zone(size, zone_type);
  if (zone == NULL)
  {
    zone = _create_alloc_zone(size, zone_type);
    if (zone == NULL)
      return NULL;
    // INFO("created new zone of type = %d and size = %llu\n", zone->alloc_type, zone->free_mem_size);
    _push_back_new_zone(zone);
  }
  void *client_ptr = _creat_client_mem_ptr(size, zone);
  if (0 != pthread_mutex_unlock(&_mutex_lock))
  {
    // return NULL;
    fprintf(stderr, "Failed to unlock mutex\n");
    abort();
  }
  return client_ptr;
}
