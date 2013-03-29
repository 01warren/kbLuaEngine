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

#ifndef __KBE_ENTITY_REF_HPP__
#define __KBE_ENTITY_REF_HPP__

#include "helper/debug_helper.hpp"
#include "cstdkbe/cstdkbe.hpp"	

namespace KBEngine{

class Entity;

#define ENTITYREF_FLAG_UNKONWN							0x00000000
#define ENTITYREF_FLAG_ENTER_CLIENT_PENDING				0x00000001	// ����ͻ����б�־
#define ENTITYREF_FLAG_LEAVE_CLIENT_PENDING				0x00000002	// �뿪�ͻ����б�־

class EntityRef
{
public:
	EntityRef(Entity* pEntity);
	~EntityRef();
	
	void flags(uint32 v){ flags_ = v; }
	void removeflags(uint32 v){ flags_ &= ~v; }
	uint32 flags(){ return flags_; }
	
	Entity* pEntity()const { return pEntity_; }
private:
	Entity* pEntity_;
	uint32 flags_;
};

class Entity;
class findif_vector_entityref_exist_handler
{
public:
	findif_vector_entityref_exist_handler(Entity* obj)
	: obj_(obj) {}

	bool operator()(const EntityRef* obj)
	{
		return obj->pEntity() == obj_;
	}
private:
	Entity* obj_;
};

}

#endif
