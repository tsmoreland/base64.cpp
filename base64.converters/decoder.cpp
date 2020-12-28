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
#include "decoder.h"

#include "../base64.shared/optional_functions.h"
#include "../base64.shared/convert.h"

using std::move;
using std::nullopt;
using std::optional;
using std::size_t;
using std::span;
using std::string;
using std::vector;

using moreland::base64::shared::map;
using moreland::base64::shared::to_byte;
using moreland::base64::shared::to_string;

namespace moreland::base64::converters
{
    decoder::decoder(bool const is_url, bool const insert_line_break, std::optional<int> const line_max, bool const do_padding) noexcept
        : is_url_{is_url}
        , insert_line_break_{insert_line_break}
        , line_max_{line_max}
        , do_padding_{do_padding}
    {
    }

    optional<vector<byte>> decoder::decode(span<byte const> const source) const
    {
        vector<byte> destination;
        return map<size_t, vector<byte>>(decode(source, destination), 
            [&destination](auto const&) {
                return destination;
            });
    }

    optional<size_t> decoder::decode(span<byte const> const source, vector<byte>& destination) const
    {


        return nullopt;
    }

    string decoder::decode_to_string_or_empty(span<byte const> const source) const
    {
        vector<byte> destination;
        return map<size_t, string>(decode(source, destination), 
            [&destination](auto const&) {
                span<byte const> const destination_view = destination;
                return to_string(destination_view);
            })
            .value_or("");
    }
    string decoder::decode_to_string_or_empty(span<char const> const source) const
    {
        vector<byte> source_bytes(begin(source), end(source));
        return decode_to_string_or_empty(source_bytes);
    }

    size_t decoder::calculate_output_length(span<byte const> const source)
    {
        span<byte const> trimmed_source = get_trimmed_span(source);



        return std::size_t();
    }

}
