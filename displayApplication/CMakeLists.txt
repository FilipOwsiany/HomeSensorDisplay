cmake_minimum_required(VERSION 3.10)
project(lvgl_hello C)
set(CMAKE_C_STANDARD 11)

add_compile_options(-Wall -Wextra -Wpedantic)
include_directories(${CMAKE_SOURCE_DIR})

if(CMAKE_CROSSCOMPILING)
    message("\n\nBuilding for RPI5\n\n")

    set(LV_BUILD_USE_KCONFIG OFF CACHE BOOL "" FORCE)
    set(LV_BUILD_CONF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config_rpi5/lv_conf.h"
        CACHE FILEPATH "LVGL config file" FORCE)

    add_subdirectory(lvgl lvgl)

    add_executable(lvgl-hello main.c)
    target_compile_definitions(lvgl-hello PRIVATE BUILDING_FOR_RPI5=1)
    target_link_libraries(lvgl-hello PRIVATE lvgl pthread m)
    target_include_directories(lvgl-hello PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/config_rpi5
        ${CMAKE_CURRENT_SOURCE_DIR})
else()
    message("\n\nBuilding for Host\n\n")

    set(LV_BUILD_USE_KCONFIG OFF CACHE BOOL "" FORCE)
    set(LV_BUILD_CONF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config_pc/lv_conf.h"
        CACHE FILEPATH "LVGL config file" FORCE)

    add_subdirectory(lvgl lvgl)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

    add_executable(lvgl-hello main.c)
    target_compile_definitions(lvgl-hello PRIVATE BUILDING_FOR_PC=1)
    target_include_directories(lvgl-hello PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/config_pc
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
    )
    target_link_libraries(lvgl-hello PRIVATE
        lvgl pthread m
        ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES}
    )
endif()
