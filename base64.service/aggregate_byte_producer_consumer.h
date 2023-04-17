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

#include <memory>
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
    class aggregate_byte_producer_consumer final : public converters::byte_producer, public converters::byte_consumer
    {
        CLIPBOARD_BYTE_PRODUCER clipboard_byte_producer_;
        CLIPBOARD_BYTE_CONSUMER clipboard_byte_consumer_;
        std::unique_ptr<FILE_BYTE_PRODUCER> file_byte_producer_;
        std::unique_ptr<FILE_BYTE_CONSUMER> file_byte_consumer_;
    public:
        explicit aggregate_byte_producer_consumer(std::span<std::string_view const> arguments)
        {
            if (!arguments.empty()) {
                file_byte_producer_ = get_producer_from_file(std_extensions::first(arguments).value_or(""));
                arguments = arguments.subspan(1);
            } 

            if (!arguments.empty()) {
                file_byte_consumer_ = get_consumer_from_file(std_extensions::first(arguments).value_or(""));
                arguments = arguments.subspan(1);
            }
        }

        [[nodiscard]]
        byte_producer& get_producer()
        {
            return static_cast<bool>(file_byte_producer_)
                ? static_cast<byte_producer&>(*file_byte_producer_)
                : static_cast<byte_producer&>(clipboard_byte_producer_);
        }
        [[nodiscard]]
        byte_producer const& get_producer() const
        {
            return static_cast<bool>(file_byte_producer_)
                ? static_cast<byte_producer const&>(*file_byte_producer_)
                : static_cast<byte_producer const&>(clipboard_byte_producer_);
        }
        [[nodiscard]]
        byte_consumer& get_consumer()
        {
            return static_cast<bool>(file_byte_consumer_)
                ? static_cast<byte_consumer&>(*file_byte_consumer_)
                : static_cast<byte_consumer&>(clipboard_byte_consumer_);
        }
        [[nodiscard]]
        byte_consumer const& get_consumer() const
        {
            return static_cast<bool>(file_byte_consumer_)
                ? static_cast<byte_consumer const&>(*file_byte_consumer_)
                : static_cast<byte_consumer const&>(clipboard_byte_consumer_);
        }

        [[nodiscard]]
        std::optional<std::vector<unsigned char>> chunk_or_empty() override
        {
            return get_producer().chunk_or_empty();
        }


        [[nodiscard]]
        bool consume(std::span<unsigned char const> const source) override
        {
            return get_consumer().consume(source);
        }

        void flush() override
        {
            return get_consumer().flush();
        }

        void reset() override
        {
            return get_consumer().reset();
        }
    private:
        [[nodiscard]]
        std::unique_ptr<FILE_BYTE_PRODUCER> get_producer_from_file(std::string_view const path)
        {
            return get_from_file<FILE_BYTE_PRODUCER>(path);
        }
        [[nodiscard]]
        std::unique_ptr<FILE_BYTE_CONSUMER> get_consumer_from_file(std::string_view const path)
        {
            return get_from_file<FILE_BYTE_CONSUMER>(path);
        }
        template <typename T>
            requires converters::ConstructedFromFile<T>  
        [[nodiscard]]
        std::unique_ptr<T> get_from_file(std::string_view const path)
        {
            auto const file = std_extensions::map<std::string_view const, std::filesystem::path>(path,
                [](auto const& arg) {
                    return std::filesystem::path(arg);
                });
            return file.has_value()
                ? std::make_unique<T>(file.value_or(""))
                : std::unique_ptr<T>{};
        }
    };
}
