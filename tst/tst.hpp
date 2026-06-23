#ifndef SPELLCHECK_TST_HPP
#define SPELLCHECK_TST_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "levenshtein.hpp"

namespace spellcheck::tst
{
    class TST
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
            char letter;
            bool is_word = false;
            std::unique_ptr<Node> left;
            std::unique_ptr<Node> middle;
            std::unique_ptr<Node> right;

            explicit Node(char c) : letter(c) {}
        };

        void collect(const Node* node, const levenshtein::SearchState& parent,
                    std::string& buffer, int max_distance,
                    std::vector<levenshtein::Suggestion>& results) const;
        void insert(Node*& node, std::string_view word, std::size_t index);

        bool contains(const Node* node, std::string_view word, std::size_t index) const;
        
        std::unique_ptr<Node> root_;
        std::size_t word_count_ = 0;
        std::size_t node_count_ = 0;
    };
}

#endif
