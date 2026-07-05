#ifndef SPELLCHECK_TERMINAL_UI_HPP
#define SPELLCHECK_TERMINAL_UI_HPP
#include "trie.hpp"
#include "tst.hpp"
namespace spellcheck::ui
{
    class TerminalUI
    {
        public:
            TerminalUI(trie::Trie& trie, tst::TST& tst);
            void run();

        private:
            trie::Trie& trie_;
            tst::TST& tst_;
            long long trieBuildTime_;
            long long tstBuildTime_;
            void normalSearch();
            void benchmark();
            void printSuggestions(const std::vector<levenshtein::Suggestion>& suggestions);
    };
}

#endif