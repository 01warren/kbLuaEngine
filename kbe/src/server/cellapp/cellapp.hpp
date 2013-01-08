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
#include "entity.hpp"
#include "spaces.hpp"
#include "updatables.hpp"
#include "server/entity_app.hpp"
#include "server/forward_messagebuffer.hpp"

//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

class Cellapp:	public EntityApp<Entity>, 
				public Singleton<Cellapp>
{
public:
	enum TimeOutType
	{
		TIMEOUT_LOADING_TICK = TIMEOUT_ENTITYAPP_MAX + 1
	};
	
	Cellapp(Mercury::EventDispatcher& dispatcher, 
		Mercury::NetworkInterface& ninterface, 
		COMPONENT_TYPE componentType,
		COMPONENT_ID componentID);

	~Cellapp();

	virtual bool installPyModules();
	virtual void onInstallPyModules();
	virtual bool uninstallPyModules();
	
	bool run();
	
	virtual bool initializeWatcher();

	/**  
		��ش���ӿ� 
	*/
	virtual void handleTimeout(TimerHandle handle, void * arg);
	virtual void handleGameTick();

	/**  
		��ʼ����ؽӿ� 
	*/
	bool initializeBegin();
	bool initializeEnd();
	void finalise();

	/**  ����ӿ�
		dbmgr��֪�Ѿ�����������baseapp����cellapp�ĵ�ַ
		��ǰapp��Ҫ������ȥ�����ǽ�������
	*/
	virtual void onGetEntityAppFromDbmgr(Mercury::Channel* pChannel, 
							int32 uid, 
							std::string& username, 
							int8 componentType, uint64 componentID, 
							uint32 intaddr, uint16 intport, uint32 extaddr, uint16 extport);

	/**  
		����һ��entity 
	*/
	static PyObject* __py_createEntity(PyObject* self, PyObject* args);

	/** 
		��dbmgr����ִ��һ�����ݿ�����
	*/
	static PyObject* __py_executeRawDatabaseCommand(PyObject* self, PyObject* args);
	void executeRawDatabaseCommand(const char* datas, uint32 size, PyObject* pycallback);
	void onExecuteRawDatabaseCommandCB(Mercury::Channel* pChannel, KBEngine::MemoryStream& s);

	/** ����ӿ�
		dbmgr���ͳ�ʼ��Ϣ
		startID: ��ʼ����ENTITY_ID ����ʼλ��
		endID: ��ʼ����ENTITY_ID �ν���λ��
		startGlobalOrder: ȫ������˳�� �������ֲ�ͬ���
		startGroupOrder: ��������˳�� ����������baseapp�еڼ���������
	*/
	void onDbmgrInitCompleted(Mercury::Channel* pChannel, GAME_TIME gametime, 
		ENTITY_ID startID, ENTITY_ID endID, int32 startGlobalOrder, int32 startGroupOrder);

	/** ����ӿ�
		dbmgr�㲥global���ݵĸı�
	*/
	void onBroadcastCellAppDataChange(Mercury::Channel* pChannel, KBEngine::MemoryStream& s);

	/** ����ӿ�
		baseEntity���󴴽���һ���µ�space��
	*/
	void onCreateInNewSpaceFromBaseapp(Mercury::Channel* pChannel, KBEngine::MemoryStream& s);

	/** ����ӿ�
		baseapp���������cellapp�ϴ���һ��entity
	*/
	void onCreateCellEntityFromBaseapp(Mercury::Channel* pChannel, KBEngine::MemoryStream& s);
	void _onCreateCellEntityFromBaseapp(std::string& entityType, ENTITY_ID createToEntityID, ENTITY_ID entityID, 
		MemoryStream* pCellData, bool hasClient, COMPONENT_ID componentID, SPACE_ID spaceID);

	/** ����ӿ�
		����ĳ��cellEntity
	*/
	void onDestroyCellEntityFromBaseapp(Mercury::Channel* pChannel, ENTITY_ID eid);

	/** ����ӿ�
		entity�յ�һ��mail, ��ĳ��app�ϵ�mailbox����
	*/
	void onEntityMail(Mercury::Channel* pChannel, KBEngine::MemoryStream& s);
	
	/** ����ӿ�
		client����entity��cell������baseappת��
	*/
	void onRemoteCallMethodFromClient(Mercury::Channel* pChannel, KBEngine::MemoryStream& s);

	/** ����ӿ�
		base�����ȡcelldata
	*/
	void reqBackupEntityCellData(Mercury::Channel* pChannel, KBEngine::MemoryStream& s);

	/** ����ӿ�
		base�����ȡWriteToDB
	*/
	void reqWriteToDBFromBaseapp(Mercury::Channel* pChannel, KBEngine::MemoryStream& s);

	/** ����ӿ�
		�ͻ���ֱ�ӷ�����Ϣ��cellʵ��
	*/
	void forwardEntityMessageToCellappFromClient(Mercury::Channel* pChannel, MemoryStream& s);

	/**
		��ȡ��Ϸʱ��
	*/
	static PyObject* __py_gametime(PyObject* self, PyObject* args);

	void updateLoad();
protected:
	GlobalDataClient*					pCellAppData_;									// cellAppData
	ForwardComponent_MessageBuffer		forward_messagebuffer_;

	Updatables updatables_;
};

}
#endif
