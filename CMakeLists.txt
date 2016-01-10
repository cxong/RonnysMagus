project(RonnysMagus C)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/build/cmake")

option(DEBUG "Enable debug build" OFF)
if(DEBUG)
  set(CMAKE_BUILD_TYPE "Debug")
else()
  add_definitions(-DNDEBUG)
endif()

find_package(PkgConfig)
pkg_search_module(SDL2 sdl2)
pkg_search_module(SDL2_IMAGE SDL2_image)
pkg_search_module(SDL2_MIXER SDL2_mixer)
if(NOT "${SDL2_FOUND}")
	message("looking for SDL2")
	find_package(SDL2 REQUIRED)
endif()
if(NOT "${SDL2_IMAGE_FOUND}")
	message("looking for SDL2_image")
	find_package(SDL2_image REQUIRED)
endif()
if(NOT "${SDL2_MIXER_FOUND}")
	message("looking for SDL2_mixer")
	find_package(SDL2_mixer REQUIRED)
endif()
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})

if(MSVC)
  add_definitions(-wd"4996")
#    add_definitions(-Wall -W4 -wd"4996" -D_USE_MATH_DEFINES)
else()
#    add_definitions(-Wall -Wextra -Wunused -Wshadow -Wunreachable-code -std=c99)
#    add_definitions(-fno-math-errno -fno-exceptions)
endif()

add_subdirectory(src/yajl)
include_directories(src)

set(MAGUS_SOURCES
  ./src/c_array.c
  ./src/digisnd.c
  ./src/events.c
  ./src/font.c
  ./src/font_utils.c
  ./src/gfx.c
  ./src/grafx.c
  ./src/items.c
  ./src/json_utils.c
  ./src/keyboard.c
  ./src/magus.c
  ./src/mouse.c
  ./src/pic.c
  ./src/pics.c
  ./src/sounds.c
  ./src/vector.c
)
set(MAGUS_HEADERS
  ./src/c_array.h
  ./src/defs.h
  ./src/digisnd.h
  ./src/events.h
  ./src/font.h
  ./src/font_utils.h
  ./src/gfx.h
  ./src/grafx.h
  ./src/items.h
  ./src/json_utils.h
  ./src/keyboard.h
  ./src/magus.h
  ./src/mouse.h
  ./src/pic.h
  ./src/pics.h
  ./src/sounds.h
  ./src/utils.h
  ./src/vector.h
)
add_executable(magus ${MAGUS_SOURCES} ${MAGUS_HEADERS})
target_link_libraries(magus yajl_s ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES})
