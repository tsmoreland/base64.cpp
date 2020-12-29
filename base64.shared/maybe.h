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

    template <class TVALUE>
    using remove_cvref_t = typename std::remove_cv<typename std::remove_reference<TVALUE>::type>::type;

    template <typename TVALUE, class TVALUE2, class container>
    using allow_direct_conversion = 
        std::bool_constant<std::conjunction_v<std::negation<
            std::is_same<remove_cvref_t<TVALUE2>, container>>,
            std::negation<std::is_same<remove_cvref_t<TVALUE2>, std::in_place_t>>,
            std::is_constructible<TVALUE, TVALUE2>>>;

    template <typename TVALUE, class TVALUE2, class container>
    concept directly_convertable = allow_direct_conversion<TVALUE, TVALUE2, container>::value;


    template <typename TVALUE, enum_t TREASON, TREASON UNKNOWN_ERROR>
    class maybe final
    {
        std::optional<TVALUE> value_{std::nullopt};
        std::optional<TREASON> reason_{std::nullopt};
    public:
        using value_type = TVALUE;
        using error_type = TREASON;

        template <class TVALUE2>
        struct allow_unwrapping
            : std::bool_constant<!std::disjunction_v<std::is_same<TVALUE, TVALUE2>
            , std::is_constructible<TVALUE, maybe<TVALUE2, TREASON, UNKNOWN_ERROR>&>
            , std::is_constructible<TVALUE, const maybe<TVALUE2, TREASON, UNKNOWN_ERROR>&>
            , std::is_constructible<TVALUE, const maybe<TVALUE2, TREASON, UNKNOWN_ERROR>>
            , std::is_constructible<TVALUE, maybe<TVALUE2, TREASON, UNKNOWN_ERROR>>
            , std::is_convertible<maybe<TVALUE2, TREASON, UNKNOWN_ERROR>&, TVALUE>
            , std::is_convertible<const maybe<TVALUE2, TREASON, UNKNOWN_ERROR>&, TVALUE>
            , std::is_convertible<const maybe<TVALUE2, TREASON, UNKNOWN_ERROR>, TVALUE>
            , std::is_convertible<maybe<TVALUE2, TREASON, UNKNOWN_ERROR>, TVALUE>>>
        {};

        static constexpr auto failed(error_type reason)
        {
            return maybe<TVALUE, TREASON, UNKNOWN_ERROR>(reason);
        }

        constexpr explicit maybe() noexcept 
            : reason_{UNKNOWN_ERROR}
        {
        }

        template <directly_convertable<TVALUE, maybe<TVALUE, TREASON, UNKNOWN_ERROR>> TVALUE2 = TVALUE>
        constexpr explicit(!std::is_convertible_v<TVALUE2, TVALUE>) maybe(TVALUE2&& right)
            : value_(std::in_place, std::forward<TVALUE2>(right))
        {}
        template <class TVALUE2,
            std::enable_if_t<std::conjunction_v<allow_unwrapping<TVALUE2>, std::is_constructible<TVALUE, TVALUE2 const&>>, int> = 0>
        explicit(!std::is_convertible_v<TVALUE2 const&, TVALUE>) maybe(maybe<TVALUE2, TREASON, UNKNOWN_ERROR> const& right)
            : value_{right}
        {
        }

        template <class TVALUE2,
            std::enable_if_t<std::conjunction_v<allow_unwrapping<TVALUE2>, std::is_constructible<TVALUE, TVALUE2>>, int> = 0>
        explicit(!std::is_convertible_v<TVALUE2, TVALUE>) maybe(maybe<TVALUE2, TREASON, UNKNOWN_ERROR>&& right) 
            : value_{std::move(right)}
        {
        }

        template <class... TYPES, std::enable_if_t<std::is_constructible_v<TVALUE, TYPES...>, int> = 0>
        constexpr explicit maybe(std::in_place_t, TYPES&&... args)
            : value_(std::in_place, std::forward<TYPES>(args)...)
        {}

        template <class ELEM, class... TYPES,
            std::enable_if_t<std::is_constructible_v<TVALUE, std::initializer_list<ELEM>&, TYPES...>, int> = 0>
        constexpr explicit maybe(std::in_place_t, std::initializer_list<ELEM> init_list, TYPES&&... args)
            : value_(std::in_place, init_list, std::forward<TYPES>(args)...) {}

        constexpr explicit maybe(error_type reason) noexcept
            : reason_(reason)
        {}


        [[nodiscard]]
        constexpr bool has_value()
        {
            return value_.has_value();
        }

        [[nodiscard]]
        constexpr TVALUE& value() &
        {
            return value_.value();
        }
        [[nodiscard]]
        constexpr TVALUE const& value() const&
        {
            return value_.value();
        }
        [[nodiscard]]
        constexpr TVALUE&& value() &&
        {
            return value_.value();
        }
        [[nodiscard]]
        constexpr TVALUE const&& value() const&&
        {
            return value_.value();
        }
        template <typename TVALUE2>
        [[nodiscard]]
        constexpr typename std::remove_cv<TVALUE>::type value_or(TVALUE2&& alternate) const&
        {
            return value_.value_or(alternate);
        }
        template <typename TVALUE2>
        [[nodiscard]]
        constexpr typename std::remove_cv<TVALUE>::type value_or(TVALUE2&& alternate) &&
        {
            return value_.value_or(std::move(alternate));
        }

        constexpr std::optional<TREASON> const& error()
        {
            return reason_;
        }

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

        template <typename TDESTINATION_RESULT, class TMAPPER>
        [[nodiscard]]
        maybe<TDESTINATION_RESULT, TREASON, UNKNOWN_ERROR> map(
            TMAPPER mapper)
        {
            return has_value()
                ? maybe<TDESTINATION_RESULT, TREASON, UNKNOWN_ERROR>(mapper(value()))
                : maybe<TDESTINATION_RESULT, TREASON, UNKNOWN_ERROR>(reason_.value_or(UNKNOWN_ERROR)); 
        }

        template <typename TDESTINATION_RESULT, class TMAPPER>
        [[nodiscard]]
        maybe<TDESTINATION_RESULT, TREASON, UNKNOWN_ERROR> flat_map(
            TMAPPER mapper)
        {
            return has_value()
                ? mapper(value())
                : maybe<TDESTINATION_RESULT, TREASON, UNKNOWN_ERROR>(reason_.value_or(UNKNOWN_ERROR)); 
        }

    };

}
