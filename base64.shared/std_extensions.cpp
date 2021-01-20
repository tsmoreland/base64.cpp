//
// Copyright © 2021 Terry Moreland
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// 

#include "pch.h"
#include "std_extensions.h"

namespace moreland::std_extensions
{

    bool string_lower_equals(std::string_view first, std::string_view second)
    {
        if (first.size() != second.size())
            return false;

        for (auto first_index = begin(first), second_index = begin(second); 
             first_index != end(first) && second_index != end(second); 
             ++first_index, ++second_index) {

            if (tolower(*first_index) != tolower(*second_index))
                return false;
        }
        return true;
    }

    std::string to_string(std::filesystem::path const& source) noexcept
    {
#ifdef _WIN32
        std::wstring_view const source_view{source.c_str()};
        std::string destination{};
        std::ranges::transform(source_view, begin(destination), 
            [](auto const& wide_char) {
                return static_cast<char>(wide_char);
            });   
        return std::string{begin(source_view), end(source_view)};
#elif __linux__
        return std::string{source.c_str()};
#else
#error  Unsupported architecture
#endif
    }
}
