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

#ifndef __SQL_COMMAND_CREATE_FROM_DATAS_H__
#define __SQL_COMMAND_CREATE_FROM_DATAS_H__

// common include	
// #define NDEBUG
#include <sstream>
#include "common.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/memorystream.hpp"
#include "helper/debug_helper.hpp"
#include "dbmgr_lib/db_interface.hpp"
#include "dbmgr_lib/entity_table.hpp"
#include "db_interface_mysql.hpp"

namespace KBEngine{ 

class SqlCommandCreateFromDatasBase
{
public:
	SqlCommandCreateFromDatasBase(DBInterface* dbi, std::string tableName, DBID parentDBID, DBID dbid, DB_W_OP_TABLE_ITEM_DATAS& tableItemDatas):
	  tableItemDatas_(tableItemDatas),
	  sqlstr_(),
	  tableName_(tableName),
	  dbid_(dbid),
	  parentDBID_(parentDBID),
	  dbi_(dbi)
	{
	}

	virtual ~SqlCommandCreateFromDatasBase()
	{
	}

	std::string& sql(){ return sqlstr_; }

	virtual bool query()
	{
		// û�����ݸ���
		if(sqlstr_ == "")
			return true;

		return static_cast<DBInterfaceMysql*>(dbi_)->query(sqlstr_.c_str(), sqlstr_.size(), false);
	}

	DBID dbid()const{ return dbid_; }
protected:
	DB_W_OP_TABLE_ITEM_DATAS& tableItemDatas_;
	std::string sqlstr_;
	std::string tableName_;
	DBID dbid_;
	DBID parentDBID_;
	DBInterface* dbi_; 
};

class SqlCommandCreateFromDatas_INSERT : public SqlCommandCreateFromDatasBase
{
public:
	SqlCommandCreateFromDatas_INSERT(DBInterface* dbi, std::string tableName, DBID parentDBID, DBID dbid, DB_W_OP_TABLE_ITEM_DATAS& tableItemDatas):
	  SqlCommandCreateFromDatasBase(dbi, tableName, parentDBID, dbid, tableItemDatas)
	{
		// insert into tbl_Account (sm_accountName) values("fdsafsad\0\fdsfasfsa\0fdsafsda");
		sqlstr_ = "insert into "ENTITY_TABLE_PERFIX"_";
		sqlstr_ += tableName;
		sqlstr_ += " (";
		sqlstr1_ = ")  values(";
		
		if(parentDBID > 0)
		{
			sqlstr_ += TABLE_PARENT_ID;
			sqlstr_ += ",";
			
			char strdbid[MAX_BUF];
			kbe_snprintf(strdbid, MAX_BUF, "%"PRDBID, parentDBID);
			sqlstr1_ += strdbid;
			sqlstr1_ += ",";
		}

		DB_W_OP_TABLE_ITEM_DATAS::iterator tableValIter = tableItemDatas.begin();
		for(; tableValIter != tableItemDatas.end(); tableValIter++)
		{
			std::tr1::shared_ptr<DB_W_OP_TABLE_ITEM_DATA> pSotvs = (*tableValIter);

			if(dbid > 0)
			{
			}
			else
			{
				sqlstr_ += pSotvs->sqlkey;
				if(pSotvs->extraDatas.size() > 0)
					sqlstr1_ += pSotvs->extraDatas;
				else
					sqlstr1_ += pSotvs->sqlval;

				sqlstr_ += ",";
				sqlstr1_ += ",";
			}
		}
		
		if(parentDBID > 0 || sqlstr_.at(sqlstr_.size() - 1) == ',')
			sqlstr_.erase(sqlstr_.size() - 1);

		if(parentDBID > 0 || sqlstr1_.at(sqlstr1_.size() - 1) == ',')
			sqlstr1_.erase(sqlstr1_.size() - 1);

		sqlstr1_ += ");";
		sqlstr_ += sqlstr1_;
	}

	virtual ~SqlCommandCreateFromDatas_INSERT()
	{
	}

	virtual bool query()
	{
		// û�����ݸ���
		if(sqlstr_ == "")
			return true;

		bool ret = SqlCommandCreateFromDatasBase::query();
		if(!ret)
		{
			ERROR_MSG("SqlCommandCreateFromDatas_INSERT::query: %s\n", dbi_->getstrerror());
			return false;
		}

		dbid_ = static_cast<DBInterfaceMysql*>(dbi_)->insertID();
		return ret;
	}

protected:
	
	std::string sqlstr1_;
};

class SqlCommandCreateFromDatas_UPDATE : public SqlCommandCreateFromDatasBase
{
public:
	SqlCommandCreateFromDatas_UPDATE(DBInterface* dbi, std::string tableName, DBID parentDBID, DBID dbid, DB_W_OP_TABLE_ITEM_DATAS& tableItemDatas):
	  SqlCommandCreateFromDatasBase(dbi, tableName, parentDBID, dbid, tableItemDatas)
	{
		if(tableItemDatas.size() == 0)
		{
			sqlstr_ = "";
			return;
		}

		// update tbl_Account set sm_accountName="fdsafsad" where id=123;
		sqlstr_ = "update "ENTITY_TABLE_PERFIX"_";
		sqlstr_ += tableName;
		sqlstr_ += " set ";

		DB_W_OP_TABLE_ITEM_DATAS::iterator tableValIter = tableItemDatas.begin();
		for(; tableValIter != tableItemDatas.end(); tableValIter++)
		{
			std::tr1::shared_ptr<DB_W_OP_TABLE_ITEM_DATA> pSotvs = (*tableValIter);
			
			sqlstr_ += pSotvs->sqlkey;
			sqlstr_ += "=";
				
			if(pSotvs->extraDatas.size() > 0)
				sqlstr_ += pSotvs->extraDatas;
			else
				sqlstr_ += pSotvs->sqlval;

			sqlstr_ += ",";
		}

		if(sqlstr_.at(sqlstr_.size() - 1) == ',')
			sqlstr_.erase(sqlstr_.size() - 1);

		sqlstr_ += " where id=";
		
		char strdbid[MAX_BUF];
		kbe_snprintf(strdbid, MAX_BUF, "%"PRDBID, dbid);
		sqlstr_ += strdbid;
		sqlstr_ += ";";
	}

	virtual ~SqlCommandCreateFromDatas_UPDATE()
	{
	}
protected:
};

class SqlCommandHelper
{
public:
	SqlCommandHelper(DBInterface* dbi, DB_TABLE_OP opType, std::string tableName, DBID parentDBID, DBID dbid, DB_W_OP_TABLE_ITEM_DATAS& tableVal)
	{
		switch(opType)
		{
		case TABLE_OP_UPDATE:
			if(dbid > 0)
				pSqlcmd_.reset(new SqlCommandCreateFromDatas_UPDATE(dbi, tableName, parentDBID, dbid, tableVal));
			else
				pSqlcmd_.reset(new SqlCommandCreateFromDatas_INSERT(dbi, tableName, parentDBID, dbid, tableVal));
			break;
		case TABLE_OP_INSERT:
			pSqlcmd_.reset(new SqlCommandCreateFromDatas_INSERT(dbi, tableName, parentDBID, dbid, tableVal));
			break;
		case TABLE_OP_DELETE:
			break;
		default:
			KBE_ASSERT(false && "no support!\n");
		};
	}

	virtual ~SqlCommandHelper()
	{
	}

	SqlCommandCreateFromDatasBase* operator ->()
	{
		return pSqlcmd_.get();
	}

	/**
		�����ݸ��µ�����
	*/
	static bool updateTable(DB_TABLE_OP optype, DBInterface* dbi, DB_W_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox)
	{
		bool ret = true;

		if(!opTableItemDataBox.isEmpty)
		{
			SqlCommandHelper sql(dbi, optype, opTableItemDataBox.tableName, opTableItemDataBox.parentTableDBID, 
				opTableItemDataBox.dbid, opTableItemDataBox.items);

			ret = sql->query();
			opTableItemDataBox.dbid = sql->dbid();
		}

		if(optype == TABLE_OP_INSERT)
		{
			// ��ʼ�������е��ӱ�
			DB_W_OP_TABLE_DATAS::iterator iter1 = opTableItemDataBox.optable.begin();
			for(; iter1 != opTableItemDataBox.optable.end(); iter1++)
			{
				DB_W_OP_TABLE_ITEM_DATA_BOX& wbox = *iter1->second.get();
				
				// �󶨱��ϵ
				wbox.parentTableDBID = opTableItemDataBox.dbid;

				// �����ӱ�
				updateTable(optype, dbi, wbox);
			}
		}
		else
		{
			std::tr1::unordered_map< std::string, std::vector<DBID> > childTableDBIDs;

			if(opTableItemDataBox.dbid > 0)
			{
				DB_W_OP_TABLE_DATAS::iterator iter1 = opTableItemDataBox.optable.begin();
				for(; iter1 != opTableItemDataBox.optable.end(); iter1++)
				{
					DB_W_OP_TABLE_ITEM_DATA_BOX& wbox = *iter1->second.get();

					std::tr1::unordered_map<std::string, std::vector<DBID> >::iterator iter = 
						childTableDBIDs.find(opTableItemDataBox.tableName);

					if(iter == childTableDBIDs.end())
					{
						std::vector<DBID> v;
						childTableDBIDs.insert(std::make_pair<std::string, std::vector<DBID>>(wbox.tableName, v));
					}
				}

				std::tr1::unordered_map< std::string, std::vector<DBID> >::iterator tabiter = childTableDBIDs.begin();
				for(; tabiter != childTableDBIDs.end(); tabiter++)
				{
					// ����и�ID���ȵõ����������ݿ���ͬ��id�������ж�����Ŀ�� ��ȡ��ÿ�����ݵ�id
					// Ȼ���ڴ��е�����˳����������ݿ⣬ ������ݿ����д��ڵ���Ŀ��˳�򸲸Ǹ������е���Ŀ�� �����������
					// �������ݿ������е���Ŀ�����ʣ������ݣ� ��������������ݿ��е���Ŀ��ɾ�����ݿ��е���Ŀ
					// select id from tbl_SpawnPoint_xxx_values where parentID = 7;

					std::string sqlstr;

					sqlstr = "select id from "ENTITY_TABLE_PERFIX"_";
					sqlstr += tabiter->first;
					sqlstr += " where "TABLE_PARENT_ID"=";

					char strdbid[MAX_BUF];
					kbe_snprintf(strdbid, MAX_BUF, "%"PRDBID, opTableItemDataBox.dbid);
					sqlstr += strdbid;
					sqlstr += ";";

					if(dbi->query(sqlstr.c_str(), sqlstr.size()))
					{
						MYSQL_RES * pResult = mysql_store_result(static_cast<DBInterfaceMysql*>(dbi)->mysql());
						if(pResult)
						{
							uint32 nrows = (uint32)mysql_num_rows(pResult);
							uint32 nfields = (uint32)mysql_num_fields(pResult);

							MYSQL_ROW arow;
							while((arow = mysql_fetch_row(pResult)) != NULL)
							{
								std::stringstream sval;
								sval << arow[0];
								DBID old_dbid;
								
								sval >> old_dbid;
								tabiter->second.push_back(old_dbid);
							}

							mysql_free_result(pResult);
						}
					}
				}
			}

			// �����Ҫ��մ˱� ��ѭ��N���Ѿ��ҵ���dbid�� ʹ���ӱ��е��ӱ�Ҳ����Чɾ��
			if(!opTableItemDataBox.isEmpty)
			{
				// ��ʼ�������е��ӱ�
				DB_W_OP_TABLE_DATAS::iterator iter1 = opTableItemDataBox.optable.begin();
				for(; iter1 != opTableItemDataBox.optable.end(); iter1++)
				{
					DB_W_OP_TABLE_ITEM_DATA_BOX& wbox = *iter1->second.get();
					
					if(wbox.isEmpty)
						continue;

					// �󶨱��ϵ
					wbox.parentTableDBID = opTableItemDataBox.dbid;

					std::tr1::unordered_map<std::string, std::vector<DBID> >::iterator iter = 
						childTableDBIDs.find(wbox.tableName);
					
					if(iter != childTableDBIDs.end())
					{
						if(iter->second.size() > 0)
						{
							wbox.dbid = iter->second.front();
							iter->second.erase(iter->second.begin());
						}

						if(iter->second.size() <= 0)
						{
							childTableDBIDs.erase(wbox.tableName);
						}
					}

					// �����ӱ�
					updateTable(optype, dbi, wbox);
				}
			}

			std::tr1::unordered_map< std::string, std::vector<DBID> >::iterator tabiter = childTableDBIDs.begin();
			for(; tabiter != childTableDBIDs.end(); tabiter++)
			{
				DB_W_OP_TABLE_DATAS::iterator iter1 = opTableItemDataBox.optable.begin();
				for(; iter1 != opTableItemDataBox.optable.end(); iter1++)
				{
					DB_W_OP_TABLE_ITEM_DATA_BOX& wbox = *iter1->second.get();
					if(wbox.tableName == tabiter->first)
					{
						std::vector<DBID>::iterator iter = tabiter->second.begin();
						for(; iter != tabiter->second.end(); iter++)
						{
							DBID dbid = (*iter);
							std::string sqlstr;

							sqlstr = "delete from "ENTITY_TABLE_PERFIX"_";
							sqlstr += tabiter->first;
							sqlstr += " where id=";

							char strdbid[MAX_BUF];
							kbe_snprintf(strdbid, MAX_BUF, "%"PRDBID, dbid);
							sqlstr += strdbid;
							sqlstr += ";";

							bool ret = dbi->query(sqlstr.c_str(), sqlstr.size());
							KBE_ASSERT(ret);
							
							wbox.parentTableDBID = opTableItemDataBox.dbid;
							wbox.dbid = dbid;
							wbox.isEmpty = true;

							// �����ӱ�
							updateTable(optype, dbi, wbox);
						}
					}
				}
			}
		}
		return ret;
	}

protected:
	std::tr1::shared_ptr<SqlCommandCreateFromDatasBase> pSqlcmd_;
};

}
#endif
