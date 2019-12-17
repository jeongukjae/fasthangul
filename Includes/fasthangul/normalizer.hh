#ifndef __FH_NORMALIZER_H__
#define __FH_NORMALIZER_H__

#include <string>

namespace fasthangul {
namespace normalizer {

inline bool isControlChar(const wchar_t character) {
  if (character == L'\n' or character == L'\r' or character == L'\t')
    return false;

  return iscntrl(character);
}

std::wstring normalizerWhitespace(std::wstring_view text);

} // namespace normalizer
} // namespace fasthangul

#endif /* __FH_NORMALIZER_H__ */
