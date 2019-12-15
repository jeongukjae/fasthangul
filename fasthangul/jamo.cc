#include "jamo.hh"
#include "PythonExtension.hh"
#include <algorithm>
#include <numeric>
#include <set>
#include <string>
#include <vector>

static const wchar_t CHOSUNG[] = {L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ',
                                  L'ㄸ', L'ㄹ', L'ㅁ', L'ㅂ',
                                  L'ㅃ', L'ㅅ', L'ㅆ', L'ㅇ',
                                  L'ㅈ', L'ㅉ', L'ㅊ', L'ㅋ',
                                  L'ㅌ', L'ㅍ', L'ㅎ'};
static const wchar_t JUNGSUNG[] = {L'ㅏ', L'ㅐ', L'ㅑ', L'ㅒ',
                                   L'ㅓ', L'ㅔ', L'ㅕ', L'ㅖ',
                                   L'ㅗ', L'ㅘ', L'ㅙ', L'ㅚ',
                                   L'ㅛ', L'ㅜ', L'ㅝ', L'ㅞ',
                                   L'ㅟ', L'ㅠ', L'ㅡ', L'ㅢ',
                                   L'ㅣ'};
static const wchar_t JONGSUNG[] = {L'\0', L'ㄱ', L'ㄲ', L'ㄳ',
                                   L'ㄴ', L'ㄵ', L'ㄶ', L'ㄷ',
                                   L'ㄹ', L'ㄺ', L'ㄻ', L'ㄼ',
                                   L'ㄽ', L'ㄾ', L'ㄿ', L'ㅀ',
                                   L'ㅁ', L'ㅂ', L'ㅄ', L'ㅅ',
                                   L'ㅆ', L'ㅇ', L'ㅈ', L'ㅊ',
                                   L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ'};

static const std::set<wchar_t> CHOSUNG_SET{CHOSUNG, CHOSUNG + 19};
static const std::set<wchar_t> JONGSUNG_SET{JONGSUNG + 1, JONGSUNG + 27};

static const wchar_t FIRST_HANGUL = L'가';
static const wchar_t LAST_HANGUL = L'힣';

static std::unordered_map<wchar_t, std::wstring> PRECOMPUTED_JAMOS;

std::wstring compose(std::wstring_view text)
{
  std::wstring resultString{};
  resultString.reserve(getLengthOfComposingText(text));

  return resultString;
}

std::wstring decompose(std::wstring_view text)
{
  std::vector<std::wstring> stringsToJoin(text.size());
  std::vector<int> totalLength(text.size());
  std::wstring resultString{};

  std::transform(
      text.begin(),
      text.end(),
      stringsToJoin.begin(),
      [](const wchar_t character) {
        if (isHangul(character))
          return PRECOMPUTED_JAMOS[character];
        return std::wstring{character};
      });

  std::transform(
      stringsToJoin.begin(),
      stringsToJoin.end(),
      totalLength.begin(),
      [](const std::wstring str) { return str.length(); });

  resultString.reserve(std::accumulate(totalLength.begin(), totalLength.end(), 0));
  for (auto iter = stringsToJoin.begin(); iter != stringsToJoin.end(); ++iter)
    resultString.append(*iter);

  return resultString;
}

void initializePrecomputedJamos()
{
  wchar_t totalHangulCount = LAST_HANGUL - FIRST_HANGUL + 1;
  for (wchar_t charIndex = 0; charIndex < totalHangulCount; ++charIndex)
  {
    wchar_t chosungIndex = charIndex / 28 / 21;
    wchar_t jungsungIndex = charIndex / 28 % 21;
    wchar_t jongsungIndex = charIndex % 28;

    if (jongsungIndex != 0)
      PRECOMPUTED_JAMOS[FIRST_HANGUL + charIndex] = std::wstring({CHOSUNG[chosungIndex],
                                                                  JUNGSUNG[jungsungIndex],
                                                                  JONGSUNG[jongsungIndex]});
    else
      PRECOMPUTED_JAMOS[FIRST_HANGUL + charIndex] = std::wstring({CHOSUNG[chosungIndex],
                                                                  JUNGSUNG[jungsungIndex]});
  }
}

bool isHangul(const wchar_t character)
{
  return character >= FIRST_HANGUL and character <= LAST_HANGUL;
}

bool isJamo(const wchar_t character)
{
  return character >= L'ㄱ' and character <= L'ㅣ';
}

bool isChosung(const wchar_t character)
{
  return CHOSUNG_SET.find(character) != CHOSUNG_SET.end();
}

bool isJungsung(const wchar_t character)
{
  return character >= L'ㅏ' and character <= L'ㅣ';
}

bool isJongsung(const wchar_t character)
{
  return JONGSUNG_SET.find(character) != JONGSUNG_SET.end();
}

size_t getLengthOfComposingText(std::wstring_view text)
{
  const size_t stringLength = text.size();
  wchar_t character;
  size_t expectedLength = stringLength;

  for (int i = 0; i < stringLength; ++i)
  {
    character = text.at(i);
    if (isJungsung(character))
    {
      if (i != 0 and isChosung(text.at(i - 1)))
        --expectedLength;
      if (i <= stringLength - 2 and isJongsung(text.at(i + 1)))
        if (i == stringLength - 2 or !isJungsung(text.at(i + 2)))
          --expectedLength;
    }
  }

  return expectedLength;
}

// -----------
// Python Functions
static PyObject *JAMO_compose(PyObject *args)
{
  PyObject *string = NULL;
  if (!PyArg_UnpackTuple(args, "args", 1, 1, &string))
    return NULL;

  if (!PyUnicode_Check(string))
  {
    PyErr_SetString(PyExc_TypeError, "arg must be str type");
    return NULL;
  }

  Py_INCREF(Py_None);
  return Py_None;
}
static PyObject *JAMO_decompose(PyObject *self, PyObject *args)
{
  PyObject *string = NULL;
  if (!PyArg_UnpackTuple(args, "args", 1, 1, &string))
    return NULL;

  if (!PyUnicode_Check(string))
  {
    PyErr_SetString(PyExc_TypeError, "arg must be str type");
    return NULL;
  }

  wchar_t *hangulString = PyUnicode_AsWideCharString(string, NULL);
  std::wstring decomposed = decompose(std::wstring_view{hangulString});
  PyObject *result = PyUnicode_FromWideChar(decomposed.c_str(), decomposed.length());

  Py_INCREF(result);
  return result;
}

/* ------------------- */
/* delcare Jamo Module */
static PyMethodDef jamoMethods[] = {
    {"compose", (PyCFunction)JAMO_compose, METH_VARARGS, "compose"},
    {"decompose", (PyCFunction)JAMO_decompose, METH_VARARGS, "decompose"},
    {NULL}};

static PyModuleDef fasthangulJamoModule = {
    PyModuleDef_HEAD_INIT,
    "fasthangul.jamo",
    "",
    -1,
    jamoMethods};

PyMODINIT_FUNC PyInit_jamo(void)
{
  PyObject *fasthangulJamo = PyModule_Create(&fasthangulJamoModule);

  if (fasthangulJamo == NULL)
    return NULL;

  initializePrecomputedJamos();

  return fasthangulJamo;
}
