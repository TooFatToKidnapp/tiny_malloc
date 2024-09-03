#include <stdio.h>
#include <stdlib.h>
/*
  DYLD_LIBRARY_PATH=..
  DYLD_INSERT_LIBRARIES=../libft_malloc.so
  DYLD_FORCE_FLAT_NAMESPACE=1

** compilation steps
** cc -Wall -Wextra -Werror -std=c90 -I src/include -I libft main.c -L ../ -lft_malloc -o main
*/

int main() {
  char * ptr = malloc(10);
  if (ptr == NULL) {
    printf("PTR == NULL");
  }
  example_fn();
  return 0;
}
