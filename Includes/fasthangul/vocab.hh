#ifndef __FH_VOCAB_H__
#define __FH_VOCAB_H__

#include <string>
#include <unordered_map>
#include <vector>

namespace fasthangul {
namespace vocab {

class Vocab {
public:
  Vocab(std::vector<std::wstring> &words, std::wstring unknownToken = L"<UNK>");

  size_t find(std::wstring word);
  bool has(std::wstring word);
  std::wstring_view at(size_t index);

private:
  size_t unknownTokenIndex;
  std::unordered_map<std::wstring, size_t> vocabData;
  std::unordered_map<size_t, std::wstring> inversedVocabData;
};

} // namespace vocab
} // namespace fasthangul

#endif /* __FH_VOCAB_H__ */
