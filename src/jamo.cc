#include "fasthangul/jamo.hh"

#include <algorithm>
#include <numeric>
#include <set>
#include <string>
#include <vector>

#define FIRST_HANGUL L'가'
#define LAST_HANGUL L'힣'

static const wchar_t CHOSUNG[] = {L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ', L'ㄸ', L'ㄹ', L'ㅁ', L'ㅂ', L'ㅃ', L'ㅅ',
                                  L'ㅆ', L'ㅇ', L'ㅈ', L'ㅉ', L'ㅊ', L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ'};
static const wchar_t JUNGSUNG[] = {L'ㅏ', L'ㅐ', L'ㅑ', L'ㅒ', L'ㅓ', L'ㅔ', L'ㅕ', L'ㅖ', L'ㅗ', L'ㅘ', L'ㅙ',
                                   L'ㅚ', L'ㅛ', L'ㅜ', L'ㅝ', L'ㅞ', L'ㅟ', L'ㅠ', L'ㅡ', L'ㅢ', L'ㅣ'};
static const wchar_t JONGSUNG[] = {L'\0', L'ㄱ', L'ㄲ', L'ㄳ', L'ㄴ', L'ㄵ', L'ㄶ', L'ㄷ', L'ㄹ', L'ㄺ',
                                   L'ㄻ', L'ㄼ', L'ㄽ', L'ㄾ', L'ㄿ', L'ㅀ', L'ㅁ', L'ㅂ', L'ㅄ', L'ㅅ',
                                   L'ㅆ', L'ㅇ', L'ㅈ', L'ㅊ', L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ'};

static const std::set<wchar_t> CHOSUNG_SET{CHOSUNG, CHOSUNG + 19};
static const std::set<wchar_t> JONGSUNG_SET{JONGSUNG + 1, JONGSUNG + 28};

enum Composing {
  C_KEEP,            // 그냥 그대로 합치는 문자
  C_IGNORE,          // 무시하는 문자
  C_COMPOSING,       // 중성인 경우 앞만 합칠 때
  C_COMPOSING_BOTH,  // 중성인 경우 앞 뒤 전부 합칠 때
};

void fasthangul::jamo::JamoConverter::initializeJamos(bool fillEmptyJongsung, wchar_t emptyJongsung) {
  PRECOMPUTED_JAMOS.clear();
  CHOSUNG_MAP.clear();
  JONGSUNG_MAP.clear();

  wchar_t totalHangulCount = LAST_HANGUL - FIRST_HANGUL + 1;
  for (wchar_t charIndex = 0; charIndex < totalHangulCount; ++charIndex) {
    wchar_t chosungIndex = charIndex / 28 / 21;
    wchar_t jungsungIndex = charIndex / 28 % 21;
    wchar_t jongsungIndex = charIndex % 28;

    if (fillEmptyJongsung && jongsungIndex == 0) {
      PRECOMPUTED_JAMOS[FIRST_HANGUL + charIndex] =
          std::wstring({CHOSUNG[chosungIndex], JUNGSUNG[jungsungIndex], emptyJongsung});
    } else if (jongsungIndex != 0) {
      PRECOMPUTED_JAMOS[FIRST_HANGUL + charIndex] =
          std::wstring({CHOSUNG[chosungIndex], JUNGSUNG[jungsungIndex], JONGSUNG[jongsungIndex]});
    } else {
      PRECOMPUTED_JAMOS[FIRST_HANGUL + charIndex] = std::wstring({CHOSUNG[chosungIndex], JUNGSUNG[jungsungIndex]});
    }
  }

  for (int i = 0; i < 19; ++i) {
    CHOSUNG_MAP[CHOSUNG[i]] = i;
  }
  for (int i = 1; i < 28; ++i) {
    JONGSUNG_MAP[JONGSUNG[i]] = i;
  }
}

std::wstring fasthangul::jamo::JamoConverter::compose(std::wstring text) const {
  std::wstring resultString{};
  const size_t textLength = text.size();

  Composing* composing = new Composing[textLength]{
      C_KEEP,
  };
  size_t expectedLength = textLength;

  for (size_t i = 0; i < textLength; ++i) {
    wchar_t character = text.at(i);
    if (isJungsung(character)) {
      if (i != 0 and isChosung(text.at(i - 1))) {
        --expectedLength;
        composing[i - 1] = C_IGNORE;

        if (i <= textLength - 2 and isJongsung(text.at(i + 1)) and
            (i == textLength - 2 or !isJungsung(text.at(i + 2)))) {
          --expectedLength;
          composing[i + 1] = C_IGNORE;
          composing[i] = C_COMPOSING_BOTH;
        } else {
          composing[i] = C_COMPOSING;
        }
      }
    }
  }

  resultString.reserve(expectedLength);

  for (size_t i = 0; i < textLength; ++i) {
    if (composing[i] == C_IGNORE)
      continue;

    if (composing[i] == C_KEEP)
      resultString += text.at(i);
    else if (composing[i] == C_COMPOSING)
      resultString += getOneHangulFromJamo(text.at(i - 1), text.at(i));
    else if (composing[i] == C_COMPOSING_BOTH)
      resultString += getOneHangulFromJamo(text.at(i - 1), text.at(i), text.at(i + 1));
  }

  delete[] composing;
  return resultString;
}

std::wstring fasthangul::jamo::JamoConverter::decompose(std::wstring text) const {
  std::vector<std::wstring> stringsToJoin(text.size());
  std::vector<int> totalLength(text.size());
  std::wstring resultString{};

  std::transform(text.begin(), text.end(), stringsToJoin.begin(),
                 [this](const wchar_t character) { return getJamosFromHangul(character); });
  std::transform(stringsToJoin.begin(), stringsToJoin.end(), totalLength.begin(),
                 [](const std::wstring& chunk) { return chunk.length(); });

  resultString.reserve(std::accumulate(totalLength.begin(), totalLength.end(), 0));
  for (auto iter = stringsToJoin.begin(); iter != stringsToJoin.end(); ++iter)
    resultString.append(*iter);

  return resultString;
}

wchar_t fasthangul::jamo::JamoConverter::getOneHangulFromJamo(wchar_t chosung, wchar_t jungsung) const {
  const auto iterator = this->CHOSUNG_MAP.find(chosung);
  if (iterator == this->CHOSUNG_MAP.end())
    throw std::runtime_error("Cannot fetch chosung");

  wchar_t chosungIndex = iterator->second;
  wchar_t jungsungIndex = jungsung - L'ㅏ';

  return FIRST_HANGUL + 28 * (21 * chosungIndex + jungsungIndex);
}

wchar_t fasthangul::jamo::JamoConverter::getOneHangulFromJamo(wchar_t chosung,
                                                              wchar_t jungsung,
                                                              wchar_t jongsung) const {
  const auto chosungIterator = this->CHOSUNG_MAP.find(chosung);
  const auto jongsungIterator = this->JONGSUNG_MAP.find(jongsung);
  if (chosungIterator == this->CHOSUNG_MAP.end() || jongsungIterator == this->JONGSUNG_MAP.end())
    throw std::runtime_error("Cannot fetch chosung or jongsung");

  wchar_t chosungIndex = chosungIterator->second;
  wchar_t jungsungIndex = jungsung - L'ㅏ';
  wchar_t jongsungIndex = jongsungIterator->second;

  return FIRST_HANGUL + 28 * (21 * chosungIndex + jungsungIndex) + jongsungIndex;
}

std::wstring fasthangul::jamo::JamoConverter::getJamosFromHangul(const wchar_t character) const {
  if (isHangul(character)) {
    const auto jamos = this->PRECOMPUTED_JAMOS.find(character);
    if (jamos != this->PRECOMPUTED_JAMOS.end()) {
      return jamos->second;
    }
  }
  return std::wstring{character};
}

bool fasthangul::jamo::isHangul(const wchar_t character) {
  return character >= FIRST_HANGUL and character <= LAST_HANGUL;
}

bool fasthangul::jamo::isJamo(const wchar_t character) {
  return character >= L'ㄱ' and character <= L'ㅣ';
}

bool fasthangul::jamo::isChosung(const wchar_t character) {
  return CHOSUNG_SET.find(character) != CHOSUNG_SET.end();
}

bool fasthangul::jamo::isJungsung(const wchar_t character) {
  return character >= L'ㅏ' and character <= L'ㅣ';
}

bool fasthangul::jamo::isJongsung(const wchar_t character) {
  return JONGSUNG_SET.find(character) != JONGSUNG_SET.end();
}
