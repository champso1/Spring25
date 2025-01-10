cmake_minimum_required(VERSION 3.20)
project(Test)

file(GLOB_RECURSE sources src/* include/*)

add_executable(main ${sources})
target_include_directories(main PRIVATE include)


# use pkg-config for GSL and LHAPDF
find_package(PkgConfig REQUIRED)

# gsl
pkg_check_modules(GSL REQUIRED IMPORTED_TARGET gsl)

target_link_libraries(main PkgConfig::GSL)
target_include_directories(main PUBLIC ${GSL_INCLUDE_DIRS})
