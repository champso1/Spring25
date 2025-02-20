cmake_minimum_required(VERSION 3.20)
project(Test)

file(GLOB_RECURSE sources src/* include/*)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LHAPDF REQUIRED IMPORTED_TARGET lhapdf)

add_executable(main ${sources})
target_include_directories(main PRIVATE include)

target_link_libraries(main PRIVATE PkgConfig::LHAPDF)
target_compile_options(main PRIVATE -O3)
