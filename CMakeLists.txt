cmake_minimum_required(VERSION 3.10)

project(usc_path_finder)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(usc_path_finder main.cpp)

target_link_libraries(usc_path_finder Qt5::Widgets)

set(DATA_DIR "${CMAKE_SOURCE_DIR}/data")

add_custom_command(TARGET usc_path_finder POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${DATA_DIR} $<TARGET_FILE_DIR:usc_path_finder>/data)

add_custom_target(copy_data ALL
        DEPENDS ${DATA_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${DATA_DIR} ${CMAKE_BINARY_DIR}/data)
