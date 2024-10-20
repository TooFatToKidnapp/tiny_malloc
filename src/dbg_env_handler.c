#include "include/malloc.h"

static uint32_t _append_number_to_str(char *buff, uint64_t nb)
{
  uint32_t number_count = 1;
  uint32_t num_str_len = 1;
  if (nb == 0)
  {
    buff[number_count] = '0';
    return number_count;
  }
  else
  {
    uint64_t nb_cpy = nb;
    while (nb_cpy != 0)
    {
      nb_cpy /= 10;
      number_count++;
    }
    num_str_len = number_count;
    while (nb != 0)
    {
      buff[--number_count] = (nb % 10) + '0';
      nb /= 10;
    }
  }
  return num_str_len;
}

static void _show_program_leaks()
{
  if (_is_dbg_env_set(DBG_ENV_SHOW_LEAKS))
  {
    uint64_t total_leaked_bytes_count = 0;
    uint64_t allocation_count = 0;
    zone_info_t *zone_list = _mem_pool.pool;

    for (; zone_list; zone_list = zone_list->next)
    {
      alloc_info_t *alloc = zone_list->alloc_pool;
      if (alloc)
      {
        for (; alloc; alloc = alloc->next)
        {
          total_leaked_bytes_count += alloc->capacity;
          allocation_count++;
        }
      }
    }
    char buff[300] = {0};
    uint32_t buff_len = 0;
    buff_len += ft_strlcat(buff, "\n**** Program Memory Leak Info ****\n", sizeof(buff));
    buff_len += ft_strlcat(buff + buff_len, "Total allocations: ", sizeof(buff));
    buff_len += _append_number_to_str(buff + buff_len, allocation_count);
    buff_len += ft_strlcat(buff + buff_len, "\n", sizeof(buff));
    buff_len += ft_strlcat(buff + buff_len, "Total leaked bytes: ", sizeof(buff));
    buff_len += _append_number_to_str(buff + buff_len, total_leaked_bytes_count);
    buff_len += ft_strlcat(buff + buff_len, "\n", sizeof(buff));
    buff_len += ft_strlcat(buff + buff_len, "**********************************\n", sizeof(buff));
    write(2, buff, buff_len);
  }
}

static void sig_handler(int sig)
{
  if (sig == SIGINT)
  {
    _show_program_leaks();
    exit(130);
  }
}

void _init_leak_handler(void)
{
  if (_is_dbg_env_set(DBG_ENV_SHOW_LEAKS))
  {
    signal(SIGINT, sig_handler);
  }
}

static void _clear_zones()
{
  zone_info_t *zones = _mem_pool.pool;
  while (zones)
  {
    zone_info_t *next = zones->next;
    uint64_t zone_size = _get_zone_mem_size(zones->alloc_pool->size, zones->alloc_type);
    if (0 > munmap((void *)zones, zone_size))
    {
      _abort_program("Failed to unmap memory", NULL);
    }
    zones = next;
  }
}

void _clear(void)
{
  if (_is_dbg_env_set(DBG_ENV_SHOW_LEAKS))
  {
    _show_program_leaks();
  }
  if (_is_dbg_env_set(DBG_ENV_CLEAR_LEAKS))
  {
    _clear_zones();
    _mem_pool.pool = NULL;
  }
}

// print information about the error if the appropriate flag is set
// this function doesn't return if the abort env is set
void _abort_program(const char *reason, void *ptr)
{
  if (_is_dbg_env_set(DBG_ENV_ABORT_ON_ERR))
  {
    char buff[300] = {0};
    u_int32_t message_len = 0;
    message_len += ft_strlcat(buff, "**** MALLOC ABORT ****\nReason:\n", sizeof(buff));
    message_len += ft_strlcat(buff + message_len, reason, sizeof(buff));
    if (ptr != NULL)
    {
      message_len += ft_strlcat(buff + message_len, "\nAdder: [", sizeof(buff));
      message_len += _format_adder(buff + message_len, (uint64_t)ptr);
      message_len += ft_strlcat(buff + message_len, "]", sizeof(buff));
    }
    message_len += ft_strlcat(buff + message_len, "\n", sizeof(buff));
    message_len += ft_strlcat(buff, "**********************\n", sizeof(buff));
    write(2, buff, message_len);
    abort();
  }
}

static const char *_dbg_env_to_string(dbg_env_e env)
{
  switch (env)
  {
  case DBG_ENV_ABORT_ON_ERR:
    return "DBG_ENV_ABORT_ON_ERR";
  case DBG_ENV_SHOW_LEAKS:
    return "DBG_ENV_SHOW_LEAKS";
  case DBG_ENV_CLEAR_LEAKS:
    return "DBG_ENV_CLEAR_LEAKS";
  default:
    return "Unknown dbg_env_e value";
  }
}

static bool _check_env_flag(dbg_env_e env)
{
  const char *env_value_to_str = _dbg_env_to_string(env);
  const char *value = getenv(env_value_to_str);
  if (value)
  {
    return ft_atoi(value) == 1 ? true : false;
  }
  return false;
}

bool _is_dbg_env_set(dbg_env_e env)
{
  static bool is_init;
  static int dbg_env_status;
  if (false == is_init)
  {
    if (true == _check_env_flag(DBG_ENV_ABORT_ON_ERR))
    {
      dbg_env_status = dbg_env_status | DBG_ENV_ABORT_ON_ERR;
    }
    if (true == _check_env_flag(DBG_ENV_SHOW_LEAKS))
    {
      dbg_env_status = dbg_env_status | DBG_ENV_SHOW_LEAKS;
    }
    if (true == _check_env_flag(DBG_ENV_CLEAR_LEAKS))
    {
      dbg_env_status = dbg_env_status | DBG_ENV_CLEAR_LEAKS;
    }
    is_init = true;
  }

  return (env & dbg_env_status);
}
