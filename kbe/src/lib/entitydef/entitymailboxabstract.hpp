/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
#ifndef __ENTITY_MAILBOX_BASE_H__
#define __ENTITY_MAILBOX_BASE_H__
	
// common include	
#include "cstdkbe/cstdkbe.hpp"
//#include "network/channel.hpp"
#include "pyscript/scriptobject.hpp"
#include "entitydef/common.hpp"

//#define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>	
#include <map>	
#include <vector>	
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#include <time.h> 
#else
// linux include
#include <errno.h>
#endif
	
namespace KBEngine{

namespace Mercury
{
class Channel;
}

class EntityMailboxAbstract : public script::ScriptObject
{
	/** ���໯ ��һЩpy�������������� */
	INSTANCE_SCRIPT_HREADER(EntityMailboxAbstract, ScriptObject)
public:
	EntityMailboxAbstract(PyTypeObject* scriptType, Mercury::Channel* pChannel, COMPONENT_ID componentID, 
	ENTITY_ID eid, uint16 utype, ENTITY_MAILBOX_TYPE type);
	virtual ~EntityMailboxAbstract();

	/** ��ȡentityID */
	ENTITY_ID getID()const{ return id_; }
	void setID(int id){ id_ = id; }
	DECLARE_PY_GET_MOTHOD(pyGetID);

	/** ������ID */
	COMPONENT_ID getComponentID(void)const{ return componentID_; }

	/** ���utype */
	uint16 getUType(void)const{ return utype_; }

	/** ���type */
	ENTITY_MAILBOX_TYPE getType(void)const{ return type_; }
	
	/** ����һ����������entity���ڻ����� */
//	virtual SocketPacket* createStream(Opcodes code);
//	virtual SocketPacket* createMail(MAIL_TYPE mailType);
//	virtual void post(Packet* sp);

	/** ֧��pickler ���� */
	static PyObject* __py_reduce_ex__(PyObject* self, PyObject* protocol);
	
	/** �������mailbox��������socketƵ�� ��proxy����giveClientTo����ʱ�� ��������ΪNULL*/
	void setChannel(Mercury::Channel* pChannel){ pChannelPtr_ = pChannel; };
	Mercury::Channel* getChannel(void)const{ return pChannelPtr_; }
protected:
	Mercury::Channel*						pChannelPtr_;			// ��mailbox��������Զ�˻���ͨ��ͨ��
	COMPONENT_ID							componentID_;			// Զ�˻��������ID
	ENTITY_MAILBOX_TYPE						type_;					// ��mailbox������
	ENTITY_ID								id_;					// entityID
	uint16									utype_;					// entity��utype  ����entities.xml�еĶ���˳��
};

}
#endif
