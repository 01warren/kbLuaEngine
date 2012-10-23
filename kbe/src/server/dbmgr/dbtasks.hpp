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

#ifndef __DBTASKS_H__
#define __DBTASKS_H__

// common include	
// #define NDEBUG
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/memorystream.hpp"
#include "thread/threadtask.hpp"
#include "helper/debug_helper.hpp"
#include "entitydef/entitydef.hpp"
#include "network/address.hpp"

namespace KBEngine{ 
/*
	���ݿ��߳����������
*/

class DBTask : public thread::TPTask
{
public:
	DBTask(const Mercury::Address& addr, MemoryStream& datas);
	DBTask():
	pDatas_(0),
	addr_(Mercury::Address::NONE)
	{
	}

	DBTask(const Mercury::Address& addr):
	pDatas_(0),
	addr_(addr)
	{
	}

	virtual ~DBTask();
	virtual bool process() = 0;
	virtual void presentMainThread(){}
protected:
	MemoryStream* pDatas_;
	Mercury::Address addr_;
};

/**
	ִ��һ��sql���
*/
class DBTaskExecuteRawDatabaseCommand : public DBTask
{
public:
	DBTaskExecuteRawDatabaseCommand(const Mercury::Address& addr, MemoryStream& datas);
	virtual ~DBTaskExecuteRawDatabaseCommand();
	virtual bool process();
	virtual void presentMainThread();
protected:
	COMPONENT_ID componentID_;
	COMPONENT_TYPE componentType_;
	std::string sdatas_;
	CALLBACK_ID callbackID_;
	std::string error_;
	MemoryStream execret_;
};

/**
	�����ݿ�дentity�� ����entityʱҲ���������
*/
class DBTaskWriteEntity : public DBTask
{
public:
	DBTaskWriteEntity(const Mercury::Address& addr, MemoryStream& datas);
	virtual ~DBTaskWriteEntity();
	virtual bool process();
	virtual void presentMainThread();
protected:
	DBID entityDBID_;
	ENTITY_SCRIPT_UID sid_;
};

/**
	����һ���˺ŵ����ݿ�
*/
class DBTaskCreateAccount : public DBTask
{
public:
	DBTaskCreateAccount(const Mercury::Address& addr, std::string& accountName, std::string& password);
	virtual ~DBTaskCreateAccount();
	virtual bool process();
	virtual void presentMainThread();
protected:
	std::string accountName_;
	std::string password_;
	
};

/**
	baseapp�����ѯaccount��Ϣ
*/
class DBTaskQueryAccount : public DBTask
{
public:
	DBTaskQueryAccount(const Mercury::Address& addr, std::string& accountName, std::string& password);
	virtual ~DBTaskQueryAccount();
	virtual bool process();
	virtual void presentMainThread();
protected:
	std::string accountName_;
	std::string password_;
};

/**
	�˺�����
*/
class DBTaskAccountOnline : public DBTask
{
public:
	DBTaskAccountOnline(const Mercury::Address& addr, std::string& accountName,
		COMPONENT_ID componentID, ENTITY_ID entityID);
	virtual ~DBTaskAccountOnline();
	virtual bool process();
	virtual void presentMainThread();
protected:
	std::string accountName_;
	COMPONENT_ID componentID_;
	ENTITY_ID entityID_;
};


/**
	�˺�����
*/
class DBTaskAccountOffline : public DBTask
{
public:
	DBTaskAccountOffline(const Mercury::Address& addr, std::string& accountName);
	virtual ~DBTaskAccountOffline();
	virtual bool process();
	virtual void presentMainThread();
protected:
	std::string accountName_;
};


/**
	һ�����û���¼�� ��Ҫ���Ϸ���
*/
class DBTaskAccountLogin : public DBTask
{
public:
	DBTaskAccountLogin(const Mercury::Address& addr, std::string& accountName, std::string& password);
	virtual ~DBTaskAccountLogin();
	virtual bool process();
	virtual void presentMainThread();
protected:
	std::string accountName_;
	std::string password_;
};

}
#endif
