configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/data_path.hpp.in
        ${CMAKE_CURRENT_BINARY_DIR}/generated/data_path.hpp
        @ONLY)

add_library(words dictionary.cpp)

target_include_directories(words
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/generated)

if (SPELLCHECK_BUILD_TESTS)
    add_subdirectory(test)
endif ()
