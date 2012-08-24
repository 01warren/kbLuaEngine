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


#ifndef __KBENGINE_DEF_METHOD_H__
#define __KBENGINE_DEF_METHOD_H__
#include "cstdkbe/cstdkbe.hpp"
#if KBE_PLATFORM == PLATFORM_WIN32
#pragma warning (disable : 4910)
#pragma warning (disable : 4251)
#endif
// common include	
#include "datatype.hpp"
#include "datatypes.hpp"
#include "helper/debug_helper.hpp"
#include "network/packet.hpp"
#include "entitymailboxabstract.hpp"
#include "pyscript/scriptobject.hpp"	
//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif

namespace KBEngine{

class MethodDescription
{
public:	
	MethodDescription(ENTITY_METHOD_UID utype, std::string name, bool isExposed = false);
	virtual ~MethodDescription();
	
	std::string& getName(void){ return name_; };

	ENTITY_METHOD_UID getUType(void)const{ return utype_; }
	void setUType(ENTITY_METHOD_UID muid){ utype_ = muid; }

	static uint32 getDescriptionCount(void){ return methodDescriptionCount_; }

	bool isExposed(void)const{ return isExposed_; }

	void setExposed(void){ isExposed_ = true; }

	bool pushArgType(DataType* dataType);

	std::vector<DataType*>& getArgTypes(void){ return argTypes_; }

	size_t getArgSize(void);
	
	/** ���һ��call�Ƿ�Ϸ� */
	bool checkArgs(PyObject* args);		
	
	/** ��ÿ�����������ӵ����� ��������������Ϣ����������ڽű�������ʱ�ﴫ��Ĳ��� */
	void addToStream(MemoryStream* mstream, PyObject* args);

	/** ��һ��call����� ������һ��PyObject���͵�args */
	PyObject* createFromStream(MemoryStream* mstream);
	
	/** ����һ������ */
	PyObject* call(PyObject* func, PyObject* args);	

	void currCallerID(ENTITY_ID eid){ currCallerID_ = eid; }
protected:	
	static uint32							methodDescriptionCount_;					// ���е���������������
	std::string								name_;										// �������������
	ENTITY_METHOD_UID						utype_;										// ���������������� ���������ϴ���ʶ��
	std::vector<DataType*>					argTypes_;									// ������ԵĲ�������б�
	bool									isExposed_;									// �Ƿ���һ����¶����
	ENTITY_ID								currCallerID_;								// ��ǰ������������ĵ�����ID, �ṩ��¶��������ʱ���ű��жϵ���Դ��ֹ����
};

}
#endif
