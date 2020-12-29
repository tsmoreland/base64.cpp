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

#include <string_view>

namespace moreland::base64::converters::tests
{

    constexpr std::string_view DECODED = R"(It is a truth universally acknowledged, that a single man in possession of a good fortune, must be in want of a wife.

However little known the feelings or views of such a man may be on his first entering a neighbourhood, this truth is so well fixed in the minds of the surrounding families, that he is considered the rightful property of some one or other of their daughters.

"My dear Mr. Bennet," said his lady to him one day, "have you heard that Netherfield Park is let at last?"

Mr. Bennet replied that he had not.

"But it is," returned she; "for Mrs. Long has just been here, and she told me all about it."

Mr. Bennet made no answer.

"Do you not want to know who has taken it?" cried his wife impatiently.)";


    constexpr std::string_view ENCODED = R"(SXQgaXMgYSB0cnV0aCB1bml2ZXJzYWxseSBhY2tub3dsZWRnZWQsIHRoYXQgYSBzaW5nbGUgbWFuIGluIHBvc3Nlc3Npb24gb2YgYSBnb29kIGZvcnR1bmUsIG11c3QgYmUgaW4gd2FudCBvZiBhIHdpZmUuDQoNCkhvd2V2ZXIgbGl0dGxlIGtub3duIHRoZSBmZWVsaW5ncyBvciB2aWV3cyBvZiBzdWNoIGEgbWFuIG1heSBiZSBvbiBoaXMgZmlyc3QgZW50ZXJpbmcgYSBuZWlnaGJvdXJob29kLCB0aGlzIHRydXRoIGlzIHNvIHdlbGwgZml4ZWQgaW4gdGhlIG1pbmRzIG9mIHRoZSBzdXJyb3VuZGluZyBmYW1pbGllcywgdGhhdCBoZSBpcyBjb25zaWRlcmVkIHRoZSByaWdodGZ1bCBwcm9wZXJ0eSBvZiBzb21lIG9uZSBvciBvdGhlciBvZiB0aGVpciBkYXVnaHRlcnMuDQoNCiJNeSBkZWFyIE1yLiBCZW5uZXQsIiBzYWlkIGhpcyBsYWR5IHRvIGhpbSBvbmUgZGF5LCAiaGF2ZSB5b3UgaGVhcmQgdGhhdCBOZXRoZXJmaWVsZCBQYXJrIGlzIGxldCBhdCBsYXN0PyINCg0KTXIuIEJlbm5ldCByZXBsaWVkIHRoYXQgaGUgaGFkIG5vdC4NCg0KIkJ1dCBpdCBpcywiIHJldHVybmVkIHNoZTsgImZvciBNcnMuIExvbmcgaGFzIGp1c3QgYmVlbiBoZXJlLCBhbmQgc2hlIHRvbGQgbWUgYWxsIGFib3V0IGl0LiINCg0KTXIuIEJlbm5ldCBtYWRlIG5vIGFuc3dlci4NCg0KIkRvIHlvdSBub3Qgd2FudCB0byBrbm93IHdobyBoYXMgdGFrZW4gaXQ/IiBjcmllZCBoaXMgd2lmZSBpbXBhdGllbnRseS4=)";

    constexpr std::string_view ENCODED_WITH_LINE_BREAKS = R"(SXQgaXMgYSB0cnV0aCB1bml2ZXJzYWxseSBhY2tub3dsZWRnZWQsIHRoYXQgYSBzaW5nbGUgbWFu
IGluIHBvc3Nlc3Npb24gb2YgYSBnb29kIGZvcnR1bmUsIG11c3QgYmUgaW4gd2FudCBvZiBhIHdp
ZmUuDQoNCkhvd2V2ZXIgbGl0dGxlIGtub3duIHRoZSBmZWVsaW5ncyBvciB2aWV3cyBvZiBzdWNo
IGEgbWFuIG1heSBiZSBvbiBoaXMgZmlyc3QgZW50ZXJpbmcgYSBuZWlnaGJvdXJob29kLCB0aGlz
IHRydXRoIGlzIHNvIHdlbGwgZml4ZWQgaW4gdGhlIG1pbmRzIG9mIHRoZSBzdXJyb3VuZGluZyBm
YW1pbGllcywgdGhhdCBoZSBpcyBjb25zaWRlcmVkIHRoZSByaWdodGZ1bCBwcm9wZXJ0eSBvZiBz
b21lIG9uZSBvciBvdGhlciBvZiB0aGVpciBkYXVnaHRlcnMuDQoNCiJNeSBkZWFyIE1yLiBCZW5u
ZXQsIiBzYWlkIGhpcyBsYWR5IHRvIGhpbSBvbmUgZGF5LCAiaGF2ZSB5b3UgaGVhcmQgdGhhdCBO
ZXRoZXJmaWVsZCBQYXJrIGlzIGxldCBhdCBsYXN0PyINCg0KTXIuIEJlbm5ldCByZXBsaWVkIHRo
YXQgaGUgaGFkIG5vdC4NCg0KIkJ1dCBpdCBpcywiIHJldHVybmVkIHNoZTsgImZvciBNcnMuIExv
bmcgaGFzIGp1c3QgYmVlbiBoZXJlLCBhbmQgc2hlIHRvbGQgbWUgYWxsIGFib3V0IGl0LiINCg0K
TXIuIEJlbm5ldCBtYWRlIG5vIGFuc3dlci4NCg0KIkRvIHlvdSBub3Qgd2FudCB0byBrbm93IHdo
byBoYXMgdGFrZW4gaXQ/IiBjcmllZCBoaXMgd2lmZSBpbXBhdGllbnRseS4=
)";

    [[nodiscard]]
    std::span<unsigned char const> get_decoded_bytes();

    [[nodiscard]]
    std::span<unsigned char const> get_encoded_bytes();
    
    [[nodiscard]]
    std::span<unsigned char const> get_encoded_with_line_breaks_bytes();

}
