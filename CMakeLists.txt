cmake_minimum_required(VERSION 3.10)
project(logging_library VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIERED ON)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(unit_tests test/unit_tests.cpp)

target_include_directories(unit_tests PRIVATE src)

target_link_libraries(unit_tests gtest_main gtest)

include(GoogleTest)
gtest_discover_tests(unit_tests)