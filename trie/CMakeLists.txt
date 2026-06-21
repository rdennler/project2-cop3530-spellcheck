add_library(trie trie.cpp)

target_include_directories(trie PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(trie PUBLIC levenshtein)

if (SPELLCHECK_BUILD_TESTS)
    add_subdirectory(test)
endif ()
