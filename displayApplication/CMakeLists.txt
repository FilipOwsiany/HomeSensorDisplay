cmake_minimum_required(VERSION 3.10)
project(lvgl_hello C)
set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

include_directories(${CMAKE_SOURCE_DIR})

if(CMAKE_CROSSCOMPILING)
    message("Building for RPI5")
    add_subdirectory(git/lvgl lvgl)

    add_executable(lvgl-hello main.c)

    target_link_libraries(lvgl-hello PRIVATE lvgl pthread m)

    target_include_directories(lvgl-hello PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
else()
    message("Building for Host")
    add_subdirectory(lvgl lvgl)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

    add_executable(lvgl-hello main.c)

    target_include_directories(lvgl-hello PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
    )

    target_link_libraries(lvgl-hello PRIVATE
        lvgl
        pthread
        m
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
    )
endif()