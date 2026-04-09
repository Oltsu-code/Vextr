cmake_minimum_required(VERSION 3.20)
project(Vextr VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE VEXTR_SRC src/*.cpp)

# include folder
add_library(vextr STATIC ${VEXTR_SRC}
        include/Vextr/core/Align.hpp
        src/core/FocusManager.cpp)
target_include_directories(vextr PUBLIC include)

# examples — inherit includes from vextr via link
add_executable(layout_example examples/layout.cpp
        examples/focus.cpp)
target_link_libraries(layout_example PRIVATE vextr)

add_executable(focus_example examples/focus.cpp
        examples/focus.cpp)
target_link_libraries(focus_example PRIVATE vextr)