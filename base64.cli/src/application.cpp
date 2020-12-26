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

#include <exception>

import moreland.base64.shared;
import moreland.base64.converters;
import std.core;
import std.threading;

using moreland::base64::shared::seh_exception;
//using moreland::base64::converters::encoder;
//using moreland::base64::converters::get_encoder;

void force_exception()
{
    int x = 9;
    int const y = 0;
    x = x / y;
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

int main()
{

    try {
        seh_exception::initialize();

        //auto const encoder = get_encoder();

    } catch (std::exception const& e) {
        std::cout << e.what() << std::endl;

    } catch (...) {
        std::cout << "unknown error occurred" << std::endl;
    }

    std::cout << "exiting program" << std::endl;
}

