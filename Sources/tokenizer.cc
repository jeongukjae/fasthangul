#include "fasthangul/tokenizer.hh"
#include <algorithm>

using namespace fasthangul;

tokenizer::WordPieceTokenizer::WordPieceTokenizer(const vocab::Vocab *vocab) { this->vocab = vocab; }

std::vector<std::wstring> tokenizer::WordPieceTokenizer::tokenize(std::wstring_view text) {
  std::vector<std::wstring_view> tokenized = tokenizer::tokenizeWhitespace(text);
  std::vector<std::vector<std::wstring>> subwordTokenized;

  // std::transform(tokenized.begin(), tokenized.end(), std::back_inserter(subwordTokenized), tokenizeSubword);
  std::vector<std::wstring> k;
  return k;
}

std::vector<std::wstring> tokenizer::WordPieceTokenizer::tokenizeSubword(const std::wstring &token) {
  std::vector<std::wstring> subwords;

  size_t tokenLength = token.size();
  size_t start = 0;
  size_t end = tokenLength;
  while (start < tokenLength) {
    std::wstring subword = token.substr(start, end);

    // todo
  }

  return subwords;
}

std::vector<std::wstring_view> tokenizer::tokenizeWhitespace(std::wstring_view text) {
  std::vector<std::wstring_view> result;

  auto first = text.data();
  auto second = text.data();
  auto last = first + text.size();
  for (; second != last && first != last; first = second + 1) {
    second = std::find(first, last, L' ');

    if (first != second)
      result.emplace_back(first, second - first);
  }

  return result;
}
