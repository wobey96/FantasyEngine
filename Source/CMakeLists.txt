cmake_minimum_required (VERSION 3.26)

file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/Source/*.h" "${CMAKE_SOURCE_DIR}/Source/*.cpp")
source_group("Source" "${CMAKE_SOURCE_DIR}/Source" ${SUBDIRECTORIES})
add_executable("Source" main.cpp ${SUBDIRECTORIES})

target_include_directories("Source" PRIVATE "${CMAKE_SOURCE_DIR}/Source")