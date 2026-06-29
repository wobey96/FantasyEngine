cmake_minimum_required (VERSION 3.26)

# INCLUDE 3RD PARTY LIBRARIES 
include("${CMAKE_SOURCE_DIR}/Scripts/FetchLibraries.cmake")

# POPULATE 3RD PARTY LIBRARIES 
FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(stbi)

# CREATE EXECUTABLE 
file(GLOB_RECURSE SUBDIRECTORIES "${CMAKE_SOURCE_DIR}/Source/*.h" "${CMAKE_SOURCE_DIR}/Source/*.cpp")
source_group("Source" "${CMAKE_SOURCE_DIR}/Source" ${SUBDIRECTORIES})
add_executable("FantasyEngine" main.cpp ${SUBDIRECTORIES})

# INCLUDE DIRECTORIES AND LINK LIBRARIES 
target_include_directories("FantasyEngine" PRIVATE "${CMAKE_SOURCE_DIR}/Source")

if(stbi_POPULATED)
	target_include_directories("FantasyEngine" PRIVATE "${stbi_SOURCE_DIR}")
	#target_compile_definitions("FantasyEngine" PUBLIC STB_IMAGE_IMPLEMENTATION=1)
endif()

target_link_libraries("FantasyEngine" PRIVATE glfw)
target_link_libraries("FantasyEngine" PRIVATE d3d11.lib) 
target_link_libraries("FantasyEngine" PRIVATE d3dcompiler.lib) 
target_link_libraries("FantasyEngine" PRIVATE dxguid.lib) 

# DEBUG AND RELEASE FLAGS 
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions("FantasyEngine" PUBLIC ENGINE_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_definitions("FantasyEngine" PUBLIC ENGINE_RELEASE)
endif()

# MACRO DEFINITIONS 
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	target_compile_definitions("FantasyEngine" PUBLIC RUNTIME_API=__declspec\(dllexport\))
	target_compile_definitions("FantasyEngine" PUBLIC FORCEINLINE=__forceinline)
else()
	message(FATAL_ERROR "Unsupported Compiler")
endif()