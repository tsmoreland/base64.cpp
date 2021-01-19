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

#include "../base64.converters/byte_producer.h"
#include "../base64.shared/std_extensions.h"
#include "../modern_win32_api.user/clipboard_concept.h"
#include "../modern_win32_api.user/clipboard_traits.h"

namespace moreland::base64::cli
{
    template <modern_win32_api::user::Clipboard CLIPBOARD = modern_win32_api::user::clipboard_traits>
    class clipboard_byte_producer final : public converters::byte_producer
    {
    public:
        explicit clipboard_byte_producer() = default;

        [[nodiscard]]
        std::optional<std::vector<unsigned char>> chunk_or_empty() override
        {
            return std_extensions::map<std::string, std::vector<unsigned char>>(CLIPBOARD::get_clipboard(), 
                [](auto const& original)  {
                    return std::vector<unsigned char>(begin(original), end(original));
                });
        }
    };

}
