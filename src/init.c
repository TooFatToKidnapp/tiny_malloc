#include "include/malloc.h"

pool_t _mem_pool = {NULL, 0, 0};
// https://www.ibm.com/docs/en/aix/7.1?topic=p-pthread-mutex-initializer-macro
// https://docs.oracle.com/cd/E19455-01/806-5257/sync-112/index.html
pthread_mutex_t _mutex_lock = PTHREAD_MUTEX_INITIALIZER;

/*
 ** init the global mem pool
 ** this function is only called on the first time any allocating function is called. malloc, realloc, calloc ...
 ** sets the zone sizes for the calling program
 ** small alloc size is set to ≈ 500 Mib
 ** tiny alloc size is set to ≈ 50 Mib
 */
void _init_global_mem_pool()
{
  int32_t mem_page_size = sysconf(_SC_PAGESIZE);
  _mem_pool.tiny_zone_max_size =
      (TINY_ZONE * mem_page_size - sizeof(zone_info_t) / 100) - sizeof(alloc_info_t);
  _mem_pool.small_zone_max_size =
      (SMALL_ZONE * mem_page_size - sizeof(zone_info_t) / 100) - sizeof(alloc_info_t);
}

/*
 ** returns the appropriate zone type based on alloc size
 ** tiny if size <= 51 Mib
 ** small if size <= 503 Mib
 ** large if size > 503 Mib
 */
e_zone _set_zone_type(uint64_t size)
{
  return (
      (0 * (size <= _mem_pool.tiny_zone_max_size)) +
      (1 * (size > _mem_pool.tiny_zone_max_size && size <= _mem_pool.small_zone_max_size)) +
      (2 * (size > _mem_pool.small_zone_max_size)));
}

/*
  returns the appropriate alloc chunk based on the requested size and type
  if no chunk is available returns null instead
*/
zone_info_t *_get_alloc_zone(uint64_t size, e_zone zone_type)
{
  if (zone_type == large)
    return NULL;

  zone_info_t *pool = _mem_pool.pool;

  while (pool)
  {
    if (pool->alloc_type == zone_type && pool->free_mem_size > (size + sizeof(alloc_info_t)))
      return pool;
    pool = pool->next;
  }
  return NULL;
}

/*
 * This function calculates and returns the memory size required for a zone.
 * It takes in two parameters: the size of the memory block to be allocated and the type of the zone.
 * The function first retrieves the system's memory page size.
 * Then, depending on the zone type, it calculates the required memory size:
 * - For 'tiny' zones, it multiplies the page size by the constant TINY_ZONE.
 * - For 'small' zones, it multiplies the page size by the constant SMALL_ZONE.
 * - For 'large' zones, it calculates the size needed to store the provided size plus the sizes of alloc_info_t and zone_info_t, rounded up to the nearest multiple of the page size.
 * The function returns the calculated memory size.
 */
uint64_t _get_zone_mem_size(uint64_t size, e_zone zone_type)
{
  int32_t mem_page_size = sysconf(_SC_PAGESIZE);
  if (zone_type == tiny)
  {
    return (mem_page_size * ((TINY_ZONE * mem_page_size + size + sizeof(zone_info_t)) / mem_page_size));
  }
  if (zone_type == small)
  {
    return (mem_page_size * ((SMALL_ZONE * mem_page_size + size + sizeof(zone_info_t)) / mem_page_size));
  }
  // For large allocations, align to page size
  return ((size + sizeof(alloc_info_t) + sizeof(zone_info_t) + mem_page_size - 1) & ~(mem_page_size - 1));
}

void *_create_alloc_zone(uint64_t size, e_zone zone_type)
{
  uint64_t allocation_size = _get_zone_mem_size(size, zone_type);
  void *adder = mmap(NULL, allocation_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  if (adder == MAP_FAILED)
  {
    _abort_program("malloc: Failed to allocated memory", NULL);
    return NULL;
  }
  zone_info_t *new_zone = (zone_info_t *)adder;
  new_zone->alloc_pool = NULL;
  new_zone->alloc_type = zone_type;
  new_zone->prev = NULL;
  new_zone->next = NULL;
  new_zone->free_mem_size = allocation_size - sizeof(zone_info_t);
  return adder;
}

void _push_back_new_zone(zone_info_t *zone)
{
  zone_info_t *head = _mem_pool.pool;
  if (head == NULL)
  {
    _mem_pool.pool = zone;
    return;
  }

  while (head->next)
    head = head->next;

  head->next = zone;
  zone->prev = head;
  return;
}

alloc_info_t *_set_new_alloc(uint64_t size, alloc_info_t *head, alloc_info_t *next, alloc_info_t *prev)
{
  alloc_info_t new_alloc = {next, prev, head + 1, size, size};
  ft_memcpy(head, &new_alloc, sizeof(*head));
  return head;
}

void _update_free_mem_size(uint64_t zone_size, zone_info_t *zone)
{
  alloc_info_t *head = zone->alloc_pool;
  long new_free_size = 0;
  ptrdiff_t offset = 0;

  if (head == NULL)
  {
    zone->free_mem_size = zone_size - sizeof(zone_info_t);
    return;
  }
  for (; head->next; head = head->next)
  {
    offset = (uint8_t **)head->next - ((uint8_t **)head->chunk + head->size);
    new_free_size = (new_free_size * (offset <= new_free_size)) + (offset * (offset > new_free_size));
  }
  offset = ((uint8_t **)zone + zone_size) - ((uint8_t **)head->chunk + head->size);
  new_free_size = (new_free_size * (offset <= new_free_size)) + (offset * (offset > new_free_size));
  zone->free_mem_size = new_free_size;
}

zone_info_t *_find_ptr_mem_zone(zone_info_t *pool, void *ptr)
{
  uint8_t **zone_border;
  zone_info_t *zone = pool;

  for (; zone; zone = zone->next)
  {
    if (zone->alloc_pool == NULL)
    {
      continue;
    }
    zone_border = (uint8_t **)zone + _get_zone_mem_size(zone->free_mem_size, zone->alloc_type);
    if (ptr >= (void *)((alloc_info_t *)(zone + 1) + 1) && (uint8_t **)ptr < zone_border)
    {
      return zone;
    }
  }
  return NULL;
}

alloc_info_t *_find_ptr_mem_alloc(zone_info_t *zone, void *ptr)
{
  if (zone == NULL)
    return NULL;
  alloc_info_t *head = zone->alloc_pool;
  for (; head; head = head->next)
  {
    if (head->chunk == ptr)
    {
      return head;
    }
  }
  return NULL;
}
