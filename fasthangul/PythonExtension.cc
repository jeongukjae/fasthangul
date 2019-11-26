#include "PythonExtension.hh"

static PyMethodDef fasthangulMethods[] = {
    {NULL}};

static PyModuleDef fasthangulModule = {
    PyModuleDef_HEAD_INIT,
    "fasthangul",
    "",
    -1,
    fasthangulMethods};

PyMODINIT_FUNC PyInit_fasthangul(void)
{
  PyObject *fasthangul = PyModule_Create(&fasthangulModule);

  if (fasthangul == NULL)
    return NULL;

  return fasthangul;
}
