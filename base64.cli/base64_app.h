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
#include <string>
#include <string_view>
#include <optional>
#include <vector>

#include "../base64.converters/decoder.h"
#include "../base64.converters/encoder.h"
#include "../base64.converters/maybe_encoded.h"
#include "../modern_win32_api.user/clipboard.h"

namespace moreland::base64::cli
{
    using byte_string = std::basic_string<unsigned char>;

    using converters::encoder;
    using converters::decoder;

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

    template <typename TClipboard>
    concept Clipboard = requires(std::string_view const data)
    {
        { TClipboard::get_clipboard() } -> std::convertible_to<std::optional<std::string>>;
        { TClipboard::set_clipboard(data) } -> std::convertible_to<bool>;
    };

    enum class operation_type
    {
        unknown,
        decode,
        encode,
    };

    enum class output_type
    {
        unknown,
        clipboard,
        file_to_clipboard,
        file_to_file,
    };

    struct clipboard_traits
    {
        [[nodiscard]]
        static std::optional<std::string> get_clipboard() 
        {
            return modern_win32_api::user::get_clipboard();
        }

        [[nodiscard]]
        static bool set_clipboard(std::string_view const data)
        {
            return modern_win32_api::user::set_clipboard(data);
        }
    };


    [[nodiscard]]
    bool string_lower_equals(std::string_view const first, std::string_view const second);

    [[nodiscard]]
    std::tuple<operation_type, output_type> get_operation_and_output_from_arguments(std::span<std::string_view const> const arguments);

    [[nodiscard]]
    std::vector<std::string_view> as_vector_of_views(char const* source[], std::size_t const length);

    template <Encoder ENCODER = encoder, Decoder DECODER = decoder, Clipboard CLIPBOARD = clipboard_traits>
    [[nodiscard]]
    bool base64_run(std::span<std::string_view const> const arguments, ENCODER const& encoder, DECODER const& decoder)
    {
        auto [operation, output] = get_operation_and_output_from_arguments(arguments);

        if (operation == operation_type::unknown) {
            return false;
        }

        switch (output)
        {
        case output_type::clipboard: {
            auto const source = CLIPBOARD::get_clipboard();
            if (!source.has_value())
                return false;
            
            byte_string const byte_source{begin(source.value()), end(source.value())};

            auto const converted = operation == operation_type::encode
                ? encoder.encode_to_string_or_empty(byte_source)
                : decoder.decode_to_string_or_empty(byte_source);
            
            if (converted.empty())
                return byte_source.empty(); 

            break;
        }
        case output_type::file_to_clipboard:
            break;
        case output_type::file_to_file:
            break;
        default:
            return false;
        }

        return false;
    }


}

