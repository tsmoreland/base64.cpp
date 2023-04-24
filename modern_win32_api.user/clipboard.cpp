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
#include "clipboard.h"

namespace modern_win32_api::user {
    class clipboard final {
        using window_handle = HWND;
        window_handle handle_{nullptr};

    public:
        clipboard() : clipboard(GetDesktopWindow()) {}
        explicit clipboard(window_handle handle) : handle_{handle} {
            if (OpenClipboard(handle_) != TRUE)
                throw std::runtime_error("failed to open clipboard");
        }
        ~clipboard() {
            CloseClipboard();
        }

        clipboard(clipboard const&)                = default;
        clipboard(clipboard&&) noexcept            = default;
        clipboard& operator=(clipboard const&)     = default;
        clipboard& operator=(clipboard&&) noexcept = default;
    };

    class global_lock final {
        HANDLE locked_memory_;

    public:
        explicit global_lock(HANDLE handle) : locked_memory_{GlobalLock(handle)} {}
        ~global_lock() {
            GlobalUnlock(locked_memory_);
        }

        [[nodiscard]] HANDLE operator*() const {
            return locked_memory_;
        }
        [[nodiscard]] bool is_locked() const {
            return locked_memory_ != nullptr;
        }
        [[nodiscard]] explicit operator bool() const {
            return is_locked();
        }


        global_lock(global_lock const&)                = delete;
        global_lock(global_lock&&) noexcept            = default;
        global_lock& operator=(global_lock const&)     = delete;
        global_lock& operator=(global_lock&&) noexcept = delete;
    };

    bool set_clipboard(std::string_view data) {
        return set_clipboard(data, nullptr);
    }
    bool set_clipboard(std::string_view data, window_handle handle) {
        clipboard desktop{handle};
        auto* const global_mem = GlobalAlloc(GMEM_MOVEABLE, (data.size() + 1) * sizeof(char));
        if (global_lock const locked_memory(global_mem); locked_memory.is_locked()) {
            auto* destination = static_cast<char*>(*locked_memory);
            for (auto value : data) {
                (*destination++) = value;
            }
            *destination = '\0';
        } else {
            return false;
        }

        EmptyClipboard();
        if (!SetClipboardData(CF_TEXT, global_mem)) {
            return false;
        }

        return true;
    }

    std::optional<std::string> get_clipboard() {
        return get_clipboard(nullptr);
    }
    std::optional<std::string> get_clipboard(window_handle handle) {
        clipboard desktop{handle};

        if (auto* const data_handle = GetClipboardData(CF_TEXT); data_handle != nullptr) {

            if (global_lock const locked_memory(data_handle); locked_memory.is_locked()) {
                char const* const clipboard_data = static_cast<char*>(*locked_memory);
                return std::optional(clipboard_data);
            } else {
                return std::nullopt;
            }
        } else {
            return std::nullopt;
        }
    }
} // namespace modern_win32_api::user
