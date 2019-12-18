#ifndef __FH_TOKENIZER_H__
#define __FH_TOKENIZER_H__

#include <string>
#include <vector>

namespace fasthangul {
namespace tokenizer {

class WordPieceTokenizer {
public:
  WordPieceTokenizer(const std::vector<std::wstring> *wordPieces);
  std::vector<std::wstring> tokenize(std::wstring_view text);

private:
  const std::vector<std::wstring> *wordPieces;

  std::vector<std::wstring> tokenizeSubword(const std::wstring_view &token);
};

std::vector<std::wstring_view> tokenizeWhitespace(std::wstring_view text);

} // namespace tokenizer
} // namespace fasthangul

#endif /* __FH_TOKENIZER_H__ */
