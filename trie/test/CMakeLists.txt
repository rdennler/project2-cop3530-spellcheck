add_executable(trie_test trie_test.cpp)
target_link_libraries(trie_test PRIVATE trie Catch2::Catch2WithMain)
catch_discover_tests(trie_test)
