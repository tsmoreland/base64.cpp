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

#include "pch.h"
#include "file_byte_consumer.h"

using byte = unsigned char;
using lock_guard = std::lock_guard<std::mutex>;
using std::make_unique;

namespace moreland::base64::service
{
    constexpr std::size_t BUFFER_SIZE = 16384;

    file_byte_consumer::file_byte_consumer(std::filesystem::path const& file_path)
        : destination_{file_path, std::ios::out}
        , buffer_{std::make_unique<byte[]>(BUFFER_SIZE)}
    {
    }
    bool file_byte_consumer::consume(std::span<byte const> const source)
    {
        lock_guard guard{read_lock_};

        return static_cast<bool>(destination_.write(source.data(), source.size()));
    }

    void file_byte_consumer::flush()
    {
        lock_guard guard{read_lock_};
        destination_.flush();
    }

    void file_byte_consumer::reset()
    {
        lock_guard guard{read_lock_};
        destination_.seekg(0, std::fstream::beg);
    }
}
