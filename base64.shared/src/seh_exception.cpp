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

namespace moreland::base64::shared
{

void seh_translator(unsigned int error_code, EXCEPTION_POINTERS* exception_pointers)
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
}

unsigned seh_exception::get_error_code() const
{
    return m_error_code;
}

void seh_exception::initialize()
{
    // failure to call destructor means we won't unregister the translator for this process, but its exiting anyway so
    // no reall harm
    static scoped_se_translator translator{seh_translator};  // NOLINT(clang-diagnostic-exit-time-destructors)
}

}

