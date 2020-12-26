//
// Copyright © 2020 Terry Moreland
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
#include "common.h"
#include "encoder.h"

#include "../base64.shared/optional_functions.h"

using std::move;
using std::optional;
using std::vector;
using std::span;
using std::nullopt;

using moreland::base64::shared::map;

namespace moreland::base64::converters
{
    encoder::encoder(bool const is_url, optional<vector<byte>> newline, optional<int const> line_max, bool const do_padding) noexcept
        : is_url_{is_url}
        , newline_{move(newline)}
        , line_max_{move(line_max)}
        , do_padding_{do_padding}
    {
    }

    optional<vector<byte>> encoder::encode(span<byte const> const source) const
    {
        auto const output_length = get_output_length(source);
        if (output_length.value_or(0UL) == 0UL) {
            return nullopt;
        }

        return nullopt;
    }

    encoder::size_type encoder::encode(span<byte const> const source, vector<byte>& destintation) const
    {
        auto const output_length = get_output_length(source);
        if (output_length.value_or(0UL) == 0UL) {
            return 0UL;
        }

        return size_type();
    }

    std::string encoder::encode_to_string_or_empty(span<byte const> const source) const
    {
        auto const output_length = get_output_length(source);
        if (output_length.value_or(0UL) == 0UL) {
            return "";
        }

        return std::string();
    }

    optional<size_type> encoder::get_output_length(span<byte const> const source) const noexcept
    {
        return calculate_output_length(
            source, 
            newline_.has_value(),
            map<vector<byte>::size_type, vector<byte>>(newline_, 
                [](auto const& container) {
                    return container.size();
                }).value_or(0UL));
    }

    optional<size_type> encoder::calculate_output_length(span<byte const> const source, bool const insert_line_breaks, size_type const new_line_size) 
    {
        size_type const ONE = 1;
        auto size = source.size() / 3 * 4;       
        if (size == 0)
            return 0;
        if (insert_line_breaks) {
            auto new_line_count = size / get_base64_line_break_position();
            if (size % new_line_count == 0) {
                --new_line_count;
            }
            size += new_line_count * new_line_size;
        }

        if (size > static_cast<size_type>(numeric_limits::maximum<int>())) {
            return nullopt;
        }

        return optional(size);
    }
    
    encoder make_encoder() noexcept
    {
        encoder rfc4648{false, nullopt, nullopt, true};  // NOLINT(clang-diagnostic-exit-time-destructors)
        return rfc4648;
    }
    encoder make_url_encoder() noexcept  // NOLINT(clang-diagnostic-exit-time-destructors)
    {
        encoder rfc4648_url_safe{true, nullopt, nullopt, true};  // NOLINT(clang-diagnostic-exit-time-destructors)
        return rfc4648_url_safe;
    }
}