add_executable(levenshtein_test levenshtein_test.cpp)
target_link_libraries(levenshtein_test PRIVATE levenshtein Catch2::Catch2WithMain)
catch_discover_tests(levenshtein_test)
