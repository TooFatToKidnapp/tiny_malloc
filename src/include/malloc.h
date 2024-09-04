#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <stddef.h>
#include "../../libft/libft.h"

#define TINY_ZONE 13
#define SMALL_ZONE 126

typedef enum zone {tiny, small, large} e_zone;

typedef struct s_alloc_info {
  struct s_alloc_info *next;
  struct s_alloc_info *prev;
  void *content;
  size_t size;
  size_t capacity;
} t_alloc_info;

typedef struct s_zone_info {
  t_alloc_info *alloc_pool;
  e_zone alloc_type;
  struct s_zone_info *prev;
  struct s_zone_info *next;
  size_t free_mem_size;
} t_zone_info;

typedef struct s_pool {
  t_zone_info *pool;
  size_t tiny_zone_max_size;
  size_t small_zone_max_size;
} t_pool;

extern t_pool g_mem_pool;


void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);


void _init_global_mem_pool();
e_zone _set_zone_type(size_t size);

#endif
