cmake_minimum_required(VERSION 3.30)
project(Test2)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

file(GLOB_RECURSE sources src/* include/*)

add_executable(main ${sources})
target_include_directories(main PRIVATE include)

# optimization
target_compile_options(main PUBLIC -O3)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/share/cmake")
find_package(LHAPDF REQUIRED)
find_package(GSL REQUIRED)

target_include_directories(main PRIVATE ${LHAPDF_INCLUDE_DIRS} ${GSL_INCLUDE_DIRS})
target_link_libraries(main PRIVATE ${LHAPDF_LIBRARIES} ${GSL_LIBRARIES})

