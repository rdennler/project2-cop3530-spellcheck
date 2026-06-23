cmake_minimum_required(VERSION 3.16)

project(spellcheck
  VERSION 0.1.0
  DESCRIPTION "Word suggestion and spell checker analyzer comparing Trie and TST"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

option(SPELLCHECK_BUILD_TESTS "Build the unit tests" ON)

if(EXISTS ${CMAKE_SOURCE_DIR}/data/words.txt)
  set(SPELLCHECK_WORDS_FILE ${CMAKE_SOURCE_DIR}/data/words.txt)
else()
  set(SPELLCHECK_WORDS_FILE ${CMAKE_SOURCE_DIR}/words.txt)
endif()

if(SPELLCHECK_BUILD_TESTS)
  include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2)
  FetchContent_MakeAvailable(Catch2)
  list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
  enable_testing()
  include(CTest)
  include(Catch)
endif()

add_subdirectory(levenshtein)
add_subdirectory(words)
add_subdirectory(trie)
add_subdirectory(tst)

add_executable(spellcheck main.cpp)
target_link_libraries(spellcheck PRIVATE trie levenshtein words)
