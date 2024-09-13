#!/bin/bash

set -ex

export DYLD_LIBRARY_PATH="./.."
export DYLD_INSERT_LIBRARIES="./../libft_malloc.so"
export DYLD_FORCE_FLAT_NAMESPACE=1

cc -Wall -Wextra -Werror -g -fsanitize=address,undefined -std=c99 -I ../src/include -I ../libft main.c -L ../ -lft_malloc -o main

./main

rm -fr main main.dSYM

