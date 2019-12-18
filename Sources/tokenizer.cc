#include "fasthangul/tokenizer.hh"
#include <algorithm>

fasthangul::tokenizer::WordPieceTokenizer::WordPieceTokenizer(const std::vector<std::wstring> *wordPieces) {
  this->wordPieces = wordPieces;
}

std::vector<std::wstring> fasthangul::tokenizer::WordPieceTokenizer::tokenize(std::wstring_view text) {
  std::vector<std::wstring_view> tokenized = fasthangul::tokenizer::tokenizeWhitespace(text);
  std::vector<std::vector<std::wstring>> subwordTokenized;

  // std::transform(tokenized.begin(), tokenized.end(), std::back_inserter(subwordTokenized), tokenizeSubword);
  std::vector<std::wstring> k;
  return k;
}

std::vector<std::wstring_view> fasthangul::tokenizer::tokenizeWhitespace(std::wstring_view text) {
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
