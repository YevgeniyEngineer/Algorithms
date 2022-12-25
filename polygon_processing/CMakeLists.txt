cmake_minimum_required(VERSION 3.18)

# Compillation instructions
# cd build
# cmake -DCMAKE_BUILD_TYPE=Release ..
# make 

project(polygon_processing)

include_directories(
    ${PROJECT_SOURCE_DIR}/convex_hull
    ${PROJECT_SOURCE_DIR}/concave_hull
    ${PROJECT_SOURCE_DIR}/matplotlib
    ${PROJECT_SOURCE_DIR}/include
)

find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)
include_directories(${Python_INCLUDE_DIRS})
include_directories(${Python_NumPy_INCLUDE_DIRS})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

add_executable(${PROJECT_NAME} 
    convex_hull/convex_hull.cpp
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    Python3::Python
    Python3::Module
    Python3::NumPy
)
