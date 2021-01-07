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

#pragma once

#include <optional>
#include <span>
#include <string_view>
#include <vector>

#include "../base64.converters/maybe_encoded.h"

namespace moreland::base64::converters
{
    template <typename TEncoder>
    concept Encoder = requires(TEncoder const& encoder, std::span<unsigned char const> const source, int x)
    {
        { encoder.encode(source) } -> std::convertible_to<std::optional<std::vector<unsigned char >>>;
        { encoder.encode_to_string_or_empty(source) } -> std::convertible_to<std::string>;
    };

    template <typename TDecoder>
    concept Decoder = requires(TDecoder const& decoder, std::span<unsigned char const> const source)
    {
        { decoder.decode(source) } -> std::convertible_to<converters::maybe_encoded<std::vector<unsigned char >>>;
        { decoder.decode_to_string_or_empty(source) } -> std::convertible_to<std::string>;
    };
}
