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

#include "library_export.h"
#include "common.h"

import moreland.base64.converters;

#include <vector>
#include <optional>
#include <string>
#include <span>

#include <memory>
#include <algorithm>
#include <limits>


using std::move;

#pragma warning(push)
#pragma warning(disable : 4251)

namespace moreland::base64::converters
{
     encoder::encoder(bool const is_url, bool insert_line_breaks, optional<int const> line_max, bool const do_padding) noexcept
        : is_url_{is_url}
        , insert_line_breaks_{insert_line_breaks}
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

    bool encoder::validate_encode_source(span<byte const> const source) const noexcept
    {
        if (source.size() == 0)
            return false;


        return false;
    }

    optional<size_type> encoder::get_output_length(span<byte const> const source) const noexcept
    {
        return calculate_output_length(source, insert_line_breaks_, 2UL);
    }
    optional<size_type> encoder::calculate_output_length(span<byte const> const source, bool const insert_line_breaks, size_type const new_line_size) 
    {
        size_type const ONE = 1;

        auto size = source.size() / 3 * 4;       
        size += ((source.size() % 3) != 0)
            ? 4
            : 0;

        if (size == 0)
            return 0;
        if (insert_line_breaks) {
            auto new_line_count = size / get_base64_line_break_position();
            if (size % new_line_count == 0) {
                --new_line_count;
            }
            size += new_line_count * new_line_size;
        }

        if (size > ONE * std::numeric_limits<int>::max()) {
            return nullopt;
        }

        return optional(size);
    }

    [[nodiscard]]
    encoder make_encoder() noexcept
    {
        encoder rfc4648{false, false, nullopt, true};
        return rfc4648;
    }

    [[nodiscard]]
    encoder make_url_encoder() noexcept
    {
        encoder rfc4648{true, false, nullopt, true};
        return rfc4648;
    }


}

#pragma warning(pop)