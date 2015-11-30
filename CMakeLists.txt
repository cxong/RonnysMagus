project(RonnysMagus C)
cmake_minimum_required(VERSION 2.8)

if(MSVC)
  add_definitions(-wd"4996")
#    add_definitions(-Wall -W4 -wd"4996" -D_USE_MATH_DEFINES)
else()
#    add_definitions(-Wall -Wextra -Wunused -Wshadow -Wunreachable-code -std=c99)
#    add_definitions(-fno-math-errno -fno-exceptions)
endif()

set(MAGUS_SOURCES
  ./src/digisnd.c
  ./src/grafx.c
  ./src/items.c
  ./src/magus.c
  ./src/pics.c
  ./src/sounds.c
)
set(MAGUS_HEADERS
  ./src/digisnd.h
  ./src/grafx.h
  ./src/items.h
  ./src/pics.h
  ./src/sounds.h
)
add_executable(magus ${MAGUS_SOURCES} ${MAGUS_HEADERS})
