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

#ifndef __CELLAPP_H__
#define __CELLAPP_H__
	
// common include	
#include "server/kbemain.hpp"
#include "server/entity_app.hpp"
#include "server/idallocate.hpp"
#include "server/serverconfig.hpp"
#include "entitydef/entities.hpp"
#include "cstdkbe/timer.hpp"

//#define NDEBUG
#include <map>	
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{
class Entity;

class Cellapp:	public EntityApp, 
				public TimerHandler, 
				public Singleton<Cellapp>
{
public:
	enum TimeOutType
	{
		TIMEOUT_GAME_TICK,
		TIMEOUT_LOADING_TICK
	};
	
	Cellapp(Mercury::EventDispatcher& dispatcher, 
		Mercury::NetworkInterface& ninterface, 
		COMPONENT_TYPE componentType,
		COMPONENT_ID componentID);

	~Cellapp();

	bool installPyModules();
	bool uninstallPyModules();
	
	bool run();
	
	/* ��ش���ӿ� */
	void handleTimeout(TimerHandle handle, void * arg);
	void handleGameTick();
	void handleTimers();

	/* ��ʼ����ؽӿ� */
	bool initializeBegin();
	bool initializeEnd();
	void finalise();


	/* ����һ��entity */
	static PyObject* __py_createEntity(PyObject* self, PyObject* args);
	Entity* createEntity(const char* entityType, PyObject* params, bool isInitializeScript = true, ENTITY_ID eid = 0);
	
	/* ͨ��entityIDѰ�ҵ���Ӧ��ʵ�� */
	Entity* findEntity(ENTITY_ID eid);
	
	/* ��mailbox�����Ի�ȡһ��entity��ʵ��
		��Ϊ�������ϲ�һ���������entity��
	*/
	PyObject* tryGetEntityByMailbox(COMPONENT_ID componentID, ENTITY_ID eid);

	/* ͨ��entityID����һ��entity */
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
	void onDbmgrInit(Mercury::Channel* pChannel, 
		ENTITY_ID startID, ENTITY_ID endID, int32 startGlobalOrder, int32 startGroupOrder);
protected:
	EntityIDClient				idClient_;
	Entities<Entity>*			pEntities_;									// �洢���е�entity������
	TimerHandle					gameTimer_;
};

}
#endif
