//
// Copyright � 2020 Terry Moreland
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
#include "test_data.h"

using std::string_view;
using std::vector;

using moreland::base64::shared::to_string;

namespace moreland::base64::converters::tests
{
    BOOST_FIXTURE_TEST_SUITE(rfc4648_encoder_tests, rfc4648_encoder_fixture)

    BOOST_AUTO_TEST_CASE(encode__returns_vector__when_input_is_non_empty)
    {
        auto const encoded = encoder().encode(get_decoded_bytes());

        BOOST_CHECK(encoded.has_value());
    }

    BOOST_AUTO_TEST_CASE(encode__returns_expected_value__when_input_is_non_empty)
    {
        auto const encoded = encoder().encode(get_decoded_bytes());

        auto const actual = to_string(encoded.value());
        auto const actual_view = string_view(actual);
        auto const expected = ENCODED;

        auto const actual_size = actual_view.size();
        auto const expected_size = expected.size();

        // semi-defy 1 assert per test, but these are just variations of the same check 
        BOOST_CHECK_MESSAGE(actual_size == expected_size, "lengths do not match");
        BOOST_CHECK_MESSAGE(actual_view == expected, "values do not match");
    }

    BOOST_AUTO_TEST_CASE(encode__return_value_matches_destination_size__when_success)
    {
        std::vector<unsigned char> destination{};
        auto const size = encoder().encode(get_decoded_bytes(), destination);

        BOOST_CHECK(size.value_or(0) == destination.size());
    }

    BOOST_AUTO_TEST_CASE(encode_to_string_or_empty__returns_non_empty_string__when_input_is_non_empty)
    {
        auto const encoded = encoder().encode_to_string_or_empty(get_decoded_bytes());

        BOOST_CHECK(!encoded.empty());
    }

    BOOST_AUTO_TEST_CASE(encode_to_string_or_empty__returns_expected_value__when_input_is_non_empty)
    {
        auto const encoded = encoder().encode_to_string_or_empty(get_decoded_bytes());

        BOOST_CHECK_MESSAGE(encoded == ENCODED, "values do not match");
    }

    BOOST_AUTO_TEST_SUITE_END()

}