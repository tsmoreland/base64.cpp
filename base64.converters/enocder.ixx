module;

#include "library_export.h"
//#include "encoder.h"

export module moreland.base64.converters:encoder;

import <vector>;
import <optional>;
import <string>;
import <span>;

#pragma warning(push)
#pragma warning(disable : 4251)
export namespace moreland::base64::converters
{
    using byte = unsigned char;
    using std::optional;
    using std::nullopt;
    using std::vector;
    using std::span;

    class BASE64_ENCODER_EXPORT encoder final 
    {
        bool is_url_;
        optional<vector<byte>> newline_;
        optional<int const> line_max_;
        bool do_padding_;

    public:
        using size_type = vector<byte>::size_type;

        explicit encoder(bool is_url, optional<vector<byte>> newline, optional<int const> line_max, bool do_padding) noexcept;
        ~encoder() = default;
        encoder(encoder const&) = default;
        encoder(encoder &&) noexcept = default;
        encoder& operator=(encoder const&) = default;
        encoder& operator=(encoder &&) noexcept = default;


        [[nodiscard]]
        optional<vector<byte>> encode(vector<byte> const& source) const;

        [[nodiscard]]
        size_type encode(vector<byte> const& source, vector<byte>& destintation) const;

        [[nodiscard]]
        std::string encode_to_string_or_empty(vector<byte> const& source) const;

    private:
        [[nodiscard]]
        bool validate_encode_source(span<byte const> const source) const noexcept;

        [[nodiscard]]
        optional<size_type> get_output_length(span<byte const> const source) const noexcept;

        [[nodiscard]]
        static optional<size_type> calculate_output_length(span<byte const> const source, bool const insert_line_breaks, size_type const new_line_size);
    };

}

#pragma warning(pop)
