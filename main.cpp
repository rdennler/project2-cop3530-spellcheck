#include "ui.hpp"
#include "dictionary.hpp"
#include "trie.hpp"
#include "tst.hpp"
#include <exception>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
int main()
{
    spellcheck::trie::Trie trie;
    spellcheck::tst::TST tst;

    vector<string> words;

    try
    {
        words = spellcheck::words::load("words.txt");
    }
    catch (const exception& error)
    {
        cout << error.what() << "\n";
        return 1;
    }

    for (const auto& word : words)
    {
        trie.insert(word);
        tst.insert(word);
    }

    spellcheck::ui::TerminalUI ui(trie, tst);
    ui.run();

    return 0;
}