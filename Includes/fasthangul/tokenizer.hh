#ifndef __FH_TOKENIZER_H__
#define __FH_TOKENIZER_H__

#include "fasthangul/vocab.hh"
#include <string>
#include <vector>

namespace fasthangul {
namespace tokenizer {

class WordPieceTokenizer {
public:
  WordPieceTokenizer(const vocab::Vocab *vocab);
  std::vector<std::wstring> tokenize(std::wstring_view text);

private:
  const vocab::Vocab *vocab;

  std::vector<std::wstring> tokenizeSubword(const std::wstring &token);
};

std::vector<std::wstring_view> tokenizeWhitespace(std::wstring_view text);

} // namespace tokenizer
} // namespace fasthangul

#endif /* __FH_TOKENIZER_H__ */
