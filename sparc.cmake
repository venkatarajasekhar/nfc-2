INCLUDE (CMakeForceCompiler)

# this one is important
SET(CMAKE_SYSTEM_NAME Generic)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
CMAKE_FORCE_C_COMPILER  (sparc-elf-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(sparc-elf-gcc GNU)

# where is the target environment 
#SET(CMAKE_FIND_ROOT_PATH  /opt/sparc-elf-3.4.4)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(CMAKE_USE_RELATIVE_PATHS ON)        # use relative path
