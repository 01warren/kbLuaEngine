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


#ifndef __CLIENT_OBJECT_BASE_H__
#define __CLIENT_OBJECT_BASE_H__

#include "event.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/memorystream.hpp"
#include "helper/debug_helper.hpp"
#include "pyscript/scriptobject.hpp"
#include "entitydef/entities.hpp"
#include "entitydef/common.hpp"
#include "server/callbackmgr.hpp"
#include "server/server_errors.hpp"

namespace KBEngine{

namespace client{
class Entity;
}

class EntityMailbox;

namespace Mercury
{
class Channel;
}

class ClientObjectBase : public script::ScriptObject
{
	/** 
		���໯ ��һЩpy�������������� 
	*/
	INSTANCE_SCRIPT_HREADER(ClientObjectBase, ScriptObject)	
public:
	ClientObjectBase(Mercury::NetworkInterface& ninterface, PyTypeObject* pyType = NULL);
	virtual ~ClientObjectBase();

	Mercury::Channel* pServerChannel()const{ return pServerChannel_; }

	void finalise(void);

	Entities<client::Entity>* pEntities()const{ return pEntities_; }

	/**
		����һ��entity 
	*/
	client::Entity* createEntityCommon(const char* entityType, PyObject* params,
		bool isInitializeScript = true, ENTITY_ID eid = 0, bool initProperty = true, 
		EntityMailbox* base = NULL, EntityMailbox* cell = NULL);

	PY_CALLBACKMGR& callbackMgr(){ return pyCallbackMgr_; }	

	/**
		ͨ��entityID����һ��entity 
	*/
	virtual bool destroyEntity(ENTITY_ID entityID);

	void tickSend();
	
	virtual Mercury::Channel* initLoginappChannel(std::string accountName, std::string passwd, std::string ip, KBEngine::uint32 port);
	virtual Mercury::Channel* initBaseappChannel();

	bool createAccount();
	bool login();
	bool loginGateWay();

	int32 appID()const{ return appID_; }
	const char* name(){ return name_.c_str(); }

	ENTITY_ID entityID(){ return entityID_; }
	DBID dbid(){ return dbid_; }

	bool registerEventHandle(EventHandle* pEventHandle);
	bool deregisterEventHandle(EventHandle* pEventHandle);
	
	void fireEvent(const EventData* pEventData);

	static PyObject* __pyget_pyGetEntities(PyObject *self, void *closure)
	{
		ClientObjectBase* pClientObjectBase = static_cast<ClientObjectBase*>(self);
		Py_INCREF(pClientObjectBase->pEntities());
		return pClientObjectBase->pEntities(); 
	}

	static PyObject* __pyget_pyGetID(PyObject *self, void *closure){
		
		ClientObjectBase* pClientObjectBase = static_cast<ClientObjectBase*>(self);
		return PyLong_FromLong(pClientObjectBase->appID());	
	}

	/**
		��mailbox�����Ի�ȡһ��channel��ʵ��
	*/
	virtual Mercury::Channel* findChannelByMailbox(EntityMailbox& mailbox);

	/** ����ӿ�
		�ͻ��������˵�һ�ν�������, ����˷���
	*/
	virtual void onHelloCB_(Mercury::Channel* pChannel, const std::string& verInfo, 
		COMPONENT_TYPE componentType);

	virtual void onHelloCB(Mercury::Channel* pChannel, MemoryStream& s);

	/** ����ӿ�
		�����˺ųɹ���ʧ�ܻص�
	   @failedcode: ʧ�ܷ����� MERCURY_ERR_SRV_NO_READY:������û��׼����, 
									MERCURY_ERR_ACCOUNT_CREATE:����ʧ�ܣ��Ѿ����ڣ�, 
									MERCURY_SUCCESS:�˺Ŵ����ɹ�

									SERVER_ERROR_CODE failedcode;
		@�����Ƹ�������:�����ƶ�������: uint32���� + bytearray
	*/
	virtual void onCreateAccountResult(Mercury::Channel * pChannel, MemoryStream& s);

	/** ����ӿ�
	   ��¼ʧ�ܻص�
	   @failedcode: ʧ�ܷ����� MERCURY_ERR_SRV_NO_READY:������û��׼����, 
									MERCURY_ERR_SRV_OVERLOAD:���������ع���, 
									MERCURY_ERR_NAME_PASSWORD:�û����������벻��ȷ
	*/
	virtual void onLoginFailed(Mercury::Channel * pChannel, MemoryStream& s);

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
	virtual void onLoginGatewayFailed(Mercury::Channel * pChannel, SERVER_ERROR_CODE failedcode);

	/** ����ӿ�
		���������Ѿ�������һ����ͻ��˹����Ĵ���Entity
	   �ڵ�¼ʱҲ�ɱ��ɹ��ص�
	   @datas: �˺�entity����Ϣ
	*/
	virtual void onCreatedProxies(Mercury::Channel * pChannel, uint64 rndUUID, 
		ENTITY_ID eid, std::string& entityType);

	/** ����ӿ�
		�������ϵ�entity�Ѿ�������Ϸ������
	*/
	virtual void onEntityEnterWorld(Mercury::Channel * pChannel, ENTITY_ID eid, 
		ENTITY_SCRIPT_UID scriptType, SPACE_ID spaceID);

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

	/** ����ӿ�
		download stream��ʼ�� 
	*/
	virtual void onStreamDataStarted(Mercury::Channel* pChannel, int16 id, uint32 datasize, std::string& descr);

	/** ����ӿ�
		���յ�streamData
	*/
	virtual void onStreamDataRecv(Mercury::Channel* pChannel, MemoryStream& s);

	/** ����ӿ�
		download stream����� 
	*/
	virtual void onStreamDataCompleted(Mercury::Channel* pChannel, int16 id);
protected:				
	int32 appID_;

	// ���������ͨ��
	Mercury::Channel*										pServerChannel_;

	// �洢���е�entity������
	Entities<client::Entity>*								pEntities_;	

	PY_CALLBACKMGR											pyCallbackMgr_;

	ENTITY_ID entityID_;
	DBID dbid_;

	std::string ip_;
	uint16 port_;

	uint64 lastSentActiveTickTime_;

	bool connectedGateway_;

	std::string name_;
	std::string password_;
	std::string extradatas_;

	CLIENT_CTYPE typeClient_;

	typedef std::map<ENTITY_ID, KBEShared_ptr<MemoryStream> > BUFFEREDMESSAGE;
	BUFFEREDMESSAGE											bufferedCreateEntityMessage_;

	EventHandler											eventHandler_;
};

}
#endif
