#include "levenshtein.hpp"

#include <algorithm>
#include <vector>

#include <catch2/catch_test_macros.hpp>

using namespace spellcheck::levenshtein;

TEST_CASE("distance handles identical and empty inputs", "[levenshtein]")
{
    CHECK(distance("", "") == 0);
    CHECK(distance("apple", "apple") == 0);
    CHECK(distance("", "apple") == 5);
    CHECK(distance("apple", "") == 5);
}

TEST_CASE("distance matches known edits", "[levenshtein]")
{
    CHECK(distance("appel", "apple") == 2);
    CHECK(distance("kitten", "sitting") == 3);
    CHECK(distance("flaw", "lawn") == 2);
    CHECK(distance("abc", "abc") == 0);
}

TEST_CASE("distance is symmetric", "[levenshtein]")
{
    CHECK(distance("sunday", "saturday") == distance("saturday", "sunday"));
}

TEST_CASE("bounded distance reports values within the threshold", "[levenshtein]")
{
    CHECK(distance_bounded("appel", "apple", 2) == 2);
    CHECK(distance_bounded("kitten", "sitting", 3) == 3);
}

TEST_CASE("bounded distance short circuits beyond the threshold", "[levenshtein]")
{
    CHECK(distance_bounded("kitten", "sitting", 2) == 3);
    CHECK(distance_bounded("abc", "xyz", 1) > 1);
    CHECK(distance_bounded("a", "aaaaaa", 2) > 2);
}

TEST_CASE("search state walks a word edit by edit", "[levenshtein]")
{
    SearchState state("apple");
    SearchState after = state.step('a').step('p').step('p').step('l').step('e');
    CHECK(after.distance() == 0);

    SearchState misspelled = state.step('a').step('p').step('p').step('e').step('l');
    CHECK(misspelled.distance() == 2);
}

TEST_CASE("search state minimum supports pruning", "[levenshtein]")
{
    SearchState state("apple");
    SearchState partial = state.step('a').step('p');
    CHECK(partial.minimum() <= partial.distance());
    CHECK(partial.minimum() >= 0);
}

TEST_CASE("search state agrees with the full distance function", "[levenshtein]")
{
    const std::string query = "spelling";
    const std::vector<std::string> words = {"spell", "spelled", "smelling", "selling", "spelling"};
    for (const auto& word : words)
    {
        SearchState state(query);
        for (char letter : word)
        {
            state = state.step(letter);
        }
        CHECK(state.distance() == distance(word, query));
    }
}

TEST_CASE("ranking sorts by distance then alphabetically", "[levenshtein]")
{
    std::vector<Suggestion> suggestions = {
        {"appeal", 2}, {"apple", 1}, {"ample", 1}, {"appealing", 4}
    };
    std::sort(suggestions.begin(), suggestions.end(), rank_before);

    CHECK(suggestions[0].word == "ample");
    CHECK(suggestions[1].word == "apple");
    CHECK(suggestions[2].word == "appeal");
    CHECK(suggestions[3].word == "appealing");
}
