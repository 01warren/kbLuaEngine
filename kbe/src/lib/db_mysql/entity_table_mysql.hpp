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

#ifndef __KBE_ENTITY_TABLE_MYSQL__
#define __KBE_ENTITY_TABLE_MYSQL__

#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/singleton.hpp"
#include "helper/debug_helper.hpp"
#include "dbmgr_lib/entity_table.hpp"

namespace KBEngine { 

class ScriptDefModule;

/*
	ά��entity�����ݿ��еı��е�һ���ֶ�
*/
class EntityTableItemMysql_INT : public EntityTableItem
{
public:
	EntityTableItemMysql_INT(){};
	virtual ~EntityTableItemMysql_INT(){};

	/**
		��ʼ��
	*/
	virtual bool initialize(DBInterface* dbi, const PropertyDescription* pPropertyDescription, const DataType* pDataType);

	/**
		ͬ��entity�����ݿ���
	*/
	virtual bool syncToDB();
protected:
};

class EntityTableItemMysql_UINT : public EntityTableItemMysql_INT
{
public:
	/**
		ͬ��entity�����ݿ���
	*/
	virtual bool syncToDB();
};

class EntityTableItemMysql_INT64 : public EntityTableItemMysql_INT
{
public:
	/**
		ͬ��entity�����ݿ���
	*/
	virtual bool syncToDB();
};

class EntityTableItemMysql_UINT64 : public EntityTableItemMysql_INT
{
public:
	/**
		ͬ��entity�����ݿ���
	*/
	virtual bool syncToDB();
};

class EntityTableItemMysql_STRING : public EntityTableItemMysql_INT
{
public:
	/**
		ͬ��entity�����ݿ���
	*/
	virtual bool syncToDB();
};

class EntityTableItemMysql_FLOAT : public EntityTableItemMysql_INT
{
public:
	/**
		ͬ��entity�����ݿ���
	*/
	virtual bool syncToDB();
};

class EntityTableItemMysql_DOUBLE : public EntityTableItemMysql_INT
{
public:
	/**
		ͬ��entity�����ݿ���
	*/
	virtual bool syncToDB();
};

class EntityTableItemMysql_ARRAY : public EntityTableItemMysql_INT
{
public:
	/**
		��ʼ��
	*/
	virtual bool initialize(DBInterface* dbi, const PropertyDescription* pPropertyDescription, const DataType* pDataType);

	/**
		ͬ��entity�����ݿ���
	*/
	virtual bool syncToDB();
protected:
	std::tr1::shared_ptr<EntityTableItem> arrayTableItem_;
};

class EntityTableItemMysql_FIXED_DICT : public EntityTableItemMysql_INT
{
public:
	typedef std::map<std::string, std::tr1::shared_ptr<EntityTableItem>> FIXEDDICT_KEYTYPE_MAP;

	/**
		��ʼ��
	*/
	virtual bool initialize(DBInterface* dbi, const PropertyDescription* pPropertyDescription, const DataType* pDataType);

	/**
		ͬ��entity�����ݿ���
	*/
	virtual bool syncToDB();
protected:
	FIXEDDICT_KEYTYPE_MAP			keyTypes_;		// ����̶��ֵ���ĸ���key������
};


/*
	ά��entity�����ݿ��еı�
*/
class EntityTableMysql : public EntityTable
{
public:
	EntityTableMysql();
	virtual ~EntityTableMysql();
	
	/**
		��ʼ��
	*/
	virtual bool initialize(DBInterface* dbi, ScriptDefModule* sm);

	/**
		ͬ��entity�����ݿ���
	*/
	virtual bool syncToDB();

	/** 
		����һ����item
	*/
	virtual EntityTableItem* createItem(std::string type);
protected:
};


}

#endif // __KBE_ENTITY_TABLE_MYSQL__
