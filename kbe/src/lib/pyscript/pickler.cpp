#include "pickler.hpp"
namespace KBEngine{ 
namespace script{
PyObject* Pickler::picklerMethod_ = NULL;
PyObject* Pickler::unPicklerMethod_ = NULL;
PyObject* Pickler::pyPickleFuncTableModule_ = NULL;
bool Pickler::isInit = false;


//-------------------------------------------------------------------------------------
bool Pickler::initialize(void)
{
	if(isInit)
		return true;
	
	PyObject* cPickleModule = PyImport_ImportModule("pickle");

	if(cPickleModule)
	{
		picklerMethod_ = PyObject_GetAttrString(cPickleModule, "dumps");
		if (!picklerMethod_)
		{
			ERROR_MSG("Pickler::initialize:get dumps is error!\n");
			PyErr_PrintEx(0);
		}

		unPicklerMethod_ = PyObject_GetAttrString(cPickleModule, "loads");
		if(!unPicklerMethod_)
		{
			ERROR_MSG("Pickler::init: get loads is error!\n");
			PyErr_PrintEx(0);
		}

		Py_DECREF(cPickleModule);
	}
	else
	{
		ERROR_MSG("can't import pickle!\n");
		PyErr_PrintEx(0);
	}
	
	isInit = picklerMethod_ && unPicklerMethod_;
	
	// ��ʼ��һ��unpickle������ģ�飬 �����Զ������unpickle��������Ҫ�ڴ�ע��
	pyPickleFuncTableModule_ = PyImport_AddModule("_upf");

	static struct PyModuleDef moduleDesc =   
	{  
			 PyModuleDef_HEAD_INIT,  
			 "_upf",  
			 "This module is created by KBEngine!",  
			 -1,  
			 NULL  
	};  

	PyModule_Create(&moduleDesc);	
	return isInit;
}

//-------------------------------------------------------------------------------------
void Pickler::finalise(void)
{
	Py_XDECREF(picklerMethod_);
	Py_XDECREF(unPicklerMethod_);
	
	picklerMethod_ = NULL;
	unPicklerMethod_ = NULL;	
	pyPickleFuncTableModule_ = NULL;
}

//-------------------------------------------------------------------------------------
std::string Pickler::pickle(PyObject* pyobj)
{
	return pickle(pyobj, 2);
}

//-------------------------------------------------------------------------------------
std::string Pickler::pickle(PyObject* pyobj, int8 protocol)
{
	PyObject* pyRet = PyObject_CallFunction(picklerMethod_, 
		const_cast<char*>("(Oi)"), pyobj, protocol);
	
	SCRIPT_ERROR_CHECK();
	
	if(pyRet)
	{
		std::string str;
		str.assign(PyBytes_AsString(pyRet), PyBytes_Size(pyRet));
		S_RELEASE(pyRet);
		return str;
	}
	
	return "";
}

//-------------------------------------------------------------------------------------
PyObject* Pickler::unpickle(const std::string& str)
{
	PyObject* pyRet = PyObject_CallFunction(unPicklerMethod_, 
			const_cast<char*>("(s#)"), str.data(), str.length());
	
	if (!pyRet)
	{
		ERROR_MSG("Pickler::unpickle: failed to unpickle[%s] len=%d.\n", str.c_str(), str.length());
		PyErr_Print();
	}
	
	return pyRet;	
}

//-------------------------------------------------------------------------------------
void Pickler::registerUnpickleFunc(PyObject* pyFunc, const char* funcName)
{
	if(PyObject_SetAttrString(pyPickleFuncTableModule_, funcName, pyFunc) == -1){
		PyErr_PrintEx(0);
		return;
	}

	// ����һ�ο������Ƚ����, dir������Ѱ�Ҷ����__dict__��������û��__dict��Ὠ��, ��ô�����__module__�ſ������óɹ�
	PyObject_Dir(pyFunc);
	
	// �о�cPickle���뷢�ֱ�������������������__module__����Ϊ��ǰģ�����ƣ� �����޷�pickle
	PyObject* pyupf = PyBytes_FromString("_upf");
	if(PyObject_SetAttrString(pyFunc, "__module__", pyupf) == -1)
	{
		ERROR_MSG("Pickler::registerUnpickleFunc: set __module__ from unpickleFunc[%s] is error!\n", funcName);
		SCRIPT_ERROR_CHECK();
		Py_DECREF(pyupf);
		return;
	}
	
	Py_DECREF(pyupf);
}

//-------------------------------------------------------------------------------------

}
}
