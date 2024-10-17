# #!/bin/sh
set -ex

export DYLD_LIBRARY_PATH=.
export DYLD_INSERT_LIBRARIES="libft_malloc.so"
export DYLD_FORCE_FLAT_NAMESPACE=1
$@

# export DYLD_LIBRARY_PATH="/Users/aabdou/Desktop/malloc"
# export DYLD_INSERT_LIBRARIES="/Users/aabdou/Desktop/malloc/libft_malloc.so"
# export DYLD_FORCE_FLAT_NAMESPACE=1
# $@



# export DYLD_LIBRARY_PATH="/usr/lib"
# export DYLD_INSERT_LIBRARIES="/usr/lib/libSystem.dylib"
# export DYLD_FORCE_FLAT_NAMESPACE=1
# $@
