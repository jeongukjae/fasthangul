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

// TEST(jamo, composeJamos)
// {
//   initializePrecomputedJamos();

//   ASSERT_STREQ(compose(L"ㅇㅏㄴㄴㅕㅇ").c_str(), L"안녕");
//   ASSERT_STREQ(compose(L"ㅇㅏㄴㄴㅕㅇ  ").c_str(), L"안녕  ");
//   ASSERT_STREQ(compose(L"abcdㅇㅏㄴㄴㅕㅇ  ").c_str(), L"abcd안녕  ");
//   ASSERT_STREQ(compose(L"ㄴㅓ ㅁㅝㅎㅐ?").c_str(), L"너 뭐해?");
// }

TEST(jamoUtils, testGetLengthOfComposingText)
{
  ASSERT_EQ(1, getLengthOfComposingText(std::wstring{L"ㅇㅏㄴ"}));
  ASSERT_EQ(2, getLengthOfComposingText(std::wstring{L"ㅇㅏㄴㅕ"}));
  ASSERT_EQ(2, getLengthOfComposingText(std::wstring{L"ㅇㅏㄴㅕㅇ"}));
  ASSERT_EQ(4, getLengthOfComposingText(std::wstring{L"ㅇㅏㄴㄴㅕㅇ  "}));
  ASSERT_EQ(8, getLengthOfComposingText(std::wstring{L"abcdㅇㅏㄴㄴㅕㅇ  "}));
}

TEST(jamoUtils, testIsHangul)
{
  ASSERT_TRUE(isHangul(L'가'));
  ASSERT_TRUE(isHangul(L'힣'));
  ASSERT_TRUE(isHangul(L'와'));
  ASSERT_FALSE(isHangul(L'ㄱ'));
  ASSERT_FALSE(isHangul(L'a'));
  ASSERT_FALSE(isHangul(L' '));
}

TEST(jamoUtils, testIsJamoLike)
{
  ASSERT_TRUE(isJamo(L'ㄱ'));
  ASSERT_TRUE(isJamo(L'ㅣ'));
  ASSERT_TRUE(isJamo(L'ㅏ'));
  ASSERT_FALSE(isJamo(L'가'));
  ASSERT_FALSE(isJamo(L'힣'));
  ASSERT_FALSE(isJamo(L'와'));

  // is chosung
  ASSERT_TRUE(isChosung(L'ㅉ'));
  ASSERT_FALSE(isChosung(L'ㄽ'));
  ASSERT_FALSE(isChosung(L'ㅣ'));

  // is jungsung
  ASSERT_TRUE(isJungsung(L'ㅣ'));
  ASSERT_FALSE(isJungsung(L'ㄽ'));
  ASSERT_FALSE(isJungsung(L'ㅉ'));

  // is jongsung
  ASSERT_TRUE(isJongsung(L'ㄽ'));
  ASSERT_FALSE(isJongsung(L'ㅣ'));
  ASSERT_FALSE(isJongsung(L'ㅉ'));
}
