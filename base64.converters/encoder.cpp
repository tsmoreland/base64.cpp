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

#include "common.h"

#include <memory>
#include <algorithm>

import moreland_base64_converters;
import moreland.base64.shared.optional_functions;

using std::move;
using moreland::base64::shared::map;

namespace moreland::base64::converters
{
    encoder::encoder(bool is_url, optional<vector<byte>> newline, optional<int const> line_max, bool do_padding) noexcept
        : is_url_{is_url}
        , newline_{move(newline)}
        , line_max_{move(line_max)}
        , do_padding_{do_padding}
    {
    }

    optional<vector<byte>> encoder::encode(vector<byte> const& source) const
    {
        auto const output_length = get_output_length(source);
        if (!output_length.value_or(0UL) == 0UL) {
            return nullopt;
        }

        return nullopt;
    }

    encoder::size_type encoder::encode(vector<byte> const& source, vector<byte>& destintation) const
    {
        auto const output_length = get_output_length(source);
        if (!output_length.value_or(0UL) == 0UL) {
            return 0UL;
        }

        return size_type();
    }

    std::string encoder::encode_to_string_or_empty(vector<byte> const& source) const
    {
        auto const output_length = get_output_length(source);
        if (!output_length.value_or(0UL) == 0UL) {
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
    
    encoder const& get_encoder() noexcept
    {
        static encoder rfc4648{false, nullopt, nullopt, true};
        return rfc4648;
    }
    encoder const& get_url_encoder() noexcept
    {
        static encoder rfc4648_url_safe{true, nullopt, nullopt, true};
        return rfc4648_url_safe;
    }
}