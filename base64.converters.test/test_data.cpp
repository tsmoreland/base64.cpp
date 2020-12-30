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
#include "test_data.h"

using std::ranges::transform;
using std::back_inserter;

namespace moreland::base64::converters::tests
{

    [[nodiscard]]
    std::vector<unsigned char> get_bytes(std::string_view const source)
    {
        std::vector<unsigned char> bytes{};
        transform(source, back_inserter(bytes), 
            [](auto const& value) {
                return static_cast<unsigned char>(value);
            });
        return bytes;
    }

    std::vector<unsigned char> get_decoded_bytes()
    {
        return get_bytes(DECODED);
    }

    std::vector<unsigned char> get_encoded_bytes()
    {
        return get_bytes(ENCODED);
    }
    std::vector<unsigned char> get_encoded_with_line_breaks_bytes()
    {
        return get_bytes(ENCODED_WITH_LINE_BREAKS);
    }
}
