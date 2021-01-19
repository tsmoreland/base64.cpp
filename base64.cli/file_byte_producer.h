//
// Copyright � 2021 Terry Moreland
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

#include "../base64.converters/byte_producer.h"
#include <optional>
#include <filesystem>
#include <vector>

namespace moreland::base64::cli
{
    // TODO: move this to it's own file
    class file_byte_producer final : public converters::byte_producer
    {
    public:
        [[nodiscard]]
        std::optional<std::vector<unsigned char>> chunk_or_empty() override;

        explicit file_byte_producer(std::filesystem::path const& file_path);
        ~file_byte_producer() override;
        file_byte_producer(file_byte_producer const&) = default;
        file_byte_producer(file_byte_producer&&) noexcept = default;
        file_byte_producer& operator=(file_byte_producer const&) = default;
        file_byte_producer& operator=(file_byte_producer&&) noexcept = default;
    };

}
