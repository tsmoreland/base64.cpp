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

using byte_string_view = std::basic_string_view<unsigned  char>;

namespace moreland::base64::service
{
    BOOST_FIXTURE_TEST_SUITE(file_byte_producer_tests, file_byte_producer_test_fixture)

    BOOST_AUTO_TEST_CASE(chunk_or_empty__has_value__when_file_is_not_empty)
    {
        auto producer = get_producer();

        auto const chunk = producer.chunk_or_empty();

        BOOST_REQUIRE(chunk.has_value());
    }

    BOOST_AUTO_TEST_CASE(chunk_or_empty__returns_expected_value__when_file_is_not_empty)
    {
        auto producer = get_producer();

        auto const chunk = producer
            .chunk_or_empty()
            .value_or(byte_vector());

        BOOST_CHECK(!chunk.empty() && chunk.size() <= get_expected().size());

        auto expected_view = byte_string_view(&get_expected()[0], chunk.size());
        BOOST_REQUIRE(std::ranges::equal(expected_view, chunk));
    }

    BOOST_AUTO_TEST_CASE(chunk_or_empty__returns_all_file_contents__when_called_until_returns_nullopt)
    {
        auto producer = get_producer();

        byte_vector complete_file{};
        complete_file.reserve(get_expected().size());

        for (auto const& chunk : producer) {
            complete_file.insert(end(complete_file), begin(chunk), end(chunk));
        }

        auto const size = get_expected().size();
        auto const expected_view = byte_string_view(&get_expected()[0], size);
        BOOST_REQUIRE(std::ranges::equal(expected_view, complete_file));
    }

    BOOST_AUTO_TEST_SUITE_END()
    
}