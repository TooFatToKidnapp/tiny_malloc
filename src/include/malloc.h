#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <stddef.h>
#include <sys/mman.h>
#include "../../libft/libft.h"
#include "limits.h"
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>

#define TINY_ZONE 5
#define SMALL_ZONE 150

#define INFO(format, ...)                                      \
  fprintf(stderr, "---- %s:%d: ------\n", __FILE__, __LINE__); \
  fprintf(stderr, format, __VA_ARGS__);                        \
  fprintf(stderr, "------------------\n");

typedef enum zone
{
  tiny,
  small,
  large
} e_zone;

typedef struct s_alloc_info
{
  struct s_alloc_info *next;
  struct s_alloc_info *prev;
  void *chunk;
  uint64_t size;
  uint64_t capacity;
} t_alloc_info;

typedef struct s_zone_info
{
  t_alloc_info *alloc_pool;
  e_zone alloc_type;
  struct s_zone_info *prev;
  struct s_zone_info *next;
  uint64_t free_mem_size;
} t_zone_info;

typedef struct s_pool
{
  t_zone_info *pool;
  uint64_t tiny_zone_max_size;
  uint64_t small_zone_max_size;
} t_pool;

extern t_pool g_mem_pool;
extern pthread_mutex_t g_mutex_lock;

void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);

void _init_global_mem_pool();
e_zone _set_zone_type(uint64_t size);
t_zone_info *_get_alloc_zone(uint64_t size, e_zone zone_type);
uint64_t _get_zone_mem_size(uint64_t size, e_zone zone_type);
void *_create_alloc_zone(uint64_t size, e_zone zone_type);
void _push_back_new_zone(t_zone_info *zone);
t_alloc_info *_set_new_alloc(uint64_t size, t_alloc_info *head, t_alloc_info *next, t_alloc_info *prev);
void _update_free_mem_size(uint64_t zone_size, t_zone_info *zone);
t_zone_info *_find_ptr_mem_zone(t_zone_info *pool, void *ptr);
t_alloc_info *_find_ptr_mem_alloc(t_zone_info *zone, void *ptr);
void show_alloc_mem();

#endif
