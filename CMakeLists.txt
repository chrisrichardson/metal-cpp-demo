cmake_minimum_required(VERSION 3.8)
project(HelloWorld)

set(CMAKE_CXX_STANDARD 17)

# Downloaded from Apple
include_directories(${PROJECT_SOURCE_DIR}/metal-cpp)

set(SOURCE_FILES vector_addition/main.cpp vector_addition/metal_adder.cpp)
add_executable(program ${SOURCE_FILES})

find_library(METAL Metal)
find_library(FOUNDATION Foundation)
find_library(QUARTZCORE QuartzCore)

add_custom_command(
  TARGET program POST_BUILD
  COMMAND xcrun -sdk macosx metal -c vector_addition/add.metal -o add.air
  COMMAND xcrun -sdk macosx metallib add.air -o add.metallib
  DEPENDS vector_addition/add.metal)

target_link_libraries(program stdc++ "-framework Metal" "-framework Foundation" "-framework QuartzCore" objc)
