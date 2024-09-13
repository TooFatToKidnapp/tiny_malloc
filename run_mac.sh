#!/bin/sh
export DYLD_LIBRARY_PATH=.
export DYLD_INSERT_LIBRARIES="libft_malloc.so"
export DYLD_FORCE_FLAT_NAMESPACE=1
$@

# cc -g -fsanitize=address,undefined -std=c99 -I src/include -I libft test0.c -L . -lft_malloc -o test0

# echo "RUNNING TEST 0"
# ./test0

# rm -fr test0 test0.dSYM

# cc -g -fsanitize=address,undefined -std=c99 -I src/include -I libft test1.c -L . -lft_malloc -o test1

# echo "RUNNING TEST 1"
# ./test1

# rm  -fr test1 test1.dSYM

# cc -g -fsanitize=address,undefined -std=c99 -I src/include -I libft test2.c -L . -lft_malloc -o test2

# echo "RUNNING TEST 2"
# ./test2

# rm  -fr test2 test2.dSYM

# cc -g -fsanitize=address,undefined -std=c99 -I src/include -I libft test3.c -L . -lft_malloc -o test3

# echo "RUNNING TEST 3"
# ./test3

# rm  -fr test3 test3.dSYM

# cc -g -fsanitize=address,undefined -std=c99 -I src/include -I libft test4.c -L . -lft_malloc -o test4

# echo "RUNNING TEST 4"
# ./test4

# rm  -fr test4 test4.dSYM

# cc -g -fsanitize=address,undefined -std=c99 -I src/include -I libft test5.c -L . -lft_malloc -o test5

# echo "RUNNING TEST 5"
# ./test5

# rm  -fr test5 test5.dSYM
