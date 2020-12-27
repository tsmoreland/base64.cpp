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
#include "../base64.shared/convert.h"

using std::move;
using std::optional;
using std::vector;
using std::span;
using std::nullopt;

using moreland::base64::shared::map;
using moreland::base64::shared::to_byte;

namespace moreland::base64::converters
{
    template <size_type position>
    [[nodiscard]]
    size_type get_output_index(span<byte const> const source, size_type const input_position);

    encoder::encoder(bool const is_url, bool const insert_line_break, optional<int> const line_max, bool const do_padding) noexcept
        : is_url_{is_url}
        , insert_line_break_{insert_line_break}
        , line_max_{line_max}
        , do_padding_{do_padding}
    {
    }

    optional<vector<byte>> encoder::encode(span<byte const> const source) const
    {
        auto const output_length = get_output_length(source);
        if (output_length.value_or(0UL) == 0UL) {
            return nullopt;
        }
        vector<byte> destination;
        return encode(source, destination).has_value()
            ? optional(destination)
            : nullopt;
    }


    std::optional<encoder::size_type> encoder::encode(span<byte const> const source, vector<byte>& destination) const
    {
        auto const output_length = get_output_length(source);
        if (output_length.value_or(0UL) == 0UL) {
            return nullopt;
        }

        destination.reserve(output_length.value());

        auto const length = source.size();
        auto const length_mod_3 = length % 3;
        auto const calc_length = length - length_mod_3;
        auto const line_break_position = get_base64_line_break_position();
        size_type character_count = 0;

        size_type input_position;
        size_type output_position = 0;

        auto const base64_table = get_base64_table();
        
        for (input_position = 0; input_position < calc_length; input_position += 3) {
            if (insert_line_break_) {
                if (character_count == line_break_position) {
                    destination[output_position++] = static_cast<byte>('\r');
                    destination[output_position++] = static_cast<byte>('\n');
                    character_count = 0UL;
                }
                character_count += 4;
            } 

            destination[output_position] = base64_table[get_output_index<0>(source, input_position)];
            destination[output_position+1] = base64_table[get_output_index<1>(source, input_position)];
            destination[output_position+2] = base64_table[get_output_index<2>(source, input_position)];
            destination[output_position+3] = base64_table[get_output_index<3>(source, input_position)];
            output_position += 4;
        }
        input_position = calc_length;
        if (insert_line_break_ && length_mod_3 !=0 && character_count == line_break_position) {
            destination[output_position++] = '\r';
            destination[output_position++] = '\n';
        }

        switch(length_mod_3)
        {
        case 2: 
            destination[output_position] = base64_table[get_output_index<0>(source, input_position)];
            destination[output_position+1] = base64_table[get_output_index<1>(source, input_position)];
            destination[output_position+2] = base64_table[get_output_index<2>(source, input_position)];
            destination[output_position+3] = base64_table[64]; //Pad
            output_position += 4;
            break;
        case 1: // Two character padding needed
            destination[output_position] = base64_table[get_output_index<0>(source, input_position)];
            destination[output_position+1] = base64_table[get_output_index<1>(source, input_position)];
            destination[output_position+2] = base64_table[64]; 
            destination[output_position+3] = base64_table[64]; 
            output_position += 4;
            break;
        default:
            break;
        }

        return optional(output_position);
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
        return calculate_output_length(source, insert_line_break_);
    }

    optional<size_type> encoder::calculate_output_length(span<byte const> const source, bool const insert_line_breaks) 
    {
        size_type const new_line_size = 2;
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
        encoder rfc4648{false, false, nullopt, true};  // NOLINT(clang-diagnostic-exit-time-destructors)
        return rfc4648;
    }
    encoder make_url_encoder() noexcept  // NOLINT(clang-diagnostic-exit-time-destructors)
    {
        encoder rfc4648_url_safe{true, false, nullopt, true};  // NOLINT(clang-diagnostic-exit-time-destructors)
        return rfc4648_url_safe;
    }

    template <size_type position>
    [[nodiscard]]
    size_type get_output_index(span<byte const> const source, size_type const input_position)
    {
        size_type index;

        static_assert(position >= 0 && position < 4, "position must be in range of [0:4)");

        switch (position)
        {
        case 0:
            index = static_cast<size_type>((source[input_position]&0xfc) >> 2);
            break;
        case 1: {
            size_type const input_position_plus_1 = input_position + 1;
            index = static_cast<size_type>((source[input_position] & to_byte(0x03))<<4 | (source[input_position_plus_1]&to_byte(0xf0))>>4);
            break;
        }
        case 2: {
            size_type const input_position_plus_1 = input_position + 1;
            size_type const input_position_plus_2 = input_position + 2;
            index = static_cast<size_type>((source[input_position_plus_1]&to_byte(0x0f))<<2 | ((source[input_position_plus_2]&to_byte(0xc0))>>6));
            break;
        }
        case 3: {
            size_type const input_position_plus_2 = input_position + 2;
            index = static_cast<size_type>(source[input_position_plus_2]&to_byte(0x3f));
            break;
        }
        default:
            throw std::out_of_range("unreachable code");
        }
        return index;
    }

}