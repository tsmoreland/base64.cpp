module;

#include "library_export.h"
#include "encoder.h"

export module moreland_base64_converters:encoder;

#pragma warning(push)
#pragma warning(disable : 4251)
export namespace moreland::base64::converters
{

    class BASE64_ENCODER_EXPORT encoder final 
    {
        bool is_url_;
        optional<vector<byte>> newline_;
        optional<int const> line_max_;
        bool do_padding_;

    public:
        using size_type = vector<byte>::size_type;

        explicit encoder(bool is_url, optional<vector<byte>> newline, optional<int const> line_max, bool do_padding) noexcept;

        [[nodiscard]]
        optional<vector<byte>> encode(vector<byte> const& source) const;

        [[nodiscard]]
        size_type encode(vector<byte> const& source, vector<byte>& destintation) const;

        [[nodiscard]]
        std::string encode_to_string_or_empty(vector<byte> const& source) const;

    private:
        [[nodiscard]]
        optional<size_type> get_output_length(span<byte const> const source) const noexcept;

    };

    export 
    [[nodiscard]]
    BASE64_ENCODER_EXPORT encoder const& get_encoder() noexcept;

    export 
    [[nodiscard]]
    BASE64_ENCODER_EXPORT encoder const& get_url_encoder() noexcept;


}

#pragma warning(pop)
