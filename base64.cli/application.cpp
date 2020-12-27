//
// Copyright © 2020 Terry Moreland
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
#include "../base64.shared/seh_exception.h"
#include "../../base64.converters/encoder.h"
#include "../../base64.shared/convert.h"

using moreland::base64::shared::seh_exception;
using moreland::base64::converters::make_encoder;

void force_exception()
{
    int x = 9;
    int const y = 0;
    x = x / y;  // NOLINT(clang-diagnostic-division-by-zero) -- intended
}
void force_exception_in_thread()
{
    std::thread exception_in_thread([]() -> void
    {
        seh_exception::initialize();
        try {
            force_exception();
        } catch (std::exception const& e) {
            std::cout << e.what() << std::endl;
        }
    });
    exception_in_thread.join();
}

using std::vector;
using byte_string = std::basic_string<unsigned char>;

namespace shared = moreland::base64::shared;  // NOLINT(misc-unused-alias-decls)

int main()
{
    try {
        seh_exception::initialize();

        auto const encoder = make_encoder();

        std::string source_string = "hello world";
        byte_string source{begin(source_string), end(source_string)};

        auto const encoded = encoder.encode(source);
        if (!encoded.has_value())
            return 1;

        auto const encoded_string = shared::to_string(encoded.value());
        char const* str = encoded_string.c_str();

        force_exception_in_thread();
        force_exception();


    } catch (std::exception const& e) {
        std::cout << e.what() << std::endl;

    } catch (...) {
        std::cout << "unknown error occurred" << std::endl;
    }

    return 0;
}

