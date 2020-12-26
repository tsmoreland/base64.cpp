module;

#include <eh.h>

export module moreland.base64.shared:scoped_se_translator;

export namespace moreland::base64::shared 
{
    class scoped_se_translator final
    {
        using translator_function = _se_translator_function;
        translator_function const previous_handler_;
    public:
        scoped_se_translator(translator_function translator) noexcept;
        ~scoped_se_translator();
        scoped_se_translator(scoped_se_translator const&) = delete;
        scoped_se_translator(scoped_se_translator &&) noexcept = delete;

        scoped_se_translator& operator=(scoped_se_translator const&) = delete;
        scoped_se_translator& operator=(scoped_se_translator &&) noexcept = delete;

    private:

    };

    scoped_se_translator::scoped_se_translator(translator_function translator) noexcept
        : previous_handler_{_set_se_translator(translator)}
    {
    }
    scoped_se_translator::~scoped_se_translator()
    {
        _set_se_translator(previous_handler_);
    }
}
