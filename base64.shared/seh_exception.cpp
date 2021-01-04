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
#include "seh_exception.h"
#include "scoped_se_translator.h"
#include "scoped_signal_translator.h"

using std::string;

namespace moreland::base64::shared
{

    [[nodiscard]]
    std::string format_what(unsigned int const error_code, EXCEPTION_POINTERS const* const exception_pointers)
    {
        using std::stringstream;
        stringstream builder{};
        builder << "SEH Error: ";
        builder << "0x" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << error_code;

        if (exception_pointers == nullptr)
            return builder.str();

        return builder.str();
    }

    [[noreturn]]
    void __cdecl signal_handler(int signal)
    {
        switch (signal)
        {
        case SIGABRT:
            throw seh_exception(STATUS_FATAL_APP_EXIT);
        case SIGFPE:
            throw seh_exception(STATUS_FLOAT_INVALID_OPERATION);
        case SIGILL:
            throw seh_exception(STATUS_ILLEGAL_INSTRUCTION);
        case SIGSEGV:
            throw seh_exception(STATUS_ACCESS_VIOLATION);
        case SIGTERM:
            throw seh_exception(STATUS_FATAL_APP_EXIT);
        default:
            throw seh_exception(signal);
        }
    }

    [[noreturn]]
    // ReSharper disable once CppParameterMayBeConst -- matching the expected function input/output for _set_se_translator
    void __cdecl seh_translator(unsigned int error_code, EXCEPTION_POINTERS* exception_pointers)
    {
        throw seh_exception(error_code, exception_pointers);
    }

    seh_exception::seh_exception(unsigned int const error_code)
        : seh_exception(error_code, nullptr)
    {
    }

    seh_exception::seh_exception(unsigned int const error_code, EXCEPTION_POINTERS const* const exception_pointers)
        : std::exception{format_what(error_code, exception_pointers).c_str()}
        , error_code_{error_code}
    {
        if (exception_pointers == nullptr) {
            return;
        }

        // do something with the exception pointers, specifically gather up the exception codes 
    }

    unsigned seh_exception::get_error_code() const
    {
        return error_code_;
    }

    void seh_exception::initialize()
    {
        // failure to call destructor means we won't unregister the translator for this process, but its exiting anyway so
        // no real harm
        thread_local scoped_se_translator translator(seh_translator);  // NOLINT(clang-diagnostic-exit-time-destructors)

        //thread_local scoped_signal_translator translator(signal_handler);
    }
}