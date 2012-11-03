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

#ifndef __WRITE_ENTITY_HELPER_H__
#define __WRITE_ENTITY_HELPER_H__

// common include	
// #define NDEBUG
#include <sstream>
#include "common.hpp"
#include "sqlstatement.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/memorystream.hpp"
#include "helper/debug_helper.hpp"
#include "dbmgr_lib/db_interface.hpp"
#include "dbmgr_lib/entity_table.hpp"
#include "db_interface_mysql.hpp"

namespace KBEngine{ 

class WriteEntityHelper
{
public:
	WriteEntityHelper()
	{
	}

	virtual ~WriteEntityHelper()
	{
	}

	static SqlStatement* createSql(DBInterface* dbi, DB_TABLE_OP opType, 
		std::string tableName, DBID parentDBID, 
		DBID dbid, DB_W_OP_TABLE_ITEM_DATAS& tableVal)
	{
		switch(opType)
		{
		case TABLE_OP_UPDATE:
			if(dbid > 0)
				return new SqlStatementUpdate(dbi, tableName, parentDBID, dbid, tableVal);
			else
				return new SqlStatementInsert(dbi, tableName, parentDBID, dbid, tableVal);
			break;
		case TABLE_OP_INSERT:
			return new SqlStatementInsert(dbi, tableName, parentDBID, dbid, tableVal);
			break;
		case TABLE_OP_DELETE:
			break;
		default:
			KBE_ASSERT(false && "no support!\n");
		};

		return NULL;
	}

	/**
		�����ݸ��µ�����
	*/
	static bool writeDB(DB_TABLE_OP optype, DBInterface* dbi, DB_W_OP_TABLE_ITEM_DATA_BOX& opTableItemDataBox)
	{
		bool ret = true;

		if(!opTableItemDataBox.isEmpty)
		{
			SqlStatement* pSqlcmd = createSql(dbi, optype, opTableItemDataBox.tableName, 
				opTableItemDataBox.parentTableDBID, 
				opTableItemDataBox.dbid, opTableItemDataBox.items);

			ret = pSqlcmd->query();
			opTableItemDataBox.dbid = pSqlcmd->dbid();
			delete pSqlcmd;
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
				writeDB(optype, dbi, wbox);
			}
		}
		else
		{
			// ����и�ID���ȵõ����������ݿ���ͬ��id�������ж�����Ŀ�� ��ȡ��ÿ�����ݵ�id
			// Ȼ���ڴ��е�����˳����������ݿ⣬ ������ݿ����д��ڵ���Ŀ��˳�򸲸Ǹ������е���Ŀ�� �����������
			// �������ݿ������е���Ŀ�����ʣ������ݣ� ��������������ݿ��е���Ŀ��ɾ�����ݿ��е���Ŀ
			// select id from tbl_SpawnPoint_xxx_values where parentID = 7;
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
						childTableDBIDs.insert(std::make_pair< std::string, std::vector<DBID> >(wbox.tableName, v));
					}
				}

				std::tr1::unordered_map< std::string, std::vector<DBID> >::iterator tabiter = childTableDBIDs.begin();
				for(; tabiter != childTableDBIDs.end(); tabiter++)
				{
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
					writeDB(optype, dbi, wbox);
				}
			}
			
			// ɾ��������������
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
							writeDB(optype, dbi, wbox);
						}
					}
				}
			}
		}
		return ret;
	}

protected:
};

}
#endif
