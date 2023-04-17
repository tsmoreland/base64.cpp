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
#include <mutex>
#include <optional>
#include <vector>

namespace moreland::base64::converters::tests
{

    class byte_producer_test_fixture 
    {
        using byte = unsigned char;
        using byte_vector = std::vector<byte>;
        using optional_byte_vector = std::optional<byte_vector>;

        const int production_count_{5};
    public:

        explicit byte_producer_test_fixture() = default;
        virtual ~byte_producer_test_fixture() = default;

        [[nodiscard]]
        constexpr int production_count() const noexcept
        {
            return production_count_;
        }

        class simple_byte_producer final : public byte_producer
        {
            int limit_;
            std::atomic<int> count_{0};
        public:
            explicit simple_byte_producer(int limit);
            optional_byte_vector chunk_or_empty() override;
        };

        [[nodiscard]]
        constexpr simple_byte_producer& producer() noexcept
        {
            return producer_;
        }
        [[nodiscard]]
        constexpr simple_byte_producer const& producer() const noexcept
        {
            return producer_;
        }

        byte_producer_test_fixture(byte_producer_test_fixture const& ) = delete;
        byte_producer_test_fixture(byte_producer_test_fixture&&) noexcept = delete;
        byte_producer_test_fixture& operator=(byte_producer_test_fixture const& ) = delete;
        byte_producer_test_fixture& operator=(byte_producer_test_fixture&&) noexcept = delete;
    private:
        simple_byte_producer producer_{production_count_};
    };

}
