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
        explicit encoder(bool is_url, optional<vector<byte>> newline, optional<int const> line_max, bool do_padding) noexcept;



    };

    export 
    [[nodiscard]]
    BASE64_ENCODER_EXPORT encoder const& get_encoder() noexcept;

    export 
    [[nodiscard]]
    BASE64_ENCODER_EXPORT encoder const& get_url_encoder() noexcept;

}

#pragma warning(pop)
