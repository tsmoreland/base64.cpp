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

#include <fstream>
#include <span>
#include <string>
#include <vector>

#include "output_type.h"
#include "operation_type.h"

#include "../base64.converters/concepts.h"
#include "../base64.converters/decoder.h"
#include "../base64.converters/encoder.h"
#include "../modern_win32_api.user/clipboard_concept.h"
#include "../modern_win32_api.user/clipboard_traits.h"

namespace moreland::base64::cli
{

    [[nodiscard]]
    bool string_lower_equals(std::string_view first, std::string_view second);

    [[nodiscard]]
    std::tuple<operation_type, output_type> get_operation_and_output_from_arguments(std::span<std::string_view const> arguments);

    [[nodiscard]]
    std::optional<std::string> get_input_filename(std::span<std::string_view const> arguments);

    [[nodiscard]]
    std::optional<std::tuple<std::string, std::string>> get_filenames(std::span<std::string_view const> arguments);

    [[nodiscard]]
    std::vector<std::string_view> as_vector_of_views(char const* source[], std::size_t length);

    template <
        converters::Encoder ENCODER = converters::encoder,
        converters::Decoder DECODER = converters::decoder,
        modern_win32_api::user::Clipboard CLIPBOARD = modern_win32_api::user::clipboard_traits
    >
    [[nodiscard]]
    bool base64_run(std::span<std::string_view const> const arguments, ENCODER const& encoder, DECODER const& decoder)
    {
        using byte = unsigned char;
        using byte_string = std::basic_string<byte>;
        using file_byte_stream = std::basic_fstream<byte, std::char_traits<byte>>;
        auto [operation, output_type] = get_operation_and_output_from_arguments(arguments);

        if (operation == operation_type::unknown) {
            return false;
        }

        switch (output_type)
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

            return CLIPBOARD::set_clipboard(converted);
        }
        case output_type::file_to_clipboard: {
            auto filename = get_input_filename(arguments.subspan(1, arguments.size() - 2));
            if (!filename.has_value())
                return false;
            auto const& input_filename = filename.value();

            byte buffer[4096]{};
            if (std::fstream input{input_filename, std::ios::in}; input.is_open()) {
                input.read(reinterpret_cast<char *>(buffer), 4096);
                auto count = input
                    ? static_cast<std::size_t>(4096)
                    : static_cast<std::size_t>(input.gcount());

                auto const converted = operation == operation_type::encode
                    ? encoder.encode_to_string_or_empty(std::span<byte const>{buffer, count})
                    : decoder.decode_to_string_or_empty(std::span<byte const>{buffer, count});
                input.close();

                return CLIPBOARD::set_clipboard(converted);
            }

            break;
        }
        case output_type::file_to_file: {
            auto filenames = get_filenames(arguments.subspan(1, arguments.size() - 2));
            if (filenames.has_value())
                return false;
            auto const& [input_filename, output_filename] = filenames.value();

            unsigned char buffer[4096]{};
            if (file_byte_stream input{input_filename, std::ios::in}; input.is_open()) {
                input.read(buffer, 4096);

                auto count = input
                    ? static_cast<std::size_t>(4096)
                    : static_cast<std::size_t>(input.gcount());

                if (file_byte_stream output{output_filename, std::ios::out}; output.is_open()) {
                    // TODO: move encode to use maybe_encoded so we can check has_value and log error rather than using value_or
                    auto const converted = operation == operation_type::encode
                        ? encoder.encode(std::span<unsigned char const>{buffer, count}).value_or(std::vector<byte>{})
                        : decoder.decode(std::span<unsigned char const>{buffer, count}).value_or(std::vector<byte>{});

                    if (converted.empty()) {
                        input.close();
                        output.close();
                        return false;
                    }

                    output.write(&converted[0], converted.size());
                    output.close();
                }

                input.close();
            }

            return true;
        }
        default:
            return false;
        }

        return false;
    }


}

