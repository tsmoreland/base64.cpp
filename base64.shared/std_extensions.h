//
// Copyright (c) 2023 Terry Moreland
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

#pragma once

#include <filesystem>
#include <optional>
#include <span>

namespace moreland::std_extensions
{
    [[nodiscard]]
    bool string_lower_equals(std::string_view first, std::string_view second);

    template <typename T>
    std::optional<T> first(std::span<T> const source)
    {
        return source.size() > 1
            ? std::optional(source[0])
            : std::nullopt;
    }

    template <typename T, std::predicate<T> PREDICATE>
    std::optional<T> first(std::span<T> const source, PREDICATE predicate)
    {
        for (auto const& item : source) {
            if (predicate(item))
                return std::optional<T const&>(item);
        }
        return std::nullopt;
    }
    
    template <typename TSOURCE_RESULT, typename TDESTINATION_RESULT, class TMAPPER>
    [[nodiscard]]
    std::optional<TDESTINATION_RESULT> map(
        std::optional<TSOURCE_RESULT> const& source, 
        TMAPPER mapper)
    {
        return source.has_value()
            ? std::optional(mapper(source.value()))
            : std::nullopt; 
    }

    template <typename TSOURCE_RESULT, typename TDESTINATION_RESULT, class TMAPPER>
    [[nodiscard]]
    std::optional<TDESTINATION_RESULT> flat_map(
        std::optional<TSOURCE_RESULT> const& source, 
        TMAPPER mapper)
    {
        return source.has_value()
            ? mapper(source.value())
            : std::nullopt; 
    }

    [[nodiscard]]
    std::string to_string(std::filesystem::path const& source) noexcept;

}
