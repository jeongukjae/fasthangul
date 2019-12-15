#include "jamo.hh"
#include "gtest/gtest.h"

TEST(jamo, decomposeJamos)
{
  initializePrecomputedJamos();

  ASSERT_STREQ(decompose(L"안녕").c_str(), L"ㅇㅏㄴㄴㅕㅇ");
  ASSERT_STREQ(decompose(L"안녕  ").c_str(), L"ㅇㅏㄴㄴㅕㅇ  ");
  ASSERT_STREQ(decompose(L"abcd안녕  ").c_str(), L"abcdㅇㅏㄴㄴㅕㅇ  ");
  ASSERT_STREQ(decompose(L"너 뭐해?").c_str(), L"ㄴㅓ ㅁㅝㅎㅐ?");
}

TEST(jamo, composeJamos)
{
  initializePrecomputedJamos();

  ASSERT_STREQ(compose(L"ㅇㅏㄴㄴㅕㅇ").c_str(), L"안녕");
  ASSERT_STREQ(compose(L"ㅇㅏㄴㄴㅕㅇ  ").c_str(), L"안녕  ");
  ASSERT_STREQ(compose(L"abcdㅇㅏㄴㄴㅕㅇ  ").c_str(), L"abcd안녕  ");
  ASSERT_STREQ(compose(L"ㄴㅓ ㅁㅝㅎㅐ?").c_str(), L"너 뭐해?");
}
