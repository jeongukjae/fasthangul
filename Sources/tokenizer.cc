#include "fasthangul/tokenizer.hh"
#include <algorithm>

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
