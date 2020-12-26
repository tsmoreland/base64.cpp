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

#pragma once

#include "library_export.h"
#include <vector>
#include <optional>
#include <string>
#include <span>

namespace moreland::base64::converters
{
    using byte = unsigned char;
    using size_type = std::vector<byte>::size_type;

#pragma warning(push)
#pragma warning(disable : 4251)
    class BASE64_CONVERTER_EXPORT encoder final 
    {
        bool is_url_;
        std::optional<std::vector<byte>> newline_;
        std::optional<int const> line_max_;
        bool do_padding_;

    public:
        using size_type = std::vector<byte>::size_type;

        explicit encoder(bool const is_url, std::optional<std::vector<byte>> newline, std::optional<int const> line_max, bool const do_padding) noexcept;
        ~encoder() = default;
        encoder(encoder const&) = default;
        encoder(encoder &&) noexcept = default;
        encoder& operator=(encoder const&) = default;
        encoder& operator=(encoder &&) noexcept = default;

        [[nodiscard]]
        std::optional<std::vector<byte>> encode(std::span<byte const> const source) const;

        [[nodiscard]]
        size_type encode(std::span<byte const> const source, std::vector<byte>& destintation) const;

        [[nodiscard]]
        std::string encode_to_string_or_empty(std::span<byte const> const source) const;

    private:
        [[nodiscard]]
        std::optional<size_type> get_output_length(std::span<byte const> const source) const noexcept;

        [[nodiscard]]
        static std::optional<size_type> calculate_output_length(std::span<byte const> const source, bool const insert_line_breaks, size_type const new_line_size);

    };
#pragma warning(pop)

    [[nodiscard]]
    BASE64_CONVERTER_EXPORT encoder build_encoder() noexcept;

    [[nodiscard]]
    BASE64_CONVERTER_EXPORT encoder build_url_encoder() noexcept;

}
