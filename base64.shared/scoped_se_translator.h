//
// Copyright (c) 2023 Terry Moreland
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

#include <eh.h>

namespace moreland::base64::shared 
{
    class scoped_se_translator final
    {
        using translator_function = _se_translator_function;
        translator_function const previous_handler_;
    public:
        explicit scoped_se_translator(translator_function translator) noexcept
            : previous_handler_{_set_se_translator(translator)}
        {
        }

        ~scoped_se_translator()
        {
            _set_se_translator(previous_handler_);
        }
        scoped_se_translator(scoped_se_translator const&) = delete;
        scoped_se_translator(scoped_se_translator &&) noexcept = delete;

        scoped_se_translator& operator=(scoped_se_translator const&) = delete;
        scoped_se_translator& operator=(scoped_se_translator &&) noexcept = delete;
    };

}
