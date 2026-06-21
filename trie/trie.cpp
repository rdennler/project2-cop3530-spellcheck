#include "trie.hpp"

#include <algorithm>

namespace spellcheck::trie
{
    void Trie::insert(std::string_view word)
    {
        Node* node = &root_;
        for (char letter : word)
        {
            auto& child = node->children[letter];
            if (!child)
            {
                child = std::make_unique<Node>();
                ++node_count_;
            }
            node = child.get();
        }
        if (!node->is_word)
        {
            node->is_word = true;
            ++word_count_;
        }
    }

    bool Trie::contains(std::string_view word) const
    {
        const Node* node = &root_;
        for (char letter : word)
        {
            auto it = node->children.find(letter);
            if (it == node->children.end())
            {
                return false;
            }
            node = it->second.get();
        }
        return node->is_word;
    }

    std::vector<levenshtein::Suggestion> Trie::suggest(std::string_view query, int max_distance,
                                                       std::size_t max_results) const
    {
        std::vector<levenshtein::Suggestion> results;
        levenshtein::SearchState state(query);
        std::string buffer;
        for (const auto& entry : root_.children)
        {
            search(*entry.second, entry.first, state, buffer, max_distance, results);
        }

        std::sort(results.begin(), results.end(), levenshtein::rank_before);
        if (max_results != 0 && results.size() > max_results)
        {
            results.resize(max_results);
        }
        return results;
    }

    void Trie::search(const Node& node, char letter, const levenshtein::SearchState& parent,
                      std::string& buffer, int max_distance,
                      std::vector<levenshtein::Suggestion>& results) const
    {
        const levenshtein::SearchState state = parent.step(letter);
        buffer.push_back(letter);

        if (node.is_word && state.distance() <= max_distance)
        {
            results.push_back({buffer, state.distance()});
        }

        if (state.minimum() <= max_distance)
        {
            for (const auto& entry : node.children)
            {
                search(*entry.second, entry.first, state, buffer, max_distance, results);
            }
        }

        buffer.pop_back();
    }

    std::size_t Trie::word_count() const { return word_count_; }

    std::size_t Trie::node_count() const { return node_count_; }

    std::size_t Trie::approximate_memory_bytes() const
    {
        return node_count_ * (sizeof(Node) + sizeof(std::unique_ptr<Node>) + sizeof(char));
    }
} // namespace spellcheck::trie
