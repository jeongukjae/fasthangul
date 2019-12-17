#include "fasthangul/tokenizer.hh"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace fasthangul::tokenizer;

TEST(tokenizer, testTokenizeWhitespace) {
  ASSERT_THAT(tokenizeWhitespace(L"안녕하세요 나는 홍길동입니다."),
              testing::ElementsAre(L"안녕하세요", L"나는", L"홍길동입니다."));
  ASSERT_THAT(tokenizeWhitespace(L"이런  공백  문자   여러개는 어떻게 하죠?"),
              testing::ElementsAre(L"이런", L"공백", L"문자", L"여러개는", L"어떻게", L"하죠?"));
  ASSERT_THAT(tokenizeWhitespace(L"와!!신나요!!"), testing::ElementsAre(L"와!!신나요!!"));
}
