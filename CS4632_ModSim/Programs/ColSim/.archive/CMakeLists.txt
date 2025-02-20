cmake_minimum_required(VERSION 3.20)
project(Logger)


file(GLOB_RECURSE sources
  ${CMAKE_CURRENT_SOURCE_DIR}/src/* ${CMAKE_CURRENT_SOURCE_DIR}/include/*)

add_executable(main ${sources})


# tell cmake where headers are
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)



# if on a unix-like system, also add sanitize options on top of all warning/errors
# this functionality is not present on windows GCC versions
if(UNIX)
  target_compile_options(main PRIVATE -Werror -Wall -Wextra -fsanitize=undefined,address)
  target_link_libraries(main PRIVATE -fsanitize=undefined,address -static-libasan)
else()
  target_compile_options(main PRIVATE -Werror -Wall -Wextra)
endif()


# copy the `res` directory to build folder
# so we can handle relative paths like "res/xx.xx" in the code
# once the executable is produced
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})



# --------------------------
#   Doxygen/Documentation
# --------------------------

option(BUILD_DOCS "Build documentation" OFF)

if(BUILD_DOCS)
  find_package(Doxygen)
  if (DOXYGEN_FOUND)
	set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
	set(DOXYGEN_GENERATE_TREEVIEW YES)
	file(GLOB_RECURSE manualfiles ${CMAKE_CURRENT_SOURCE_DIR}/res/manual/*)
	doxygen_add_docs(doxygen
	  ${sources} ${manualfiles}
	  ALL)
  endif(DOXYGEN_FOUND)
endif(BUILD_DOCS)
