project(RonnysMagus C)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/build/cmake")

find_package(PkgConfig)
pkg_search_module(SDL2 sdl2)
pkg_search_module(SDL2_IMAGE SDL2_image)
pkg_search_module(SDL2_MIXER SDL2_mixer)
if(NOT "${SDL2_FOUND}")
	message("looking for SDL2")
	find_package(SDL2 REQUIRED)
endif()
if(NOT "${SDL2_IMAGE_FOUND}")
	find_package(SDL2_image REQUIRED)
endif()
if(NOT "${SDL2_MIXER_FOUND}")
	find_package(SDL2_mixer REQUIRED)
endif()
include_directories(${SDL2_INCLUDE_DIRS})
link_directories(
  ${SDL2_LIBRARY_DIRS} ${SDL2_IMAGE_LIBRARY_DIRS} ${SDL2_MIXER_LIBRARY_DIRS})

if(MSVC)
  add_definitions(-wd"4996")
#    add_definitions(-Wall -W4 -wd"4996" -D_USE_MATH_DEFINES)
else()
#    add_definitions(-Wall -Wextra -Wunused -Wshadow -Wunreachable-code -std=c99)
#    add_definitions(-fno-math-errno -fno-exceptions)
endif()

set(MAGUS_SOURCES
  ./src/digisnd.c
  ./src/events.c
  ./src/gfx.c
  ./src/grafx.c
  ./src/items.c
  ./src/keyboard.c
  ./src/magus.c
  ./src/mouse.c
  ./src/pics.c
  ./src/sounds.c
  ./src/vector.c
)
set(MAGUS_HEADERS
  ./src/defs.h
  ./src/digisnd.h
  ./src/events.h
  ./src/gfx.h
  ./src/grafx.h
  ./src/items.h
  ./src/keyboard.h
  ./src/mouse.h
  ./src/pics.h
  ./src/sounds.h
  ./src/vector.h
)
add_executable(magus ${MAGUS_SOURCES} ${MAGUS_HEADERS})
target_link_libraries(magus ${SDL2_LIBRARY})
