cmake_minimum_required(VERSION 3.18)
project(concave_hull)
set(CMAKE_BUILD_TYPE "Release")

# Compillation instructions
# cd build
# cmake -DCMAKE_BUILD_TYPE=Release ..
# make 

find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
include_directories(${Python_INCLUDE_DIRS})
include_directories(${Python_NumPy_INCLUDE_DIRS})

add_executable(main
    main.cpp
)

target_include_directories(main
    PRIVATE
        ../plotting
)

target_link_libraries(main
    Python3::Python
    Python3::Module
    Python3::NumPy
)