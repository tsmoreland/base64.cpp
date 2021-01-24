//
// Copyright © 2021 Terry Moreland
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

#include "operation_type.h"
#include "../base64.shared/concepts.h"
#include "../base64.shared/std_extensions.h"
#include "../base64.converters/converter.h"
#include "../base64.converters/decoder.h"
#include "../base64.converters/encoder.h"
#include "clipboard_byte_producer.h"
#include "file_byte_producer.h"
#include "byte_producer_consumer_container.h"

namespace moreland::base64::service
{
    template <
        converters::Converter CONVERTER,
        shared::Producer<CONVERTER> CONVERTER_PRODUCER,
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
            std::is_default_constructible_v<CONVERTER_PRODUCER>
    [[nodiscard]]
    bool convert(byte_producer_consumer_container<FILE_BYTE_PRODUCER, FILE_BYTE_CONSUMER, CLIPBOARD_BYTE_PRODUCER, CLIPBOARD_BYTE_CONSUMER>&& byte_producer_consumer)
    {
        using byte = unsigned char;
        CONVERTER_PRODUCER converter_producer{};
        auto const converter = converter_producer();

        auto& producer = byte_producer_consumer.get_producer();
        auto& consumer = byte_producer_consumer.get_consumer();

        for (auto const &chunk : producer) {
            std::span<byte const> chunk_value_view{chunk.value()};
            if (auto const converted = converter.convert(chunk_value_view); converted.has_value()) {
                if (!consumer.consume(converted.value())) {
                    return false;
                }
            } else
                return false;
        }
        consumer.flush();
        return true;
    }

    template <
        converters::Converter ENCODER = converters::encoder,
        converters::Converter DECODER = converters::decoder,
        converters::ByteProducer FILE_BYTE_PRODUCER = file_byte_producer,
        converters::ByteConsumer FILE_BYTE_CONSUMER = file_byte_consumer,
        converters::ByteProducer CLIPBOARD_BYTE_PRODUCER = clipboard_byte_producer<modern_win32_api::user::clipboard_traits>,
        converters::ByteConsumer CLIPBOARD_BYTE_CONSUMER = clipboard_byte_consumer<modern_win32_api::user::clipboard_traits>,
        shared::Producer<ENCODER> ENCODER_PRODUCER = converters::encoder_factory,
        shared::Producer<DECODER> DECODER_PRODUCER = converters::decoder_factory
    > 
        requires
            converters::ConstructedFromFile<FILE_BYTE_PRODUCER> && 
            converters::ConstructedFromFile<FILE_BYTE_CONSUMER> && 
            std::is_default_constructible_v<CLIPBOARD_BYTE_PRODUCER> &&
            std::is_default_constructible_v<CLIPBOARD_BYTE_CONSUMER>
    [[nodiscard]]
    bool convert(std::span<std::string_view const> const arguments)
    {
        auto const type = get_operation_type(std_extensions::first(arguments).value_or(""));
        if (type == operation_type::unknown)
            return false;

        try {
            using container_t = byte_producer_consumer_container<FILE_BYTE_PRODUCER, FILE_BYTE_CONSUMER, CLIPBOARD_BYTE_PRODUCER, CLIPBOARD_BYTE_CONSUMER>;
            if (type == operation_type::encode) {
                return convert<ENCODER, ENCODER_PRODUCER>(container_t{arguments.subspan(1)});
            }
            if (type == operation_type::decode) {
                return convert<DECODER, DECODER_PRODUCER>(container_t{arguments.subspan(1)});
            }

            return false;

        } catch (std::exception const& ex) {
            std::cerr << ex.what() << std::endl;
            return false;
        }
    }

}
