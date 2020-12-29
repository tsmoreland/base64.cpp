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

namespace moreland::base64::converters::tests
{

    [[nodiscard]]
    std::span<unsigned char const> get_bytes(std::string_view const source)
    {
        return std::span<unsigned char const>{
            reinterpret_cast<unsigned char const*const>(std::string(source).c_str()),
            source.size()};
    }

    std::span<unsigned char const> get_decoded_bytes()
    {
        return get_bytes(DECODED);
    }

    std::span<unsigned char const> get_encoded_bytes()
    {
        return get_bytes(ENCODED);
    }
    std::span<unsigned char const> get_encoded_with_line_breaks_bytes()
    {
        return get_bytes(ENCODED_WITH_LINE_BREAKS);
    }
}
