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
#include "arguments_view.h"
#include "../base64.service/convert.h"
#include "../base64.shared/seh_exception.h"

using moreland::base64::shared::seh_exception;

using moreland::base64::cli::get_arguments_as_vector;
using moreland::base64::service::convert;

using args_vector = std::vector<std::string_view>;
using args_view = std::span<std::string_view const>;

int main(int argc, char const* argv[])
{
    try {
        seh_exception::initialize();
        args_vector args{get_arguments_as_vector(argv, argc)};

        if (!convert(args_view{args}.subspan(1))) {
            return 1;
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

