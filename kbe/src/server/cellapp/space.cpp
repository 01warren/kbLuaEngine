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

#include "cellapp.hpp"
#include "space.hpp"	
#include "entity.hpp"
#include "witness.hpp"	
#include "entitydef/entities.hpp"
#include "client_lib/client_interface.hpp"

#include "../../server/baseappmgr/baseappmgr_interface.hpp"
#include "../../server/cellappmgr/cellappmgr_interface.hpp"
#include "../../server/baseapp/baseapp_interface.hpp"
#include "../../server/cellapp/cellapp_interface.hpp"
#include "../../server/dbmgr/dbmgr_interface.hpp"

namespace KBEngine{	

//-------------------------------------------------------------------------------------
Space::Space(SPACE_ID spaceID):
id_(spaceID),
entities_(),
isLoadGeometry_(false),
loadGeometryPathName_(),
pCell_(NULL),
rangeList_()
{
}

//-------------------------------------------------------------------------------------
Space::~Space()
{
	SAFE_RELEASE(pCell_);
}

//-------------------------------------------------------------------------------------
void Space::loadSpaceGeometry(const char* path)
{
	loadGeometryPathName_ = path;
}

//-------------------------------------------------------------------------------------
void Space::update()
{
	if(!isLoadGeometry_)
		return;
}

//-------------------------------------------------------------------------------------
void Space::addEntityAndEnterWorld(Entity* pEntity, bool isRestore)
{
	addEntity(pEntity);
	addEntityToNode(pEntity);
	onEnterWorld(pEntity);
}

//-------------------------------------------------------------------------------------
void Space::addEntity(Entity* pEntity)
{
	pEntity->setSpaceID(this->id_);
	pEntity->spaceEntityIdx(entities_.size());
	entities_.push_back(pEntity);
}

//-------------------------------------------------------------------------------------
void Space::addEntityToNode(Entity* pEntity)
{
	pEntity->installRangeNodes(&rangeList_);
}

//-------------------------------------------------------------------------------------
void Space::removeEntity(Entity* pEntity)
{
	pEntity->setSpaceID(0);
	
	// �Ȼ�ȡ������λ��
	SPACE_ENTITIES::size_type idx = pEntity->spaceEntityIdx();

	KBE_ASSERT(idx < entities_.size());
	KBE_ASSERT(entities_[ idx ] == pEntity);

	// �����2�������ϵ�entity�����һ��entity����ɾ�������entity����λ��
	Entity* pBack = entities_.back().get();
	pBack->spaceEntityIdx(idx);
	entities_[idx] = pBack;
	pEntity->spaceEntityIdx(SPACE_ENTITIES::size_type(-1));
	entities_.pop_back();

	onLeaveWorld(pEntity);

	// ��������onLeaveWorld֮�� ��Ϊ����rangeTrigger��Ҫ�ο�pEntityRangeNode
	pEntity->uninstallRangeNodes(&rangeList_);

	// ���û��entity������Ҫ����space, ��Ϊspace���ٴ���һ��entity
	// ���entityͨ����spaceEntity
	if(entities_.empty())
	{
		Spaces::destroySpace(this->getID(), this->creatorID());
	}
}

//-------------------------------------------------------------------------------------
void Space::_onEnterWorld(Entity* pEntity)
{
	if(pEntity->hasWitness())
	{
		pEntity->pWitness()->onEnterSpace(this);
	}
}

//-------------------------------------------------------------------------------------
void Space::onEnterWorld(Entity* pEntity)
{
	KBE_ASSERT(pEntity != NULL);
	
	// �����һ����Witness(ͨ�������)����Ҫ����ǰ�����Ѿ���������client���ֵ�entity�㲥����
	// ������һ����ͨ��entity�������磬 ��ô��Ҫ�����entity�㲥�����п���������Witness��entity��
	if(pEntity->hasWitness())
	{
		_onEnterWorld(pEntity);
	}
}

//-------------------------------------------------------------------------------------
void Space::onEntityAttachWitness(Entity* pEntity)
{
	KBE_ASSERT(pEntity != NULL && pEntity->hasWitness());
	_onEnterWorld(pEntity);
}

//-------------------------------------------------------------------------------------
void Space::onLeaveWorld(Entity* pEntity)
{
	if(!pEntity->getScriptModule()->hasClient())
		return;
	
	// �������˿ͻ��˹㲥�Լ����뿪
	// ��ͻ��˷���onLeaveWorld��Ϣ
	if(pEntity->hasWitness())
	{
		pEntity->pWitness()->onLeaveSpace(this);
	}
}

//-------------------------------------------------------------------------------------
bool Space::destroy(ENTITY_ID entityID)
{
	if(this->entities().size() == 0)
		return true;

	SPACE_ENTITIES entitieslog;
	
	Entity* creator = NULL;

	SPACE_ENTITIES::const_iterator iter = this->entities().begin();
	for(; iter != this->entities().end(); iter++)
	{
		const Entity* entity = (*iter).get();

		if(entity->getID() == this->creatorID())
			creator = const_cast<Entity*>(entity);
		else
			entitieslog.push_back((*iter));
	}

	iter = entitieslog.begin();
	for(; iter != entitieslog.end(); iter++)
	{
		Entity* entity = const_cast<Entity*>((*iter).get());
		entity->destroyEntity();
	}

	// ������ٴ�����
	if(creator)
		creator->destroyEntity();

	return true;
}

//-------------------------------------------------------------------------------------
}
