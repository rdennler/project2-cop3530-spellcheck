#ifndef SPELLCHECK_LEVENSHTEIN_HPP
#define SPELLCHECK_LEVENSHTEIN_HPP

#include <string>
#include <string_view>
#include <vector>

namespace spellcheck::levenshtein
{
    int distance(std::string_view source, std::string_view target);

    int distance_bounded(std::string_view source, std::string_view target, int max_distance);

    struct Suggestion
    {
        std::string word;
        int distance;
    };

    bool rank_before(const Suggestion& a, const Suggestion& b);

    class SearchState
    {
    public:
        explicit SearchState(std::string_view query);

        SearchState step(char letter) const;

        int distance() const;

        int minimum() const;

    private:
        SearchState(std::string_view query, std::vector<int> row);

        std::string_view query_;
        std::vector<int> row_;
    };
} // namespace spellcheck::levenshtein

#endif
