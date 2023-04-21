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

#include "../base64.service/file_byte_producer.h"

namespace moreland::base64::service {

    class file_byte_producer_test_fixture {
        char const* const test_data_filename_ = "sample.txt";
        std::basic_string<unsigned char> test_data_content_;

    public:
        using byte_string = std::basic_string<unsigned char>;

        [[nodiscard]] file_byte_producer get_producer() const;
        [[nodiscard]] byte_string const& get_expected() const noexcept;

        explicit file_byte_producer_test_fixture();
        virtual ~file_byte_producer_test_fixture()                                             = default;
        file_byte_producer_test_fixture(file_byte_producer_test_fixture const&)                = delete;
        file_byte_producer_test_fixture(file_byte_producer_test_fixture&&) noexcept            = delete;
        file_byte_producer_test_fixture& operator=(file_byte_producer_test_fixture const&)     = delete;
        file_byte_producer_test_fixture& operator=(file_byte_producer_test_fixture&&) noexcept = delete;

    protected:
        using byte                 = unsigned char;
        using byte_vector          = std::vector<byte>;
        using optional_byte_vector = std::optional<byte_vector>;
    };

} // namespace moreland::base64::service
