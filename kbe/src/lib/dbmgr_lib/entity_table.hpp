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

#ifndef __KBE_ENTITY_TABLE__
#define __KBE_ENTITY_TABLE__

#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/singleton.hpp"
#include "helper/debug_helper.hpp"
#include "entitydef/common.hpp"

namespace KBEngine { 

class DBUtil;
class DBInterface;
class ScriptDefModule;
class DataType;
class PropertyDescription;
class EntityTable;

/*
	ά��entity�����ݿ��еı��е�һ���ֶ�
*/
class EntityTableItem
{
public:
	EntityTableItem(std::string itemDBType, uint32 datalength):
		itemName_(),
		utype_(0),
		pdbi_(NULL),
		pParentTable_(NULL),
		pDataType_(NULL),
		pPropertyDescription_(NULL),
		itemDBType_(itemDBType),
		datalength_(datalength)
	{
	};

	virtual ~EntityTableItem(){};

	void itemName(std::string name){ itemName_ = name; }
	const char* itemName(){ return itemName_.c_str(); }

	void utype(ENTITY_PROPERTY_UID utype){ utype_ = utype; }
	ENTITY_PROPERTY_UID utype(){ return utype_; }

	void pParentTable(EntityTable* v){ pParentTable_ = v; }
	EntityTable* pParentTable(){ return pParentTable_; }

	/**
		��ʼ��
	*/
	virtual bool initialize(DBInterface* dbi, const PropertyDescription* pPropertyDescription, const DataType* pDataType) = 0;

	/**
		ͬ��entity�����ݿ���
	*/
	virtual bool syncToDB() = 0;
protected:
	// �ֶ�����
	std::string itemName_;
	ENTITY_PROPERTY_UID utype_;

	DBInterface* pdbi_;

	EntityTable* pParentTable_;

	const DataType* pDataType_;
	const PropertyDescription* pPropertyDescription_;

	std::string itemDBType_;
	uint32 datalength_;
};

/*
	ά��entity�����ݿ��еı�
*/
class EntityTable
{
public:
	typedef std::map<ENTITY_PROPERTY_UID, std::tr1::shared_ptr<EntityTableItem> > TABLEITEM_MAP;

	EntityTable(){};
	virtual ~EntityTable(){};
	
	void tableName(std::string name){ tableName_ = name; }
	const char* tableName(){ return tableName_.c_str(); }

	/**
		��ʼ��
	*/
	virtual bool initialize(DBInterface* dbi, ScriptDefModule* sm) = 0;

	/**
		ͬ��entity�����ݿ���
	*/
	virtual bool syncToDB() = 0;

	/** 
		����һ����item
	*/
	virtual EntityTableItem* createItem(std::string type) = 0;

	/** 
		������б��ֶ�
	*/
	const EntityTable::TABLEITEM_MAP& tableItems()const { return tableItems_; }
protected:

	// ������
	std::string tableName_;

	// ���е��ֶ�
	TABLEITEM_MAP tableItems_;

	DBInterface* pdbi_;
};

class EntityTables : public Singleton<EntityTables>
{
public:
	typedef std::tr1::unordered_map<std::string, std::tr1::shared_ptr<EntityTable> > TABLES_MAP;
	EntityTables();
	virtual ~EntityTables();
	
	bool load(DBInterface* dbi);

	bool syncToDB();

	/** 
		������б�
	*/
	const EntityTables::TABLES_MAP& tables()const { return tables_; }
protected:
	// ���е��ֶ�
	TABLES_MAP tables_;
	DBInterface* pdbi_;
};

}

#endif // __KBE_ENTITY_TABLE__
