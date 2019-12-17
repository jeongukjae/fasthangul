#include "fasthangul/normalizer.hh"
#include "gtest/gtest.h"

using namespace fasthangul::normalizer;

TEST(normalizer, testFilterInvalidCharacter) {
  ASSERT_STREQ(filterInvalidCharacter(L"가나다라").c_str(), L"가나다라");
  ASSERT_STREQ(filterInvalidCharacter(L"가나다라\t\r\n").c_str(), L"가나다라\t\r\n");
  ASSERT_STREQ(filterInvalidCharacter(L"some long \ntext \x01 to test").c_str(), L"some long \ntext  to test");
}

TEST(normalizer, testNormalizerWhitespace) {
  ASSERT_STREQ(normalizeWhitespace(L"안녕하세요").c_str(), L"안녕하세요");
  ASSERT_STREQ(normalizeWhitespace(L"가나다라\t\r\n마").c_str(), L"가나다라 마");
  ASSERT_STREQ(normalizeWhitespace(L"가나 \t \r \n다라").c_str(), L"가나 다라");
  ASSERT_STREQ(normalizeWhitespace(L"some long \ntext to test").c_str(), L"some long text to test");
  ASSERT_STREQ(normalizeWhitespace(L"가나다라\t\r\n").c_str(), L"가나다라");
  ASSERT_STREQ(normalizeWhitespace(L"\t\r\n가나다라\t\r\n").c_str(), L"가나다라");
  ASSERT_STREQ(normalizeWhitespace(L"  가나다라  ").c_str(), L"가나다라");
  ASSERT_STREQ(normalizeWhitespace(L" \n\t\r").c_str(), L"");
}

TEST(normalizer, testIsControlChar) {
  ASSERT_TRUE(isControlChar(L'\x00'));
  ASSERT_TRUE(isControlChar(L'\x10'));
  ASSERT_TRUE(isControlChar(L'\x1f'));
  ASSERT_TRUE(isControlChar(L'\x7f')); // DEL

  ASSERT_FALSE(isControlChar('\n'));
  ASSERT_FALSE(isControlChar('\t'));
  ASSERT_FALSE(isControlChar('\r'));
  ASSERT_FALSE(isControlChar(L'\n'));
  ASSERT_FALSE(isControlChar(L'\t'));
  ASSERT_FALSE(isControlChar(L'\r'));
  ASSERT_FALSE(isControlChar(L'ㄱ'));
}

TEST(normalizer, testIsInvalidChar) {
  ASSERT_TRUE(isInvalidChar(L'\x00'));
  ASSERT_TRUE(isInvalidChar(L'\x10'));
  ASSERT_TRUE(isInvalidChar(L'\x1f'));
  ASSERT_TRUE(isInvalidChar(L'\x7f')); // DEL
  ASSERT_TRUE(isInvalidChar(L'\xFFFD'));

  ASSERT_FALSE(isInvalidChar('\n'));
  ASSERT_FALSE(isInvalidChar('\t'));
  ASSERT_FALSE(isInvalidChar('\r'));
  ASSERT_FALSE(isInvalidChar(L'\n'));
  ASSERT_FALSE(isInvalidChar(L'\t'));
  ASSERT_FALSE(isInvalidChar(L'\r'));
  ASSERT_FALSE(isInvalidChar(L'ㄱ'));
}
