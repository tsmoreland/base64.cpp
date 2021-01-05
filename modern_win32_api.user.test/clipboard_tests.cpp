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
#include "../modern_win32_api.user/clipboard.h"

using std::string;

namespace modern_win32_api::user::tests
{
    BOOST_AUTO_TEST_CASE(set_clipboard__returns_true__when_have_desktop_access)
    {
        string const data = "sample-text";

        auto const actual = set_clipboard(data, GetDesktopWindow());

        BOOST_TEST(actual);
    }

    BOOST_AUTO_TEST_CASE(get_clipboard__returns_set_value__when_set_clipboard_used)
    {
        string const expected = "sample-text";
        static_cast<void>(set_clipboard(expected));

        auto actual = get_clipboard(GetDesktopWindow()).value_or("");

        BOOST_TEST(actual == expected);
    }

}