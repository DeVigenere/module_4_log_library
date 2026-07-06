add_executable(logging_example example.cpp)

target_link_libraries(logging_example logging_lib)

target_include_directories(logging_example PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Filter
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Enricher
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Formatter
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Sink
)