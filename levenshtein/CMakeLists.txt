add_library(levenshtein levenshtein.cpp)

target_include_directories(levenshtein PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (SPELLCHECK_BUILD_TESTS)
    add_subdirectory(test)
endif ()
