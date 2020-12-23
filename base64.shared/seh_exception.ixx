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

module;

#define WIN32_LEAN_AND_MEAN 

#include <Windows.h>
#include <eh.h>
#include <exception>
import moreland.base64.shared.scoped_se_translator;

export module moreland.base64.shared.seh_exception;


export class seh_exception final : public std::exception
{
    unsigned int const m_error_code;
public:

    explicit seh_exception(unsigned int const error_code);
    explicit seh_exception(unsigned int const error_code, EXCEPTION_POINTERS const* const exception_pointers);

    [[nodiscard]] 
    unsigned int get_error_code() const;

    static void initialize();
};

module :private; 


[[noreturn]]
void __cdecl seh_translator(unsigned int error_code, struct _EXCEPTION_POINTERS* exception_pointers)
{
    throw seh_exception(error_code, exception_pointers);
}
    
seh_exception::seh_exception(unsigned int const error_code)
    : seh_exception(error_code, nullptr)
{
}

seh_exception::seh_exception(unsigned int const error_code, EXCEPTION_POINTERS const* const exception_pointers)
    : m_error_code{error_code}
{
    if (exception_pointers == nullptr) {
        return;
    }

    // do something with the exception pointers, specifically gather up the exception codes 
}

unsigned seh_exception::get_error_code() const
{
    return m_error_code;
}

void seh_exception::initialize()
{
    // failure to call destructor means we won't unregister the translator for this process, but its exiting anyway so
    // no real harm
    static scoped_se_translator translator(seh_translator);  // NOLINT(clang-diagnostic-exit-time-destructors)
}
