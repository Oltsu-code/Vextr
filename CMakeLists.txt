cmake_minimum_required(VERSION 3.20)
project(Vextr VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler warnings (informational only, don't break builds)
if(MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra)
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# ============================================================================
# LIBRARY: Vextr
# ============================================================================

file(GLOB_RECURSE VEXTR_SRC src/*.cpp)

add_library(vextr STATIC ${VEXTR_SRC}
    include/Vextr/core/Align.hpp
    src/core/FocusManager.cpp
)

target_include_directories(vextr PUBLIC include)

# ============================================================================
# EXAMPLES
# ============================================================================

# Layout showcase - demonstrates grid, stack, and absolute layouts
add_executable(layout_example examples/layout/main.cpp)
target_link_libraries(layout_example PRIVATE vextr)
target_include_directories(layout_example PRIVATE examples)

# Focus demo - demonstrates focus management and custom widgets
add_executable(focus_example examples/focus/main.cpp)
target_link_libraries(focus_example PRIVATE vextr)
target_include_directories(focus_example PRIVATE examples)

add_executable(button_example examples/buttons/main.cpp)
target_link_libraries(button_example PRIVATE vextr)
target_include_directories(button_example PRIVATE examples)

add_executable(navigator_example examples/navigator/main.cpp)
target_link_libraries(navigator_example PRIVATE vextr)
target_include_directories(navigator_example PRIVATE examples)

add_executable(input_example examples/input/main.cpp)
target_link_libraries(input_example PRIVATE vextr)
target_include_directories(input_example PRIVATE examples)

# ============================================================================
# TESTING
# ============================================================================

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

enable_testing()

add_executable(vextr_tests
    tests/main.cpp
    tests/test_geometry.cpp
    tests/test_input_parser.cpp
    tests/test_label_widget.cpp
    tests/test_unicode.cpp
    tests/test_app.cpp
)

target_link_libraries(vextr_tests PRIVATE vextr gtest gtest_main)
target_include_directories(vextr_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

gtest_discover_tests(vextr_tests WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

