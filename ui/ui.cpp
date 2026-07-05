#include "ui.hpp"
#include <chrono>
#include <iostream>
#include <string>
using namespace std;
namespace spellcheck::ui

{
    TerminalUI::TerminalUI(trie::Trie& trie, tst::TST& tst) : trie_(trie), tst_(tst)
{
}

void TerminalUI::run()
{
    while (true)
    {
        cout << "\n--------- Spell Checker ---------\n";
        cout << "1. Check Word\n";
        cout << "2. Benchmark Trie vs TST\n";
        cout << "3. Exit\n";
        cout << "Selection: ";

        string choice;
        getline(cin, choice);

        if (choice == "1")
            normalSearch();
        else if (choice == "2")
            benchmark();
        else if (choice == "3")
            return;
        else
            cout <<"Invalid option\n";
    }
}

void TerminalUI::normalSearch()
{
    string word;
    cout << "\n Enter a word: ";
    getline(cin, word);
    if (trie_.contains(word))
    {
        cout << "\"" << word << "\" is spelled correctly.\n";
        return;
    }
    cout << "\"" << word << "\" not found.\n";

    auto suggestions = trie_.suggest(word, 2, 5);
    printSuggestions(suggestions);
}

void TerminalUI::benchmark()
{
    cout << "Work in progress";
}

void TerminalUI::printSuggestions(const vector<levenshtein::Suggestion>& suggestions)
{
    if (suggestions.empty())
    {
        cout << "No suggestions found.\n";
        return;
    }

    cout << "Suggested Words:\n";
    for (size_t i = 0; i < suggestions.size(); i++)
    {
        cout << i + 1 << "."
            << suggestions[i].word
             << " (distance "
             << suggestions[i].distance
             << ")\n";

    }
}
}