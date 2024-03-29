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
#include "encoder.h"
#include "../base64.shared/convert.h"
#include "../base64.shared/std_extensions.h"
#include "common.h"

using std::move;
using std::nullopt;
using std::optional;
using std::size_t;
using std::span;
using std::string;
using std::vector;

using moreland::base64::shared::to_byte;
using moreland::std_extensions::map;

namespace moreland::base64::converters {
    template <size_t position>
    [[nodiscard]] size_t get_output_index(span<byte const> source, size_t input_position);

    encoder::encoder(
        bool const is_url, bool const insert_line_break, optional<int> const line_max, bool const do_padding) noexcept
        : is_url_{is_url}, insert_line_break_{insert_line_break}, line_max_{line_max}, do_padding_{do_padding} {}

    maybe_converted<vector<byte>> encoder::convert(span<byte const> const source) const {
        vector<byte> destination;
        return convert(source, destination).map<vector<byte>>([&destination](auto const&) { return destination; });
    }

    maybe_converted<size_t> encoder::convert(span<byte const> const source, vector<byte>& destination) const {
        auto const output_length = calculate_output_length(source, insert_line_break_);
        if (output_length.value_or(0UL) == 0UL) {
            return maybe_size_t(base64_failure_reason::bad_format);
        }

        auto const base64_table = get_base64_table();

        destination.reserve(output_length.value());
        destination.clear();

        auto const length                  = source.size();
        auto const length_mod_3            = length % 3;
        auto const calc_length             = length - length_mod_3;
        constexpr auto line_break_position = get_base64_line_break_position();
        size_t character_count             = 0;

        size_t input_position;
        size_t output_position = 0;

        for (input_position = 0; input_position < calc_length; input_position += 3) {
            if (insert_line_break_) {
                if (character_count == line_break_position) {
                    destination.emplace_back(to_byte('\r'));
                    destination.emplace_back(to_byte('\n'));
                    output_position += 2;
                    character_count = 0UL;
                }
                character_count += 4;
            }

            destination.emplace_back(base64_table[get_output_index<0>(source, input_position)]);
            destination.emplace_back(base64_table[get_output_index<1>(source, input_position)]);
            destination.emplace_back(base64_table[get_output_index<2>(source, input_position)]);
            destination.emplace_back(base64_table[get_output_index<3>(source, input_position)]);
            output_position += 4;
        }
        input_position = calc_length;
        if (insert_line_break_ && length_mod_3 != 0 && character_count == line_break_position) {
            destination[output_position++] = '\r';
            destination[output_position++] = '\n';
        }

        switch (length_mod_3) {
        case 2:
            destination.emplace_back(base64_table[get_output_index<0>(source, input_position)]);
            destination.emplace_back(base64_table[get_output_index<1>(source, input_position)]);
            destination.emplace_back(
                base64_table[(static_cast<size_t>(source[input_position + 1] & to_byte(0x0f))) << 2]);
            destination.emplace_back(base64_table[64]); // Pad
            output_position += 4;
            break;
        case 1: // Two character padding needed
            destination.emplace_back(base64_table[get_output_index<0>(source, input_position)]);
            destination.emplace_back(base64_table[(static_cast<size_t>(source[input_position] & to_byte(0x03))) << 4]);
            destination.emplace_back(base64_table[64]); // Pad
            destination.emplace_back(base64_table[64]); // Pad

            // outChars[j+1] = base64[(inData[i]&0x03)<<4]
            output_position += 4;
            break;
        default:
            break;
        }

        return maybe_size_t{static_cast<size_t>(output_position)};
    }

    string encoder::convert_to_string_or_empty(span<byte const> const source) const {
        vector<byte> destination;
        return convert(source, destination)
            .map<string>([&destination](auto const&) -> string {
                span<byte const> const destination_view{destination};
                return shared::to_string(destination_view);
            })
            .value_or("");
    }

    string encoder::convert_to_string_or_empty(span<char const> const source) const {
        vector<byte> source_bytes(begin(source), end(source));
        return convert_to_string_or_empty(source_bytes);
    }

    maybe_converted<size_t> encoder::calculate_output_length(
        span<byte const> const source, bool const insert_line_breaks) {
        constexpr size_t const new_line_size = 2;
        constexpr size_t const ONE           = 1;
        size_t size                          = source.size() / 3 * 4;
        size += source.size() % 3 != 0 ? 4 : 0;

        if (size == 0)
            return 0;
        if (insert_line_breaks) {
            auto new_line_count = size / get_base64_line_break_position();
            if (size % new_line_count == 0) {
                --new_line_count;
            }
            size += new_line_count * new_line_size;
        }

        if (size > static_cast<size_t>(numeric_limits::maximum<int>())) {
            return maybe_size_t(base64_failure_reason::bad_length);
        }

        return maybe_size_t{static_cast<size_t>(size)};
    }

    encoder make_encoder() noexcept {
        encoder const rfc4648{false, false, nullopt, true}; // NOLINT(clang-diagnostic-exit-time-destructors)
        return rfc4648;
    }
    encoder make_url_encoder() noexcept {
        encoder const rfc4648_url_safe{true, false, nullopt, true}; // NOLINT(clang-diagnostic-exit-time-destructors)
        return rfc4648_url_safe;
    }

    template <size_t position>
    [[nodiscard]] size_t get_output_index(span<byte const> source, size_t input_position) {
        size_t index;

        static_assert(position >= 0 && position < 4, "position must be in range of [0:4)");

        switch (position) {
        case 0:
            index = static_cast<size_t>((source[input_position] & 0xfc) >> 2);
            break;
        case 1:
            {
                size_t const input_position_plus_1 = input_position + 1;
                index                              = static_cast<size_t>((source[input_position] & to_byte(0x03)) << 4
                                            | (source[input_position_plus_1] & to_byte(0xf0)) >> 4);
                break;
            }
        case 2:
            {
                size_t const input_position_plus_1 = input_position + 1;
                size_t const input_position_plus_2 = input_position + 2;
                index = static_cast<size_t>((source[input_position_plus_1] & to_byte(0x0f)) << 2
                                            | ((source[input_position_plus_2] & to_byte(0xc0)) >> 6));
                break;
            }
        case 3:
            {
                size_t const input_position_plus_2 = input_position + 2;
                index                              = static_cast<size_t>(source[input_position_plus_2] & to_byte(0x3f));
                break;
            }
        default:
            throw std::out_of_range("unreachable code");
        }
        return index;
    }

} // namespace moreland::base64::converters