cmake_minimum_required(VERSION 3.20)
project(Vextr VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

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

add_library(vextr STATIC ${VEXTR_SRC})

target_include_directories(vextr PUBLIC include)

# link utf8proc
target_link_libraries(vextr PUBLIC utf8proc)

# ============================================================================
# EXAMPLES
# ============================================================================

function(add_example name)
    add_executable(${name} examples/${name}/main.cpp)
    target_link_libraries(${name} PRIVATE vextr)
    target_include_directories(${name} PRIVATE examples)
endfunction()

add_example(layout_example)
add_example(focus_example)
add_example(button_example)
add_example(navigator_example)
add_example(input_example)

# ============================================================================
# EXTERNAL DEPENDENCIES
# ============================================================================

include(FetchContent)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# GoogleTest
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# utf8proc
FetchContent_Declare(
        utf8proc
        GIT_REPOSITORY https://github.com/JuliaStrings/utf8proc.git
        GIT_TAG v2.9.0
)

FetchContent_MakeAvailable(utf8proc)

# ============================================================================
# TESTING
# ============================================================================

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

