#include "fasthangul/normalizer.hh"
#include "gtest/gtest.h"

using namespace fasthangul::normalizer;

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
