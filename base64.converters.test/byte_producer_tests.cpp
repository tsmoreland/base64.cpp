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

#include "pch.h"
#include "byte_producer_test_fixture.h"

namespace moreland::base64::converters::tests
{
    BOOST_FIXTURE_TEST_SUITE(byte_producer_tests, byte_producer_test_fixture)
    
    BOOST_AUTO_TEST_CASE(begin__returns_iterator_with_value)
    {
        auto first = producer().begin();

        BOOST_REQUIRE(first->empty() != true);
    }

    BOOST_AUTO_TEST_CASE(end__returns_iterator_with_empty)
    {
        auto last = producer().end();

        BOOST_REQUIRE_THROW(static_cast<void>(last->empty()), std::out_of_range);
    }
    BOOST_AUTO_TEST_CASE(byte_producer__supports_for_loop)
    {
        auto& items = producer();
        int count{0};
        for (auto const& item : items) {
            static_cast<void>(item);
            count++;
        }

        BOOST_REQUIRE(count == production_count());
    }

    BOOST_AUTO_TEST_SUITE_END()
}
