cmake_minimum_required(VERSION 3.5)

project(RayTracing)

# Organize CMake folders (ZERO_CHECKS and ALL_BUILD)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PREDEFINED_TARGETS_FOLDER "CustomTargets")

# Set the executable output path
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Set SFML directory
set(SDL2_DIR "lib/SDL2-2.0.9")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

#Bring the headers into the project
include_directories(include)

#Add the sources
file(GLOB HEADERS "include/*.h")

#Add the sources
file(GLOB SOURCES "src/*.cpp")

add_executable(RayTracing ${SOURCES} ${HEADERS})
if (UNIX)
    target_link_libraries(RayTracing pthread)
endif(UNIX)
target_link_libraries(RayTracing ${SDL2_LIBRARIES})