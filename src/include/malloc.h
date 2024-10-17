#ifndef MALLOC_H
#define MALLOC_H

#include <unistd.h>
#include <stddef.h>
#include <sys/mman.h>
#include "../../libft/libft.h"
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#define TINY_ZONE 11
#define SMALL_ZONE 111

// #define INFO(format, ...)                                      \
//   fprintf(stderr, "---- %s:%d: ------\n", __FILE__, __LINE__); \
//   fprintf(stderr, format, __VA_ARGS__);                        \
//   fprintf(stderr, "------------------\n");

void __attribute__((constructor)) _init_leak_handler(void);

typedef enum dbg_env {
  DBG_ENV_ABORT_ON_ERR = 1 << 0,
  DBG_ENV_SHOW_LEAKS = 1 << 1,
} dbg_env_e;

#define PUBLIC __attribute__ ((visibility ("default")))

typedef enum zone {
  tiny,
  small,
  large
} e_zone;

typedef struct alloc_info_s {
  struct alloc_info_s *next;
  struct alloc_info_s *prev;
  void *chunk;
  uint64_t size;
  uint64_t capacity;
} alloc_info_t;

typedef struct zone_info_s {
  alloc_info_t *alloc_pool;
  e_zone alloc_type;
  struct zone_info_s *prev;
  struct zone_info_s *next;
  uint64_t free_mem_size;
} zone_info_t;

typedef struct pool_s {
  zone_info_t *pool;
  uint64_t tiny_zone_max_size;
  uint64_t small_zone_max_size;
} pool_t;

extern pool_t _mem_pool;
extern pthread_mutex_t _mutex_lock;

PUBLIC void free(void *ptr);
PUBLIC void *malloc(size_t size);
PUBLIC void *realloc(void *ptr, size_t size);
PUBLIC void *calloc(size_t count, size_t size);
PUBLIC void show_alloc_mem();


void _init_global_mem_pool();
e_zone _set_zone_type(uint64_t size);
zone_info_t *_get_alloc_zone(uint64_t size, e_zone zone_type);
uint64_t _get_zone_mem_size(uint64_t size, e_zone zone_type);
void *_create_alloc_zone(uint64_t size, e_zone zone_type);
void _push_back_new_zone(zone_info_t *zone);
alloc_info_t *_set_new_alloc(uint64_t size, alloc_info_t *head, alloc_info_t *next, alloc_info_t *prev);
void _update_free_mem_size(uint64_t zone_size, zone_info_t *zone);
zone_info_t *_find_ptr_mem_zone(zone_info_t *pool, void *ptr);
alloc_info_t *_find_ptr_mem_alloc(zone_info_t *zone, void *ptr);

bool  _is_dbg_env_set(dbg_env_e env);
void _abort_program(const char *reason);
void _init_leak_handler(void);

#endif
