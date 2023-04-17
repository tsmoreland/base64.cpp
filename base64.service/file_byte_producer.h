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

#include "../base64.converters/byte_producer.h"
#include <format>
#include <fstream>
#include <mutex>
#include <optional>
#include <span>
#include <vector>

namespace moreland::base64::service
{
    class file_byte_producer final : public converters::byte_producer
    {
        const std::size_t BUFFER_SIZE = 16384;
        using file_byte_stream = std::basic_fstream<unsigned char, std::char_traits<unsigned char>>;
        using lock_guard = std::scoped_lock<std::mutex>;
        using bytes_view = std::span<unsigned char>;

        file_byte_stream source_;
        std::unique_ptr<unsigned char[]> const buffer_;
        std::mutex read_lock_;
    public:
        using byte_vector = std::vector<unsigned char>;
        using optional_byte_vector = std::optional<byte_vector>;

        [[nodiscard]]
        optional_byte_vector chunk_or_empty() override
        {
            lock_guard guard{read_lock_};
            source_.read(buffer_.get(), static_cast<std::streamsize>(BUFFER_SIZE));
            auto const count = source_
                ? BUFFER_SIZE
                : static_cast<std::size_t>(source_.gcount());

            if (count == 0) {
                return std::nullopt;
            }

            bytes_view buffer_view{&buffer_[0], count};
            return count > 0
                ? optional_byte_vector(byte_vector{std::begin(buffer_view), std::end(buffer_view)})
                : std::nullopt;
        }

        explicit file_byte_producer(std::filesystem::path const& file_path)
            : source_{file_path, std::ios::in}
            , buffer_{std::make_unique<unsigned char[]>(BUFFER_SIZE)}
        {
            static_assert(converters::ConstructedFromFile<file_byte_producer>);
            if (!source_.is_open()) {
                throw std::invalid_argument(std::format("unable to read {}", std_extensions::to_string(file_path)));
            }
        }
    };

}
