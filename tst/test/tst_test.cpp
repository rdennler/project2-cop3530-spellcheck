#include "tst.hpp"

#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

using namespace spellcheck::tst;
using spellcheck::levenshtein::Suggestion;

namespace
{
    TST build(const std::vector<std::string>& words)
    {
        TST tst;
        for (const auto& word : words)
        {
            tst.insert(word);
        }
        return tst;
    }
} // namespace

TEST_CASE("contains finds inserted words only for tst", "[tst]")
{
    TST tst = build({"apple", "appeal", "ample"});
    CHECK(tst.contains("apple"));
    CHECK(tst.contains("appeal"));
    CHECK_FALSE(tst.contains("app"));
    CHECK_FALSE(tst.contains("apples"));
    CHECK_FALSE(tst.contains("banana"));
}

TEST_CASE("word count tracks unique insertions for tst", "[tst]")
{
    TST tst = build({"apple", "apple", "ample"});
    CHECK(tst.word_count() == 2);
}

TEST_CASE("prefixes are not words unless inserted for tst", "[tst]")
{
    TST tst = build({"apple"});
    CHECK_FALSE(tst.contains("app"));
    tst.insert("app");
    CHECK(tst.contains("app"));
    CHECK(tst.word_count() == 2);
}

TEST_CASE("suggest returns candidates within the distance threshold for tst", "[tst]")
{
    TST tst = build({"apple", "appeal", "appealing", "ample", "maple", "banana"});
    std::vector<Suggestion> results = tst.suggest("appel", 2);

    REQUIRE(results.size() == 2);
    CHECK(results[0].word == "appeal");
    CHECK(results[0].distance == 1);
    CHECK(results[1].word == "apple");
    CHECK(results[1].distance == 2);
}

TEST_CASE("suggest orders by distance then alphabetically for tst", "[tst]")
{
    TST tst = build({"apple", "ample", "appeal"});
    std::vector<Suggestion> results = tst.suggest("apple", 2);

    for (std::size_t i = 1; i < results.size(); ++i)
    {
        const bool ordered = results[i - 1].distance < results[i].distance ||
        (results[i - 1].distance == results[i].distance &&
            results[i - 1].word <= results[i].word);
        CHECK(ordered);
    }
}

TEST_CASE("suggest can cap the number of results for tst", "[tst]")
{
    TST tst = build({"apple", "ample", "appeal", "maple", "staple"});
    std::vector<Suggestion> results = tst.suggest("apple", 5, 2);
    CHECK(results.size() == 2);
}

TEST_CASE("suggest finds an exact match at distance zero for tst", "[tst]")
{
    TST tst = build({"apple", "ample"});
    std::vector<Suggestion> results = tst.suggest("apple", 1);
    REQUIRE_FALSE(results.empty());
    CHECK(results.front().word == "apple");
    CHECK(results.front().distance == 0);
}

TEST_CASE("node count grows with shared prefixes only once for tst", "[tst]")
{
    TST tst;
    tst.insert("car");
    const std::size_t after_first = tst.node_count();
    tst.insert("cart");
    CHECK(tst.node_count() == after_first + 1);
}
