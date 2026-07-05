#include "ui.hpp"

#include "dictionary.hpp"
#include "trie.hpp"
#include "tst.hpp"

#include <chrono>
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

    auto trieBuildStart = chrono::high_resolution_clock::now();

    for (const string& word : words)
    {
        trie.insert(word);
    }
    auto trieBuildEnd = chrono::high_resolution_clock::now();
    auto tstBuildStart = chrono::high_resolution_clock::now();

    for (const string& word : words)
    {
        tst.insert(word);
    }

    auto tstBuildEnd = chrono::high_resolution_clock::now();

    auto trieBuildTime = chrono::duration_cast<chrono::milliseconds>(tstBuildEnd - tstBuildStart).count();

    auto tstBuildTime = chrono::duration_cast<chrono::milliseconds>(tstBuildEnd - tstBuildStart).count();

    cout << "Loaded " << words.size() << " words.\n";

    spellcheck::ui::TerminalUI ui(trie, tst, trieBuildTime, tstBuildTime);
    ui.run();

    return 0;
}