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


#ifndef __PROXY_H__
#define __PROXY_H__
	
// common include
#include "base.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "helper/debug_helper.hpp"
//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{


namespace Mercury
{
class Channel;
}

class Proxy : public Base
{
	/** ���໯ ��һЩpy�������������� */
	BASE_SCRIPT_HREADER(Proxy, Base)	
public:
	Proxy(ENTITY_ID id, ScriptModule* scriptModule);
	~Proxy();
	
	/**
		���entity��������, �ڿͻ��˳�ʼ���ö�Ӧ��entity�� �������������
	*/
	void onEntitiesEnabled(void);
	
	/**
		��½���ԣ� �������ĵ�½ʧ��֮�� ��������ӿ��ٽ��г��� 
	*/
	void onLogOnAttempt(std::string& addr, uint32& port, std::string& password);
	
	/** 
		��������entity��Ӧ�Ŀͻ���socket�Ͽ�ʱ������ 
	*/
	void onClientDeath(void);
	
	/** 
		���ͻ��������������entity��cell������ʱ�������� 
	*/
	void onClientGetCell(void);

	/**
		ÿ��proxy����֮�󶼻���ϵͳ����һ��uuid�� �ṩ�ṩǰ���ص�½ʱ�������ʶ��
	*/
	uint64 rndUUID()const{ return rndUUID_; }
	void rndUUID(uint64 uid){ rndUUID_ = uid; }
public:
	/** ���������������Ŀͻ���ת����һ��proxyȥ���� */
	void giveClientTo(Proxy* proxy);
	void onGiveClientToMe(Mercury::Channel* lpChannel);
	DECLARE_PY_MOTHOD_ARG1(pyGiveClientTo, PyObject_ptr);
protected:
	uint64 rndUUID_;
};

}
#endif
