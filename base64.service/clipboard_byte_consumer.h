//
// Copyright (c) 2023 Terry Moreland
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
// Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#pragma once

#include "../base64.converters/byte_consumer.h"
#include "../modern_win32_api.user/clipboard_concept.h"
#include <string>

namespace moreland::base64::service {
    template <modern_win32_api::user::Clipboard CLIPBOARD>
    class clipboard_byte_consumer final : public converters::byte_consumer {
        using byte             = unsigned char;
        using byte_string      = std::basic_string<byte>;
        using byte_string_view = std::basic_string_view<byte>;
        byte_string buffer_;

    public:
        [[nodiscard]] bool consume(std::span<byte const> const source) override {
            byte_string_view const source_view(source.data(), source.size());
            buffer_.append(source_view);
            return true;
        }
        void flush() override {
            std::string_view buffer_view{reinterpret_cast<char const*>(buffer_.c_str()), buffer_.size()};
            static_cast<void>(CLIPBOARD::set_clipboard(buffer_view));
        }
        void reset() noexcept override {
            buffer_.clear();
        }
    };

} // namespace moreland::base64::service
