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

#include <span>
#include <string_view>
#include <vector>

#include "../base64.converters/maybe_converted.h"
#include "byte_producer.h"
#include "byte_consumer.h"

namespace moreland::base64::converters
{
    template <typename TCONVERTER>
    concept Converter = requires(TCONVERTER const& converter, std::span<unsigned char const> const source)
    {
        { converter.convert(source) } -> std::convertible_to<maybe_converted<std::vector<unsigned char >>>;
        { converter.convert_to_string_or_empty(source) } -> std::convertible_to<std::string>;
    };

    template <Converter CONVERTER, ByteProducer BYTE_PRODUCER, ByteConsumer BYTE_CONSUMER>
    class converter final
    {
        CONVERTER const& converter_;
        BYTE_PRODUCER& producer_;
        BYTE_CONSUMER& consumer_;
    public:
        constexpr explicit converter(CONVERTER const& converter, BYTE_PRODUCER& producer, BYTE_CONSUMER const& consumer)
            : converter_{converter}
            , producer_{producer}
            , consumer_{consumer}
        {
        }
    };

}