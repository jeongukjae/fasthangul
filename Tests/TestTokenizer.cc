#include "fasthangul/tokenizer.hh"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace fasthangul::tokenizer;
using namespace fasthangul::vocab;

TEST(tokenizer, testTokenizeWhitespace) {
  ASSERT_THAT(tokenizeWhitespace(L"안녕하세요 나는 홍길동입니다."),
              testing::ElementsAre(L"안녕하세요", L"나는", L"홍길동입니다."));
  ASSERT_THAT(tokenizeWhitespace(L"이런  공백  문자   여러개는 어떻게 하죠?"),
              testing::ElementsAre(L"이런", L"공백", L"문자", L"여러개는", L"어떻게", L"하죠?"));
  ASSERT_THAT(tokenizeWhitespace(L"와!!신나요!!"), testing::ElementsAre(L"와!!신나요!!"));
}

TEST(tokenizer, testSplitOnPunctuation) {
  ASSERT_THAT(tokenizePunctuation(L"안녕하세요.나는 홍길동입니다."),
              testing::ElementsAre(L"안녕하세요", L".", L"나는 홍길동입니다", L"."));

  ASSERT_THAT(tokenizePunctuation(L"안녕하세요!!!!나는 홍길동입니다!?!"),
              testing::ElementsAre(L"안녕하세요", L"!!!!", L"나는 홍길동입니다", L"!?!"));

  ASSERT_THAT(tokenizePunctuation(L"이건 문장부호가 없어"), testing::ElementsAre(L"이건 문장부호가 없어"));
  ASSERT_THAT(tokenizePunctuation(L"!"), testing::ElementsAre(L"!"));
}

TEST(tokenizer, testWordPieceTokenizer) {
  std::vector<std::wstring> words{L"안녕", L"##하세요", L"안", L"<UNK>"};
  Vocab *vocab = new Vocab{words};

  WordPieceTokenizer tokenizer{vocab};

  ASSERT_THAT(tokenizer.tokenize(L"안녕하세요"), testing::ElementsAre(L"안녕", L"##하세요"));
  ASSERT_THAT(tokenizer.tokenize(L"안녕"), testing::ElementsAre(L"안녕"));
  ASSERT_THAT(tokenizer.tokenize(L"안"), testing::ElementsAre(L"안"));
  ASSERT_THAT(tokenizer.tokenize(L"안하세요"), testing::ElementsAre(L"안", L"##하세요"));
  ASSERT_THAT(tokenizer.tokenize(L"없는단어"), testing::ElementsAre(L"<UNK>"));

  delete vocab;
}
