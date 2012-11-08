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

#ifndef __DBMGR_H__
#define __DBMGR_H__
	
// common include	
#include "server/kbemain.hpp"
#include "pyscript/script.hpp"
#include "pyscript/pyobject_pointer.hpp"
#include "entitydef/entitydef.hpp"
#include "server/serverapp.hpp"
#include "server/idallocate.hpp"
#include "server/serverconfig.hpp"
#include "server/globaldata_client.hpp"
#include "server/globaldata_server.hpp"
#include "cstdkbe/timer.hpp"
#include "network/endpoint.hpp"
#include "resmgr/resmgr.hpp"
#include "thread/threadpool.hpp"

//#define NDEBUG
#include <map>	
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

class DBInterface;

class Dbmgr :	public ServerApp, 
				public Singleton<Dbmgr>
{
public:
	enum TimeOutType
	{
		TIMEOUT_TICK = TIMEOUT_SERVERAPP_MAX + 1,
		TIMEOUT_CHECK_STATUS
	};
	
	Dbmgr(Mercury::EventDispatcher& dispatcher, 
		Mercury::NetworkInterface& ninterface, 
		COMPONENT_TYPE componentType,
		COMPONENT_ID componentID);

	~Dbmgr();
	
	bool run();
	
	void handleTimeout(TimerHandle handle, void * arg);
	void handleMainTick();
	void handleCheckStatusTick();

	/* ��ʼ����ؽӿ� */
	bool initializeBegin();
	bool inInitialize();
	bool initializeEnd();
	void finalise();

	/** ��ȡID������ָ�� */
	IDServer<ENTITY_ID>& idServer(void){ return idServer_; }

	/** ����ӿ�
		�������һ��ENTITY_ID��
	*/
	void onReqAllocEntityID(Mercury::Channel* pChannel, int8 componentType, COMPONENT_ID componentID);

	/* ����ӿ�
		ע��һ���¼����baseapp����cellapp����dbmgr
		ͨ����һ���µ�app�������ˣ� ����Ҫ��ĳЩ���ע���Լ���
	*/
	virtual void onRegisterNewApp(Mercury::Channel* pChannel, 
							int32 uid, 
							std::string& username, 
							int8 componentType, uint64 componentID, 
							uint32 intaddr, uint16 intport, uint32 extaddr, uint16 extport);


	/** ����ӿ�
		dbmgr�㲥global���ݵĸı�
	*/
	void onGlobalDataClientLogon(Mercury::Channel* pChannel, COMPONENT_TYPE componentType);
	void onBroadcastGlobalDataChange(Mercury::Channel* pChannel, KBEngine::MemoryStream& s);
	
	/** ����ӿ�
		���󴴽��˺�
	*/
	void reqCreateAccount(Mercury::Channel* pChannel, std::string& accountName, std::string& password);

	/** ����ӿ�
		һ�����û���¼�� ��Ҫ���Ϸ���
	*/
	void onAccountLogin(Mercury::Channel* pChannel, std::string& accountName, std::string& password);

	/** ����ӿ�
		baseapp�����ѯaccount��Ϣ
	*/
	void queryAccount(Mercury::Channel* pChannel, std::string& accountName, std::string& password);

	/** ����ӿ�
		�˺Ŵ�baseapp������
	*/
	void onAccountOnline(Mercury::Channel* pChannel, std::string& accountName, 
		COMPONENT_ID componentID, ENTITY_ID entityID);

	/** ����ӿ�
		�˺Ŵ�baseapp������
	*/
	void onAccountOffline(Mercury::Channel* pChannel, std::string& accountName);

	/** ����ӿ�
		ִ�����ݿ��ѯ
	*/
	void executeRawDatabaseCommand(Mercury::Channel* pChannel, KBEngine::MemoryStream& s);

	/** ����ӿ�
		ĳ��entity�浵
	*/
	void writeEntity(Mercury::Channel* pChannel, KBEngine::MemoryStream& s);

	/**
		��ȡdb�ӿ�
	*/
	DBInterface* pDBInterface()const{ return pDBInterface_; }

	/** ����ӿ�
		�����db��ȡentity����������
	*/
	void queryEntity(Mercury::Channel* pChannel, COMPONENT_ID componentID, DBID dbid, 
		std::string& entityType, CALLBACK_ID callbackID);

	/** ����ӿ�
		ͬ��entity��ģ��
	*/
	void syncEntityStreamTemplate(Mercury::Channel* pChannel, KBEngine::MemoryStream& s);
	
protected:
	TimerHandle											loopCheckTimerHandle_;
	TimerHandle											mainProcessTimer_;

	// entityID��������
	IDServer<ENTITY_ID>									idServer_;									

	// globalData
	GlobalDataServer*									pGlobalData_;								

	// globalBases
	GlobalDataServer*									pGlobalBases_;								

	// cellAppData
	GlobalDataServer*									pCellAppData_;														
	
	DBInterface*										pDBInterface_;
};

}
#endif
