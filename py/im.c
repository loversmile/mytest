
#include "/usr/include/python2.7/python.h"
int main()
{ 
Py_Initialize();
PyObject * pModule = NULL;
PyObject * pFunc   = NULL;
pModule = PyImport_ImportModule("sys");
pFunc   = PyObject_GetAttrString(pModule, "Hello");
PyEval_CallObject(pFunc, NULL);
Py_Finalize();
return 0;
} 
