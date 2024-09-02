cmake_minimum_required(VERSION 3.28)

set(VCPKG_TARGET_TRIPLET x64-mingw-dynamic)
set(VCPKG_HOST_TRIPLET x64-mingw-dynamic)
set(SRC_DIRECTORY src)

project(Physics_SimulationV2 LANGUAGES CXX)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_DIR})

find_package(sdl2 REQUIRED)
find_package(fmt REQUIRED)

add_library(physicsEngine OBJECT ${SRC_DIRECTORY}/physicsEngine.cpp)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	target_compile_options(physicsEngine PRIVATE -Wall -Wextra -Wpedantic)
endif()
target_include_directories(physicsEngine PUBLIC ${SRC_DIRECTORY})
target_link_libraries(physicsEngine PRIVATE fmt::fmt)

add_executable(${PROJECT_NAME} ${SRC_DIRECTORY}/main.cpp)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIRECTORY})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} PRIVATE physicsEngine)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)