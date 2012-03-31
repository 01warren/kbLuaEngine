/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
#ifndef __KBENGINE_DEF_METHOD_H__
#define __KBENGINE_DEF_METHOD_H__
#pragma warning (disable : 4910)
#pragma warning (disable : 4251)

// common include	
#include "datatype.hpp"
#include "datatypes.hpp"
#include "log/debug_helper.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "network/Packet.hpp"
#include "entitymailboxabstract.hpp"
#include "pyscript/scriptobject.hpp"	
//#define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>	
#include <map>	
#include <vector>
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#include <errno.h>
#endif
	
#define LIB_DLLAPI  __declspec(dllexport)

#ifdef __cplusplus  
extern "C" {  
#endif  

#ifdef __cplusplus  
}
#endif 

namespace KBEngine{

class LIB_DLLAPI MethodDescription
{
protected:	
	static uint32				methodDescriptionCount_;						// ���е���������������
	std::string					m_name_;										// �������������
	uint32						m_utype_;										// ���������������� ���������ϴ���ʶ��
	std::vector<DataType*>		m_argTypes_;									// ������ԵĲ�������б�
	bool						m_isExposed_;									// �Ƿ���һ����¶����
public:	
	MethodDescription(std::string name, bool isExposed = false);
	virtual ~MethodDescription();
	
	std::string& getName(void){ return m_name_; };
	uint32 getUType(void)const{ return m_utype_; }
	static uint32 getDescriptionCount(void){ return methodDescriptionCount_; }
	bool isExposed(void)const{ return m_isExposed_; }
	void setExposed(void){ m_isExposed_ = true; }
	bool pushArgType(DataType* dataType);
	std::vector<DataType*>& getArgTypes(void){ return m_argTypes_; }
	size_t getArgSize(void);
	
	/** ���һ��call�Ƿ�Ϸ� */
	bool checkArgs(PyObject* args);		
	
	/** ��ÿ�����������ӵ����� ��������������Ϣ����������ڽű�������ʱ�ﴫ��Ĳ��� */
	void addToStream(MemoryStream* mstream, PyObject* args);
	/** ��һ��call����� ������һ��PyObject���͵�args */
	PyObject* createFromStream(MemoryStream* mstream);
	
	/** ����һ������ */
	PyObject* call(PyObject* func, PyObject* args);	
};

class LIB_DLLAPI script::ScriptObject;
class LIB_DLLAPI RemoteEntityMethod : public script::ScriptObject
{
	/** ���໯ ��һЩpy�������������� */
	INSTANCE_SCRIPT_HREADER(RemoteEntityMethod, script::ScriptObject)	
protected:	
	MethodDescription*		m_methodDescription_;					// �������������
	EntityMailboxAbstract*	m_pMailbox_;							// �������������mailbox
public:	
	RemoteEntityMethod(MethodDescription* methodDescription, EntityMailboxAbstract* mailbox);
	virtual ~RemoteEntityMethod();
	std::string& getName(void){ return m_methodDescription_->getName(); };
	MethodDescription* getDescription(void)const{ return m_methodDescription_; }
	static PyObject* tp_call(PyObject* self, PyObject* args, PyObject* kwds);
	EntityMailboxAbstract* getMailbox(void)const { return m_pMailbox_; }
};
}
#endif
