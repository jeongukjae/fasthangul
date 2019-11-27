#include "jamo.hh"
#include "PythonExtension.hh"
#include <algorithm>
#include <numeric>
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
static const wchar_t FIRST_HANGUL = L'가';
static const wchar_t LAST_HANGUL = L'힣';
static std::unordered_map<wchar_t, std::wstring> PRECOMPUTED_JAMOS;

std::wstring decompose(std::wstring_view hangul);

std::wstring decompose(std::wstring_view hangul)
{
  std::vector<std::wstring> stringsToJoin(hangul.size());
  std::vector<int> totalLength(hangul.size());
  std::wstring resultString{};

  std::transform(
      hangul.begin(),
      hangul.end(),
      stringsToJoin.begin(),
      [](const wchar_t character) {
        if (character >= FIRST_HANGUL and character <= LAST_HANGUL)
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
