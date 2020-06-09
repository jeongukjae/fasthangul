#ifndef __FH_JAMO_H__
#define __FH_JAMO_H__

#include <unordered_map>
#include <vector>

#define DEFAULT_EMPTY_JONGSUNG L'ᴥ'

namespace fasthangul {
namespace jamo {

class JamoConverter {
public:
  void initializeJamos(bool fillEmptyJongsung = false, wchar_t emptyJongsung = DEFAULT_EMPTY_JONGSUNG);
  std::wstring compose(std::wstring hangul) const;
  std::wstring decompose(std::wstring hangul) const;

  wchar_t getOneHangulFromJamo(wchar_t chosung, wchar_t jungsung) const;
  wchar_t getOneHangulFromJamo(wchar_t chosung, wchar_t jungsung, wchar_t jongsung) const;
  std::wstring getJamosFromHangul(const wchar_t hangul) const;

protected:
  std::unordered_map<wchar_t, int> CHOSUNG_MAP;
  std::unordered_map<wchar_t, int> JONGSUNG_MAP;

  std::unordered_map<wchar_t, std::wstring> PRECOMPUTED_JAMOS;
};

bool isHangul(const wchar_t character);
bool isJamo(const wchar_t character);
bool isChosung(const wchar_t character);
bool isJungsung(const wchar_t character);
bool isJongsung(const wchar_t character);

} // namespace jamo
} // namespace fasthangul

#endif /* __FH_JAMO_H__ */
