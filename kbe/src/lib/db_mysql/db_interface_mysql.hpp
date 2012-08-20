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

#ifndef __KBE_DB_INTERFACE_MYSQL__
#define __KBE_DB_INTERFACE_MYSQL__

#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/singleton.hpp"
#include "helper/debug_helper.hpp"
#include "dbmgr_lib/db_interface.hpp"

#include "mysql/mysql.h"
#if KBE_PLATFORM == PLATFORM_WIN32
#ifdef _DEBUG
#pragma comment (lib, "libmysql_d.lib")
#pragma comment (lib, "mysqlclient_d.lib")
#else
#pragma comment (lib, "libmysql.lib")
#pragma comment (lib, "mysqlclient.lib")
#endif
#endif

namespace KBEngine { 

/*
	���ݿ�ӿ�
*/
class DBInterfaceMysql : public DBInterface
{
public:
	DBInterfaceMysql();
	virtual ~DBInterfaceMysql();

	/**
		��ĳ�����ݿ����
	*/
	virtual bool attach(const char* databaseName);
	virtual bool detach();

	virtual bool query(const char* strCommand);

	MYSQL* mysql(){ return pMysql_; }

	/**
		��������ӿڵ�����
	*/
	virtual const char* c_str();
protected:
	MYSQL* pMysql_;
};


}

#endif // __KBE_DB_INTERFACE_MYSQL__
