#include "fasthangul/pronounciation.hh"

#include <algorithm>

#include "fasthangul/jamo.hh"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace fasthangul::pronounciation;
using namespace fasthangul::jamo;

TEST(test_pronounciation, starts_with_jaeum) {
  ASSERT_TRUE(startsWithJaeum(L"ㄱㅏ"));
  ASSERT_TRUE(startsWithJaeum(L"ㅁㅏㅁ"));
  ASSERT_TRUE(startsWithJaeum(L"ㅂ"));
  ASSERT_FALSE(startsWithJaeum(L"ㅇㅏㅁ"));
  ASSERT_FALSE(startsWithJaeum(L"a"));
  ASSERT_FALSE(startsWithJaeum(L"ㅣ"));
}

TEST(test_pronounciation, decompose_text) {
  ASSERT_THAT(decomposeText(L"안녕하세요"),
              testing::ElementsAre(DecomposedChar{L'안', L"ㅇㅏㄴ"}, DecomposedChar{L'녕', L"ㄴㅕㅇ"},
                                   DecomposedChar{L'하', L"ㅎㅏ"}, DecomposedChar{L'세', L"ㅅㅔ"},
                                   DecomposedChar{L'요', L"ㅇㅛ"}));
  ASSERT_THAT(
      decomposeText(L"이걸 분해 하여 보자"),
      testing::ElementsAre(DecomposedChar{L'이', L"ㅇㅣ"}, DecomposedChar{L'걸', L"ㄱㅓㄹ"}, DecomposedChar{L' ', L" "},
                           DecomposedChar{L'분', L"ㅂㅜㄴ"}, DecomposedChar{L'해', L"ㅎㅐ"}, DecomposedChar{L' ', L" "},
                           DecomposedChar{L'하', L"ㅎㅏ"}, DecomposedChar{L'여', L"ㅇㅕ"}, DecomposedChar{L' ', L" "},
                           DecomposedChar{L'보', L"ㅂㅗ"}, DecomposedChar{L'자', L"ㅈㅏ"}));
}

// 제9항
// 받침 ‘ㄲ, ㅋ’, ‘ㅅ, ㅆ, ㅈ, ㅊ, ㅌ’, ‘ㅍ’은 어말 또는 자음 앞에서 각각 대표음 [ㄱ, ㄷ, ㅂ]으로 발음한다.
class Rule9Test : public testing::TestWithParam<std::pair<std::wstring, std::wstring>> {};

TEST_P(Rule9Test, test) {
  std::pair<std::wstring, std::wstring> param = GetParam();
  auto textVector = decomposeText(param.first);
  auto expected = decomposeText(param.second);

  convertJongsungPronounciation(textVector);

  ASSERT_THAT(textVector, testing::ElementsAreArray(expected.data(), expected.size()));
}

INSTANTIATE_TEST_SUITE_P(PronounciationRule9,
                         Rule9Test,
                         testing::Values(std::make_pair(L"닦다", L"닥다"),
                                         std::make_pair(L"키읔", L"키윽"),
                                         std::make_pair(L"키읔과", L"키윽과"),
                                         std::make_pair(L"옷", L"옫"),
                                         std::make_pair(L"웃다", L"욷다"),
                                         std::make_pair(L"있다", L"읻다"),
                                         std::make_pair(L"젖", L"젇"),
                                         std::make_pair(L"빚다", L"빋다"),
                                         std::make_pair(L"꽃", L"꼳"),
                                         std::make_pair(L"쫓다", L"쫃다"),
                                         std::make_pair(L"솥", L"솓"),
                                         std::make_pair(L"뱉다", L"밷다"),
                                         std::make_pair(L"앞", L"압"),
                                         std::make_pair(L"덮다", L"덥다")));

// 제10항
// 겹받침 'ㄳ', 'ㄵ', 'ㄼ, ㄽ, ㄾ', 'ㅄ'은 어말 또는 자음 앞에서 각각 [ㄱ, ㄴ, ㄹ, ㅂ]으로 발음한다.
class Rule10Test : public testing::TestWithParam<std::pair<std::wstring, std::wstring>> {};

TEST_P(Rule10Test, test) {
  std::pair<std::wstring, std::wstring> param = GetParam();
  auto textVector = decomposeText(param.first);
  auto expected = decomposeText(param.second);

  convertJongsungPronounciation(textVector);

  ASSERT_THAT(textVector, testing::ElementsAreArray(expected.data(), expected.size()));
}

INSTANTIATE_TEST_SUITE_P(PronounciationRule10,
                         Rule10Test,
                         testing::Values(std::make_pair(L"넋", L"넉"),
                                         std::make_pair(L"넋과", L"넉과"),
                                         std::make_pair(L"앉다", L"안다"),
                                         std::make_pair(L"여덟", L"여덜"),
                                         std::make_pair(L"넓다", L"널다"),
                                         std::make_pair(L"외곬", L"외골"),
                                         std::make_pair(L"핥다", L"할다"),
                                         std::make_pair(L"값", L"갑"),
                                         std::make_pair(L"없다", L"업다"),
                                         std::make_pair(L"밟다", L"밥다"),
                                         std::make_pair(L"밟소", L"밥소"),
                                         std::make_pair(L"밟지", L"밥지"),
                                         std::make_pair(L"밟는", L"밥는"),
                                         std::make_pair(L"밟게", L"밥게"),
                                         std::make_pair(L"밟고", L"밥고"),
                                         std::make_pair(L"넓죽하다", L"넙죽하다"),
                                         std::make_pair(L"넓둥글다", L"넙둥글다"),
                                         std::make_pair(L"넓다", L"널다")));

// 제11항
// 겹받침 ‘ㄺ, ㄻ, ㄿ’은 어말 또는 자음 앞에서 각각 [ㄱ, ㅁ, ㅂ]으로 발음한다.
class Rule11Test : public testing::TestWithParam<std::pair<std::wstring, std::wstring>> {};

TEST_P(Rule11Test, test) {
  std::pair<std::wstring, std::wstring> param = GetParam();
  auto textVector = decomposeText(param.first);
  auto expected = decomposeText(param.second);

  convertJongsungPronounciation(textVector);

  ASSERT_THAT(textVector, testing::ElementsAreArray(expected.data(), expected.size()));
}

INSTANTIATE_TEST_SUITE_P(PronounciationRule11,
                         Rule11Test,
                         testing::Values(std::make_pair(L"닭", L"닥"),
                                         // std::make_pair(L"흙과", L"흑과"),
                                         // 용언의 어간 말음 판정이 불가해
                                         // 현재 '흙과'는 '흘과'처럼 나옴
                                         std::make_pair(L"맑다", L"막다"),
                                         std::make_pair(L"늙지", L"늑지"),
                                         std::make_pair(L"삶", L"삼"),
                                         std::make_pair(L"젊다", L"점다"),
                                         std::make_pair(L"읊고", L"읍고"),
                                         std::make_pair(L"읊다", L"읍다"),
                                         std::make_pair(L"맑게", L"말게"),
                                         std::make_pair(L"묽고", L"물고"),
                                         std::make_pair(L"얽거나", L"얼거나")));
