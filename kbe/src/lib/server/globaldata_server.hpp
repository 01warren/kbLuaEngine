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
#ifndef __GLOBAL_DATA_SERVER_H__
#define __GLOBAL_DATA_SERVER_H__

#include "cstdkbe/cstdkbe.hpp"
#include "helper/debug_helper.hpp"
	
namespace KBEngine{
namespace Mercury
{
	class Channel;
}

class GlobalDataServer
{	
public:	
	GlobalDataServer();
	~GlobalDataServer();
			
	/** д���� */
	bool write(Mercury::Channel* handler, const std::string& key, const std::string& value);
	
	/** ɾ������ */
	bool del(Mercury::Channel* handler, const std::string& key);	
	
	/** ��Ӹ÷���������Ҫ���ĵ������� */
	void addConcernComponentType(COMPONENT_TYPE ct){ concernComponentTypes_.push_back(ct); }
	
	/** �㲥һ�����ݵĸı�������ĵ���� */
	void broadcastDataChange(Mercury::Channel* handler, const std::string& key, 
							const std::string& value, bool isDelete = false);
	
	/** һ���µĿͻ��˵�½ */
	void onGlobalDataClientLogon(Mercury::Channel* client);
private:
	std::vector<COMPONENT_TYPE> concernComponentTypes_;						// ��GlobalDataServer����Ҫ���ĵ�������
	typedef std::map<std::string, std::string> DATA_MAP;
	typedef DATA_MAP::iterator DATA_MAP_KEY;
	DATA_MAP dict_;
} ;

}
#endif