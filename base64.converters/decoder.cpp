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

#include "pch.h"
#include "decoder.h"

#include "../base64.shared/optional_functions.h"
#include "../base64.shared/convert.h"

using std::move;
using std::nullopt;
using std::optional;
using std::size_t;
using std::span;
using std::string;
using std::vector;

using moreland::base64::shared::map;
using moreland::base64::shared::to_byte;
using moreland::base64::shared::to_string;

namespace moreland::base64::converters
{
    decoder::decoder(bool const is_url, bool const insert_line_break, std::optional<int> const line_max, bool const do_padding) noexcept
        : is_url_{is_url}
        , insert_line_break_{insert_line_break}
        , line_max_{line_max}
        , do_padding_{do_padding}
    {
    }

    optional<vector<byte>> decoder::decode(span<byte const> const source) const
    {
        vector<byte> destination;
        return map<size_t, vector<byte>>(decode(source, destination), 
            [&destination](auto const&) {
                return destination;
            });
    }

    optional<size_t> decoder::decode(span<byte const> const source, vector<byte>& destination) const
    {
        span<byte const> trimmed_source = get_trimmed_span(source);

        auto const output_length = calculate_output_length(source).value_or(0);
        if (output_length == 0) {
            return nullopt;
        }

        destination.reserve(output_length);
        destination.clear();

        constexpr byte upper_a          =  'A';            
        constexpr byte a                =  'a';            
        constexpr byte zero             =  '0';         
        constexpr byte eq               =  '=';
        constexpr byte plus             =  '+';
        constexpr byte slash            =  '/';
        constexpr byte space            =  ' ';
        constexpr byte tab              =  '\t';
        constexpr byte newline          =  '\n';
        constexpr byte carriage_return  =  '\r';
        constexpr byte a_to_z           =  ('Z' - 'A');  // = ('z' - 'a')
        constexpr byte zero_to_9        =  ('9' - '0');

        auto current = begin(source);
        auto const after_last = end(source);

        unsigned int value{}; 
        unsigned int current_block_codes = 255u;

        auto equality_encountered = [&value, output_length, &current_block_codes, &current, &after_last, &destination]()-> optional<size_t> {
            if (current == after_last) {
 
                // Code is zero for trailing '=':
                current_block_codes <<= 6;
 
                // The '=' did not complete a 4-group. The input must be bad:
                if ((current_block_codes & 0x80000000u) == 0u) {
                    return nullopt;
                }
 
                if (output_length - destination.size() < 2) {
                    return nullopt;
                }
 
                // We are good, store bytes form this past group. We had a single "=", so we take two bytes:
                destination.emplace_back(current_block_codes >> 16);
                destination.emplace_back(current_block_codes >> 8);
 
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
                
                    // Code is zero for each of the two '=':
                    current_block_codes <<= 12;
 
                    // The '=' did not complete a 4-group. The input must be bad:
                    if ((current_block_codes & 0x80000000u) == 0u)
                        return nullopt;
 
                    if (output_length - destination.size() < 1) {
                        return nullopt;
                    }
 
                    // We are good, store bytes form this past group. We had a "==", so we take only one byte:
                    destination.emplace_back(current_block_codes >> 16);
                    current_block_codes = 0x000000FFu;
                    
                } else  // '=' is not ok at places other than the end:
                    return nullopt;
                            
            }
            return (current_block_codes == 0x000000FFu) 
                ? optional(output_length)
                : nullopt;
        };

        while (true) {
            if (current == after_last) {
                return (current_block_codes == 0x000000FFu) 
                    ? optional(output_length)
                    : nullopt;
            }

            value = *current;
            ++current;
            if (value - upper_a <= a_to_z)
                value -= upper_a;                

            else if (value - a <= a_to_z)
                value -= (a - 26u);                

            else if (value - zero <= zero_to_9)
                value -= (zero - 52u);

            else {
                // Use the slower switch for less common cases:
                switch(value) {
                    case plus:  
                        value = 62u;
                        break;
                    case slash: 
                        value = 63u;
                        break;
                    case carriage_return:  
                    case newline:
                    case space:
                    case tab:                             
                        continue;

                    // The equality char is only legal at the end of the input.
                    // Jump after the loop to make it easier for the JIT register predictor to do a good job for the loop itself:
                    case eq:
                        return equality_encountered();
                    default:
                        return nullopt;
                }
            }

            current_block_codes = (current_block_codes << 6) | value;

            auto const and_value = current_block_codes & 0x80000000u;

            // Last bit in currBlockCodes will be on after in shifted right 4 times:
            if ((current_block_codes & 0x80000000u) != 0u) {

                if (output_length - destination.size() < 3) {
                    return nullopt;
                }

                destination.emplace_back(current_block_codes >> 16);
                destination.emplace_back(current_block_codes >> 8);
                destination.emplace_back(current_block_codes);

                current_block_codes = 0x000000FFu;
            }

        }
    }

    string decoder::decode_to_string_or_empty(span<byte const> const source) const
    {
        vector<byte> destination;
        return map<size_t, string>(decode(source, destination), 
            [&destination](auto const&) {
                span<byte const> const destination_view = destination;
                return to_string(destination_view);
            })
            .value_or("");
    }
    string decoder::decode_to_string_or_empty(span<char const> const source) const
    {
        vector<byte> source_bytes(begin(source), end(source));
        return decode_to_string_or_empty(source_bytes);
    }

    optional<size_t> decoder::calculate_output_length(span<byte const> const source)
    {
        constexpr byte equals = '=';
        constexpr byte space = ' ';

        auto useful_input_length = source.size();
        auto padding = 0;

        for (auto const value : source) {
            if (value <= space) {
                if (--useful_input_length == 0) {
                    return nullopt;
                }
            } else if (value == equals) {
                if (--useful_input_length == 0) {
                    return nullopt;
                }
                padding++;
            }
        }

        switch (padding) {
        default: // > 2
            return nullopt;
        case 2:
            padding += 1;
            break;
        case 1:
            padding += 2;
            break;
        case 0:
            break;
        }
        return optional((useful_input_length / 4) * 3 + padding);
    }

    decoder make_decoder() noexcept
    {
        return decoder{false, false, nullopt, true};  
    }
    decoder make_url_decoder() noexcept  
    {
        return decoder{true, false, nullopt, true};  
    }

}
