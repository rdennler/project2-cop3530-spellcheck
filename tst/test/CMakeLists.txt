add_executable(tst_test tst_test.cpp)

target_link_libraries(tst_test PRIVATE tst Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tst_test)