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
#include "../base64.service/file_byte_producer.h"
#include "file_byte_producer_test_fixture.h"
#include <fstream>

namespace moreland::base64::service
{
    file_byte_producer file_byte_producer_test_fixture::get_producer() const
    {
        return file_byte_producer{test_data_filename_};
    }
    file_byte_producer_test_fixture::byte_string const& file_byte_producer_test_fixture::get_expected() const noexcept
    {
        return test_data_content_;
    }
    file_byte_producer_test_fixture::file_byte_producer_test_fixture()
    {
        using std::fstream;
        using file_byte_stream = std::basic_fstream<unsigned char, std::char_traits<unsigned char>>;
        using input_file_byte_stream = std::basic_ifstream<unsigned char, std::char_traits<unsigned char>>;
        using byte_string_view = std::basic_string_view<unsigned char>;

        input_file_byte_stream source{test_data_filename_};

        BOOST_CHECK(source.is_open());

        const int read_size = 16384;
        auto const buffer = std::make_unique<unsigned char[]>(read_size);

        while (!source.bad() && !source.eof()) {
            source.read(buffer.get(), read_size); 
            if (source.gcount() == 0)
                break;
            byte_string_view buffer_view{buffer.get(), static_cast<byte_string_view::size_type>(source.gcount())};
            test_data_content_.append(buffer_view);
        }
    }
}
