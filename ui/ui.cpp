#include "ui.hpp"
#include <iostream>
#include <string>

#include "../words/dictionary.hpp"
#include <chrono>

using namespace std;
namespace spellcheck::ui{
    TerminalUI::TerminalUI(trie::Trie &trie, tst::TST &tst) : trie_(trie), tst_(tst){
    }

    void TerminalUI::run(){
        while(true){
            cout << "\n--------- Spell Checker ---------\n";
            cout << "1. Check Word\n";
            cout << "2. Benchmark Trie vs TST\n";
            cout << "3. Exit\n";
            cout << "Selection: ";

            string choice;
            getline(cin, choice);

            if(choice == "1")
                normalSearch();
            else if(choice == "2")
                benchmark();
            else if(choice == "3")
                return;
            else
                cout << "Invalid option\n";
        }
    }

    void TerminalUI::normalSearch(){
        string word;
        cout << "\n Enter a word: ";
        getline(cin, word);
        if(trie_.contains(word)){
            cout << "\"" << word << "\" is spelled correctly.\n";
            return;
        }
        cout << "\"" << word << "\" not found.\n";

        auto suggestions = trie_.suggest(word, 2, 5);
        printSuggestions(suggestions);
    }

    // benchmark implementation
    void TerminalUI::benchmark(){
        cout << "\n> :bench ";
        string word;
        getline(cin, word);

        // Lookup time
        auto start = chrono::high_resolution_clock::now();
        trie_.contains(word);
        auto end = chrono::high_resolution_clock::now();
        long long trieLookupTime = chrono::duration_cast<chrono::microseconds>(end - start).count();

        start = chrono::high_resolution_clock::now();
        tst_.contains(word);
        end = chrono::high_resolution_clock::now();
        long long tstLookupTime = chrono::duration_cast<chrono::microseconds>(end - start).count();

        // Suggestion time
        start = chrono::high_resolution_clock::now();
        trie_.suggest(word, 2, 5);
        end = chrono::high_resolution_clock::now();
        long long trieSuggestTime = chrono::duration_cast<chrono::microseconds>(end - start).count();

        start = chrono::high_resolution_clock::now();
        tst_.suggest(word, 2, 5);
        end = chrono::high_resolution_clock::now();
        long long tstSuggestTime = chrono::duration_cast<chrono::microseconds>(end - start).count();

        cout << "Trie: lookup " << trieLookupTime << "us, suggest " << trieSuggestTime << "us\n";
        cout << "TST: lookup " << tstLookupTime << "us, suggest " << tstSuggestTime << "us\n";

        if(trieBuildTime_ == 0 && tstBuildTime_ == 0){
            vector<string> words;
            try{
                words = spellcheck::words::load("words.txt");
            }
            catch(const exception &){
                words.clear();
            }

            if(! words.empty()){
                trie::Trie freshTrie;
                start = chrono::high_resolution_clock::now();
                for(const auto &w: words)
                    freshTrie.insert(w);
                end = chrono::high_resolution_clock::now();
                trieBuildTime_ = chrono::duration_cast<chrono::milliseconds>(end - start).count();

                tst::TST freshTst;
                start = chrono::high_resolution_clock::now();
                for(const auto &w: words)
                    freshTst.insert(w);
                end = chrono::high_resolution_clock::now();
                tstBuildTime_ = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            }
        }

        const size_t trieMemoryMB = trie_.approximate_memory_bytes()/(1024*1024);
        const size_t tstMemoryMB = tst_.approximate_memory_bytes()/(1024*1024);

        cout << "\nMetric        Trie        TST\n";
        cout << "Build time    " << trieBuildTime_ << " ms      " << tstBuildTime_ << " ms\n";
        cout << "Lookup time   " << trieLookupTime << " us       " << tstLookupTime << " us\n";
        cout << "Suggest time  " << trieSuggestTime << " us      " << tstSuggestTime << " us\n";
        cout << "Memory        " << trieMemoryMB << " MB      " << tstMemoryMB << " MB\n";

        // Report which structure performed better overall
        int trieWins = (trieLookupTime <= tstLookupTime) + (trieSuggestTime <= tstSuggestTime)
                       + (trieBuildTime_ <= tstBuildTime_) + (trieMemoryMB <= tstMemoryMB);
        cout << "\n" << (trieWins >= 2? "Trie" : "TST") << " performed better overall.\n";
    }

    void TerminalUI::printSuggestions(const vector <levenshtein::Suggestion> &suggestions){
        if(suggestions.empty()){
            cout << "No suggestions found.\n";
            return;
        }

        cout << "Suggested Words:\n";
        for(size_t i = 0; i < suggestions.size(); i ++){
            cout << i + 1 << "."
                 << suggestions[i].word
                 << " (distance "
                 << suggestions[i].distance
                 << ")\n";

        }
    }
}
