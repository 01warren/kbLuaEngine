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
#include "base.hpp"
#include "server/entity_app.hpp"
#include "server/pendingLoginmgr.hpp"
#include "network/endpoint.hpp"

//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

namespace Mercury{
	class Channel;
}

class Baseapp :	public EntityApp<Base>, 
				public Singleton<Baseapp>
{
public:
	enum TimeOutType
	{
		TIMEOUT_MAX = TIMEOUT_ENTITYAPP_MAX + 1
	};
	
	Baseapp(Mercury::EventDispatcher& dispatcher, 
		Mercury::NetworkInterface& ninterface, 
		COMPONENT_TYPE componentType,
		COMPONENT_ID componentID);

	~Baseapp();
	
	virtual bool installPyModules();
	virtual void onInstallPyModules();
	virtual bool uninstallPyModules();

	bool run();
	
	/* ��ش���ӿ� */
	virtual void handleTimeout(TimerHandle handle, void * arg);
	virtual void handleGameTick();

	/* ��ʼ����ؽӿ� */
	bool initializeBegin();
	bool initializeEnd();
	void finalise();
	
	/* ����ӿ�
		dbmgr��֪�Ѿ�����������baseapp����cellapp�ĵ�ַ
		��ǰapp��Ҫ������ȥ�����ǽ�������
	*/
	virtual void onGetEntityAppFromDbmgr(Mercury::Channel* pChannel, 
							int32 uid, 
							std::string& username, 
							int8 componentType, uint64 componentID, 
							uint32 intaddr, uint16 intport, uint32 extaddr, uint16 extport);

	virtual Base* onCreateEntityCommon(PyObject* pyEntity, ScriptModule* sm, ENTITY_ID eid);

	/* ����һ��entity */
	static PyObject* __py_createBase(PyObject* self, PyObject* args);
	static PyObject* __py_createBaseAnywhere(PyObject* self, PyObject* args);

	/** ����һ���µ�space */
	void createInNewSpace(Base* base, PyObject* cell);

	/** ��һ�����ؽϵ͵�baseapp�ϴ���һ��baseEntity */
	void createBaseAnywhere(const char* entityType, PyObject* params, PyObject* pyCallback);

	/** �յ�baseappmgr������ĳ��baseappҪ��createBaseAnywhere�������ڱ�baseapp��ִ�� 
		@param entityType	: entity����� entities.xml�еĶ���ġ�
		@param strInitData	: ���entity��������Ӧ�ø�����ʼ����һЩ���ݣ� ��Ҫʹ��pickle.loads���.
		@param componentID	: ���󴴽�entity��baseapp�����ID
	*/
	void onCreateBaseAnywhere(Mercury::Channel* pChannel, MemoryStream& s);

	/** baseapp ��createBaseAnywhere�Ļص� */
	void onCreateBaseAnywhereCallback(Mercury::Channel* pChannel, KBEngine::MemoryStream& s);
	void _onCreateBaseAnywhereCallback(Mercury::Channel* pChannel, CALLBACK_ID callbackID, 
		std::string& entityType, ENTITY_ID eid, COMPONENT_ID componentID);

	/** Ϊһ��baseEntity���ƶ���cell�ϴ���һ��cellEntity */
	void createCellEntity(EntityMailboxAbstract* createToCellMailbox, Base* base);
	
	/** ����ӿ�
		createCellEntity��cellʵ�崴���ɹ��ص���
	*/
	void onEntityGetCell(Mercury::Channel* pChannel, ENTITY_ID id, COMPONENT_ID componentID);

	/** ֪ͨ�ͻ��˴���һ��proxy��Ӧ��ʵ�� */
	bool createClientProxyEntity(Mercury::Channel* pChannel, Base* base);

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
	void onBroadcastGlobalBasesChange(Mercury::Channel* pChannel, KBEngine::MemoryStream& s);

	/** ����ӿ�
	ע�ὫҪ��¼���˺�, ע����������¼��������
	*/
	void registerPendingLogin(Mercury::Channel* pChannel, std::string& accountName, std::string& password);

	/** ����ӿ�
		���û������¼��������
	*/
	void loginGateway(Mercury::Channel* pChannel, std::string& accountName, std::string& password);

	/*
	   ��¼ʧ��
	   @failedcode: ʧ�ܷ����� 0=��¼�Ƿ�����ʱ���߷Ƿ����룩, 
							   1=�˺Ż������벻��ȷ
	*/
	void loginGatewayFailed(Mercury::Channel* pChannel, std::string& accountName, int8 failedcode);

protected:
	GlobalDataClient*					pGlobalBases_;								// globalBases

	// ��¼��¼������������δ������ϵ��˺�
	PendingLoginMgr pendingLoginMgr_;
};

}
#endif
