#ifndef SPELLCHECK_DICTIONARY_HPP
#define SPELLCHECK_DICTIONARY_HPP

#include <string>
#include <string_view>
#include <vector>

namespace spellcheck::words
{
    struct LoadOptions
    {
        bool lowercase = true;
        bool skip_blank_lines = true;
    };

    std::vector<std::string> load(std::string_view path, const LoadOptions& options = {});
} // namespace spellcheck::words

#endif
