#include "jamo.hh"
#include "gtest/gtest.h"

TEST(jamo, compose)
{
  initializePrecomputedJamos();

  ASSERT_STREQ(decompose(L"안녕").c_str(), L"ㅇㅏㄴㄴㅕㅇ");
  ASSERT_STREQ(decompose(L"안녕  ").c_str(), L"ㅇㅏㄴㄴㅕㅇ  ");
  ASSERT_STREQ(decompose(L"abcd안녕  ").c_str(), L"abcdㅇㅏㄴㄴㅕㅇ  ");
  ASSERT_STREQ(decompose(L"너 뭐해?").c_str(), L"ㄴㅓ ㅁㅝㅎㅐ?");
}
