#include "fasthangul/tokenizer.hh"
#include <algorithm>
#include <cctype>

using namespace fasthangul;

tokenizer::WordPieceTokenizer::WordPieceTokenizer(vocab::Vocab *vocab, std::wstring unknown_token,
                                                  std::wstring_view subwordPrefix) {
  this->vocab = vocab;
  this->unknownTokenVector.push_back(unknown_token);
  this->subwordPrefix = subwordPrefix;
}

std::vector<std::wstring> tokenizer::WordPieceTokenizer::tokenize(std::wstring_view text) {
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

std::vector<std::wstring> tokenizer::WordPieceTokenizer::tokenizeSubword(const std::wstring &token) {
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

std::vector<std::wstring_view> tokenizer::tokenizeStringWithLambda(std::wstring_view text, bool ignoreDelimiter,
                                                                   std::function<bool(const wchar_t)> lambda) {
  std::vector<std::wstring_view> result;

  auto first = text.data();
  auto second = text.data();
  auto third = text.data();
  auto last = first + text.size();

  for (; second != last && first != last; first = third) {
    second = std::find_if(first, last, [lambda](const char element) { return lambda(element); });
    third = std::find_if(second, last, [lambda](const char element) { return !lambda(element); });

    if (first != second) {
      result.emplace_back(first, second - first);
    }
    if (!ignoreDelimiter && second != third) {
      result.emplace_back(second, third - second);
    }
  }

  return result;
}

std::vector<std::wstring_view> tokenizer::tokenizeWhitespace(std::wstring_view text) {
  return tokenizer::tokenizeStringWithLambda(text, true, (bool (*)(wchar_t))std::isspace);
}

std::vector<std::wstring_view> tokenizer::tokenizePunctuation(std::wstring_view text) {
  return tokenizer::tokenizeStringWithLambda(text, false, (bool (*)(wchar_t))std::ispunct);
}
