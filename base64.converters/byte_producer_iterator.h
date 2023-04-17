//
// Copyright (c) 2023 Terry Moreland
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
// Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#pragma once

#include "byte_producer.h"

namespace moreland::base64::converters {
    template <ByteProducer BYTE_PRODUCER>
    class byte_producer_iterator final {
        BYTE_PRODUCER& container_;
        std::optional<std::vector<unsigned char>> current_{std::nullopt};

    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = std::vector<unsigned char>;
        using pointer           = value_type*;
        using reference         = value_type const&;

        explicit byte_producer_iterator(byte_producer& container, std::optional<std::vector<unsigned char>> value)
            : container_{container}, current_{std::move(value)} {}

        [[nodiscard]] reference operator*() const {
            if (current_.has_value())
                return current_.value();
            throw std::out_of_range("invalid iterator access");
        }
        [[nodiscard]] pointer operator->() {
            if (current_.has_value())
                return &current_.value();
            throw std::out_of_range("invalid iterator access");
        }
        byte_producer_iterator& operator++() {
            current_ = container_.chunk_or_empty();
            return *this;
        }
        byte_producer_iterator operator++(int) {
            byte_producer_iterator tmp = *this;
            current_                   = container_.chunk_or_empty();
            return tmp;
        }
        [[nodiscard]] friend bool operator==(
            byte_producer_iterator const& first, byte_producer_iterator const& second) {
            if (first.current_.has_value() != second.current_.has_value())
                return false;
            if (!first.current_.has_value())
                return true;
            if (first.current_.value().size() != second.current_.value().size())
                return false;
            if (first.current_.value().empty())
                return true;
            for (auto i = std::begin(first.current_.value()), j = std::begin(second.current_.value());
                 i != std::end(first.current_.value()) && j != std::end(second.current_.value()); ++i, ++j) {

                if (*i != *j)
                    return false;
            }
            return true;
        }
        [[nodiscard]] friend bool operator!=(
            byte_producer_iterator const& first, byte_producer_iterator const& second) {
            return !(first == second);
        }
    };
};
}
