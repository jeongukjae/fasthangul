#include "fasthangul/jamo.hh"
#include "PythonExtension.hh"
#include <string>
#include <string_view>

static PyObject *JAMO_compose(PyObject *self, PyObject *args) {
  PyObject *string = NULL;
  if (!PyArg_UnpackTuple(args, "args", 1, 1, &string))
    return NULL;

  if (!PyUnicode_Check(string)) {
    PyErr_SetString(PyExc_TypeError, "arg must be str type");
    return NULL;
  }

  wchar_t *hangulString = PyUnicode_AsWideCharString(string, NULL);
  std::wstring composed = fasthangul::jamo::compose(std::wstring_view{hangulString});
  PyObject *result = PyUnicode_FromWideChar(composed.c_str(), composed.length());

  Py_INCREF(result);
  return result;
}

static PyObject *JAMO_decompose(PyObject *self, PyObject *args) {
  PyObject *string = NULL;
  if (!PyArg_UnpackTuple(args, "args", 1, 1, &string))
    return NULL;

  if (!PyUnicode_Check(string)) {
    PyErr_SetString(PyExc_TypeError, "arg must be str type");
    return NULL;
  }

  wchar_t *hangulString = PyUnicode_AsWideCharString(string, NULL);
  std::wstring decomposed = fasthangul::jamo::decompose(std::wstring_view{hangulString});
  PyObject *result = PyUnicode_FromWideChar(decomposed.c_str(), decomposed.length());

  Py_INCREF(result);
  return result;
}

/* ------------------- */
/* delcare Jamo Module */
static PyMethodDef jamoMethods[] = {{"compose", (PyCFunction)JAMO_compose, METH_VARARGS, "compose"},
                                    {"decompose", (PyCFunction)JAMO_decompose, METH_VARARGS, "decompose"},
                                    {NULL}};

static PyModuleDef fasthangulJamoModule = {PyModuleDef_HEAD_INIT, "fasthangul.jamo", "", -1, jamoMethods};

PyMODINIT_FUNC PyInit_jamo(void) {
  PyObject *fasthangulJamo = PyModule_Create(&fasthangulJamoModule);

  if (fasthangulJamo == NULL)
    return NULL;

  fasthangul::jamo::initializeJamos();

  return fasthangulJamo;
}
