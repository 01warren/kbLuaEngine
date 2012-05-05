/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
/*
	kbengine-�ű�ϵͳ::�ض���python���������
		��дsys.stdout stderr
		
		ʹ������:
			����ģ��ʵ������ python.sys.stdout and stderr �����ˣ� 
			ʹ���껹ԭstdout and stderr
*/
#ifndef __SCRIPTSTDOUTERR_H__
#define __SCRIPTSTDOUTERR_H__
#include "helper/debug_helper.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "scriptobject.hpp"

namespace KBEngine{ namespace script{
class ScriptStdOutErr: public ScriptObject
{
	/** ���໯ ��һЩpy�������������� */
	INSTANCE_SCRIPT_HREADER(ScriptStdOutErr, ScriptObject)							
public:	
	ScriptStdOutErr();
	virtual ~ScriptStdOutErr();

	/** pythonִ��д���� */
	static PyObject* write(PyObject* self, PyObject *args);	
	static PyObject* flush(PyObject* self, PyObject *args);

	/** ��װ��ж�����ģ�� */
	bool install(void);
	bool uninstall(void);
	bool isInstall(void)const{ return isInstall_; }

	virtual void onPrint(const char* msg);
protected:
	bool softspace_;
	PyObject* sysModule_;
	PyObject* prevStderr_;
	PyObject* prevStdout_;
	bool isInstall_;
} ;

}
}
#endif