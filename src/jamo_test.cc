#include "fasthangul/jamo.hh"

#include "gtest/gtest.h"

using namespace fasthangul::jamo;

TEST(jamo, composeJamos) {
  JamoConverter converter;
  converter.initializeJamos();

  ASSERT_STREQ(converter.compose(L"ㅇㅏㄴㄴㅕㅇ").c_str(), L"안녕");
  ASSERT_STREQ(converter.compose(L"ㅇㅏㄴㄴㅕㅇ  ").c_str(), L"안녕  ");
  ASSERT_STREQ(converter.compose(L"abcdㅇㅏㄴㄴㅕㅇ  ").c_str(), L"abcd안녕  ");
  ASSERT_STREQ(converter.compose(L"ㄴㅓ ㅁㅝㅎㅐ?").c_str(), L"너 뭐해?");
  ASSERT_STREQ(converter.compose(L"ㄴㅓ ㅎㅁㅝㅎㅐ?").c_str(), L"너 ㅎ뭐해?");
  ASSERT_STREQ(converter.compose(L"ㅉㅡㅎㅂㅛㅎ").c_str(), L"쯯뵿");
}

TEST(jamo, decomposeJamos) {
  JamoConverter converter;
  converter.initializeJamos();

  ASSERT_STREQ(converter.decompose(L"안녕").c_str(), L"ㅇㅏㄴㄴㅕㅇ");
  ASSERT_STREQ(converter.decompose(L"안녕  ").c_str(), L"ㅇㅏㄴㄴㅕㅇ  ");
  ASSERT_STREQ(converter.decompose(L"abcd안녕  ").c_str(), L"abcdㅇㅏㄴㄴㅕㅇ  ");
  ASSERT_STREQ(converter.decompose(L"너 뭐해?").c_str(), L"ㄴㅓ ㅁㅝㅎㅐ?");
}

TEST(jamo, decomposeJamosWithEmptyJongsung) {
  JamoConverter converter;
  converter.initializeJamos(true, L'e');

  ASSERT_STREQ(converter.decompose(L"아니 이게 아닌데").c_str(), L"ㅇㅏeㄴㅣe ㅇㅣeㄱㅔe ㅇㅏeㄴㅣㄴㄷㅔe");
  ASSERT_STREQ(converter.decompose(L"너 뭐해?").c_str(), L"ㄴㅓe ㅁㅝeㅎㅐe?");
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
  JamoConverter converter;
  converter.initializeJamos();

  ASSERT_EQ(L'가', converter.getOneHangulFromJamo(L'ㄱ', L'ㅏ'));
  ASSERT_EQ(L'각', converter.getOneHangulFromJamo(L'ㄱ', L'ㅏ', L'ㄱ'));
  ASSERT_EQ(L'뿌', converter.getOneHangulFromJamo(L'ㅃ', L'ㅜ'));
  ASSERT_EQ(L'뿝', converter.getOneHangulFromJamo(L'ㅃ', L'ㅜ', L'ㅂ'));
}

TEST(jamo, testGetJamosFromHangul) {
  JamoConverter converter;
  converter.initializeJamos();

  ASSERT_EQ(L"ㄱㅏ", converter.getJamosFromHangul(L'가'));
  ASSERT_EQ(L"ㄱㅏㄱ", converter.getJamosFromHangul(L'각'));
  ASSERT_EQ(L"ㅃㅜ", converter.getJamosFromHangul(L'뿌'));
  ASSERT_EQ(L"ㅃㅜㅂ", converter.getJamosFromHangul(L'뿝'));
  ASSERT_EQ(L" ", converter.getJamosFromHangul(L' '));
  ASSERT_EQ(L"ㄱ", converter.getJamosFromHangul(L'ㄱ'));
}
