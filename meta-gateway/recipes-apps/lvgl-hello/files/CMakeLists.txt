cmake_minimum_required(VERSION 3.10)
project(lvgl_hello C)
set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
include_directories(${CMAKE_SOURCE_DIR})

# dodaj LVGL jako subdir (bo jest w git/lvgl)
add_subdirectory(git/lvgl lvgl)

add_executable(lvgl-hello main.c)

target_link_libraries(lvgl-hello PRIVATE lvgl pthread m)

# żeby lv_conf.h było widoczne
target_include_directories(lvgl-hello PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
