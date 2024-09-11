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

  // puts("----------------");
  // char * ptr = strdup("Hello World");
  // assert(ptr != NULL);
  // printf("ptr = [%s]\n", ptr);
  // puts("----------------");

  puts("----------------");
  int * arr = malloc(sizeof(int) * 1000000);
  assert(arr != NULL);
  arr[0] = 0;
  arr[1] = 1;

  for (int i = 2 ; i < 10 ; ++i) {
    arr[i] = arr[i - 1] + arr[i - 2];
  }
  printf("The first 10 fibonacci sequence's\n");
  for(int j = 0; j < 10 ; ++j) {
    printf("%d ", arr[j]);
  }
  printf("\n");
  puts("----------------");

  // puts("----------------");
  // for (int i = 1 ; i < 10000; ++i) {
  //   void *ptr = malloc(i);
  //   assert(ptr != NULL);
  // }
  // puts("\nalloced i = 0 -> 10000 times");
  // puts("----------------");

  void * ptr = malloc(20430);
  // show_alloc_mem();
  // assert(ptr != NULL);
  free(arr);
  ptr = malloc(1000000);
  // ptr[0] = 'A';
  // printf("ptr[0] = %c\n", ptr[0]);
  puts("===========");
  show_alloc_mem();

  return 0;
}
