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

#include "../base64.shared/std_extensions.h"
#include "../base64.converters/converter.h"
#include "../base64.converters/byte_producer.h"
#include "clipboard_byte_producer.h"
#include "clipboard_byte_consumer.h"
#include "file_byte_producer.h"
#include "file_byte_consumer.h"

namespace moreland::base64::service
{

    template <
        converters::ByteProducer FILE_BYTE_PRODUCER = file_byte_producer,
        converters::ByteConsumer FILE_BYTE_CONSUMER = file_byte_consumer,
        converters::ByteProducer CLIPBOARD_BYTE_PRODUCER = clipboard_byte_producer<modern_win32_api::user::clipboard_traits>,
        converters::ByteConsumer CLIPBOARD_BYTE_CONSUMER = clipboard_byte_consumer<modern_win32_api::user::clipboard_traits>
    > 
        requires
            converters::ConstructedFromFile<FILE_BYTE_PRODUCER> && 
            converters::ConstructedFromFile<FILE_BYTE_CONSUMER> && 
            std::is_default_constructible_v<CLIPBOARD_BYTE_PRODUCER> &&
            std::is_default_constructible_v<CLIPBOARD_BYTE_CONSUMER> &&
            std::is_base_of_v<converters::byte_producer, FILE_BYTE_PRODUCER> &&
            std::is_base_of_v<converters::byte_consumer, FILE_BYTE_CONSUMER> &&
            std::is_base_of_v<converters::byte_producer, CLIPBOARD_BYTE_PRODUCER> &&
            std::is_base_of_v<converters::byte_consumer, CLIPBOARD_BYTE_CONSUMER> 
    class byte_producer_consumer_container final
    {
        CLIPBOARD_BYTE_PRODUCER clipboard_byte_producer_;
        CLIPBOARD_BYTE_CONSUMER clipboard_byte_consumer_;
        std::vector<FILE_BYTE_PRODUCER> file_byte_producer_container_;
        std::vector<FILE_BYTE_CONSUMER> file_byte_consumer_container_;
    public:
        explicit byte_producer_consumer_container(std::span<std::string_view const> arguments)
        {
            if (!arguments.empty()) {
                static_cast<void>(try_add_producer_using_file(std_extensions::first(arguments).value_or(""), file_byte_producer_container_));
                arguments = arguments.subspan(1);
            } 

            if (!arguments.empty()) {
                static_cast<void>(try_add_consumer_using_file(std_extensions::first(arguments).value_or(""), file_byte_consumer_container_));
                arguments = arguments.subspan(1);
            }
        }

        [[nodiscard]]
        converters::byte_producer& get_producer()
        {
            return !file_byte_producer_container_.empty()
                ? static_cast<converters::byte_producer&>(file_byte_producer_container_[0])
                : static_cast<converters::byte_producer&>(clipboard_byte_producer_);
        }
        [[nodiscard]]
        converters::byte_producer const& get_producer() const
        {
            return !file_byte_producer_container_.empty()
                ? static_cast<converters::byte_producer const&>(file_byte_producer_container_[0])
                : static_cast<converters::byte_producer const&>(clipboard_byte_producer_);
        }
        [[nodiscard]]
        converters::byte_consumer& get_consumer()
        {
            return !file_byte_consumer_container_.empty()
                ? static_cast<converters::byte_consumer&>(file_byte_consumer_container_[0])
                : static_cast<converters::byte_consumer&>(clipboard_byte_consumer_);
        }
        [[nodiscard]]
        converters::byte_consumer const& get_consumer() const
        {
            return !file_byte_consumer_container_.empty()
                ? static_cast<converters::byte_consumer const&>(file_byte_consumer_container_[0])
                : static_cast<converters::byte_consumer const&>(clipboard_byte_consumer_);
        }

    private:
        [[nodiscard]]
        static bool try_add_producer_using_file(std::string_view const path, std::vector<FILE_BYTE_PRODUCER>& container)
        {
            return try_add_to_vector<FILE_BYTE_PRODUCER>(path, container);
        }
        [[nodiscard]]
        static bool try_add_consumer_using_file(std::string_view const path, std::vector<FILE_BYTE_CONSUMER>& container)
        {
            return try_add_to_vector<FILE_BYTE_CONSUMER>(path, container);
        }
        template <typename T>
        [[nodiscard]]
        static bool try_add_to_vector(std::string_view const path, std::vector<T>& container)
        {
            auto const file = std_extensions::map<std::string_view const, std::filesystem::path>(path,
                [](auto const& arg) {
                    return std::filesystem::path(arg);
                });
            if (!file.has_value())
                return false;
            container.emplace_back(file.value());
            return true;
        }

    };
}
