cmake_minimum_required(VERSION 3.18)

# Compillation instructions
# cd build
# cmake -DCMAKE_BUILD_TYPE=Release ..
# make 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(kdtree)

include_directories(
    ${PROJECT_SOURCE_DIR}/kdtree
)

add_executable(${PROJECT_NAME} 
    main.cpp
    kdtree.cpp
)