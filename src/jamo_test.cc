#include "fasthangul/jamo.hh"
#include "gtest/gtest.h"

using namespace fasthangul::jamo;

TEST(jamo, composeJamos) {
  initializeJamos();

  ASSERT_STREQ(compose(L"ㅇㅏㄴㄴㅕㅇ").c_str(), L"안녕");
  ASSERT_STREQ(compose(L"ㅇㅏㄴㄴㅕㅇ  ").c_str(), L"안녕  ");
  ASSERT_STREQ(compose(L"abcdㅇㅏㄴㄴㅕㅇ  ").c_str(), L"abcd안녕  ");
  ASSERT_STREQ(compose(L"ㄴㅓ ㅁㅝㅎㅐ?").c_str(), L"너 뭐해?");
  ASSERT_STREQ(compose(L"ㄴㅓ ㅎㅁㅝㅎㅐ?").c_str(), L"너 ㅎ뭐해?");
  ASSERT_STREQ(compose(L"ㅉㅡㅎㅂㅛㅎ").c_str(), L"쯯뵿");
}

TEST(jamo, decomposeJamos) {
  initializeJamos();

  ASSERT_STREQ(decompose(L"안녕").c_str(), L"ㅇㅏㄴㄴㅕㅇ");
  ASSERT_STREQ(decompose(L"안녕  ").c_str(), L"ㅇㅏㄴㄴㅕㅇ  ");
  ASSERT_STREQ(decompose(L"abcd안녕  ").c_str(), L"abcdㅇㅏㄴㄴㅕㅇ  ");
  ASSERT_STREQ(decompose(L"너 뭐해?").c_str(), L"ㄴㅓ ㅁㅝㅎㅐ?");
}

TEST(jamo, testIsHangul) {
  ASSERT_TRUE(isHangul(L'가'));
  ASSERT_TRUE(isHangul(L'힣'));
  ASSERT_TRUE(isHangul(L'와'));
  ASSERT_FALSE(isHangul(L'ㄱ'));
  ASSERT_FALSE(isHangul(L'a'));
  ASSERT_FALSE(isHangul(L' '));
}

TEST(jamo, testIsJamoLike) {
  // is jamo
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

TEST(jamo, testGetOneHangulFromJamo) {
  initializeJamos();

  ASSERT_EQ(L'가', getOneHangulFromJamo(L'ㄱ', L'ㅏ'));
  ASSERT_EQ(L'각', getOneHangulFromJamo(L'ㄱ', L'ㅏ', L'ㄱ'));
  ASSERT_EQ(L'뿌', getOneHangulFromJamo(L'ㅃ', L'ㅜ'));
  ASSERT_EQ(L'뿝', getOneHangulFromJamo(L'ㅃ', L'ㅜ', L'ㅂ'));
}
