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
#include "base64_app.h"
#include "../base64.shared/seh_exception.h"
#include "../../base64.converters/encoder.h"
#include "../../base64.converters/decoder.h"

using moreland::base64::shared::seh_exception;
using moreland::base64::converters::make_encoder;
using moreland::base64::converters::make_decoder;
using moreland::base64::converters::encoder;
using moreland::base64::converters::decoder;

using std::vector;
using byte_string = std::basic_string<unsigned char>;

using moreland::base64::cli::as_vector_of_views;
using moreland::base64::cli::base64_run;

namespace shared = moreland::base64::shared;  // NOLINT(misc-unused-alias-decls)

int main(int argc, char const* argv[])
{
    try {
        seh_exception::initialize();

        auto const base64_encoder = make_encoder();
        auto const base64_decoder = make_decoder();

        ++argv;
        if (--argc < 0)
            return -1;

        auto const arguments = as_vector_of_views(argv, argc);
        if (auto const result = base64_run(arguments, base64_encoder, base64_decoder); !result) {
            std::cout << "operation failed" << std::endl;
        }

        return 0;

    } catch (std::exception const& e) {
        std::cout << e.what() << std::endl;
        return -1;

    } catch (...) {
        std::cout << "unknown error occurred" << std::endl;
        return -1;
    }

}

