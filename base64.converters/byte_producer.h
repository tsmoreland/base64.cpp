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

#include <filesystem>
#include <iterator>
#include <optional>
#include <vector>

namespace moreland::base64::converters {
    template <typename T>
    concept ConstructedFromFile = requires(std::filesystem::path const& file_path) { T(file_path); };

    class byte_producer {
    public:
        class iterator;

        [[nodiscard]] virtual std::optional<std::vector<unsigned char>> chunk_or_empty() = 0;

        explicit byte_producer()                           = default;
        virtual ~byte_producer()                           = default;
        byte_producer(byte_producer const&)                = default;
        byte_producer(byte_producer&&) noexcept            = default;
        byte_producer& operator=(byte_producer const&)     = default;
        byte_producer& operator=(byte_producer&&) noexcept = default;

        iterator begin() {
            // reset(); -- add reset to producer, use it to reset position back to start
            return iterator(*this, chunk_or_empty());
        }

        iterator end() {
            return iterator(*this, std::nullopt);
        }

        class iterator {
            byte_producer& container_;
            std::optional<std::vector<unsigned char>> current_{std::nullopt};

        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = std::vector<unsigned char>;
            using pointer           = value_type*;
            using reference         = value_type const&;

            explicit iterator(byte_producer& container, std::optional<std::vector<unsigned char>> value)
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
            iterator& operator++() {
                current_ = container_.chunk_or_empty();
                return *this;
            }
            iterator operator++(int) {
                iterator tmp = *this;
                current_     = container_.chunk_or_empty();
                return tmp;
            }
            [[nodiscard]] friend bool operator==(iterator const& first, iterator const& second) {
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
            [[nodiscard]] friend bool operator!=(iterator const& first, iterator const& second) {
                return !(first == second);
            }
        };
    };

    template <typename TPRODUCER>
    concept ByteProducer = requires(TPRODUCER producer, std::string_view const argument) {
                               std::forward_iterator<typename TPRODUCER::iterator>;
                               std::is_base_of<byte_producer, TPRODUCER>::value;
                               { producer.chunk_or_empty() } -> std::same_as<std::optional<std::vector<unsigned char>>>;
                           };


} // namespace moreland::base64::converters
