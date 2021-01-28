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

namespace moreland::base64::service
{
    BOOST_FIXTURE_TEST_SUITE(file_byte_producer_tests, file_byte_producer_test_fixture)

    BOOST_AUTO_TEST_CASE(chunk_or_empty__has_value__when_file_is_not_empty)
    {
        auto producer = get_producer();

        auto const chunk = producer.chunk_or_empty();

        BOOST_REQUIRE(chunk.has_value());
    }

    BOOST_AUTO_TEST_SUITE_END()
    
}