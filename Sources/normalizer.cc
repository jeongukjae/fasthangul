#include "fasthangul/normalizer.hh"
#include <algorithm>
#include <cctype>

std::wstring fasthangul::normalizer::filterInvalidCharacter(std::wstring_view text) {
  std::wstring cleanText;
  std::copy_if(text.begin(), text.end(), std::back_inserter(cleanText),
               [](const wchar_t &character) { return not fasthangul::normalizer::isInvalidChar(character); });
  return cleanText;
}

std::wstring fasthangul::normalizer::normalizeWhitespace(std::wstring text) {
  std::transform(text.begin(), text.end(), text.begin(),
                 [](const wchar_t &character) { return std::isspace(character) ? L' ' : character; });

  // remove duplicated spaces
  text.erase(std::unique(text.begin(), text.end(),
                         [](wchar_t &left, wchar_t &right) { return (left == L' ') && (left == right); }),
             text.end());
  // trim
  text.erase(0, text.find_first_not_of(L' '));
  text.erase(text.find_last_not_of(L' ') + 1);

  return text;
}
