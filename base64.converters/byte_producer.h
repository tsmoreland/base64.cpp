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

#include <iterator>
#include <optional>
#include <vector>
#include <filesystem>

namespace moreland::base64::converters
{
    template <typename TPRODUCER>
    concept ByteProducer = requires(TPRODUCER producer, std::string_view const argument)
    {
        //std::forward_iterator<typename TPRODUCER::iterator>;
        { producer.chunk_or_empty() } -> std::same_as<std::optional<std::vector<unsigned char>>>;
    };

    template <typename T>
    concept ConstructedFromFile = requires(std::filesystem::path const& file_path)
    {
        T(file_path);
    };

    class byte_producer
    {
    public:
        [[nodiscard]]
        virtual std::optional<std::vector<unsigned char>> chunk_or_empty() = 0;

        explicit byte_producer() = default;
        virtual ~byte_producer() = default;
        byte_producer(byte_producer const&) = default;
        byte_producer(byte_producer&&) noexcept = default;
        byte_producer& operator=(byte_producer const&) = default;
        byte_producer& operator=(byte_producer&&) noexcept = default;

        struct iterator
        {

        };
    };


}
