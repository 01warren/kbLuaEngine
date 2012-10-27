/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
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

	/** 
		pythonִ��д���� 
	*/
	static PyObject* __py_write(PyObject* self, PyObject *args);	
	static PyObject* __py_flush(PyObject* self, PyObject *args);

	/** 
		��װ��ж�����ģ�� 
	*/
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
	std::string sbuffer_;
} ;

}
}
#endif
