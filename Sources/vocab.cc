#include "fasthangul/vocab.hh"
#include <cassert>
#include <iostream>

fasthangul::vocab::Vocab::Vocab(std::vector<std::wstring> &words, std::wstring unknownToken) {
  vocabData.reserve(words.size());
  inversedVocabData.reserve(words.size());

  size_t index = 0;

  for (auto &word : words) {
    vocabData[word] = index;
    inversedVocabData[index] = word;
    index += 1;
  }

  auto found = vocabData.find(unknownToken);
  assert(found != vocabData.end());
  unknownTokenIndex = found->second;
}

size_t fasthangul::vocab::Vocab::find(std::wstring word) {
  auto found = vocabData.find(word);
  if (found == vocabData.end())
    return unknownTokenIndex;
  return found->second;
}

bool fasthangul::vocab::Vocab::has(std::wstring word) {
  auto found = vocabData.find(word);
  return found != vocabData.end();
}

std::wstring_view fasthangul::vocab::Vocab::at(size_t index) {
  auto found = inversedVocabData.find(index);
  assert(found != inversedVocabData.end());
  return found->second;
}
