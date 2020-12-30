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

    // new lines will be seen as lf ('\n') not crlf ('\r\n')
    constexpr std::string_view DECODED = R"(It is a truth universally acknowledged, that a single man in possession of a good fortune, must be in want of a wife.

However little known the feelings or views of such a man may be on his first entering a neighbourhood, this truth is so well fixed in the minds of the surrounding families, that he is considered the rightful property of some one or other of their daughters.

"My dear Mr. Bennet," said his lady to him one day, "have you heard that Netherfield Park is let at last?"

Mr. Bennet replied that he had not.

"But it is," returned she; "for Mrs. Long has just been here, and she told me all about it."

Mr. Bennet made no answer.

"Do you not want to know who has taken it?" cried his wife impatiently.)";

    constexpr std::string_view ENCODED = R"(SXQgaXMgYSB0cnV0aCB1bml2ZXJzYWxseSBhY2tub3dsZWRnZWQsIHRoYXQgYSBzaW5nbGUgbWFuIGluIHBvc3Nlc3Npb24gb2YgYSBnb29kIGZvcnR1bmUsIG11c3QgYmUgaW4gd2FudCBvZiBhIHdpZmUuCgpIb3dldmVyIGxpdHRsZSBrbm93biB0aGUgZmVlbGluZ3Mgb3Igdmlld3Mgb2Ygc3VjaCBhIG1hbiBtYXkgYmUgb24gaGlzIGZpcnN0IGVudGVyaW5nIGEgbmVpZ2hib3VyaG9vZCwgdGhpcyB0cnV0aCBpcyBzbyB3ZWxsIGZpeGVkIGluIHRoZSBtaW5kcyBvZiB0aGUgc3Vycm91bmRpbmcgZmFtaWxpZXMsIHRoYXQgaGUgaXMgY29uc2lkZXJlZCB0aGUgcmlnaHRmdWwgcHJvcGVydHkgb2Ygc29tZSBvbmUgb3Igb3RoZXIgb2YgdGhlaXIgZGF1Z2h0ZXJzLgoKIk15IGRlYXIgTXIuIEJlbm5ldCwiIHNhaWQgaGlzIGxhZHkgdG8gaGltIG9uZSBkYXksICJoYXZlIHlvdSBoZWFyZCB0aGF0IE5ldGhlcmZpZWxkIFBhcmsgaXMgbGV0IGF0IGxhc3Q/IgoKTXIuIEJlbm5ldCByZXBsaWVkIHRoYXQgaGUgaGFkIG5vdC4KCiJCdXQgaXQgaXMsIiByZXR1cm5lZCBzaGU7ICJmb3IgTXJzLiBMb25nIGhhcyBqdXN0IGJlZW4gaGVyZSwgYW5kIHNoZSB0b2xkIG1lIGFsbCBhYm91dCBpdC4iCgpNci4gQmVubmV0IG1hZGUgbm8gYW5zd2VyLgoKIkRvIHlvdSBub3Qgd2FudCB0byBrbm93IHdobyBoYXMgdGFrZW4gaXQ/IiBjcmllZCBoaXMgd2lmZSBpbXBhdGllbnRseS4=)";

    constexpr std::string_view ENCODED_WITH_LINE_BREAKS = 
        "SXQgaXMgYSB0cnV0aCB1bml2ZXJzYWxseSBhY2tub3dsZWRnZWQsIHRoYXQgYSBzaW5nbGUgbWFu\r\n"
        "IGluIHBvc3Nlc3Npb24gb2YgYSBnb29kIGZvcnR1bmUsIG11c3QgYmUgaW4gd2FudCBvZiBhIHdp\r\n"
        "ZmUuDQoNCkhvd2V2ZXIgbGl0dGxlIGtub3duIHRoZSBmZWVsaW5ncyBvciB2aWV3cyBvZiBzdWNo\r\n"
        "IGEgbWFuIG1heSBiZSBvbiBoaXMgZmlyc3QgZW50ZXJpbmcgYSBuZWlnaGJvdXJob29kLCB0aGlz\r\n"
        "IHRydXRoIGlzIHNvIHdlbGwgZml4ZWQgaW4gdGhlIG1pbmRzIG9mIHRoZSBzdXJyb3VuZGluZyBm\r\n"
        "YW1pbGllcywgdGhhdCBoZSBpcyBjb25zaWRlcmVkIHRoZSByaWdodGZ1bCBwcm9wZXJ0eSBvZiBz\r\n"
        "b21lIG9uZSBvciBvdGhlciBvZiB0aGVpciBkYXVnaHRlcnMuDQoNCiJNeSBkZWFyIE1yLiBCZW5u\r\n"
        "ZXQsIiBzYWlkIGhpcyBsYWR5IHRvIGhpbSBvbmUgZGF5LCAiaGF2ZSB5b3UgaGVhcmQgdGhhdCBO\r\n"
        "ZXRoZXJmaWVsZCBQYXJrIGlzIGxldCBhdCBsYXN0PyINCg0KTXIuIEJlbm5ldCByZXBsaWVkIHRo\r\n"
        "YXQgaGUgaGFkIG5vdC4NCg0KIkJ1dCBpdCBpcywiIHJldHVybmVkIHNoZTsgImZvciBNcnMuIExv\r\n"
        "bmcgaGFzIGp1c3QgYmVlbiBoZXJlLCBhbmQgc2hlIHRvbGQgbWUgYWxsIGFib3V0IGl0LiINCg0K\r\n"
        "TXIuIEJlbm5ldCBtYWRlIG5vIGFuc3dlci4NCg0KIkRvIHlvdSBub3Qgd2FudCB0byBrbm93IHdo\r\n"
        "byBoYXMgdGFrZW4gaXQ/IiBjcmllZCBoaXMgd2lmZSBpbXBhdGllbnRseS4=\r\n";

    [[nodiscard]]
    std::vector<unsigned char> get_decoded_bytes();

    [[nodiscard]]
    std::vector<unsigned char> get_encoded_bytes();
    
    [[nodiscard]]
    std::vector<unsigned char> get_encoded_with_line_breaks_bytes();

}
