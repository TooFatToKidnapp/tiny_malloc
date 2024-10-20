#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "../src/include/malloc.h"

int main() {

  void *s = malloc(0);
  void *ptr = reallocf(s, -1);
  (void)ptr;
  // assert(ptr != NULL);
  // free(ptr);
  show_alloc_mem();
  return 0;
}
