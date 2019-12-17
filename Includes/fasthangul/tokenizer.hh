#ifndef __FH_TOKENIZER_H__
#define __FH_TOKENIZER_H__

#include <string>
#include <vector>

namespace fasthangul {
namespace tokenizer {

std::vector<std::wstring_view> tokenizeWhitespace(std::wstring_view text);

} // namespace tokenizer
} // namespace fasthangul

#endif /* __FH_TOKENIZER_H__ */
