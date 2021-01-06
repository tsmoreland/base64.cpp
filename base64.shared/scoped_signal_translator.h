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

#pragma once

#include <csignal>

namespace moreland::base64::shared
{
    class scoped_signal_translator final
    {
        using signal_function = _crt_signal_t;
        signal_function const previous_abort_handler_;
        signal_function const previous_floating_point_exception_handler_;
        signal_function const previous_illegal_instruction_handler_;
        signal_function const previous_illegal_storage_access_handler_;
        signal_function const previous_termination_request_handler_;

    public:
        explicit scoped_signal_translator(signal_function const translator)
            : previous_abort_handler_{signal(SIGABRT, translator)}
            , previous_floating_point_exception_handler_{signal(SIGFPE, translator)}
            , previous_illegal_instruction_handler_{signal(SIGILL, translator)}
            , previous_illegal_storage_access_handler_{signal(SIGSEGV, translator)}
            , previous_termination_request_handler_{signal(SIGTERM, translator)}
        {
            
        }
        ~scoped_signal_translator()
        {
            signal(SIGABRT, previous_abort_handler_);
            signal(SIGFPE, previous_floating_point_exception_handler_);
            signal(SIGILL, previous_illegal_instruction_handler_);
            signal(SIGSEGV, previous_illegal_storage_access_handler_);
            signal(SIGTERM, previous_termination_request_handler_);
        }

        scoped_signal_translator(scoped_signal_translator const &) = default;
        scoped_signal_translator(scoped_signal_translator &&) noexcept = default;
        scoped_signal_translator& operator=(scoped_signal_translator const &) = delete;
        scoped_signal_translator& operator=(scoped_signal_translator &&) noexcept = delete;
    };
}

