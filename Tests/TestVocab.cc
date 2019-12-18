#include "fasthangul/vocab.hh"
#include "gtest/gtest.h"
#include <iostream>

using namespace fasthangul::vocab;

TEST(vocab, initialize) {
  std::vector<std::wstring> words{L"<PAD>", L"<UNK>", L"abc", L"def", L"asdf"};
  Vocab vocab{words};

  ASSERT_STREQ(vocab.at(0).data(), L"<PAD>");
  ASSERT_STREQ(vocab.at(1).data(), L"<UNK>");
  ASSERT_STREQ(vocab.at(2).data(), L"나");
  ASSERT_STREQ(vocab.at(3).data(), L"너");
  ASSERT_STREQ(vocab.at(4).data(), L"우리");

  ASSERT_EQ(vocab.find(L"<PAD>"), 0);
  ASSERT_EQ(vocab.find(L"<UNK>"), 1);
  ASSERT_EQ(vocab.find(L"나"), 2);
  ASSERT_EQ(vocab.find(L"너"), 3);
  ASSERT_EQ(vocab.find(L"우리"), 4);
  ASSERT_EQ(vocab.find(L"이거 없다"), 1);
}
