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

#include <optional>
#include <type_traits>

namespace moreland::base64::shared
{
    template <typename T>
    concept enum_t = std::is_enum<T>::value;

    template <typename TVALUE, enum_t TREASON, TREASON UNKNOWN_ERROR>
    class maybe final
    {
        std::optional<TVALUE> value_{std::nullopt};
        std::optional<TREASON> reason_{std::nullopt};
    public:
        using value_type = TVALUE;
        using error_type = TREASON;

        constexpr explicit maybe() noexcept 
            : reason_{UNKNOWN_ERROR}
        {
        }
        constexpr explicit maybe(error_type reason) noexcept
            : reason_(reason)
        {}

        template <class... TYPES, std::enable_if_t<std::is_constructible_v<TVALUE, TYPES...>, int> = 0>
        constexpr explicit maybe(std::in_place_t, TYPES&&... args)
            : value_(std::in_place, std::forward<TYPES>(args)...)
        {}

        template <class ELEM, class... TYPES,
            std::enable_if_t<std::is_constructible_v<TVALUE, std::initializer_list<ELEM>&, TYPES...>, int> = 0>
        constexpr explicit maybe(std::in_place_t, std::initializer_list<ELEM> init_list, TYPES&&... args)
            : value_(std::in_place, init_list, std::forward<TYPES>(args)...) {}

        void reset()
        {
            value_.reset();
            reason_ = UNKNOWN_ERROR;
        }
        
        maybe& operator=(error_type reason)
        {
            reason_.reset();
            reason_ =  reason;
            return *this;
        }
        maybe& operator=(value_type&& value)
        {
            reason_.reset();
            value_ = value;
            return *this;
        }
        maybe& operator=(value_type const& value)
        {
            if (value) {
                value_ = value;
                reason_.reset();
            } else {
                reset();
                reason_ = UNKNOWN_ERROR;
            }
            return *this;
        }
    };

}
