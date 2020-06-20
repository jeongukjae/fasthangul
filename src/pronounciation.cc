#include "fasthangul/pronounciation.hh"

#include <algorithm>
#include <map>
#include <numeric>
#include <set>

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
// 12항에서 사용하는 종성
const std::map<wchar_t, wchar_t> RULE_12_JONGSUNG = {{L'ㅎ', L'\0'}, {L'ㅀ', L'ㄹ'}, {L'ㄶ', L'ㄴ'}};
// 제 13항에서 사용하는 종성으로, 홑받침 혹은 쌍받침
const std::set<wchar_t> SINGLE_OR_DOUBLE_JONGSUNG = {L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ', L'ㄹ', L'ㅁ', L'ㅂ', L'ㅅ',
                                                     L'ㅆ', L'ㅈ', L'ㅊ', L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ'};
// 제 14항에서 사용하는 종성으로, 겹받침
const std::map<wchar_t, std::pair<wchar_t, wchar_t>> COMPOSED_JONGSUNG = {
    {L'ㄵ', {L'ㄴ', L'ㅈ'}}, {L'ㄶ', {L'ㄴ', L'ㅎ'}}, {L'ㄳ', {L'ㄱ', L'ㅆ'}}, {L'ㄺ', {L'ㄹ', L'ㄱ'}},
    {L'ㄻ', {L'ㄹ', L'ㅁ'}}, {L'ㄼ', {L'ㄹ', L'ㅂ'}}, {L'ㄽ', {L'ㄹ', L'ㅆ'}}, {L'ㄾ', {L'ㄹ', L'ㅌ'}},
    {L'ㄿ', {L'ㄹ', L'ㅍ'}}, {L'ㅀ', {L'ㄹ', L'ㅎ'}}, {L'ㅄ', {L'ㅂ', L'ㅆ'}}};
// 제 16항에서 사용하는 특수 패턴들
const std::map<std::wstring, wchar_t> RULE_16_EXCEPTION = {{L"디귿", L'ㅅ'}, {L"지읒", L'ㅅ'}, {L"치읓", L'ㅅ'},
                                                           {L"키읔", L'ㄱ'}, {L"티읕", L'ㅅ'}, {L"피읖", L'ㅂ'},
                                                           {L"히읗", L'ㅅ'}};
// 제 18항
const std::map<wchar_t, wchar_t> RULE_18_PATTERN = {{L'ㄱ', L'ㅇ'}, {L'ㄷ', L'ㄴ'}, {L'ㅂ', L'ㅁ'}};
// 제 19항
const std::set<wchar_t> RULE_19_JONGSUNG = {L'ㅁ', L'ㅇ', L'ㄱ', L'ㅂ'};
// 제 20항
const std::set<wchar_t> RULE_20_JONGSUNG = {L'ㄹ', L'ㅀ', L'ㄾ'};

std::wstring convertPronounciation(std::wstring text) {
  std::vector<DecomposedChar> textVector = decomposeText(text);

  convertJongsungPronounciation(textVector);
  convertAssimilationPronounciation(textVector);

  return composeText(textVector);
}

void convertJongsungPronounciation(std::vector<DecomposedChar>& textVector) {
  std::map<wchar_t, wchar_t>::const_iterator iterator;
  std::map<wchar_t, std::pair<wchar_t, wchar_t>>::const_iterator pairIterator;
  std::map<std::wstring, wchar_t>::const_iterator wstringIterator;

  for (size_t i = 0; i < textVector.size(); i++) {
    auto text = textVector[i];
    if (text.decomposed.size() == 3) {
      // 종성이 존재하는 경우

      bool isLastChar = (i == textVector.size() - 1);
      bool isEomal = isLastChar || (textVector[i + 1].decomposed.size() == 1);

      // 제 16항
      if (!isEomal && i > 0 && textVector[i + 1].decomposed[0] == L'ㅇ' &&
          (wstringIterator = RULE_16_EXCEPTION.find(std::wstring{textVector[i - 1].composed, text.composed})) !=
              RULE_16_EXCEPTION.end()) {
        removeJongsung(textVector[i]);
        replaceChosung(textVector[i + 1], wstringIterator->second);
      }

      // 9항, 10항, 11항의 받침으로 끝나고, (어말이거나, 자음 앞인경우)
      else if ((iterator = RULE_JONGSUNG.find(text.decomposed[2])) != RULE_JONGSUNG.end() &&
               (isEomal || startsWithJaeum(textVector[i + 1].decomposed))) {
        replaceJongsung(textVector[i], iterator->second);
      }

      // 10항의 받침 중 ㄼ이면서, 어말이 아니고, (밟 + 자음) 혹은 (넓죽, 넓둥) 인 경우
      else if (text.decomposed[2] == L'ㄼ') {
        if (!isEomal && ((text.composed == L'밟' && startsWithJaeum(textVector[i + 1].decomposed)) ||
                         (text.composed == L'넓' &&
                          (textVector[i + 1].composed == L'죽' || textVector[i + 1].composed == L'둥')))) {
          replaceJongsung(textVector[i], L'ㅂ');
        } else {
          replaceJongsung(textVector[i], L'ㄹ');
        }
      }

      // 11항의 받침 중 ㄺ이면서, 어말이거나, 자음 앞인 경우
      else if (text.decomposed[2] == L'ㄺ' && (isEomal || startsWithJaeum(textVector[i + 1].decomposed))) {
        // TODO
        // 여기서 용언의 어간 말음이 'ㄺ' 이면서 'ㄱ'앞일 때이지만,
        // 용언의 어간 말음 판정이 힘들어 지금 당장은 다음 초성만을 판정함
        if (!isEomal && (textVector[i + 1].decomposed[0] == L'ㄱ')) {
          replaceJongsung(textVector[i], L'ㄹ');
        } else {
          replaceJongsung(textVector[i], L'ㄱ');
        }
      }

      // 12항
      else if ((iterator = RULE_12_JONGSUNG.find(text.decomposed[2])) != RULE_12_JONGSUNG.end()) {
        // 어말이 아닐 때 (끝 문자가 아니고, 뒤 단어가 decomposed 될 때 (한글일 때))
        if (!isEomal && textVector[i + 1].decomposed.size() > 1) {
          switch (textVector[i + 1].decomposed[0]) {
            // 1. ‘ㅎ(ㄶ, ㅀ)’ 뒤에 ‘ㄱ, ㄷ, ㅈ’이 결합되는 경우에는, 뒤 음절 첫소리와 합쳐서 [ㅋ, ㅌ, ㅊ]으로 발음한다.
            // TODO: 붙임 1, 2
            case L'ㄱ':
              replaceJongsung(textVector[i], iterator->second);
              replaceChosung(textVector[i + 1], L'ㅋ');
              break;
            case L'ㄷ':
              replaceJongsung(textVector[i], iterator->second);
              replaceChosung(textVector[i + 1], L'ㅌ');
              break;
            case L'ㅈ':
              replaceJongsung(textVector[i], iterator->second);
              replaceChosung(textVector[i + 1], L'ㅊ');
              break;
            // 2. ‘ㅎ(ㄶ, ㅀ)’ 뒤에 ‘ㅅ’이 결합되는 경우에는, ‘ㅅ’을 [ㅆ]으로 발음한다.
            case L'ㅅ':
              replaceJongsung(textVector[i], iterator->second);
              replaceChosung(textVector[i + 1], L'ㅆ');
              break;
            // 3. ‘ㅎ’ 뒤에 ‘ㄴ’이 결합되는 경우에는, [ㄴ]으로 발음한다.
            case L'ㄴ':
              if (text.decomposed[2] == L'ㅎ')
                replaceJongsung(textVector[i], L'ㄴ');
              else
                // ‘ㄶ, ㅀ’ 뒤에 ‘ㄴ’이 결합되는 경우에는, ‘ㅎ’을 발음하지 않는다.
                replaceJongsung(textVector[i], iterator->second);
              break;
            // 4. ‘ㅎ(ㄶ, ㅀ)’ 뒤에 모음으로 시작된 어미나 접미사가 결합되는 경우에는, ‘ㅎ’을 발음하지 않는다.
            case L'ㅇ':
              removeJongsung(textVector[i]);
              if (iterator->second != L'\0')
                replaceChosung(textVector[i + 1], iterator->second);
              break;
            default:
              if (iterator->first == L'ㅎ')
                replaceJongsung(textVector[i], L'ㄷ');
              else
                replaceJongsung(textVector[i], iterator->second);
          }
        } else {
          if (iterator->first == L'ㅎ')
            // 어말의 받침이 ㅎ인 경우
            // https://www.korean.go.kr/front/onlineQna/onlineQnaView.do?mn_id=216&qna_seq=39360
            replaceJongsung(textVector[i], L'ㄷ');
          else
            replaceJongsung(textVector[i], iterator->second);
        }
      }
      // 13항 다음 문자가 모음으로 시작할 때 종성을 다음 문자의 초성으로 옮겨주기
      else if (!isEomal && textVector[i + 1].decomposed[0] == L'ㅇ' &&
               (SINGLE_OR_DOUBLE_JONGSUNG.find(text.decomposed[2]) != SINGLE_OR_DOUBLE_JONGSUNG.end())) {
        if (textVector[i + 1].decomposed[1] == L'ㅣ') {
          switch (text.decomposed[2]) {
            // 제 17항 'ㄷ, ㅌ(ㄾ)'이 'ㅣ'을 만날 때는 'ㅈ', 'ㅊ'으로 바꾸어서 넘겨주자
            case L'ㄷ':
              replaceChosung(textVector[i + 1], L'ㅈ');
              removeJongsung(textVector[i]);
              break;
            case L'ㅌ':
            case L'ㄾ':
              replaceChosung(textVector[i + 1], L'ㅊ');
              removeJongsung(textVector[i]);
              break;
            default:
              // 원래의 13항
              replaceChosung(textVector[i + 1], text.decomposed[2]);
              removeJongsung(textVector[i]);
          }
        } else {
          // 원래의 13항
          replaceChosung(textVector[i + 1], text.decomposed[2]);
          removeJongsung(textVector[i]);
        }
      }
      // 제 14항
      else if (!isEomal && (pairIterator = COMPOSED_JONGSUNG.find(text.decomposed[2])) != COMPOSED_JONGSUNG.end()) {
        replaceJongsung(textVector[i], pairIterator->second.first);
        replaceChosung(textVector[i + 1], pairIterator->second.second);
      }
      // 제 15항
      // 받침 뒤에 모음 ‘ㅏ, ㅓ, ㅗ, ㅜ, ㅟ’들로 시작되는 실질 형태소가 연결되는 경우에는, 대표음으로 바꾸어서 뒤 음절
      // 첫소리로 옮겨 발음한다.
      //
      // "밭 아래[바다래]", "늪 앞[느밥]" 같은 예시가 있어서 구현하기 까다로워 TODO 로 남겨놓음
    }
  }
}

void convertAssimilationPronounciation(std::vector<DecomposedChar>& textVector) {
  std::map<wchar_t, wchar_t>::const_iterator iterator;

  for (size_t i = 0; i < textVector.size(); i++) {
    auto text = textVector[i];
    if (text.decomposed.size() == 3) {
      bool isLastChar = (i == textVector.size() - 1);
      bool isEomal = isLastChar || (textVector[i + 1].decomposed.size() == 1);

      // 제 19항
      // 받침 ‘ㅁ, ㅇ, ㄱ, ㅂ’ 뒤에 연결되는 ‘ㄹ’은 [ㄴ]으로 발음한다.
      if (!isEomal && (RULE_19_JONGSUNG.find(text.decomposed[2]) != RULE_19_JONGSUNG.end()) &&
          textVector[i + 1].decomposed[0] == L'ㄹ') {
        replaceChosung(textVector[i + 1], L'ㄴ');
      }

      // 제 17항 [붙임] ‘ㄷ’ 뒤에 접미사 ‘히’가 결합되어 ‘티’를 이루는 것은 [치]로 발음한다.
      if (!isEomal && text.decomposed[2] == L'ㄷ' && textVector[i + 1].composed == L'히') {
        removeJongsung(textVector[i]);
        replaceChosung(textVector[i + 1], L'ㅊ');
      }

      // 18항
      // 받침 ‘ㄱ(ㄲ, ㅋ, ㄳ, ㄺ), ㄷ(ㅅ, ㅆ, ㅈ, ㅊ, ㅌ, ㅎ), ㅂ(ㅍ, ㄼ, ㄿ, ㅄ)’은 ‘ㄴ, ㅁ’ 앞에서 [ㅇ, ㄴ, ㅁ]으로
      // 발음한다. 위의 함수에서 이미 ㄱ, ㄷ, ㅂ으로 변환하고 난 후이니 ㄱ, ㄷ, ㅂ만 체크한다.
      else if (!isEomal && (textVector[i + 1].decomposed[0] == L'ㄴ' || textVector[i + 1].decomposed[0] == L'ㅁ') &&
               (iterator = RULE_18_PATTERN.find(text.decomposed[2])) != RULE_18_PATTERN.end()) {
        replaceJongsung(textVector[i], iterator->second);
      }

      // 제 20항
      // ‘ㄴ’은 ‘ㄹ’의 앞이나 뒤에서 [ㄹ]로 발음한다.
      // TODO 붙임의 예외 케이스 확인
      if (!isEomal && (RULE_20_JONGSUNG.find(text.decomposed[2]) != RULE_20_JONGSUNG.end()) &&
          textVector[i + 1].decomposed[0] == L'ㄴ') {
        replaceChosung(textVector[i + 1], L'ㄹ');
      }
      if (!isEomal && text.decomposed[2] == L'ㄴ' && textVector[i + 1].decomposed[0] == L'ㄹ') {
        replaceJongsung(textVector[i], L'ㄹ');
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

std::wstring composeText(std::vector<DecomposedChar> decomposed) {
  return std::accumulate(decomposed.begin(), decomposed.end(), std::wstring(L""),
                         [](const std::wstring init, const DecomposedChar second) { return init + second.composed; });
}

bool startsWithJaeum(std::wstring text) {
  return text.size() > 0 && jamo::isChosung(text[0]) && text[0] != L'ㅇ';
}

void replaceChosung(DecomposedChar& character, wchar_t chosung) {
  if (character.decomposed.size() == 3) {
    character.composed =
        jamo::defaultJamoConverter.getOneHangulFromJamo(chosung, character.decomposed[1], character.decomposed[2]);
    character.decomposed = std::wstring{chosung, character.decomposed[1], character.decomposed[2]};
  } else {
    character.composed = jamo::defaultJamoConverter.getOneHangulFromJamo(chosung, character.decomposed[1]);
    character.decomposed = std::wstring{chosung, character.decomposed[1]};
  }
}

void replaceJongsung(DecomposedChar& character, wchar_t jongsung) {
  if (jongsung == '\0') {
    removeJongsung(character);
  } else {
    character.composed =
        jamo::defaultJamoConverter.getOneHangulFromJamo(character.decomposed[0], character.decomposed[1], jongsung);
    character.decomposed = std::wstring{character.decomposed[0], character.decomposed[1], jongsung};
  }
}

void removeJongsung(DecomposedChar& character) {
  character.composed =
      jamo::defaultJamoConverter.getOneHangulFromJamo(character.decomposed[0], character.decomposed[1]);
  character.decomposed = std::wstring{character.decomposed[0], character.decomposed[1]};
}

}  // namespace pronounciation
}  // namespace fasthangul
