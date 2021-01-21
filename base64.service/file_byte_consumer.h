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

#include <filesystem>
#include "../base64.converters/byte_consumer.h"

namespace moreland::base64::service
{
    class file_byte_consumer final : public converters::byte_consumer
    {
        using file_byte_stream = std::basic_fstream<unsigned char, std::char_traits<unsigned char>>;

        file_byte_stream destination_;
        std::unique_ptr<unsigned char[]> const buffer_;
        std::mutex read_lock_;
    public:

        [[nodiscard]]
        bool consume(std::span<unsigned char const> const source) override;
        void flush() override;
        void reset() override;

        explicit file_byte_consumer(std::filesystem::path const& file_path);
    };
    
}
