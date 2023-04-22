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

#include "pch.h"
#include "decoder.h"

#include "../base64.shared/convert.h"
#include "../base64.shared/std_extensions.h"

using std::move;
using std::nullopt;
using std::optional;
using std::size_t;
using std::span;
using std::string;
using std::vector;

using moreland::base64::shared::to_byte;
using moreland::base64::shared::to_string;
using moreland::std_extensions::map;

namespace moreland::base64::converters {
    using iterator = span<byte const>::iterator;

    maybe_converted<size_t> equality_encountered(size_t const output_length, unsigned int& current_block_codes,
        iterator& current, iterator const& after_last, vector<byte>& destination);

    decoder::decoder(bool const is_url, bool const insert_line_break, std::optional<int> const line_max,
        bool const do_padding) noexcept
        : is_url_{is_url}, insert_line_break_{insert_line_break}, line_max_{line_max}, do_padding_{do_padding} {}

    maybe_converted<vector<byte>> decoder::convert(span<byte const> const source) const {
        vector<byte> destination;
        return convert(source, destination).map<vector<byte>>([&destination](auto const&) { return destination; });
    }

    maybe_converted<size_t> decoder::convert(span<byte const> const source, vector<byte>& destination) const {
        span<byte const> trimmed_source = get_trimmed_span(source);

        auto const output_length = calculate_output_length(source).value_or(static_cast<size_t>(0));
        if (output_length == 0) {
            return maybe_size_t(base64_failure_reason::bad_length);
        }

        destination.reserve(output_length);
        destination.clear();

        auto current          = begin(source);
        auto const after_last = end(source);

        unsigned int current_block_codes = 255u;

        while (true) {
            if (current == after_last) {
                return (current_block_codes == 0x000000FFu) ? maybe_size_t(output_length)
                                                            : maybe_size_t(base64_failure_reason::bad_format);
            }

            unsigned int value = *current;
            ++current;
            if (value - to_byte('A') <= to_byte('Z' - 'A'))
                value -= to_byte('A');

            else if (value - to_byte('a') <= to_byte('Z' - 'A'))
                value -= 0x00000047u;

            else if (value - to_byte('0') <= to_byte('9' - '0'))
                value -= 0xfffffffcu;

            else {
                switch (value) {
                case to_byte('+'):
                    value = 62u;
                    break;
                case to_byte('/'):
                    value = 63u;
                    break;
                case to_byte('\r'):
                case to_byte('\n'):
                case to_byte(' '):
                case to_byte('\t'):
                    continue;
                case to_byte('='):
                    return equality_encountered(output_length, current_block_codes, current, after_last, destination);
                default:
                    return maybe_size_t(base64_failure_reason::bad_format);
                }
            }

            current_block_codes = (current_block_codes << 6) | value;

            // Last bit in currBlockCodes will be on after in shifted right 4 times:
            if (auto const and_value = current_block_codes & 0x80000000u; and_value != 0u) {

                if (output_length - destination.size() < 3) {
                    return maybe_size_t(base64_failure_reason::bad_length);
                }

                destination.emplace_back(to_byte(to_byte(current_block_codes >> 16)));
                destination.emplace_back(to_byte(to_byte(current_block_codes >> 8)));
                destination.emplace_back(to_byte(to_byte(current_block_codes)));

                current_block_codes = 0x000000FFu;
            }
        }
    }

    string decoder::convert_to_string_or_empty(span<byte const> const source) const {
        vector<byte> destination;
        return convert(source, destination)
            .map<string>([&destination](auto const&) {
                span<byte const> const destination_view = destination;
                return to_string(destination_view);
            })
            .value_or("");
    }
    string decoder::convert_to_string_or_empty(span<char const> const source) const {
        vector<byte> source_bytes(begin(source), end(source));
        return convert_to_string_or_empty(source_bytes);
    }

    maybe_size_t decoder::calculate_output_length(span<byte const> const source) {
        constexpr byte equals = '=';
        constexpr byte space  = ' ';

        auto useful_input_length = source.size();
        auto padding             = 0;

        for (auto const value : source) {
            if (value <= space) {
                if (--useful_input_length == 0) {
                    return maybe_size_t(base64_failure_reason::bad_length);
                }
            } else if (value == equals) {
                if (--useful_input_length == 0) {
                    return maybe_size_t(base64_failure_reason::bad_length);
                }
                padding++;
            }
        }

        switch (padding) {
        default: // > 2
            return maybe_size_t(base64_failure_reason::bad_format);
        case 2:
            padding += 1;
            break;
        case 1:
            padding += 2;
            break;
        case 0:
            break;
        }
        return maybe_size_t((useful_input_length / 4) * 3 + padding);
    }

    decoder make_decoder() noexcept {
        return decoder{false, false, nullopt, true};
    }
    decoder make_url_decoder() noexcept {
        return decoder{true, false, nullopt, true};
    }

    maybe_size_t equality_encountered(size_t const output_length, unsigned int& current_block_codes, iterator& current,
        iterator const& after_last, vector<byte>& destination) {
        if (current == after_last) {

            // Code is zero for trailing '=':
            current_block_codes <<= 6;

            if ((current_block_codes & 0x80000000u) == 0u) {
                return maybe_size_t(base64_failure_reason::bad_format);
            }

            if (output_length - destination.size() < 2) {
                return maybe_size_t(base64_failure_reason::bad_format);
            }

            // We are good, store bytes form this past group. We had a single "=", so we take two bytes:
            destination.emplace_back(to_byte(current_block_codes >> 16));
            destination.emplace_back(to_byte(current_block_codes >> 8));

            current_block_codes = 0x000000FFu;

        } else { // '=' can also be at the pre-last position iff the last is also a '=' excluding the white spaces:

            // We need to get rid of any intermediate white spaces.
            // Otherwise we would be rejecting input such as "abc= =":
            auto const last = after_last - 1;
            while ((current <=> last) < nullptr) {

                if (auto const last_value = *current;
                    last_value != ' ' && last_value != '\n' && last_value != '\r' && last_value != '\t') {
                    break;
                }
                ++current;
            }

            if (current == after_last - 1 && *current == '=') {

                current_block_codes <<= 12;
                if ((current_block_codes & 0x80000000u) == 0u)
                    return maybe_size_t(base64_failure_reason::bad_format);

                if (output_length - destination.size() < 1) {
                    return maybe_size_t(base64_failure_reason::bad_format);
                }
                destination.emplace_back(to_byte(current_block_codes >> 16));
                current_block_codes = 0x000000FFu;

            } else // '=' is not ok at places other than the end:
                return maybe_size_t(base64_failure_reason::bad_format);
        }
        return (current_block_codes == 0x000000FFu) ? maybe_size_t(output_length)
                                                    : maybe_size_t(base64_failure_reason::bad_format);
    }
} // namespace moreland::base64::converters
