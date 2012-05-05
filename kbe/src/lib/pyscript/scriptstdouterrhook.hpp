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
#ifndef __SCRIPTSTDOUTERRHOOK_H__
#define __SCRIPTSTDOUTERRHOOK_H__
#include "cstdkbe/cstdkbe.hpp"
#include "scriptobject.hpp"
#include "scriptstdouterr.hpp"

namespace KBEngine{ namespace script{

class ScriptStdOutErrHook : public ScriptStdOutErr
{
	/** ���໯ ��һЩpy�������������� */
	INSTANCE_SCRIPT_HREADER(ScriptStdOutErrHook, ScriptStdOutErr)	
public:
	ScriptStdOutErrHook();
	~ScriptStdOutErrHook();
	void onPrint(const char* msg);
	void setHookBuffer(std::string* buffer){ buffer_ = buffer; };
protected:
	std::string* buffer_;
} ;

}
}
#endif