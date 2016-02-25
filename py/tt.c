#include <stdio.h>
#include "/usr/include/python2.7/pythonrun.h"
#include "/usr/include/python2.7/import.h"
int main()
{
	PyObject *modelname,*model;
	
	Py_Initialize();
	if (!Py_IsInitialized())
	{
        printf("初始化失败\n");
        return -1;
    }
     //直接运行Python语句
    PyRun_SimpleString("print '初始化成功'");
    
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
    
     //导入Python模块
    modelname=PyString_FromString("sys");
    model=PyImport_Import(modelname);
   
    if (model){
        printf("Load model ok\n");
    }
    else{
        printf("Load model failed");
        return -1;
    }
}

