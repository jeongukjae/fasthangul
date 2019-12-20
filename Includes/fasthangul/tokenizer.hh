#ifndef __FH_TOKENIZER_H__
#define __FH_TOKENIZER_H__

#include "fasthangul/vocab.hh"
#include <string>
#include <vector>

namespace fasthangul {
namespace tokenizer {

// greedy longest-match-first algorithm
class WordPieceTokenizer {
public:
  WordPieceTokenizer(vocab::Vocab *vocab, std::wstring unknown_token = L"<UNK>",
                     std::wstring_view subwordPrefix = L"##");
  std::vector<std::wstring> tokenize(std::wstring_view text);

private:
  vocab::Vocab *vocab;
  std::vector<std::wstring> unknownTokenVector;
  std::wstring subwordPrefix;

  std::vector<std::wstring> tokenizeSubword(const std::wstring &token);
};

std::vector<std::wstring_view> tokenizeWhitespace(std::wstring_view text);
std::vector<std::wstring_view> tokenizePunctuation(std::wstring_view text);

} // namespace tokenizer
} // namespace fasthangul

#endif /* __FH_TOKENIZER_H__ */
