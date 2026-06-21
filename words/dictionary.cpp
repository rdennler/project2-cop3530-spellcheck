#include "dictionary.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <stdexcept>

namespace spellcheck::words
{
    namespace
    {
        void trim(std::string& line)
        {
            const auto not_space = [](unsigned char ch) { return !std::isspace(ch); };
            line.erase(line.begin(), std::find_if(line.begin(), line.end(), not_space));
            line.erase(std::find_if(line.rbegin(), line.rend(), not_space).base(), line.end());
        }
    } // namespace

    std::vector<std::string> load(std::string_view path, const LoadOptions& options)
    {
        std::ifstream stream{std::string(path)};
        if (!stream)
        {
            throw std::runtime_error("unable to open dictionary file: " + std::string(path));
        }

        std::vector<std::string> words;
        std::string line;
        while (std::getline(stream, line))
        {
            if (!line.empty() && line.back() == '\r')
            {
                line.pop_back();
            }
            trim(line);

            if (options.skip_blank_lines && line.empty())
            {
                continue;
            }

            if (options.lowercase)
            {
                std::transform(line.begin(), line.end(), line.begin(),
                               [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
            }

            words.push_back(std::move(line));
        }

        return words;
    }
} // namespace spellcheck::words
