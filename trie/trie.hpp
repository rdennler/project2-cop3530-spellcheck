#ifndef SPELLCHECK_TRIE_HPP
#define SPELLCHECK_TRIE_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "levenshtein.hpp"

namespace spellcheck::trie
{
    class Trie
    {
    public:
        void insert(std::string_view word);

        bool contains(std::string_view word) const;

        std::vector<levenshtein::Suggestion> suggest(std::string_view query, int max_distance,
                                                     std::size_t max_results = 0) const;

        std::size_t word_count() const;

        std::size_t node_count() const;

        std::size_t approximate_memory_bytes() const;

    private:
        struct Node
        {
            std::unordered_map<char, std::unique_ptr<Node>> children;
            bool is_word = false;
        };

        void search(const Node& node, char letter, const levenshtein::SearchState& parent,
                    std::string& buffer, int max_distance,
                    std::vector<levenshtein::Suggestion>& results) const;

        Node root_;
        std::size_t word_count_ = 0;
        std::size_t node_count_ = 1;
    };
} // namespace spellcheck::trie

#endif
