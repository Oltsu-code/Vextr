cmake_minimum_required(VERSION 4.1)
project(Vextr VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# grab all cpp files automatically
file(GLOB_RECURSE VEXTR_SRC
        src/*.cpp
)

# include folder
add_library(vextr STATIC ${VEXTR_SRC})

target_include_directories(vextr PUBLIC include)