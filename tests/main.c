#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "./../src/include/malloc.h"

/*
  DYLD_LIBRARY_PATH=..
  DYLD_INSERT_LIBRARIES=../libft_malloc.so
  DYLD_FORCE_FLAT_NAMESPACE=1

** compilation steps
** cc -Wall -Wextra -Werror -std=c90 -I src/include -I libft main.c -L ../ -lft_malloc -o main
*/

int main() {

  // int *arr = malloc(sizeof(int) * 10);
  // assert(arr != NULL);
  // arr[0] = 0;
  // arr[1] = 1;

  // for (size_t i = 2; i < 10; ++i) {
  //   arr[i] = arr[i - 1] + arr[i - 2];
  // }

  // for (size_t i = 0 ; i < 10; ++i) {
  //   printf("%d ", arr[i]);
  // }
  // printf("\n");

  // int * ptr = realloc(arr, sizeof(int) * 15 );
  // assert(ptr != NULL);

  // for (size_t i = 0 ; i < 15; ++i) {
  //   printf("%d ", ptr[i]);
  // }
  // printf("\n");

  // return 0;
  uint64_t total = 10;
  void *s = malloc(10);
  for (uint32_t i = 0 ; i < 200 ; ++i) {
    // fprintf(stderr, "i == %d\n", i);
    s = malloc(i + 1);
    total += i + 1;
    // assert(s);
    free(s);
  };
  printf("Total = %llu\n", total);
  // while (true) {};
  // show_alloc_mem();
  // free(p);
  // puts("======");
  // show_alloc_mem();

  return 0;
}
