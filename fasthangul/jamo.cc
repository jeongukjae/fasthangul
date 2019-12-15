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
static const std::set<wchar_t> JONGSUNG_SET{JONGSUNG + 1, JONGSUNG + 28};
static std::unordered_map<wchar_t, int> CHOSUNG_MAP;
static std::unordered_map<wchar_t, int> JONGSUNG_MAP;

static const wchar_t FIRST_HANGUL = L'가';
static const wchar_t LAST_HANGUL = L'힣';

static std::unordered_map<wchar_t, std::wstring> PRECOMPUTED_JAMOS;

enum Composing
{
  C_KEEP,           // 그냥 그대로 합치는 문자
  C_IGNORE,         // 무시하는 문자
  C_COMPOSING,      // 중성인 경우 앞만 합칠 때
  C_COMPOSING_BOTH, // 중성인 경우 앞 뒤 전부 합칠 때
};

std::wstring compose(std::wstring_view text)
{
  std::wstring resultString{};
  const size_t textLength = text.size();

  Composing *composing = new Composing[textLength]{
      C_KEEP,
  };
  size_t expectedLength = textLength;

  wchar_t character;

  for (size_t i = 0; i < textLength; ++i)
  {
    character = text.at(i);
    if (isJungsung(character))
    {
      if (i != 0 and isChosung(text.at(i - 1)))
      {
        --expectedLength;
        composing[i - 1] = C_IGNORE;

        if (i <= textLength - 2 and isJongsung(text.at(i + 1)) and (i == textLength - 2 or !isJungsung(text.at(i + 2))))
        {
          --expectedLength;
          composing[i + 1] = C_IGNORE;
          composing[i] = C_COMPOSING_BOTH;
        }
        else
        {
          composing[i] = C_COMPOSING;
        }
      }
    }
  }

  resultString.reserve(expectedLength);

  for (size_t i = 0; i < textLength; ++i)
  {
    if (composing[i] == C_IGNORE)
      continue;

    if (composing[i] == C_KEEP)
      resultString += text.at(i);
    else if (composing[i] == C_COMPOSING)
      resultString += getOneHangulFromJamo(text.at(i - 1), text.at(i));
    else if (composing[i] == C_COMPOSING_BOTH)
      resultString += getOneHangulFromJamo(text.at(i - 1), text.at(i), text.at(i + 1));
  }

  delete[] composing;
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

void initializeJamos()
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

  for (int i = 0; i < 19; ++i)
  {
    CHOSUNG_MAP[CHOSUNG[i]] = i;
  }
  for (int i = 1; i < 28; ++i)
  {
    JONGSUNG_MAP[JONGSUNG[i]] = i;
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

wchar_t getOneHangulFromJamo(wchar_t chosung, wchar_t jungsung)
{
  wchar_t chosungIndex = CHOSUNG_MAP[chosung];
  wchar_t jungsungIndex = jungsung - L'ㅏ';

  return FIRST_HANGUL + 28 * (21 * chosungIndex + jungsungIndex);
}

wchar_t getOneHangulFromJamo(wchar_t chosung, wchar_t jungsung, wchar_t jongsung)
{
  wchar_t chosungIndex = CHOSUNG_MAP[chosung];
  wchar_t jungsungIndex = jungsung - L'ㅏ';
  wchar_t jongsungIndex = JONGSUNG_MAP[jongsung];

  return FIRST_HANGUL + 28 * (21 * chosungIndex + jungsungIndex) + jongsungIndex;
}

// -----------
// Python Functions
static PyObject *JAMO_compose(PyObject *self, PyObject *args)
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
  std::wstring composed = compose(std::wstring_view{hangulString});
  PyObject *result = PyUnicode_FromWideChar(composed.c_str(), composed.length());

  Py_INCREF(result);
  return result;
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

  initializeJamos();

  return fasthangulJamo;
}
