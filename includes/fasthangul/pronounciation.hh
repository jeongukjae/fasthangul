#ifndef __FH_PRONOUNCIATION_H__
#define __FH_PRONOUNCIATION_H__

#include <string>
#include <vector>

#include "fasthangul/jamo.hh"

namespace fasthangul {
namespace pronounciation {

// '각' 이 composed이라면,
// decomposed는 "ㄱㅏㄱ"
struct DecomposedChar {
  wchar_t composed;
  std::wstring decomposed;
};

bool operator==(const DecomposedChar& lhs, const DecomposedChar& rhs) {
  return lhs.composed == rhs.composed && lhs.decomposed == rhs.decomposed;
}

std::wstring convertPronounciation(std::wstring text);
void convertJongsungPronounciation(std::vector<DecomposedChar>& text);

std::vector<DecomposedChar> decomposeText(std::wstring text,
                                          jamo::JamoConverter converter = jamo::defaultJamoConverter);
// text가 'ㅇ'이 아닌 초성으로 시작하는지
// 여기서 text는 자모로 분리된 string
bool startsWithJaeum(std::wstring text);
// DecomposedChar의 초성을 교체
void replaceChosung(DecomposedChar& character, wchar_t chosung);
// DecomposedChar의 받침을 교체
void replaceJongsung(DecomposedChar& character, wchar_t jongsung);
void removeJongsung(DecomposedChar& character);

}  // namespace pronounciation
}  // namespace fasthangul

#endif  // __FH_PRONOUNCIATION_H__
