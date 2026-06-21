#include "dictionary.hpp"

#include <string>

#include <catch2/catch_test_macros.hpp>

using namespace spellcheck::words;

namespace
{
    std::string fixture(const std::string& name)
    {
        return std::string(SPELLCHECK_TEST_FIXTURE_DIR) + "/" + name;
    }
} // namespace

TEST_CASE("load reads every non blank line", "[words]")
{
    std::vector<std::string> words = load(fixture("sample_words.txt"));
    REQUIRE(words.size() == 4);
}

TEST_CASE("load lowercases and trims by default", "[words]")
{
    std::vector<std::string> words = load(fixture("sample_words.txt"));
    CHECK(words[0] == "apple");
    CHECK(words[1] == "banana");
    CHECK(words[2] == "cherry");
    CHECK(words[3] == "date");
}

TEST_CASE("load can preserve original casing", "[words]")
{
    LoadOptions options;
    options.lowercase = false;
    std::vector<std::string> words = load(fixture("sample_words.txt"), options);
    CHECK(words[1] == "Banana");
}

TEST_CASE("load throws when the file is missing", "[words]")
{
    CHECK_THROWS(load(fixture("does_not_exist.txt")));
}
