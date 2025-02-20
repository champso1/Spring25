file(GLOB_RECURSE sources src/* inc/*)

add_library(ColSim_Math STATIC ${sources})
add_library(ColSim::Math ALIAS ColSim_Math)

target_include_directories(ColSim_Math PUBLIC inc)
target_link_libraries(ColSim_Math PUBLIC ColSim::Core)
