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
#include "network/address.hpp"
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

#define LOG_ON_REJECT  0
#define LOG_ON_ACCEPT  1
#define LOG_ON_WAIT_FOR_DESTROY 2

class Proxy : public Base
{
	/** ���໯ ��һЩpy�������������� */
	BASE_SCRIPT_HREADER(Proxy, Base)	
public:
	Proxy(ENTITY_ID id, const ScriptDefModule* scriptModule);
	~Proxy();
	
	INLINE void addr(const Mercury::Address& address);
	INLINE const Mercury::Address& addr()const;

	/**
		���entity��������, �ڿͻ��˳�ʼ���ö�Ӧ��entity�� �������������
	*/
	void onEntitiesEnabled(void);
	
	/**
		��½���ԣ� �������ĵ�½ʧ��֮�� ��������ӿ��ٽ��г��� 
	*/
	int32 onLogOnAttempt(const char* addr, uint32 port, const char* password);
	
	/**
		��ʼ���ͻ���proxy������
	*/
	void initClientBasePropertys();
	void initClientCellPropertys();

	/** 
		��������entity��Ӧ�Ŀͻ���socket�Ͽ�ʱ������ 
	*/
	void onClientDeath(void);
	
	/** ����ӿ�
		���ͻ��������������entity��cell������ʱ�������� 
	*/
	void onClientGetCell(Mercury::Channel* pChannel);

	/**
		ÿ��proxy����֮�󶼻���ϵͳ����һ��uuid�� �ṩǰ���ص�½ʱ�������ʶ��
	*/
	INLINE uint64 rndUUID()const;
	INLINE void rndUUID(uint64 uid);
public:
	/** ���������������Ŀͻ���ת����һ��proxyȥ���� */
	void giveClientTo(Proxy* proxy);
	void onGiveClientTo(Mercury::Channel* lpChannel);
	void onGiveClientToFailure();
	DECLARE_PY_MOTHOD_ARG1(pyGiveClientTo, PyObject_ptr);
protected:
	uint64 rndUUID_;
	Mercury::Address addr_;
};

}


#ifdef CODE_INLINE
#include "proxy.ipp"
#endif

#endif
