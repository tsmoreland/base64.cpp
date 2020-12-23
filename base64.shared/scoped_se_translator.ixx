module;

#include <eh.h>

export module moreland.base64.shared:scoped_se_translator;

export class scoped_se_translator final
{
    using translator_function = _se_translator_function;
    translator_function const m_previous_handler;
public:
    scoped_se_translator(translator_function translator) noexcept;
    ~scoped_se_translator();
    scoped_se_translator(scoped_se_translator const&) = delete;
    scoped_se_translator(scoped_se_translator &&) noexcept = delete;

    scoped_se_translator& operator=(scoped_se_translator const&) = delete;
    scoped_se_translator& operator=(scoped_se_translator &&) noexcept = delete;
};

scoped_se_translator::scoped_se_translator(translator_function translator) noexcept
    : m_previous_handler{_set_se_translator(translator)}
{
}
scoped_se_translator::~scoped_se_translator()
{
    _set_se_translator(m_previous_handler);
}
