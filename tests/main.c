#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

int main() {

  void *ptr = calloc(1, sizeof(int));
  assert(ptr != NULL);
  free(ptr);

  return 0;
}
