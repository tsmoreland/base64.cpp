//
// Copyright � 2020 Terry Moreland
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

#include "encoder.h"
#include <memory>

import moreland_base64_converters;

#include <algorithm>

using std::move;

namespace  moreland::base64::converters
{
    auto const& get_byte_to_char_mapping()
    {
        static std::vector<byte> values{
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
            'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
            'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
        };
        return values;
    }


    encoder::encoder(bool is_url, optional<vector<byte>> newline, optional<int const> line_max, bool do_padding) noexcept
        : is_url_{is_url}
        , newline_{move(newline)}
        , line_max_{move(line_max)}
        , do_padding_{do_padding}
    {
    }

    vector<byte> encoder::encode(vector<byte> const& source) const
    {
        return vector<byte>();
    }

    encoder::size_type encoder::encode(vector<byte> const& source, vector<byte>& destintation) const
    {
        return size_type();
    }

    std::string encoder::encode_to_string(vector<byte> const& source) const
    {
        return std::string();
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