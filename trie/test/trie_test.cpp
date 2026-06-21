#include "trie.hpp"

#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

using namespace spellcheck::trie;
using spellcheck::levenshtein::Suggestion;

namespace
{
    Trie build(const std::vector<std::string>& words)
    {
        Trie trie;
        for (const auto& word : words)
        {
            trie.insert(word);
        }
        return trie;
    }
} // namespace

TEST_CASE("contains finds inserted words only", "[trie]")
{
    Trie trie = build({"apple", "appeal", "ample"});
    CHECK(trie.contains("apple"));
    CHECK(trie.contains("appeal"));
    CHECK_FALSE(trie.contains("app"));
    CHECK_FALSE(trie.contains("apples"));
    CHECK_FALSE(trie.contains("banana"));
}

TEST_CASE("word count tracks unique insertions", "[trie]")
{
    Trie trie = build({"apple", "apple", "ample"});
    CHECK(trie.word_count() == 2);
}

TEST_CASE("prefixes are not words unless inserted", "[trie]")
{
    Trie trie = build({"apple"});
    CHECK_FALSE(trie.contains("app"));
    trie.insert("app");
    CHECK(trie.contains("app"));
    CHECK(trie.word_count() == 2);
}

TEST_CASE("suggest returns candidates within the distance threshold", "[trie]")
{
    Trie trie = build({"apple", "appeal", "appealing", "ample", "maple", "banana"});
    std::vector<Suggestion> results = trie.suggest("appel", 2);

    REQUIRE(results.size() == 2);
    CHECK(results[0].word == "appeal");
    CHECK(results[0].distance == 1);
    CHECK(results[1].word == "apple");
    CHECK(results[1].distance == 2);

    for (const auto& suggestion : results)
    {
        CHECK(suggestion.distance <= 2);
    }
}

TEST_CASE("suggest orders by distance then alphabetically", "[trie]")
{
    Trie trie = build({"apple", "ample", "appeal"});
    std::vector<Suggestion> results = trie.suggest("apple", 2);

    for (std::size_t i = 1; i < results.size(); ++i)
    {
        const bool ordered = results[i - 1].distance < results[i].distance ||
        (results[i - 1].distance == results[i].distance &&
            results[i - 1].word <= results[i].word);
        CHECK(ordered);
    }
}

TEST_CASE("suggest can cap the number of results", "[trie]")
{
    Trie trie = build({"apple", "ample", "appeal", "maple", "staple"});
    std::vector<Suggestion> results = trie.suggest("apple", 5, 2);
    CHECK(results.size() == 2);
}

TEST_CASE("suggest finds an exact match at distance zero", "[trie]")
{
    Trie trie = build({"apple", "ample"});
    std::vector<Suggestion> results = trie.suggest("apple", 1);
    REQUIRE_FALSE(results.empty());
    CHECK(results.front().word == "apple");
    CHECK(results.front().distance == 0);
}

TEST_CASE("node count grows with shared prefixes only once", "[trie]")
{
    Trie trie;
    trie.insert("car");
    const std::size_t after_first = trie.node_count();
    trie.insert("cart");
    CHECK(trie.node_count() == after_first + 1);
}
