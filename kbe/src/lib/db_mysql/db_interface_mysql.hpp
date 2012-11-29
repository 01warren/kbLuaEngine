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

#include "common.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/singleton.hpp"
#include "cstdkbe/memorystream.hpp"
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

struct TABLE_FIELD
{
	std::string name;
	int32 length;
	uint64 maxlength;
	unsigned int flags;
	enum_field_types type;
};

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

	virtual bool query(const char* strCommand, uint32 size, bool showexecinfo = true);

	bool execute(const char* strCommand, uint32 size, MemoryStream * resdata);

	/**
		��ȡ���ݿ����еı���
	*/
	virtual bool getTableNames( std::vector<std::string>& tableNames, const char * pattern);

	/**
		��ȡ���ݿ�ĳ�������е��ֶ�����
	*/
	virtual bool getTableItemNames(const char* tablename, std::vector<std::string>& itemNames);

	/** 
		�����ݿ�ɾ��entity���ֶ�
	*/
	virtual bool dropEntityTableItemFromDB(const char* tablename, const char* tableItemName);

	MYSQL* mysql(){ return pMysql_; }

	my_ulonglong insertID()		{ return mysql_insert_id( pMysql_ ); }

	my_ulonglong affectedRows()	{ return mysql_affected_rows( pMysql_ ); }

	const char* info()			{ return mysql_info( pMysql_ ); }

	const char* getLastError()	{ return mysql_error( pMysql_ ); }

	unsigned int getLastErrorNum() { return mysql_errno( pMysql_ ); }

	typedef std::tr1::unordered_map<std::string, TABLE_FIELD> TABLE_FIELDS;
	void getFields(TABLE_FIELDS& outs, const char* tablename);

	/**
		��������ӿڵ�����
	*/
	virtual const char* c_str();

	/** 
		��ȡ����
	*/
	virtual const char* getstrerror();

	/** 
		��ȡ������
	*/
	virtual int getlasterror();

	/**
		����һ��entity�洢��
	*/
	virtual EntityTable* createEntityTable();

	/** 
		�����ݿ�ɾ��entity��
	*/
	virtual bool dropEntityTableFromDB(const char* tablename);
protected:
	MYSQL* pMysql_;
	bool hasLostConnection_;
};


}

#endif // __KBE_DB_INTERFACE_MYSQL__
