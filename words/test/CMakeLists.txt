add_executable(words_test dictionary_test.cpp)
target_link_libraries(words_test PRIVATE words Catch2::Catch2WithMain)
target_compile_definitions(words_test PRIVATE
        SPELLCHECK_TEST_FIXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/fixtures")
catch_discover_tests(words_test)
