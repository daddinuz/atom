cmake_minimum_required(VERSION 3.8)
project(atom C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

# dependencies
include_directories(deps)
include(deps/panic/build.cmake)
include(deps/option/build.cmake)
include(deps/alligator/build.cmake)
include(deps/traits-unit/build.cmake)

# archive
include_directories(sources)
include(sources/build.cmake)

# tests
include(tests/unit/build.cmake)
