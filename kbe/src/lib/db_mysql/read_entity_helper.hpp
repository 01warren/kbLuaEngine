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

#ifndef KBE_READ_ENTITY_HELPER_HPP
#define KBE_READ_ENTITY_HELPER_HPP

// common include	
// #define NDEBUG
#include <sstream>
#include "common.hpp"
#include "sqlstatement.hpp"
#include "entity_sqlstatement_mapping.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/memorystream.hpp"
#include "helper/debug_helper.hpp"
#include "dbmgr_lib/db_interface.hpp"
#include "dbmgr_lib/entity_table.hpp"
#include "db_interface_mysql.hpp"

namespace KBEngine{ 

class ReadEntityHelper
{
public:
	ReadEntityHelper()
	{
	}

	virtual ~ReadEntityHelper()
	{
	}

	/**
		�ӱ��в�ѯ����
	*/
	static bool queryDB(DBInterface* dbi, DBContext& context)
	{
		SqlStatement* pSqlcmd = new SqlStatementQuery(dbi, context.tableName, 
			context.parentTableDBID, 
			context.dbid, context.items);

		bool ret = pSqlcmd->query();
		context.dbid = pSqlcmd->dbid();
		delete pSqlcmd;
		
		if(!ret)
			return ret;

		MYSQL_RES * pResult = mysql_store_result(static_cast<DBInterfaceMysql*>(dbi)->mysql());

		if(pResult)
		{
			MYSQL_ROW arow;

			while((arow = mysql_fetch_row(pResult)) != NULL)
			{
				uint32 nfields = (uint32)mysql_num_fields(pResult);
				if(nfields <= 0)
					continue;

				unsigned long *lengths = mysql_fetch_lengths(pResult);

				std::stringstream sval;
				sval << arow[0];
				DBID item_dbid;
				
				sval >> item_dbid;
				context.dbids[context.parentTableDBID > 0 ? 
							context.parentTableDBID : context.dbid].push_back(item_dbid);

				if(nfields > 1)
				{
					KBE_ASSERT(nfields == context.items.size() + 1);
					for (uint32 i = 1; i < nfields; i++)
					{
						KBEShared_ptr<DBContext::DB_ITEM_DATA> pSotvs = context.items[i - 1];
						std::string data;
						data.assign(arow[i], lengths[i]);

						context.results.push_back(data);
					}
				}
			}

			mysql_free_result(pResult);
		}
		
		std::vector<DBID>& dbids = context.dbids[context.parentTableDBID > 0 ? 
							context.parentTableDBID : context.dbid];

		if(dbids.size() == 0)
			return true;

		std::vector<DBID>::iterator dbidIter = dbids.begin();
		for(; dbidIter != dbids.end(); dbidIter++)
		{
			DBContext::DB_RW_CONTEXTS::iterator iter1 = context.optable.begin();
			for(; iter1 != context.optable.end(); iter1++)
			{
				DBContext& wbox = *iter1->second.get();
				
				// �󶨱��ϵ
				wbox.parentTableDBID = (*dbidIter);

				if(!queryDB(dbi, wbox))
					return false;
			}
		}

		return ret;
	}

protected:
};

}
#endif // KBE_READ_ENTITY_HELPER_HPP
