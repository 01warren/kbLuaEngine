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


#ifndef __BASEAPP_H__
#define __BASEAPP_H__
	
// common include	
#include "server/kbemain.hpp"
#include "server/entity_app.hpp"
#include "server/idallocate.hpp"
#include "server/serverconfig.hpp"
#include "server/globaldata_client.hpp"
#include "server/globaldata_server.hpp"
#include "cstdkbe/timer.hpp"
#include "network/endpoint.hpp"
#include "entitydef/entities.hpp"

//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

class Base;
class Baseapp :	public EntityApp, 
					public TimerHandler, 
					public Singleton<Baseapp>
{
public:
	enum TimeOutType
	{
		TIMEOUT_GAME_TICK
	};
	
	Baseapp(Mercury::EventDispatcher& dispatcher, 
		Mercury::NetworkInterface& ninterface, 
		COMPONENT_TYPE componentType,
		COMPONENT_ID componentID);
	~Baseapp();
	
	bool installPyModules();
	bool uninstallPyModules();

	bool run();
	
	void handleTimeout(TimerHandle handle, void * arg);
	void handleGameTick();
	void handleTimers();

	/* ��ʼ����ؽӿ� */
	bool initializeBegin();
	bool initializeEnd();
	void finalise();
	
	virtual bool destroyEntity(ENTITY_ID entityID);

	/** ����ӿ�
		�������һ��ENTITY_ID�εĻص�
	*/
	void onReqAllocEntityID(Mercury::Channel* pChannel, ENTITY_ID startID, ENTITY_ID endID);

	/** ����ӿ�
		dbmgr���ͳ�ʼ��Ϣ
		startID: ��ʼ����ENTITY_ID ����ʼλ��
		endID: ��ʼ����ENTITY_ID �ν���λ��
		startGlobalOrder: ȫ������˳�� �������ֲ�ͬ���
		startGroupOrder: ��������˳�� ����������baseapp�еڼ���������
	*/
	void onDbmgrInitCompleted(Mercury::Channel* pChannel, 
		ENTITY_ID startID, ENTITY_ID endID, int32 startGlobalOrder, int32 startGroupOrder);

	/** ����ӿ�
		dbmgr�㲥global���ݵĸı�
	*/
	void onBroadcastGlobalDataChange(Mercury::Channel* pChannel, std::string& key, std::string& value, bool isDelete);
	void onBroadcastGlobalBasesChange(Mercury::Channel* pChannel, std::string& key, std::string& value, bool isDelete);

protected:
	EntityIDClient						idClient_;
	Entities<Base>*						pBases_;									// �洢���е�base������
	TimerHandle							gameTimer_;

	GlobalDataClient*					pGlobalData_;								// globalData
	GlobalDataClient*					pGlobalBases_;								// globalBases
};

}
#endif
