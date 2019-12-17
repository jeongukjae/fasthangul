#ifndef __FH_NORMALIZER_H__
#define __FH_NORMALIZER_H__

#include <string>

namespace fasthangul {
namespace normalizer {

std::wstring filterInvalidCharacter(std::wstring_view text);
std::wstring normalizeWhitespace(std::wstring text);

inline bool isControlChar(const wchar_t character) {
  if (character == L'\n' or character == L'\r' or character == L'\t')
    return false;

  return iscntrl(character);
}

inline bool isInvalidChar(const wchar_t character) {
  return character == L'\x0' or character == L'\xFFFD' or isControlChar(character);
}

} // namespace normalizer
} // namespace fasthangul

#endif /* __FH_NORMALIZER_H__ */
