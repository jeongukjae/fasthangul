#include "fasthangul/tokenizer.hh"
#include <algorithm>
#include <cwctype>

fasthangul::tokenizer::WordPieceTokenizer::WordPieceTokenizer(fasthangul::vocab::Vocab *vocab,
                                                              std::wstring unknown_token,
                                                              std::wstring_view subwordPrefix) {
  this->vocab = vocab;
  this->unknownTokenVector.push_back(unknown_token);
  this->subwordPrefix = subwordPrefix;
}

std::vector<std::wstring> fasthangul::tokenizer::WordPieceTokenizer::tokenize(std::wstring_view text) {
  std::vector<std::wstring_view> tokenized = tokenizer::tokenizeWhitespace(text);
  std::vector<std::vector<std::wstring>> subwordTokenized;

  std::transform(tokenized.begin(), tokenized.end(), std::back_inserter(subwordTokenized),
                 [this](const std::wstring_view &token) { return this->tokenizeSubword(std::wstring(token)); });

  std::vector<std::wstring> k;
  for (auto &subwords : subwordTokenized) {
    for (auto &subword : subwords) {
      k.push_back(subword);
    }
  }
  return k;
}

std::vector<std::wstring> fasthangul::tokenizer::WordPieceTokenizer::tokenizeSubword(const std::wstring &token) {
  std::vector<std::wstring> subwords;

  size_t tokenLength = token.size();
  size_t start = 0;
  size_t end = tokenLength;
  while (start < tokenLength) {
    if (end <= start) {
      return this->unknownTokenVector;
    }

    std::wstring subword = token.substr(start, end);
    if (start > 0)
      subword = subwordPrefix + subword;

    if (vocab->has(subword)) {
      subwords.push_back(subword);
      start = end;
      end = tokenLength;
    } else {
      end--;
    }
  }

  return subwords;
}

std::vector<std::wstring_view>
fasthangul::tokenizer::tokenizeStringWithLambda(std::wstring_view text, bool ignoreDelimiter,
                                                std::function<bool(const wchar_t)> lambda) {
  std::vector<std::wstring_view> result;

  auto first = text.data();
  auto second = text.data();
  auto third = text.data();
  auto last = first + text.size();

  for (; second != last && first != last; first = third) {
    second = std::find_if(first, last, lambda);
    third = std::find_if_not(second, last, lambda);

    if (first != second) {
      result.emplace_back(first, second - first);
    }
    if (!ignoreDelimiter && second != third) {
      result.emplace_back(second, third - second);
    }
  }

  return result;
}

std::vector<std::wstring_view> fasthangul::tokenizer::tokenizeWhitespace(std::wstring_view text) {
  return fasthangul::tokenizer::tokenizeStringWithLambda(text, true, [](const wchar_t c) { return std::iswspace(c); });
}

std::vector<std::wstring_view> fasthangul::tokenizer::tokenizePunctuation(std::wstring_view text) {
  return fasthangul::tokenizer::tokenizeStringWithLambda(text, false, [](const wchar_t c) { return std::iswpunct(c); });
}
