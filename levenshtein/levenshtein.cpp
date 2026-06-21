#include "levenshtein.hpp"

#include <algorithm>
#include <numeric>

namespace spellcheck::levenshtein
{
    int distance(std::string_view source, std::string_view target)
    {
        const std::size_t rows = source.size();
        const std::size_t cols = target.size();

        std::vector<int> previous(cols + 1);
        std::vector<int> current(cols + 1);
        std::iota(previous.begin(), previous.end(), 0);

        for (std::size_t i = 1; i <= rows; ++i)
        {
            current[0] = static_cast<int>(i);
            for (std::size_t j = 1; j <= cols; ++j)
            {
                const int substitution = previous[j - 1] + (source[i - 1] == target[j - 1] ? 0 : 1);
                const int insertion = current[j - 1] + 1;
                const int deletion = previous[j] + 1;
                current[j] = std::min({substitution, insertion, deletion});
            }
            std::swap(previous, current);
        }

        return previous[cols];
    }

    int distance_bounded(std::string_view source, std::string_view target, int max_distance)
    {
        if (max_distance < 0)
        {
            return max_distance + 1;
        }

        const int length_gap = std::abs(static_cast<int>(source.size()) - static_cast<int>(target.size()));
        if (length_gap > max_distance)
        {
            return max_distance + 1;
        }

        const std::size_t cols = target.size();
        std::vector<int> previous(cols + 1);
        std::vector<int> current(cols + 1);
        std::iota(previous.begin(), previous.end(), 0);

        for (std::size_t i = 1; i <= source.size(); ++i)
        {
            current[0] = static_cast<int>(i);
            int row_minimum = current[0];
            for (std::size_t j = 1; j <= cols; ++j)
            {
                const int substitution = previous[j - 1] + (source[i - 1] == target[j - 1] ? 0 : 1);
                const int insertion = current[j - 1] + 1;
                const int deletion = previous[j] + 1;
                current[j] = std::min({substitution, insertion, deletion});
                row_minimum = std::min(row_minimum, current[j]);
            }
            if (row_minimum > max_distance)
            {
                return max_distance + 1;
            }
            std::swap(previous, current);
        }

        return std::min(previous[cols], max_distance + 1);
    }

    bool rank_before(const Suggestion& a, const Suggestion& b)
    {
        if (a.distance != b.distance)
        {
            return a.distance < b.distance;
        }
        return a.word < b.word;
    }

    SearchState::SearchState(std::string_view query) : query_(query), row_(query.size() + 1)
    {
        std::iota(row_.begin(), row_.end(), 0);
    }

    SearchState::SearchState(std::string_view query, std::vector<int> row)
        : query_(query), row_(std::move(row))
    {
    }

    SearchState SearchState::step(char letter) const
    {
        std::vector<int> next(row_.size());
        next[0] = row_[0] + 1;
        for (std::size_t i = 1; i < row_.size(); ++i)
        {
            const int substitution = row_[i - 1] + (query_[i - 1] == letter ? 0 : 1);
            const int insertion = next[i - 1] + 1;
            const int deletion = row_[i] + 1;
            next[i] = std::min({substitution, insertion, deletion});
        }
        return SearchState(query_, std::move(next));
    }

    int SearchState::distance() const { return row_.back(); }

    int SearchState::minimum() const { return *std::min_element(row_.begin(), row_.end()); }
} // namespace spellcheck::levenshtein
