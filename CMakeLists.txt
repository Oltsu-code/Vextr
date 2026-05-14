cmake_minimum_required(VERSION 3.20)
project(Vextr VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(VEXTR_IS_TOPLEVEL ON)
else()
    set(VEXTR_IS_TOPLEVEL OFF)
endif()

option(VEXTR_BUILD_TESTS    "Build Vextr tests"    ${VEXTR_IS_TOPLEVEL})
option(VEXTR_BUILD_EXAMPLES "Build Vextr examples" ${VEXTR_IS_TOPLEVEL})
option(VEXTR_BUILD_DOCS     "Build Vextr docs"     ${VEXTR_IS_TOPLEVEL})

if(MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# ============================================================================
# DEPENDENCIES
# ============================================================================

include(FetchContent)

# utf8proc — always needed (Vextr links it)
FetchContent_Declare(
        utf8proc
        GIT_REPOSITORY https://github.com/JuliaStrings/utf8proc.git
        GIT_TAG v2.9.0
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(utf8proc)

# GoogleTest — only when building tests
if(VEXTR_BUILD_TESTS)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.14.0
            EXCLUDE_FROM_ALL
    )
    FetchContent_MakeAvailable(googletest)
endif()

# ============================================================================
# LIBRARY: Vextr
# ============================================================================

file(GLOB_RECURSE VEXTR_SRC CONFIGURE_DEPENDS src/*.cpp)

add_library(vextr STATIC ${VEXTR_SRC})
add_library(vextr::vextr ALIAS vextr)

target_include_directories(vextr
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(vextr PUBLIC utf8proc)

# ============================================================================
# INSTALL
# ============================================================================

include(GNUInstallDirs)

install(TARGETS vextr
        EXPORT vextrTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# ============================================================================
# EXAMPLES
# ============================================================================

if(VEXTR_BUILD_EXAMPLES)
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
    add_example(border_example)
    add_example(overlay_example)
    add_example(scrollview_example)
    add_example(text_widgets_example)
endif()

# ============================================================================
# TESTING
# ============================================================================

if(VEXTR_BUILD_TESTS)
    include(GoogleTest)
    enable_testing()

    add_executable(vextr_tests
            tests/main.cpp
            tests/test_geometry.cpp
            tests/test_input_parser.cpp
            tests/test_label_widget.cpp
            tests/test_text_view.cpp
            tests/test_text_area.cpp
            tests/test_scroll_view.cpp
            tests/test_unicode.cpp
            tests/test_app.cpp
    )

    target_link_libraries(vextr_tests PRIVATE vextr gtest gtest_main)
    target_include_directories(vextr_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

    gtest_discover_tests(vextr_tests WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

# ============================================================================
# DOCUMENTATION (Doxygen)
# ============================================================================

if(VEXTR_BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_PROJECT_NAME ${PROJECT_NAME})
        set(DOXYGEN_PROJECT_NUMBER ${PROJECT_VERSION})
        set(DOXYGEN_GENERATE_HTML NO)
        set(DOXYGEN_GENERATE_XML YES)
        set(DOXYGEN_XML_OUTPUT ${CMAKE_SOURCE_DIR}/docs/doxygen-xml)
        set(DOXYGEN_RECURSIVE YES)

        doxygen_add_docs(vextr_docs
                ${CMAKE_CURRENT_SOURCE_DIR}/include
                ${CMAKE_CURRENT_SOURCE_DIR}/src
                COMMENT "Generating Doxygen XML"
        )
    else()
        message(STATUS "Doxygen not found, skipping docs target.")
    endif()
endif()