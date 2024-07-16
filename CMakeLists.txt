cmake_minimum_required(VERSION 3.10)

project(usc_path_finder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Add the headers to the source list to ensure MOC processes them
set(SOURCES
        main.cpp
        path-manager.cpp
        input-form.cpp
        path-manager.h
        input-form.h
        main-window.h
)

add_executable(usc_path_finder ${SOURCES})

target_link_libraries(usc_path_finder Qt5::Widgets)

set(DATA_DIR "${CMAKE_SOURCE_DIR}/data")

add_custom_command(TARGET usc_path_finder POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${DATA_DIR} $<TARGET_FILE_DIR:usc_path_finder>/data)

add_custom_target(copy_data ALL
        DEPENDS ${DATA_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${DATA_DIR} ${CMAKE_BINARY_DIR}/data)
