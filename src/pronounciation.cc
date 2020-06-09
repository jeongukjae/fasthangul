#include "fasthangul/pronounciation.hh"

#include <algorithm>
#include <map>

#include "fasthangul/jamo.hh"

namespace fasthangul {
namespace pronounciation {

// 제9항 받침 'ㄲ, ㅋ', 'ㅅ, ㅆ, ㅈ, ㅊ, ㅌ', 'ㅍ'은 어말 또는 자음 앞에서 각각 대표음 [ㄱ, ㄷ, ㅂ]으로 발음한다.
// 제10항 겹받침 'ㄳ', 'ㄵ', 'ㄼ, ㄽ, ㄾ', 'ㅄ'은 어말 또는 자음 앞에서 각각 [ㄱ, ㄴ, ㄹ, ㅂ]으로 발음한다.
// 제11항 겹받침 ‘ㄺ, ㄻ, ㄿ’은 어말 또는 자음 앞에서 각각 [ㄱ, ㅁ, ㅂ]으로 발음한다.
const std::map<wchar_t, wchar_t> RULE_JONGSUNG = {{L'ㄲ', L'ㄱ'}, {L'ㅋ', L'ㄱ'}, {L'ㅅ', L'ㄷ'}, {L'ㅆ', L'ㄷ'},
                                                  {L'ㅈ', L'ㄷ'}, {L'ㅊ', L'ㄷ'}, {L'ㅌ', L'ㄷ'}, {L'ㅍ', L'ㅂ'},
                                                  {L'ㄳ', L'ㄱ'}, {L'ㄵ', L'ㄴ'}, {L'ㄽ', L'ㄹ'}, {L'ㄾ', L'ㄹ'},
                                                  {L'ㅄ', L'ㅂ'}, {L'ㄻ', L'ㅁ'}, {L'ㄿ', L'ㅂ'}};

std::wstring convertPronounciation(std::wstring text) {}

void convertJongsungPronounciation(std::vector<DecomposedChar>& textVector) {
  for (size_t i = 0; i < textVector.size(); i++) {
    auto text = textVector[i];
    if (text.decomposed.size() == 3) {
      // 종성이 존재하는 경우

      // 9항의 받침 또는 10항의 받침, 11항의 받침으로 끝나고, (어말이거나, 자음 앞인경우)
      const auto iterator = RULE_JONGSUNG.find(text.decomposed[2]);
      if (iterator != RULE_JONGSUNG.end() &&
          (i == textVector.size() - 1 || startsWithJaeum(textVector[i + 1].decomposed))) {
        replaceJongsung(textVector[i], iterator->second);
      }

      // 10항의 받침 중 ㄼ이면서, 어말이 아니고, (밟 + 자음) 혹은 (넓죽, 넓둥) 인 경우
      else if (text.decomposed[2] == L'ㄼ') {
        if (i != textVector.size() - 1 && ((text.composed == L'밟' && startsWithJaeum(textVector[i + 1].decomposed)) ||
                                           (text.composed == L'넓' && (textVector[i + 1].composed == L'죽' ||
                                                                       textVector[i + 1].composed == L'둥')))) {
          replaceJongsung(textVector[i], L'ㅂ');
        } else {
          replaceJongsung(textVector[i], L'ㄹ');
        }
      }

      // 11항의 받침 중 ㄺ이면서, 어말이 아니고, 다음 초성이 'ㄱ'일때
      else if (text.decomposed[2] == L'ㄺ') {
        // TODO
        // 여기서 용언의 어간 말음이 'ㄺ' 이면서 'ㄱ'앞일 때이지만,
        // 용언의 어간 말음 판정이 힘들어 지금 당장은 다음 초성만을 판정함
        if ((i != textVector.size() - 1) && (textVector[i + 1].decomposed[0] == L'ㄱ')) {
          replaceJongsung(textVector[i], L'ㄹ');
        } else {
          replaceJongsung(textVector[i], L'ㄱ');
        }
      }
    }
  }
}

std::vector<DecomposedChar> decomposeText(std::wstring text, jamo::JamoConverter converter) {
  std::vector<DecomposedChar> decomposed;
  decomposed.reserve(text.size());

  std::transform(text.begin(), text.end(), std::back_inserter(decomposed), [converter](const wchar_t character) {
    return DecomposedChar{character, converter.getJamosFromHangul(character)};
  });

  return decomposed;
}

bool startsWithJaeum(std::wstring text) {
  return text.size() > 0 && jamo::isChosung(text[0]) && text[0] != L'ㅇ';
}

void replaceJongsung(DecomposedChar& character, wchar_t jongsung) {
  character.composed =
      jamo::defaultJamoConverter.getOneHangulFromJamo(character.decomposed[0], character.decomposed[1], jongsung);
  character.decomposed = std::wstring{character.decomposed[0], character.decomposed[1], jongsung};
}

}  // namespace pronounciation
}  // namespace fasthangul
