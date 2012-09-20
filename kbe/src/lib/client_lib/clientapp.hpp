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


#ifndef __CLIENT_APP_H__
#define __CLIENT_APP_H__
// common include
#include "cstdkbe/cstdkbe.hpp"
#if KBE_PLATFORM == PLATFORM_WIN32
#pragma warning (disable : 4996)
#endif
//#define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>	
#include <stdarg.h> 
#include "helper/debug_helper.hpp"
#include "xmlplus/xmlplus.hpp"	
#include "cstdkbe/singleton.hpp"
#include "cstdkbe/smartpointer.hpp"
#include "cstdkbe/timer.hpp"
#include "network/interfaces.hpp"
#include "network/event_dispatcher.hpp"
#include "network/network_interface.hpp"
#include "server/mercury_errors.hpp"

// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#include <errno.h>
#endif
	
namespace KBEngine{

namespace Mercury
{

class Channel;
}

class ClientApp : 
	public Singleton<ClientApp>,
	public TimerHandler, 
	public Mercury::ChannelTimeOutHandler,
	public Mercury::ChannelDeregisterHandler
{
public:
	enum TimeOutType
	{
		TIMEOUT_GAME_TICK
	};
public:
	ClientApp(Mercury::EventDispatcher& dispatcher, 
			Mercury::NetworkInterface& ninterface, 
			COMPONENT_TYPE componentType,
			COMPONENT_ID componentID);

	~ClientApp();

	virtual bool initialize();
	virtual bool initializeBegin(){return true;};
	virtual bool inInitialize(){ return true; }
	virtual bool initializeEnd(){return true;};
	virtual void finalise();
	virtual bool run();
	
	bool installSingnals();

	virtual bool loadConfig();
	const char* name(){return COMPONENT_NAME_EX(componentType_);}
	
	virtual void handleTimeout(TimerHandle, void * pUser);

	GAME_TIME time() const { return time_; }
	Timers & timers() { return timers_; }
	double gameTimeInSeconds() const;
	void handleTimers();

	Mercury::EventDispatcher & getMainDispatcher()				{ return mainDispatcher_; }
	Mercury::NetworkInterface & getNetworkInterface()			{ return networkInterface_; }

	COMPONENT_ID componentID()const	{ return componentID_; }
	COMPONENT_TYPE componentType()const	{ return componentType_; }
		

	virtual void onChannelTimeOut(Mercury::Channel * pChannel);
	virtual void onChannelDeregister(Mercury::Channel * pChannel);

	void shutDown();
	
	/** ����ӿ�
		�����˺ųɹ���ʧ�ܻص�
	   @failedcode: ʧ�ܷ����� MERCURY_ERR_SRV_NO_READY:������û��׼����, 
									MERCURY_ERR_ACCOUNT_CREATE:����ʧ�ܣ��Ѿ����ڣ�, 
									MERCURY_SUCCESS:�˺Ŵ����ɹ�
	*/
	virtual void onCreateAccountResult(Mercury::Channel * pChannel, MERCURY_ERROR_CODE failedcode);

	/** ����ӿ�
	   ��¼ʧ�ܻص�
	   @failedcode: ʧ�ܷ����� MERCURY_ERR_SRV_NO_READY:������û��׼����, 
									MERCURY_ERR_SRV_OVERLOAD:���������ع���, 
									MERCURY_ERR_NAME_PASSWORD:�û����������벻��ȷ
	*/
	virtual void onLoginFailed(Mercury::Channel * pChannel, MERCURY_ERROR_CODE failedcode);

	/** ����ӿ�
	   ��¼�ɹ�
	   @ip: ������ip��ַ
	   @port: �������˿�
	*/
	virtual void onLoginSuccessfully(Mercury::Channel * pChannel, MemoryStream& s);

	/** ����ӿ�
	   ��¼ʧ�ܻص�
	   @failedcode: ʧ�ܷ����� MERCURY_ERR_SRV_NO_READY:������û��׼����, 
									MERCURY_ERR_ILLEGAL_LOGIN:�Ƿ���¼, 
									MERCURY_ERR_NAME_PASSWORD:�û����������벻��ȷ
	*/
	virtual void onLoginGatewayFailed(Mercury::Channel * pChannel, MERCURY_ERROR_CODE failedcode);

	/** ����ӿ�
		���������Ѿ�������һ����ͻ��˹����Ĵ���Entity
	   �ڵ�¼ʱҲ�ɱ��ɹ��ص�
	   @datas: �˺�entity����Ϣ
	*/
	virtual void onCreatedProxies(Mercury::Channel * pChannel, uint64 rndUUID, 
		ENTITY_ID eid, std::string& entityType);

	/** ����ӿ�
		���������Ѿ�������һ��Entity
	*/
	virtual void onCreatedEntity(Mercury::Channel * pChannel, ENTITY_ID eid, std::string& entityType);

	/** ����ӿ�
		�������ϵ�entity�Ѿ�����һ��cell����
	*/
	virtual void onEntityGetCell(Mercury::Channel * pChannel, ENTITY_ID eid);

	/** ����ӿ�
		�������ϵ�entity�Ѿ�������Ϸ������
	*/
	virtual void onEntityEnterWorld(Mercury::Channel * pChannel, ENTITY_ID eid, SPACE_ID spaceID);

	/** ����ӿ�
		�������ϵ�entity�Ѿ��뿪��Ϸ������
	*/
	virtual void onEntityLeaveWorld(Mercury::Channel * pChannel, ENTITY_ID eid, SPACE_ID spaceID);

	/** ����ӿ�
		���߿ͻ���ĳ��entity�����ˣ� ����entityͨ���ǻ�δonEntityEnterWorld
	*/
	virtual void onEntityDestroyed(Mercury::Channel * pChannel, ENTITY_ID eid);

	/** ����ӿ�
		�������ϵ�entity�Ѿ�����space��
	*/
	virtual void onEntityEnterSpace(Mercury::Channel * pChannel, SPACE_ID spaceID, ENTITY_ID eid);

	/** ����ӿ�
		�������ϵ�entity�Ѿ��뿪space��
	*/
	virtual void onEntityLeaveSpace(Mercury::Channel * pChannel, SPACE_ID spaceID, ENTITY_ID eid);

	/** ����ӿ�
		Զ�̵���entity�ķ��� 
	*/
	virtual void onRemoteMethodCall(Mercury::Channel* pChannel, MemoryStream& s);

	/** ����ӿ�
		����������entity����
	*/
	virtual void onUpdatePropertys(Mercury::Channel* pChannel, MemoryStream& s);
protected:
	COMPONENT_TYPE											componentType_;
	COMPONENT_ID											componentID_;									// �������ID

	Mercury::EventDispatcher& 								mainDispatcher_;
	Mercury::NetworkInterface&								networkInterface_;
	
	GAME_TIME												time_;
	Timers													timers_;

};

}
#endif
