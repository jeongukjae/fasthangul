#include "fasthangul/normalizer.hh"
#include "PythonExtension.hh"
#include <string>
#include <string_view>

static PyObject *NORMALIZER_clean_text(PyObject *self, PyObject *args) {
  PyObject *string = NULL;
  if (!PyArg_UnpackTuple(args, "args", 1, 1, &string))
    return NULL;

  if (!PyUnicode_Check(string)) {
    PyErr_SetString(PyExc_TypeError, "arg must be str type");
    return NULL;
  }

  wchar_t *hangulString = PyUnicode_AsWideCharString(string, NULL);
  std::wstring filtered = fasthangul::normalizer::filterInvalidCharacter(std::wstring_view{hangulString});
  filtered = fasthangul::normalizer::normalizeWhitespace(filtered);
  PyObject *result = PyUnicode_FromWideChar(filtered.c_str(), filtered.length());

  Py_INCREF(result);
  return result;
}

/* ------------------------- */
/* delcare normalizer Module */
static PyMethodDef normalizerMethods[] = {
    {"clean_text", (PyCFunction)NORMALIZER_clean_text, METH_VARARGS,
     "텍스트 자체를 정제해주는 함수입니다. \n\n한글에 대한 처리는 별도로 이루어지지 않습니다."},
    {NULL}};

static PyModuleDef fasthangulNormalizerModule = {PyModuleDef_HEAD_INIT, "fasthangul.normalizer", "", -1,
                                                 normalizerMethods};

PyMODINIT_FUNC PyInit_normalizer(void) {
  PyObject *fasthangulNormalizer = PyModule_Create(&fasthangulNormalizerModule);

  if (fasthangulNormalizer == NULL)
    return NULL;

  return fasthangulNormalizer;
}
