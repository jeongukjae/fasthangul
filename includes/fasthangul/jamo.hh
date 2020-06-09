#ifndef __FH_JAMO_H__
#define __FH_JAMO_H__

#include <unordered_map>
#include <vector>

#define DEFAULT_EMPTY_JONGSUNG L'ᴥ'

namespace fasthangul {
namespace jamo {

void initializeJamos(bool fillEmptyJongsung = false, wchar_t emptyJongsung = DEFAULT_EMPTY_JONGSUNG);
std::wstring compose(std::wstring hangul);
std::wstring decompose(std::wstring hangul);

bool isHangul(const wchar_t character);
bool isJamo(const wchar_t character);
bool isChosung(const wchar_t character);
bool isJungsung(const wchar_t character);
bool isJongsung(const wchar_t character);
wchar_t getOneHangulFromJamo(wchar_t chosung, wchar_t jungsung);
wchar_t getOneHangulFromJamo(wchar_t chosung, wchar_t jungsung, wchar_t jongsung);
std::wstring getJamosFromHangul(const wchar_t hangul);

} // namespace jamo
} // namespace fasthangul

#endif /* __FH_JAMO_H__ */
