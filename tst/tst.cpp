#include "tst.hpp"

#include <algorithm>

namespace spellcheck::tst
{
    void TST::insert(std::string_view word)
    {
        if (word.empty())
            return;

            Node* raw_root = root_.get();
            insert(raw_root, word, 0);

            if (!root_)
                root_.reset(raw_root);
    }

    void TST::insert(Node*& node, std::string_view word, std::size_t index)
    {
        char letter = word[index];

        if (node == nullptr)
        {
            node = new Node(letter);
            ++node_count_;
        }

        if (letter < node->letter)
        {
            Node* left = node->left.get();
            insert(left, word, index);
            if (!node->left)
            {
                node->left.reset(left);
            }
        }
        else if (letter > node->letter)
        {
            Node* right = node->right.get();
            insert(right, word, index);
            if (!node->right)
            {
                node->right.reset(right);
            }
        }
        else
        {
            if (index + 1 == word.size())
            {
                if (!node->is_word)
                {
                    node->is_word = true;
                    ++word_count_;
                }
            }
            else
            {
                Node* middle = node->middle.get();
                insert(middle, word, index + 1);
                if (!node->middle)
                {
                    node->middle.reset(middle);
                }
            }
        }
    }

    bool TST::contains(std::string_view word) const
    {
        if (word.empty())
            return false;
        return contains(root_.get(), word, 0);
    }

    bool TST::contains(const Node* node, std::string_view word, std::size_t index) const
    {
        if (node == nullptr)
            return false;
        char letter = word[index];

        if (letter < node->letter)
        {
            return contains(node->left.get(), word, index);
        }
        if (letter > node->letter)
        {
            return contains(node->right.get(), word, index);
        }
        if (index + 1 == word.size())
        {
            return node->is_word;
        }

        return contains(node->middle.get(), word, index + 1);
    }
    std::vector<levenshtein::Suggestion> TST::suggest(std::string_view query, int max_distance,
                                                       std::size_t max_results) const
    {
        std::vector<levenshtein::Suggestion> results;
        levenshtein::SearchState state(query);
        std::string buffer;
        
        collect(root_.get(), state, buffer, max_distance, results);

        std::sort(results.begin(), results.end(), levenshtein::rank_before);
        if (max_results != 0 && results.size() > max_results)
        {
            results.resize(max_results);
        }
        return results;
    }

    void TST::collect(const Node* node, const levenshtein::SearchState& parent,
                        std::string& buffer, int max_distance,
                        std::vector<levenshtein::Suggestion>& results) const
    {
        if (node == nullptr)
            return;
        collect(node->left.get(), parent, buffer, max_distance, results);

        levenshtein::SearchState state = parent.step(node->letter);
        buffer.push_back(node->letter);

        if (node->is_word && state.distance() <= max_distance)
            results.push_back({buffer, state.distance()});
        if (state.minimum() <= max_distance)
            collect(node->middle.get(), state, buffer, max_distance, results);
        buffer.pop_back();

        collect(node->right.get(), parent, buffer, max_distance, results);
    }



    std::size_t TST::word_count() const { return word_count_; }

    std::size_t TST::node_count() const { return node_count_; }

    std::size_t TST::approximate_memory_bytes() const
    {
        return node_count_ * sizeof(Node);
    }
}
