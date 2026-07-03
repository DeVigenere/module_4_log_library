include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(unit_tests unit_tests.cpp)

target_link_libraries(unit_tests 
    logging_lib 
    gtest_main 
    gtest
)

target_include_directories(unit_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Filter
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Enricher
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Formatter
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Sink
)

if(MSVC)
    target_compile_definitions(unit_tests PRIVATE _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
    target_link_options(unit_tests PRIVATE /NODEFAULTLIB:LIBCMT)
endif()

include(GoogleTest)
gtest_discover_tests(unit_tests)