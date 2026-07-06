cmake_minimum_required(VERSION 3.10)
project(logging_library VERSION 1.3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)

if(MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

file(GLOB_RECURSE SOURCES 
    "src/Core/*.cpp"
    "src/Filter/*.cpp"
    "src/Enricher/*.cpp"
    "src/Formatter/*.cpp"
    "src/Sink/*.cpp"
)

add_library(logging_lib STATIC ${SOURCES})

target_include_directories(logging_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Filter
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Enricher
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Formatter
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Sink
)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()