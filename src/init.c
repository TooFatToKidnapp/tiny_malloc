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
  int mem_page_size = sysconf(_SC_PAGESIZE);
  g_mem_pool.tiny_zone_max_size =
      (TINY_ZONE * mem_page_size - sizeof(t_zone_info) / 100) - sizeof(t_alloc_info);
  g_mem_pool.small_zone_max_size =
      (SMALL_ZONE * mem_page_size - sizeof(t_zone_info) / 100) - sizeof(t_alloc_info);
}

/*
 ** returns the appropriate zone type based on alloc size
 ** tiny if size <= 51 Mib
 ** small if size <= 503 Mib
 ** large if size > 503 Mib
 */
e_zone _set_zone_type(size_t size)
{
  return (
      (0 * (size <= g_mem_pool.tiny_zone_max_size)) +
      (1 * (size > g_mem_pool.tiny_zone_max_size && size <= g_mem_pool.small_zone_max_size)) +
      (2 * (size > g_mem_pool.small_zone_max_size)));
}

/*
  returns the appropriate alloc chunk based on the requested size and type
  if no chunk is available returns null instead
*/
t_zone_info *_get_alloc_zone(size_t size, e_zone zone_type)
{
  if (zone_type == large)
    return NULL;

  t_zone_info *pool = g_mem_pool.pool;

  while (pool)
  {
    if (pool->alloc_type == zone_type && pool->free_mem_size > (size + sizeof(t_alloc_info)))
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
 * - For 'large' zones, it calculates the size needed to store the provided size plus the sizes of t_alloc_info and t_zone_info, rounded up to the nearest multiple of the page size.
 * The function returns the calculated memory size.
 */
size_t _get_zone_mem_size(size_t size, e_zone zone_type)
{
  int mem_page_size = sysconf(_SC_PAGESIZE);
  INFO("page size = %d\nsize = %ld\nzone type = %d\n", mem_page_size, size, zone_type);

  return
      ((mem_page_size * TINY_ZONE) * (zone_type == tiny)) +
      ((mem_page_size * SMALL_ZONE) * (zone_type == small)) +
      ((((size + sizeof(t_alloc_info) + sizeof(t_zone_info)) +
         (mem_page_size - 1)) &
        ~(mem_page_size - 1)) *
       (zone_type == large));
}

void *_create_alloc_zone(size_t size, e_zone zone_type)
{
  size_t allocation_size = _get_zone_mem_size(size, zone_type);
  INFO("Creating a new allocation zone %ld\n", allocation_size);

  t_zone_info new_zone = {NULL, zone_type, NULL, NULL, allocation_size - sizeof(t_zone_info)};

  void *adder = mmap(NULL, allocation_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
  if (adder == MAP_FAILED) return NULL;
  ft_memmove(adder, &new_zone, sizeof(new_zone));
  return adder;
}

void _push_back_new_zone(t_zone_info *zone) {
  t_zone_info * head = g_mem_pool.pool;
  if (head == NULL) {
    g_mem_pool.pool = zone;
    return;
  }

  while(head->next) head = head->next;

  head->next = zone;
  zone->prev = head;
  return;
}

t_alloc_info *_set_new_alloc(size_t size, t_alloc_info * head, t_alloc_info* next, t_alloc_info* prev ) {
  t_alloc_info new_alloc = {next, prev, head + 1, size, size};
  ft_memcpy(head, &new_alloc, sizeof(*head));
  return head;
}

void _update_free_mem_size( size_t zone_size, t_zone_info *zone) {
  t_alloc_info *head = zone->alloc_pool;
  long new_free_size = 0;
  ptrdiff_t offset = 0;

  if (head == NULL) {
    zone->free_mem_size = zone_size - sizeof(t_alloc_info);
  } else {
    for (; head->next; head = head->next) {
      offset = (char*)head->next - (char*)(head->chunk + zone_size);
      new_free_size = (new_free_size * (offset <= new_free_size)) + (offset * (offset > new_free_size));
    }
    offset = ((char *)zone + zone_size) - (char*)(head->chunk + zone_size);
    new_free_size = (new_free_size * (offset <= new_free_size)) + (offset * (offset > new_free_size));
    zone->free_mem_size = new_free_size;
  }
}
