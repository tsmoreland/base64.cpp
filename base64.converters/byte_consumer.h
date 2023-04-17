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

#include <span>

namespace moreland::base64::converters {
    template <typename TCONSUMER>
    concept ByteConsumer = requires(TCONSUMER consumer, std::span<unsigned char const> const source) {
                               { consumer.consume(source) } -> std::same_as<bool>;
                               { consumer.flush() } -> std::same_as<void>;
                               { consumer.reset() } -> std::same_as<void>;
                           };

    class byte_consumer {
    public:
        [[nodiscard]] virtual bool consume(std::span<unsigned char const> const source) = 0;
        virtual void flush()                                                            = 0;
        virtual void reset()                                                            = 0;

        explicit byte_consumer()                           = default;
        virtual ~byte_consumer()                           = default;
        byte_consumer(byte_consumer const&)                = default;
        byte_consumer(byte_consumer&&) noexcept            = default;
        byte_consumer& operator=(byte_consumer const&)     = default;
        byte_consumer& operator=(byte_consumer&&) noexcept = default;
    };

} // namespace moreland::base64::converters
