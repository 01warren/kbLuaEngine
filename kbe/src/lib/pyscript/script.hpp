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

#ifndef __KBENGINE_SCRIPT_H__
#define __KBENGINE_SCRIPT_H__
#include "helper/debug_helper.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/singleton.hpp"
#include "scriptobject.hpp"
#include "scriptstdouterr.hpp"
#include "scriptstdouterrhook.hpp"

namespace KBEngine{ namespace script{

/** �ű�ϵͳ·�� */
#ifdef _LP64
#define SCRIPT_PATH												\
					L"../../res/scripts;"						\
					L"../../res/scripts/common;"				\
					L"../../res/scripts/common/lib-dynload64;"	\
					L"../../res/scripts/common/Lib"
#else
#define SCRIPT_PATH												\
					L"../../res/scripts;"						\
					L"../../res/scripts/common;"				\
					L"../../res/scripts/common/lib-dynload;"	\
					L"../../res/scripts/common/Lib"
#endif

// �ű������ĺ�׺���ͻ����ļ���
#define SCRIPT_BIN_TAG "cpython-32"
#define SCRIPT_BIN_CACHEDIR "__pycache__"

class Script
{						
public:	
	Script();
	virtual ~Script();
	
	/** 
		��װ��ж�ؽű�ģ�� 
	*/
	virtual bool install(const wchar_t* pythonHomeDir, std::wstring pyPaths, 
		const char* moduleName, COMPONENT_TYPE componentType);

	virtual bool uninstall(void);
	
	bool installExtraModule(const char* moduleName);

	/** 
		���һ����չ�ӿڵ�������չģ�� 
	*/
	bool registerExtraMethod(const char* attrName, PyMethodDef* pyFunc);

	/** 
		���һ����չ���Ե�������չģ�� 
	*/
	bool registerExtraObject(const char* attrName, PyObject* pyObj);

	/** 
		��ȡ�ű�����ģ�� 
	*/
	INLINE PyObject* getModule(void)const;

	/** 
		��ȡ�ű���չģ�� 
	*/
	INLINE PyObject* getExtraModule(void)const;

	int run_simpleString(const char* command, std::string* retBufferPtr);
	INLINE int run_simpleString(std::string command, std::string* retBufferPtr);

	int registerToModule(const char* attrName, PyObject* pyObj);
	int unregisterToModule(const char* attrName);

	void initThread( bool plusOwnInterpreter = false );
	void finiThread( bool plusOwnInterpreter = false );

	static void acquireLock();
	static void releaseLock();

	INLINE ScriptStdOutErrHook* pyStdouterrHook()const;
protected:
	PyObject* 					module_;
	PyObject*					extraModule_;		// ��չ�ű�ģ��

	ScriptStdOutErr*			pyStdouterr_;
	ScriptStdOutErrHook*		pyStdouterrHook_;	// �ṩtelnet ִ�нű�������
} ;

}
}

#ifdef CODE_INLINE
#include "script.ipp"
#endif

#endif
