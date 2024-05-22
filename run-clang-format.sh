# this line does quite a bit, so let's break it down
# 
# find mm
# use "find" to look in the "mm" directory
# this ensures we don't try to format stuff in the submodules
# 
# -type f
# only look for files
# 
# -name "*.c" -o -name "*.cpp"
# find all .c and .cpp files
# 
# -name "*.h" ! -path "mm/src/**.h" ! -path "mm/include/**.h"
# find all .h files that aren't in mm/src or mm/include
# this is because zret decomp only runs clang-format on c files
# https://github.com/zeldaret/mm/blob/b7e5468ca16315a7e322055eff3d97fe980bbc25/format.py#L182
# 
# ! -path "mm/assets/*"
# asset headers are autogenerated, don't fight them
#
# | sed 's| |\\ |g'
# pipe the result of find into sed to
# ensure all the paths returned by find have spaces escaped
# 
# | xargs clang-format-14 -i
# use xargs to take each path we've found
# and pass it as an argument to clang-format

find mm -type f \( -name "*.c" -o -name "*.cpp" -o \( -name "*.h" ! -path "mm/src/**.h" ! -path "mm/include/**.h" \) \) ! -path "mm/assets/*" | sed 's| |\\ |g' | xargs clang-format-mp-14 -i
