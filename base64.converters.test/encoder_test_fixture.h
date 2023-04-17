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

#include "../base64.converters/encoder.h"

namespace moreland::base64::converters::tests
{
    template <bool is_url, bool insert_line_break, int line_max, bool do_padding>
    class encoder_test_fixture 
    {
        encoder encoder_{is_url, insert_line_break, line_max >= 0 ? std::optional(line_max) : std::nullopt, do_padding};


    public:
        constexpr encoder_test_fixture() = default;
        virtual ~encoder_test_fixture() = default;

        [[nodiscard]]
        constexpr encoder const& encoder() const&
        {
            return encoder_;
        }

        encoder_test_fixture(encoder_test_fixture const&) = default;
        encoder_test_fixture(encoder_test_fixture &&) noexcept = default;
        encoder_test_fixture& operator=(encoder_test_fixture const&) = default;
        encoder_test_fixture& operator=(encoder_test_fixture &&) noexcept = default;
    };

    using rfc4648_encoder_fixture = encoder_test_fixture<false, false, -1, false>;
    using url_rfc4648_encoder_fixture = encoder_test_fixture<true, false, -1, false>;
    using rfc2045_encoder_fixture = encoder_test_fixture<false, true, -1, false>;
    using url_rfc2045_encoder_fixture = encoder_test_fixture<true, true, -1, false>;

}
