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

#include <optional>
#include <string>

namespace moreland::base64::cli::tests
{
    class mock_clipboard_traits final
    {
        static std::optional<std::string> get_result_;
        static bool set_result_;
        static std::string_view set_data_;

    public:
        static auto set_get_clipboard_return_value(std::optional<std::string> const& data) -> void;
        static auto set_set_clipboard_return_value(bool result) -> void;

        static auto get_clipboard() -> std::optional<std::string>;
        static auto set_clipboard(std::string_view const data) -> bool;
        static auto get_set_clipboard_argument() -> std::string_view;
    };
}

