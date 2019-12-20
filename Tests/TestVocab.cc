#include "fasthangul/vocab.hh"
#include "gtest/gtest.h"
#include <iostream>

using namespace fasthangul::vocab;

TEST(vocab, testVocab) {
  std::vector<std::wstring> words{L"<PAD>", L"<UNK>", L"나", L"너", L"우리"};
  Vocab vocab{words};

  ASSERT_EQ(vocab.find(L"<PAD>"), 0);
  ASSERT_EQ(vocab.find(L"<UNK>"), 1);
  ASSERT_EQ(vocab.find(L"나"), 2);
  ASSERT_EQ(vocab.find(L"너"), 3);
  ASSERT_EQ(vocab.find(L"우리"), 4);
  ASSERT_EQ(vocab.find(L"이거 없다"), 1);

  ASSERT_TRUE(vocab.has(L"<PAD>"));
  ASSERT_TRUE(vocab.has(L"<UNK>"));
  ASSERT_TRUE(vocab.has(L"나"));
  ASSERT_TRUE(vocab.has(L"너"));
  ASSERT_TRUE(vocab.has(L"우리"));
  ASSERT_FALSE(vocab.has(L"이거 없다"));

  ASSERT_STREQ(vocab.at(0).data(), L"<PAD>");
  ASSERT_STREQ(vocab.at(1).data(), L"<UNK>");
  ASSERT_STREQ(vocab.at(2).data(), L"나");
  ASSERT_STREQ(vocab.at(3).data(), L"너");
  ASSERT_STREQ(vocab.at(4).data(), L"우리");
}
