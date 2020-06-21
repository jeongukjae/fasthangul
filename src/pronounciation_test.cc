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
  ASSERT_EQ(convertPronounciation(param.first), param.second);
}

INSTANTIATE_TEST_SUITE_P(PronounciationRule9,
                         Rule9Test,
                         testing::Values(std::make_pair(L"닦다", L"닥따"),
                                         std::make_pair(L"키읔", L"키윽"),
                                         std::make_pair(L"키읔과", L"키윽꽈"),
                                         std::make_pair(L"옷", L"옫"),
                                         std::make_pair(L"웃다", L"욷따"),
                                         std::make_pair(L"있다", L"읻따"),
                                         std::make_pair(L"젖", L"젇"),
                                         std::make_pair(L"빚다", L"빋따"),
                                         std::make_pair(L"꽃", L"꼳"),
                                         std::make_pair(L"쫓다", L"쫃따"),
                                         std::make_pair(L"솥", L"솓"),
                                         std::make_pair(L"뱉다", L"밷따"),
                                         std::make_pair(L"앞", L"압"),
                                         std::make_pair(L"덮다", L"덥따")));

// 제10항
// 겹받침 'ㄳ', 'ㄵ', 'ㄼ, ㄽ, ㄾ', 'ㅄ'은 어말 또는 자음 앞에서 각각 [ㄱ, ㄴ, ㄹ, ㅂ]으로 발음한다.
class Rule10Test : public testing::TestWithParam<std::pair<std::wstring, std::wstring>> {};

TEST_P(Rule10Test, test) {
  std::pair<std::wstring, std::wstring> param = GetParam();
  ASSERT_EQ(convertPronounciation(param.first), param.second);
}

INSTANTIATE_TEST_SUITE_P(PronounciationRule10,
                         Rule10Test,
                         testing::Values(std::make_pair(L"넋", L"넉"),
                                         std::make_pair(L"넋과", L"넉꽈"),
                                         std::make_pair(L"앉다", L"안다"),  // TODO 안따
                                         std::make_pair(L"여덟", L"여덜"),
                                         std::make_pair(L"넓다", L"널다"),  // TODO 널따
                                         std::make_pair(L"외곬", L"외골"),
                                         std::make_pair(L"핥다", L"할다"),  // TODO 할따
                                         std::make_pair(L"값", L"갑"),
                                         std::make_pair(L"없다", L"업따"),
                                         std::make_pair(L"밟다", L"밥따"),
                                         std::make_pair(L"밟소", L"밥쏘"),
                                         std::make_pair(L"밟지", L"밥찌"),
                                         std::make_pair(L"밟는", L"밤는"),
                                         std::make_pair(L"밟게", L"밥께"),
                                         std::make_pair(L"밟고", L"밥꼬"),
                                         std::make_pair(L"넓죽하다", L"넙쭈카다"),
                                         std::make_pair(L"넓둥글다", L"넙뚱글다")));

// 제11항
// 겹받침 ‘ㄺ, ㄻ, ㄿ’은 어말 또는 자음 앞에서 각각 [ㄱ, ㅁ, ㅂ]으로 발음한다.
class Rule11Test : public testing::TestWithParam<std::pair<std::wstring, std::wstring>> {};

TEST_P(Rule11Test, test) {
  std::pair<std::wstring, std::wstring> param = GetParam();
  ASSERT_EQ(convertPronounciation(param.first), param.second);
}

INSTANTIATE_TEST_SUITE_P(PronounciationRule11,
                         Rule11Test,
                         testing::Values(std::make_pair(L"닭", L"닥"),
                                         // std::make_pair(L"흙과", L"흑과"),
                                         // 용언의 어간 말음 판정이 불가해
                                         // 현재 '흙과'는 '흘과'처럼 나옴
                                         std::make_pair(L"맑다", L"막따"),
                                         std::make_pair(L"늙지", L"늑찌"),
                                         std::make_pair(L"삶", L"삼"),
                                         std::make_pair(L"젊다", L"점다"),  // TODO 점따
                                         std::make_pair(L"읊고", L"읍꼬"),
                                         std::make_pair(L"읊다", L"읍따"),
                                         std::make_pair(L"맑게", L"말게"),        // TODO 말께
                                         std::make_pair(L"묽고", L"물고"),        // TODO 물꼬
                                         std::make_pair(L"얽거나", L"얼거나")));  // TODO 얼꺼나

// 제12항
// 받침 ‘ㅎ’의 발음은 다음과 같다.
class Rule12Test : public testing::TestWithParam<std::pair<std::wstring, std::wstring>> {};

TEST_P(Rule12Test, test) {
  std::pair<std::wstring, std::wstring> param = GetParam();
  ASSERT_EQ(convertPronounciation(param.first), param.second);
}

INSTANTIATE_TEST_SUITE_P(PronounciationRule12,
                         Rule12Test,
                         testing::Values(std::make_pair(L"히읗", L"히읃"),
                                         std::make_pair(L"쌍히읗", L"쌍히읃"),
                                         std::make_pair(L"많", L"만"),
                                         std::make_pair(L"맗", L"말"),
                                         std::make_pair(L"놓고", L"노코"),
                                         std::make_pair(L"좋던", L"조턴"),
                                         std::make_pair(L"쌓지", L"싸치"),
                                         std::make_pair(L"많고", L"만코"),
                                         std::make_pair(L"닳지", L"달치"),
                                         std::make_pair(L"않던", L"안턴"),
                                         std::make_pair(L"닿소", L"다쏘"),
                                         std::make_pair(L"많소", L"만쏘"),
                                         std::make_pair(L"싫소", L"실쏘"),
                                         std::make_pair(L"않네", L"안네"),
                                         std::make_pair(L"않는", L"안는"),
                                         std::make_pair(L"뚫네", L"뚤레"),
                                         std::make_pair(L"뚫는", L"뚤른"),
                                         std::make_pair(L"낳은", L"나은"),
                                         std::make_pair(L"놓아", L"노아"),
                                         std::make_pair(L"쌓이다", L"싸이다"),
                                         std::make_pair(L"많아", L"마나"),
                                         std::make_pair(L"않은", L"아는"),
                                         std::make_pair(L"닳아", L"다라"),
                                         std::make_pair(L"싫어도", L"시러도"),
                                         std::make_pair(L"많ㅋㅋ", L"만ㅋㅋ"),
                                         std::make_pair(L"할많하않", L"할만하안")));

// 제13항
// 홑받침이나 쌍받침이 모음으로 시작된 조사나 어미, 접미사와 결합되는 경우에는, 제 음가대로 뒤 음절 첫소리로 옮겨
// 발음한다.
class Rule13Test : public testing::TestWithParam<std::pair<std::wstring, std::wstring>> {};

TEST_P(Rule13Test, test) {
  std::pair<std::wstring, std::wstring> param = GetParam();
  ASSERT_EQ(convertPronounciation(param.first), param.second);
}

INSTANTIATE_TEST_SUITE_P(PronounciationRule13,
                         Rule13Test,
                         testing::Values(std::make_pair(L"깎아", L"까까"),
                                         std::make_pair(L"옷이", L"오시"),
                                         std::make_pair(L"있어", L"이써"),
                                         std::make_pair(L"낮이", L"나지"),
                                         std::make_pair(L"꽂아", L"꼬자"),
                                         std::make_pair(L"꽃을", L"꼬츨"),
                                         std::make_pair(L"쫓아", L"쪼차"),
                                         std::make_pair(L"밭에", L"바테"),
                                         std::make_pair(L"앞으로", L"아프로"),
                                         std::make_pair(L"덮이다", L"더피다")));

// 제14항
// 겹받침이 모음으로 시작된 조사나 어미, 접미사와 결합되는 경우에는, 뒤엣것만을 뒤 음절 첫소리로 옮겨 발음한다.(이 경우,
// ‘ㅅ’은 된소리로 발음함.)
class Rule14Test : public testing::TestWithParam<std::pair<std::wstring, std::wstring>> {};

TEST_P(Rule14Test, test) {
  std::pair<std::wstring, std::wstring> param = GetParam();
  ASSERT_EQ(convertPronounciation(param.first), param.second);
}

INSTANTIATE_TEST_SUITE_P(PronounciationRule14,
                         Rule14Test,
                         testing::Values(std::make_pair(L"넋이", L"넉씨"),
                                         //  제 24항에 걸림 TODO
                                         //  std::make_pair(L"앉아", L"안자"),
                                         std::make_pair(L"닭을", L"달글"),
                                         std::make_pair(L"젊어", L"절머"),
                                         std::make_pair(L"곬이", L"골씨"),
                                         std::make_pair(L"핥아", L"할타"),
                                         std::make_pair(L"읊어", L"을퍼"),
                                         std::make_pair(L"값을", L"갑쓸"),
                                         std::make_pair(L"없어", L"업써")));

// 제16항
// 한글 자모의 이름은 그 받침소리를 연음하되, ‘ㄷ, ㅈ, ㅊ, ㅋ, ㅌ, ㅍ, ㅎ’의 경우에는 특별히 다음과 같이 발음한다.
class Rule16Test : public testing::TestWithParam<std::pair<std::wstring, std::wstring>> {};

TEST_P(Rule16Test, test) {
  std::pair<std::wstring, std::wstring> param = GetParam();
  ASSERT_EQ(convertPronounciation(param.first), param.second);
}

INSTANTIATE_TEST_SUITE_P(PronounciationRule16,
                         Rule16Test,
                         testing::Values(std::make_pair(L"디귿이", L"디그시"),
                                         std::make_pair(L"디귿을", L"디그슬"),
                                         std::make_pair(L"디귿에", L"디그세"),
                                         std::make_pair(L"지읒이", L"지으시"),
                                         std::make_pair(L"지읒을", L"지으슬"),
                                         std::make_pair(L"지읒에", L"지으세"),
                                         std::make_pair(L"치읓이", L"치으시"),
                                         std::make_pair(L"치읓을", L"치으슬"),
                                         std::make_pair(L"치읓에", L"치으세"),
                                         std::make_pair(L"키읔이", L"키으기"),
                                         std::make_pair(L"키읔을", L"키으글"),
                                         std::make_pair(L"키읔에", L"키으게"),
                                         std::make_pair(L"티읕이", L"티으시"),
                                         std::make_pair(L"티읕을", L"티으슬"),
                                         std::make_pair(L"티읕에", L"티으세"),
                                         std::make_pair(L"피읖이", L"피으비"),
                                         std::make_pair(L"피읖을", L"피으블"),
                                         std::make_pair(L"피읖에", L"피으베"),
                                         std::make_pair(L"히읗이", L"히으시"),
                                         std::make_pair(L"히읗을", L"히으슬"),
                                         std::make_pair(L"히읗에", L"히으세")));

// 제17항
// 받침 ‘ㄷ, ㅌ(ㄾ)’이 조사나 접미사의 모음 ‘ㅣ’와 결합되는 경우에는, [ㅈ, ㅊ]으로 바꾸어서 뒤 음절 첫소리로 옮겨
// 발음한다.
class Rule17Test : public testing::TestWithParam<std::pair<std::wstring, std::wstring>> {};

TEST_P(Rule17Test, test) {
  std::pair<std::wstring, std::wstring> param = GetParam();
  ASSERT_EQ(convertPronounciation(param.first), param.second);
}

INSTANTIATE_TEST_SUITE_P(PronounciationRule17,
                         Rule17Test,
                         testing::Values(std::make_pair(L"곧이듣다", L"고지듣따"),
                                         std::make_pair(L"굳이", L"구지"),
                                         std::make_pair(L"미닫이", L"미다지"),
                                         std::make_pair(L"땀받이", L"땀바지"),
                                         std::make_pair(L"밭이", L"바치"),
                                         //  TODO 제 14항과 겹침
                                         //  std::make_pair(L"벼훑이", L"벼훌치"),
                                         std::make_pair(L"굳히다", L"구치다"),
                                         std::make_pair(L"닫히다", L"다치다"),
                                         std::make_pair(L"묻히다", L"무치다")));

// 제 18항
// 받침 ‘ㄱ(ㄲ, ㅋ, ㄳ, ㄺ), ㄷ(ㅅ, ㅆ, ㅈ, ㅊ, ㅌ, ㅎ), ㅂ(ㅍ, ㄼ, ㄿ, ㅄ)’은 ‘ㄴ, ㅁ’ 앞에서 [ㅇ, ㄴ, ㅁ]으로
// 발음한다.
class Rule18Test : public testing::TestWithParam<std::pair<std::wstring, std::wstring>> {};

TEST_P(Rule18Test, test) {
  std::pair<std::wstring, std::wstring> param = GetParam();
  ASSERT_EQ(convertPronounciation(param.first), param.second);
}

INSTANTIATE_TEST_SUITE_P(PronounciationRule18,
                         Rule18Test,
                         testing::Values(std::make_pair(L"먹는", L"멍는"),
                                         std::make_pair(L"국물", L"궁물"),
                                         std::make_pair(L"깎는", L"깡는"),
                                         std::make_pair(L"키읔만", L"키응만"),
                                         std::make_pair(L"몫몫이", L"몽목씨"),
                                         std::make_pair(L"긁는", L"긍는"),
                                         std::make_pair(L"흙만", L"흥만"),
                                         std::make_pair(L"닫는", L"단는"),
                                         std::make_pair(L"짓는", L"진는"),
                                         std::make_pair(L"옷맵시", L"온맵씨"),
                                         std::make_pair(L"있는", L"인는"),
                                         std::make_pair(L"맞는", L"만는"),
                                         std::make_pair(L"젖멍울", L"전멍울"),
                                         std::make_pair(L"쫓는", L"쫀는"),
                                         std::make_pair(L"꽃망울", L"꼰망울"),
                                         std::make_pair(L"붙는", L"분는"),
                                         std::make_pair(L"놓는", L"논는"),
                                         std::make_pair(L"잡는", L"잠는"),
                                         std::make_pair(L"밥물", L"밤물"),
                                         std::make_pair(L"앞마당", L"암마당"),
                                         std::make_pair(L"밟는", L"밤는"),
                                         std::make_pair(L"읊는", L"음는"),
                                         std::make_pair(L"없는", L"엄는"),
                                         std::make_pair(L"값매다", L"감매다")));

// 제 19항
// 받침 ‘ㅁ, ㅇ’ 뒤에 연결되는 ‘ㄹ’은 [ㄴ]으로 발음한다.
class Rule19Test : public testing::TestWithParam<std::pair<std::wstring, std::wstring>> {};

TEST_P(Rule19Test, test) {
  std::pair<std::wstring, std::wstring> param = GetParam();
  ASSERT_EQ(convertPronounciation(param.first), param.second);
}

INSTANTIATE_TEST_SUITE_P(PronounciationRule19,
                         Rule19Test,
                         testing::Values(std::make_pair(L"담력", L"담녁"),
                                         std::make_pair(L"침략", L"침냑"),
                                         std::make_pair(L"강릉", L"강능"),
                                         std::make_pair(L"항로", L"항노"),
                                         std::make_pair(L"대통령", L"대통녕"),
                                         std::make_pair(L"막론", L"망논"),
                                         std::make_pair(L"백리", L"뱅니"),
                                         std::make_pair(L"협력", L"혐녁"),
                                         std::make_pair(L"십리", L"심니")));

// 제 20항
// ‘ㄴ’은 ‘ㄹ’의 앞이나 뒤에서 [ㄹ]로 발음한다.
class Rule20Test : public testing::TestWithParam<std::pair<std::wstring, std::wstring>> {};

TEST_P(Rule20Test, test) {
  std::pair<std::wstring, std::wstring> param = GetParam();
  ASSERT_EQ(convertPronounciation(param.first), param.second);
}

INSTANTIATE_TEST_SUITE_P(PronounciationRule20,
                         Rule20Test,
                         testing::Values(std::make_pair(L"난로", L"날로"),
                                         std::make_pair(L"신라", L"실라"),
                                         std::make_pair(L"천리", L"철리"),
                                         std::make_pair(L"광한루", L"광할루"),
                                         std::make_pair(L"대관령", L"대괄령"),
                                         std::make_pair(L"칼날", L"칼랄"),
                                         std::make_pair(L"물난리", L"물랄리"),
                                         std::make_pair(L"줄넘기", L"줄럼기")
                                         //  제 24항에 걸림 TODO
                                         //  , std::make_pair(L"할는지", L"할른지")
                                         ));

// 제 21항
// 위에서 지적한 이외의 자음동화는 인정하지 않는다.
class Rule21Test : public testing::TestWithParam<std::pair<std::wstring, std::wstring>> {};

TEST_P(Rule21Test, test) {
  std::pair<std::wstring, std::wstring> param = GetParam();
  ASSERT_EQ(convertPronounciation(param.first), param.second);
}

INSTANTIATE_TEST_SUITE_P(PronounciationRule21,
                         Rule21Test,
                         testing::Values(std::make_pair(L"감기", L"감기"),
                                         std::make_pair(L"옷감", L"옫깜"),
                                         std::make_pair(L"있고", L"읻꼬"),
                                         std::make_pair(L"꽃길", L"꼳낄"),
                                         std::make_pair(L"젖먹이", L"전머기"),
                                         std::make_pair(L"문법", L"문법"),  //  TODO 문뻡
                                         std::make_pair(L"꽃밭", L"꼳빧")));

// 제 23항
// 받침 ‘ㄱ(ㄲ, ㅋ, ㄳ, ㄺ), ㄷ(ㅅ, ㅆ, ㅈ, ㅊ, ㅌ), ㅂ(ㅍ, ㄼ, ㄿ,ㅄ)’ 뒤에 연결되는 ‘ㄱ, ㄷ, ㅂ, ㅅ, ㅈ’은 된소리로
// 발음한다.
class Rule23Test : public testing::TestWithParam<std::pair<std::wstring, std::wstring>> {};

TEST_P(Rule23Test, test) {
  std::pair<std::wstring, std::wstring> param = GetParam();
  ASSERT_EQ(convertPronounciation(param.first), param.second);
}

INSTANTIATE_TEST_SUITE_P(PronounciationRule23,
                         Rule23Test,
                         testing::Values(std::make_pair(L"국밥", L"국빱"),
                                         std::make_pair(L"깎다", L"깍따"),
                                         std::make_pair(L"넋받이", L"넉빠지"),
                                         std::make_pair(L"삯돈", L"삭똔"),
                                         std::make_pair(L"닭장", L"닥짱"),
                                         std::make_pair(L"칡범", L"칙뻠"),
                                         std::make_pair(L"뻗대다", L"뻗때다"),
                                         std::make_pair(L"옷고름", L"옫꼬름"),
                                         std::make_pair(L"있던", L"읻떤"),
                                         std::make_pair(L"꽂고", L"꼳꼬"),
                                         std::make_pair(L"꽃다발", L"꼳따발"),
                                         std::make_pair(L"낯설다", L"낟썰다"),
                                         std::make_pair(L"밭갈이", L"받까리"),
                                         std::make_pair(L"솥전", L"솓쩐"),
                                         std::make_pair(L"곱돌", L"곱똘"),
                                         std::make_pair(L"덮개", L"덥깨"),
                                         std::make_pair(L"옆집", L"엽찝"),
                                         std::make_pair(L"넓죽하다", L"넙쭈카다"),
                                         std::make_pair(L"읊조리다", L"읍쪼리다"),
                                         std::make_pair(L"값지다", L"갑찌다")));
