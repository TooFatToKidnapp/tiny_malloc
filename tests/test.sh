#!/bin/bash

set -ex

export DYLD_LIBRARY_PATH="./.."
export DYLD_INSERT_LIBRARIES="./../libft_malloc.so"

if [ "clear" == "$1" ]; then
  rm -f main
  exit
elif [ "run" = "$1" ]; then
  ./main
  exit
fi

cc -Wall -Wextra -Werror -std=c90 -I ../src/include -I ../libft main.c -L ../ -lft_malloc -o main
# export DYLD_LIBRARY_PATH="./../../malloc/"
# export DYLD_INSERT_LIBRARIES="./../../malloc/libft_malloc.so"
# cc -Wall -Wextra -Werror -std=c90 -I ../../malloc/srcs/includes -I ../../malloc/libft main.c -L ../../malloc/ -lft_malloc -o main

./main

rm main

