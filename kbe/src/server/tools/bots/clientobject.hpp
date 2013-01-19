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

#ifndef __CLIENT_OBJECT_H__
#define __CLIENT_OBJECT_H__

// common include	
// #define NDEBUG
#include "entity.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/memorystream.hpp"
#include "thread/threadtask.hpp"
#include "helper/debug_helper.hpp"
#include "entitydef/entitydef.hpp"
#include "entitydef/entities.hpp"
#include "entitydef/entity_mailbox.hpp"
#include "network/address.hpp"
#include "network/endpoint.hpp"
#include "network/bundle.hpp"
#include "network/tcp_packet.hpp"
#include "network/tcp_packet_receiver.hpp"
#include "pyscript/script.hpp"
#include "pyscript/scriptobject.hpp"
#include "pyscript/pyobject_pointer.hpp"

namespace KBEngine{ 

/*
*/

class ClientObject : public script::ScriptObject, Mercury::TCPPacketReceiver
{
	/** 
		���໯ ��һЩpy�������������� 
	*/
	INSTANCE_SCRIPT_HREADER(ClientObject, ScriptObject)	
public:
	enum C_ERROR
	{
		C_ERROR_NONE = 0,
		C_ERROR_INIT_NETWORK_FAILED = 1,
		C_ERROR_CREATE_FAILED = 2,
		C_ERROR_LOGIN_FAILED = 3,
		C_ERROR_LOGIN_GATEWAY_FAILED = 4,
	};

	enum C_STATE
	{
		C_STATE_INIT = 0,
		C_STATE_LOGIN = 1,
		C_STATE_LOGIN_GATEWAY = 2,
		C_STATE_PLAY = 3,
	};

	ClientObject(std::string name);
	virtual ~ClientObject();

	bool processSocket(bool expectingPacket);

	bool initCreate();
	bool createAccount();
	bool login();

	bool initLoginGateWay();
	bool loginGateWay();

	void gameTick();

	const char* name(){ return name_.c_str(); }

	Mercury::Channel* pChannel(){ return pChannel_; }

	DECLARE_PY_GET_MOTHOD(pyGetEntities);

	/**
		�����˺ųɹ���ʧ�ܻص�
	   @failedcode: ʧ�ܷ����� MERCURY_ERR_SRV_NO_READY:������û��׼����, 
									MERCURY_ERR_ACCOUNT_CREATE:����ʧ�ܣ��Ѿ����ڣ�, 
									MERCURY_SUCCESS:�˺Ŵ����ɹ�

									SERVER_ERROR_CODE failedcode;
		@�����Ƹ�������:�����ƶ�������: uint32���� + bytearray
	*/
	void onCreateAccountResult(MemoryStream& s);

	ClientObject::C_ERROR lasterror(){ return error_; }

	/** ����ӿ�
	   ��¼ʧ�ܻص�
	   @failedcode: ʧ�ܷ����� MERCURY_ERR_SRV_NO_READY:������û��׼����, 
									MERCURY_ERR_SRV_OVERLOAD:���������ع���, 
									MERCURY_ERR_NAME_PASSWORD:�û����������벻��ȷ
	*/
	virtual void onLoginFailed(MemoryStream& s);

	/** ����ӿ�
	   ��¼�ɹ�
	   @ip: ������ip��ַ
	   @port: �������˿�
	*/
	virtual void onLoginSuccessfully(MemoryStream& s);

	/** ����ӿ�
	   ��¼ʧ�ܻص�
	   @failedcode: ʧ�ܷ����� MERCURY_ERR_SRV_NO_READY:������û��׼����, 
									MERCURY_ERR_ILLEGAL_LOGIN:�Ƿ���¼, 
									MERCURY_ERR_NAME_PASSWORD:�û����������벻��ȷ
	*/
	virtual void onLoginGatewayFailed(SERVER_ERROR_CODE failedcode);

	/** ����ӿ�
		���������Ѿ�������һ����ͻ��˹����Ĵ���Entity
	   �ڵ�¼ʱҲ�ɱ��ɹ��ص�
	   @datas: �˺�entity����Ϣ
	*/
	virtual void onCreatedProxies(uint64 rndUUID, 
		ENTITY_ID eid, std::string& entityType);

	/** ����ӿ�
		���������Ѿ�������һ��Entity
	*/
	virtual void onCreatedEntity(ENTITY_ID eid, std::string& entityType);

	/** ����ӿ�
		�������ϵ�entity�Ѿ�����һ��cell����
	*/
	virtual void onEntityGetCell(ENTITY_ID eid);

	/** ����ӿ�
		�������ϵ�entity�Ѿ�������Ϸ������
	*/
	virtual void onEntityEnterWorld(ENTITY_ID eid, SPACE_ID spaceID);

	/** ����ӿ�
		�������ϵ�entity�Ѿ��뿪��Ϸ������
	*/
	virtual void onEntityLeaveWorld(ENTITY_ID eid, SPACE_ID spaceID);

	/** ����ӿ�
		���߿ͻ���ĳ��entity�����ˣ� ����entityͨ���ǻ�δonEntityEnterWorld
	*/
	virtual void onEntityDestroyed(ENTITY_ID eid);

	/** ����ӿ�
		�������ϵ�entity�Ѿ�����space��
	*/
	virtual void onEntityEnterSpace(SPACE_ID spaceID, ENTITY_ID eid);

	/** ����ӿ�
		�������ϵ�entity�Ѿ��뿪space��
	*/
	virtual void onEntityLeaveSpace(SPACE_ID spaceID, ENTITY_ID eid);

	/** ����ӿ�
		Զ�̵���entity�ķ��� 
	*/
	virtual void onRemoteMethodCall(MemoryStream& s);

	/** ����ӿ�
		����������entity����
	*/
	virtual void onUpdatePropertys(MemoryStream& s);

	void sendTick();

	Entities<Entity>* pEntities()const{ return pEntities_; }

	/**
		����һ��entity 
	*/
	Entity* createEntityCommon(const char* entityType, PyObject* params,
		bool isInitializeScript = true, ENTITY_ID eid = 0, bool initProperty = true);

	PY_CALLBACKMGR& callbackMgr(){ return pyCallbackMgr_; }	

	int32 appID()const{ return appID_; }
	static PyObject* __pyget_pyGetID(ClientObject *self, void *closure){
		return PyLong_FromLong(self->appID());	
	}

protected:
	int32 appID_;

	Mercury::Channel* pChannel_;

	std::string name_;
	std::string password_;

	PyObjectPtr	entryScript_;

	C_ERROR error_;
	C_STATE state_;

	ENTITY_ID entityID_;
	DBID dbid_;

	std::string ip_;
	uint16 port_;

	uint64 lastSentActiveTickTime_;

	bool connectedGateway_;

	// �洢���е�entity������
	Entities<Entity>* pEntities_;	

	PY_CALLBACKMGR	pyCallbackMgr_;
};


}
#endif
