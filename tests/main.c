#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "malloc.h"

/*
  DYLD_LIBRARY_PATH=..
  DYLD_INSERT_LIBRARIES=../libft_malloc.so
  DYLD_FORCE_FLAT_NAMESPACE=1

** compilation steps
** cc -Wall -Wextra -Werror -std=c90 -I src/include -I libft main.c -L ../ -lft_malloc -o main
*/

int main() {
  int * arr = malloc(sizeof(int) * 30);
  assert(arr != NULL);
  arr[0] = 0;
  arr[1] = 1;

  for (int i = 2 ; i < 30 ; ++i) {
    arr[i] = arr[i - 1] + arr[i - 2];
  }
  for(int j = 0; j < 30 ; ++j) {
    printf("%d ", arr[j]);
  }
  printf("\n");


  // for (int i = 1 ; i < 10000; ++i) {
  //   void *ptr = malloc(i);
  //   assert(ptr != NULL);
  // }
  // puts("\nalloced i = 0 -> 10000 times");


  void * ptr = malloc(20430);
  assert(ptr != NULL);

  ptr = malloc(1000000);
  assert(ptr != NULL);

  show_alloc_mem();
  free(arr);
  free(ptr);


  show_alloc_mem();

  return 0;
}
