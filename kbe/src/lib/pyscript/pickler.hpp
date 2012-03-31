/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
#ifndef __SCRIPT_PICKLER_H__
#define __SCRIPT_PICKLER_H__
#include "cstdkbe/cstdkbe.hpp"
#include "scriptobject.hpp"

namespace KBEngine{ namespace script{
class Pickler
{						
public:	
	/** ���� cPicket.dumps */
	static std::string pickle(PyObject* pyobj);
	static std::string pickle(PyObject* pyobj, int8 protocol);
	/** ���� cPicket.loads */
	static PyObject* unpickle(const std::string& str);
	/** ��ʼ��pickler */
	static bool initialize(void);
	static void finalise(void);
	
	/** ��ȡunpickle������ģ����� */
	static PyObject* getUnpickleFuncTableModule(void){ return m_pyPickleFuncTableModule_; }
	static PyObject* getUnpickleFunc(const char* funcName){ return PyObject_GetAttrString(m_pyPickleFuncTableModule_, funcName); }
	static void registerUnpickleFunc(PyObject* pyFunc, const char* funcName);
private:
	static PyObject* m_picklerMethod_;						// cPicket.dumps����ָ��
	static PyObject* m_unPicklerMethod_;					// cPicket.loads����ָ��
	static PyObject* m_pyPickleFuncTableModule_;			// unpickle������ģ����� �����Զ������unpickle��������Ҫ�ڴ�ע��
	static bool	isInit;										// �Ƿ��Ѿ�����ʼ��
} ;

}
}
#endif