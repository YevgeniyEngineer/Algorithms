cmake_minimum_required(VERSION 3.18)

# Compillation instructions
# cd build
# cmake -DCMAKE_BUILD_TYPE=Release ..
# make 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(kdtree)

include_directories(
    ${PROJECT_SOURCE_DIR}
)

add_executable(${PROJECT_NAME}
    main.cpp
)

find_package(GTest REQUIRED)
find_package(TBB REQUIRED)

enable_testing()

add_executable(${PROJECT_NAME}_test
    test.cpp
)

target_link_libraries(${PROJECT_NAME}
    TBB::tbb
)

target_link_libraries(${PROJECT_NAME}_test
    GTest::gtest_main
    TBB::tbb
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)