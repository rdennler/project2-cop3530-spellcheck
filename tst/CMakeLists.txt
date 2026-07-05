add_library(tst tst.cpp)

target_include_directories(tst PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tst PUBLIC levenshtein)

if (SPELLCHECK_BUILD_TESTS)
    add_subdirectory(test)
endif ()