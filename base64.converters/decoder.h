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

#include "common.h"
#include "library_export.h"
#include "maybe_converted.h"
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace moreland::base64::converters {

#pragma warning(push)
#pragma warning(disable : 4251)
    class BASE64_CONVERTER_EXPORT decoder final {
        bool is_url_;
        bool insert_line_break_;
        std::optional<int> line_max_;
        bool do_padding_;

    public:
        explicit decoder(bool const is_url, bool const insert_line_break, std::optional<int> const line_max,
            bool const do_padding) noexcept;

        [[nodiscard]] maybe_converted<std::vector<byte>> convert(std::span<byte const> const source) const;

        [[nodiscard]] maybe_converted<std::size_t> convert(
            std::span<byte const> const source, std::vector<byte>& destination) const;

        [[nodiscard]] std::string convert_to_string_or_empty(std::span<byte const> const source) const;

        [[nodiscard]] std::string convert_to_string_or_empty(std::span<char const> const source) const;

        ~decoder()                             = default;
        decoder(decoder const&)                = default;
        decoder(decoder&&) noexcept            = default;
        decoder& operator=(decoder const&)     = default;
        decoder& operator=(decoder&&) noexcept = default;

    private:
        [[nodiscard]] static maybe_converted<std::size_t> calculate_output_length(std::span<byte const> const source);
    };
#pragma warning(pop)

    [[nodiscard]] BASE64_CONVERTER_EXPORT decoder make_decoder() noexcept;

    [[nodiscard]] BASE64_CONVERTER_EXPORT decoder make_url_decoder() noexcept;

    class decoder_factory final {
    public:
        [[nodiscard]] decoder operator()() const {
            return make_decoder();
        }
    };

} // namespace moreland::base64::converters
