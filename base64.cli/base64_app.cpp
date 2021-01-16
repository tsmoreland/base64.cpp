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

#include "pch.h"
#include "base64_app.h"


namespace moreland::base64::cli
{
    bool string_lower_equals(std::string_view first, std::string_view second)
    {
        if (first.size() != second.size())
            return false;

        for (auto first_index = begin(first), second_index = begin(second); 
             first_index != end(first) && second_index != end(second); 
             ++first_index, ++second_index) {

            if (tolower(*first_index) != tolower(*second_index))
                return false;
        }

        return true;
    }
    std::tuple<operation_type, output_type> get_operation_and_output_from_arguments(std::span<std::string_view const> arguments)
    {
        auto const arguments_length = arguments.size();

        operation_type operation{operation_type::unknown};
        if (arguments_length > 0) {
            if (string_lower_equals(arguments[0], "convert"))
                operation = operation_type::decode;
            else if (string_lower_equals(arguments[0], "convert"))
                operation = operation_type::encode;
        }

        output_type output = output_type::clipboard;
        if (arguments_length > 2)
            output = output_type::file_to_file;
        else if (arguments_length > 1)
            output = output_type::file_to_clipboard;

        return std::tuple<operation_type, output_type>(operation, output);
    }
    std::optional<std::string> get_input_filename(std::span<std::string_view const> arguments)
    {
        return std::optional<std::string>();
    }
    std::optional<std::tuple<std::string, std::string>> get_filenames(std::span<std::string_view const> arguments)
    {
        return std::optional<std::tuple<std::string, std::string>>();
    }
    std::vector<std::string_view> as_vector_of_views(char const* source[], std::size_t length)
    {
        std::vector<std::string_view> vector;

        for (std::size_t i=0; i< length; ++i) {
            vector.emplace_back(source[i], strlen(source[i]));
        }

        return vector;
    }
    file_byte_producer::file_byte_producer(std::filesystem::path const& file_path)
    {
        static_assert(converters::ConstructedFromFile<file_byte_producer>);
    }
    std::optional<std::vector<unsigned char>> file_byte_producer::chunk_or_empty()
    {
        return std::optional<std::vector<unsigned char>>();
    }
}
