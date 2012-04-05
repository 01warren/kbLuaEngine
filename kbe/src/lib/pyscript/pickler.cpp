#include "pickler.hpp"
namespace KBEngine{ 
namespace script{
PyObject* Pickler::m_picklerMethod_ = NULL;
PyObject* Pickler::m_unPicklerMethod_ = NULL;
PyObject* Pickler::m_pyPickleFuncTableModule_ = NULL;
bool Pickler::isInit = false;


//-------------------------------------------------------------------------------------
bool Pickler::initialize(void)
{
	if(isInit)
		return true;
	
	PyObject* cPickleModule = PyImport_ImportModule("pickle");

	if(cPickleModule)
	{
		m_picklerMethod_ = PyObject_GetAttrString(cPickleModule, "dumps");
		if (!m_picklerMethod_)
		{
			ERROR_MSG("Pickler::initialize:get dumps is error!\n");
			PyErr_PrintEx(0);
		}

		m_unPicklerMethod_ = PyObject_GetAttrString(cPickleModule, "loads");
		if(!m_unPicklerMethod_)
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
	
	isInit = m_picklerMethod_ && m_unPicklerMethod_;
	
	// ��ʼ��һ��unpickle������ģ�飬 �����Զ������unpickle��������Ҫ�ڴ�ע��
	m_pyPickleFuncTableModule_ = PyImport_AddModule("_upf");

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
	Py_XDECREF(m_picklerMethod_);
	Py_XDECREF(m_unPicklerMethod_);
	
	m_picklerMethod_ = NULL;
	m_unPicklerMethod_ = NULL;	
	m_pyPickleFuncTableModule_ = NULL;
}

//-------------------------------------------------------------------------------------
std::string Pickler::pickle(PyObject* pyobj)
{
	return pickle(pyobj, 2);
}

//-------------------------------------------------------------------------------------
std::string Pickler::pickle(PyObject* pyobj, int8 protocol)
{
	PyObject* pyRet;
	char fmt[] = "(Oi)";
	pyRet = PyObject_CallFunction(m_picklerMethod_, fmt, pyobj, protocol);
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
	PyObject* pyRet = NULL;
	char fmt[] = "(s#)";
	pyRet = PyObject_CallFunction(m_unPicklerMethod_, fmt, str.data(), str.length());
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
	if(PyObject_SetAttrString(m_pyPickleFuncTableModule_, funcName, pyFunc) == -1){
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
