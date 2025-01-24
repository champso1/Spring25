cmake_minimum_required(VERSION 3.20)
project(Logger)

file(GLOB_RECURSE sources src/* include/*)


add_executable(main ${sources})
target_include_directories(main PRIVATE include)

find_package(Doxygen)
if (DOXYGEN_FOUND)
  set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
  doxygen_add_docs(doxygen
	${sources}
	ALL
	CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
  
endif (DOXYGEN_FOUND)


# copy the `res` directory to build folder
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
