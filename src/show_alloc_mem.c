#include "include/malloc.h"

static zone_info_t *_get_smallest_address_zone(uint64_t *adder)
{
  uint64_t max_adder = ULONG_MAX;
  uint64_t zone_adder = 0;
  zone_info_t *zone = _mem_pool.pool;

  while (zone)
  {
    zone_adder = (uint64_t)zone;
    if (zone_adder > *adder && zone_adder <= max_adder)
    {
      max_adder = zone_adder;
    }
    zone = zone->next;
  }
  *adder = max_adder;
  return (zone_info_t *)(*adder * (*adder != ULONG_MAX));
}

static uint64_t _format_adder(char *buffer, unsigned long adder)
{
  uint64_t len = 0;
  const char hex_chars[] = "0123456789ABCDEF";
  bool found_first_non_zero = false;
  buffer[len++] = '0';
  buffer[len++] = 'x';
  for (int32_t i = sizeof(adder) * 8 - 4; i >= 0; i -= 4)
  {
    unsigned long digit = (adder >> i) & 0xf;
    if (digit != 0 && !found_first_non_zero)
    {
      found_first_non_zero = true;
    }
    if (found_first_non_zero)
    {
      buffer[len++] = hex_chars[digit];
    }
  }

  return len;
}

static void _print_zone_info_t(zone_info_t *zone)
{
  char buffer[200] = {0};
  const char zone_type[3][9] = {"TINY : ", "SMALL : ", "LARGE : "};
  uint64_t zone_len = ft_strlen(zone_type[zone->alloc_type]);

  ft_memcpy(buffer, zone_type[zone->alloc_type], zone_len);

  uint64_t adder_len = _format_adder(buffer + zone_len , (unsigned long)zone);

  buffer[zone_len + adder_len] = '\n';

  write(1, buffer, zone_len + adder_len + 1);
}

static uint64_t _format_size_as_bytes(char *buffer, uint64_t size)
{
  uint64_t size_len = 0;
  uint64_t tmp_size = size;

  do
  {
    size_len++;
    tmp_size /= 10;
  } while (tmp_size != 0);

  for (uint64_t i = size_len; i > 0; --i)
  {
    buffer[i - 1] = (size % 10) + '0';
    size /= 10;
  }

  return size_len;
}

/*
0xA0020 - 0xA004A : 42 bytes
0xA006A - 0xA00BE : 84 bytes
SMALL : 0xAD000
0xAD020 - 0xADEAD : 3725 bytes
LARGE : 0xB0000
0xB0020 - 0xBBEEF : 48847 bytes
Total : 52698 bytes
*/

uint64_t _print_alloc(zone_info_t *zone)
{
  uint64_t max_size = 0;
  alloc_info_t *head = zone->alloc_pool;

  while (head)
  {
    max_size += head->capacity;
    char buffer[200] = {0};
    uint64_t buffer_len = 0;
    _format_adder(buffer, (unsigned long)head->chunk);
    buffer_len = ft_strlcat(buffer, " - ", sizeof(buffer));
    _format_adder(buffer + buffer_len, (unsigned long)(head->chunk + head->capacity));
    buffer_len = ft_strlcat(buffer, " : ", sizeof(buffer));
    buffer_len += _format_size_as_bytes(buffer + buffer_len, head->capacity);
    ft_memcpy(buffer + buffer_len, " bytes\n", 7);
    buffer[buffer_len + 7] = '\n';
    write(1, buffer, buffer_len + 7);
    head = head->next;
  }

  return max_size;
}

void show_alloc_mem()
{
  if (0 != pthread_mutex_lock(&_mutex_lock))
  {
    return;
  }
  uint64_t min_zone_adder = 0;
  uint64_t max_allocated_bits = 0;
  zone_info_t *zone = _get_smallest_address_zone(&min_zone_adder);
  while (zone)
  {
    _print_zone_info_t(zone);
    max_allocated_bits += _print_alloc(zone);
    zone = _get_smallest_address_zone(&min_zone_adder);
  }
  write(1, "Total : ", 8);
  ft_putnbr_fd(max_allocated_bits, 1);
  write(1, " bytes\n", 7);
  if (0 != pthread_mutex_unlock(&_mutex_lock))
  {
    return;
  }
}
