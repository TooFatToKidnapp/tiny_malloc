#include "include/malloc.h"
#include <stdio.h>

/**
 * iter over the mempool to find the first chunk that fits our required size
 * then set the new alloc and update the previous and next alloc so the linked list of alloc stays consistent
 * returns the newly created t_alloc_info
 */
t_alloc_info *_get_alloc(uint64_t size, t_zone_info *zone, void *alloc_end)
{
  t_alloc_info *head = zone->alloc_pool;
  t_alloc_info *ptr = NULL;

  if (!head)
  { // poll is empty
    ptr = _set_new_alloc(size, (t_alloc_info *)(zone + sizeof(t_alloc_info)), NULL, NULL);
    zone->alloc_pool = ptr;
  }
  else if ((char *)head > (char *)(zone + 1) + sizeof(t_alloc_info) + size)
  {
    ptr = _set_new_alloc(size, (t_alloc_info *)(zone + sizeof(t_alloc_info)), head, NULL);
    zone->alloc_pool = ptr;
    head->prev = ptr;
  }
  else if ((char *)alloc_end >= (char *)head->chunk + size + head->size + sizeof(t_alloc_info))
  {
    ptr = (t_alloc_info *)(head->chunk + head->size);
    head->next = ptr;
    ptr = _set_new_alloc(size, head, ptr, NULL);
  }
  else
  {
    for (; head->next; head = head->next)
    {
      if ((char *)head->next > (char *)head->chunk + size + head->size + sizeof(t_alloc_info))
      {
        ptr = _set_new_alloc(size, (t_alloc_info *)(head->chunk + head->size), head, head->next);
        head->next->prev = ptr;
        head->prev = ptr;
        break;
      }
    }
  }
  return ptr;
}

void *_creat_client_mem_ptr(uint64_t size, t_zone_info *zone)
{
  uint64_t zone_size = _get_zone_mem_size(size, zone->alloc_type);
  t_alloc_info *head = _get_alloc(size, zone, zone + zone_size);
  if (head == NULL)
    return NULL;
  _update_free_mem_size(zone_size, zone);
  return head->chunk;
}

void *malloc(size_t size)
{
  if (size == 0)
    return NULL;

  if (0 != pthread_mutex_lock(&g_mutex_lock)) {
    return NULL;
  }
  // init the global mem pool on the first user allocation
  if (g_mem_pool.small_zone_max_size == 0 || g_mem_pool.tiny_zone_max_size == 0)
    _init_global_mem_pool();
  // printf("small size = %ld bytes = %ld Mib\n", g_mem_pool.small_zone_max_size, g_mem_pool.small_zone_max_size / 1024);
  // printf("tiny size = %ld bytes = %ld Mib\n", g_mem_pool.tiny_zone_max_size, g_mem_pool.tiny_zone_max_size / 1024);
  e_zone zone_type = _set_zone_type(size);
  // printf("zone type %d\n", zone_type);

  t_zone_info *zone = _get_alloc_zone(size, zone_type);
  if (zone == NULL)
  {
    zone = _create_alloc_zone(size, zone_type);
    if (zone == NULL)
      return NULL;
    // INFO("created new zone of type = %d and size = %ld\n", zone->alloc_type, zone->free_mem_size);
    _push_back_new_zone(zone);
  }
  void *client_ptr = _creat_client_mem_ptr(size, zone);
  if (0 != pthread_mutex_unlock(&g_mutex_lock)) {
    return NULL;
  }
  return client_ptr;
}
