#include "cellapp.hpp"
#include "space.hpp"	
#include "entity.hpp"	
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
pCell_(NULL)
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
void Space::addEntity(Entity* pEntity)
{
	pEntity->setSpaceID(this->id_);
	pEntity->spaceEntityIdx(entities_.size());
	entities_.push_back(pEntity);

	onEnterWorld(pEntity);
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

	// ���û��entity������Ҫ����space, ��Ϊspace���ٴ���һ��entity
	if(entities_.empty())
	{
	}
}

//-------------------------------------------------------------------------------------
void Space::_onEnterWorld(Entity* pEntity)
{
	if(pEntity->hasWitness())
	{
		Mercury::Bundle* pSendBundle = Mercury::Bundle::ObjPool().createObject();
		Mercury::Bundle* pForwardBundle = Mercury::Bundle::ObjPool().createObject();
		Mercury::Bundle* pForwardPosDirBundle = Mercury::Bundle::ObjPool().createObject();
		
		(*pForwardPosDirBundle).newMessage(ClientInterface::onUpdatePropertys);
		MemoryStream* s1 = MemoryStream::ObjPool().createObject();
		(*pForwardPosDirBundle) << pEntity->getID();
		pEntity->addPositionAndDirectionToStream(*s1);
		(*pForwardPosDirBundle).append(*s1);
		MemoryStream::ObjPool().reclaimObject(s1);
		MERCURY_ENTITY_MESSAGE_FORWARD_CLIENT(pEntity->getID(), (*pSendBundle), (*pForwardPosDirBundle));

		(*pForwardBundle).newMessage(ClientInterface::onEntityEnterWorld);
		(*pForwardBundle) << pEntity->getID();
		(*pForwardBundle) << this->getID();

		MERCURY_ENTITY_MESSAGE_FORWARD_CLIENT(pEntity->getID(), (*pSendBundle), (*pForwardBundle));
		pEntity->getClientMailbox()->postMail(*pSendBundle);

		Mercury::Bundle::ObjPool().reclaimObject(pSendBundle);
		Mercury::Bundle::ObjPool().reclaimObject(pForwardBundle);
		Mercury::Bundle::ObjPool().reclaimObject(pForwardPosDirBundle);
	}
}

//-------------------------------------------------------------------------------------
void Space::onEnterWorld(Entity* pEntity)
{
	KBE_ASSERT(pEntity != NULL);
	
	SPACE_ENTITIES viewEntitys;
	SPACE_ENTITIES aoiEntitys;
	getAOIEntities(pEntity, aoiEntitys);
	
	// �����һ����Witness(ͨ�������)����Ҫ����ǰ�����Ѿ���������client���ֵ�entity�㲥����
	// ������һ����ͨ��entity�������磬 ��ô��Ҫ�����entity�㲥�����п���������Witness��entity��
	if(pEntity->hasWitness())
	{
		_onEnterWorld(pEntity);
		if(aoiEntitys.size() > 0)
		{
			Mercury::Bundle* pSendBundle = Mercury::Bundle::ObjPool().createObject();
			MERCURY_ENTITY_MESSAGE_FORWARD_CLIENT_START(pEntity->getID(), (*pSendBundle));

			SPACE_ENTITIES::iterator iter = aoiEntitys.begin();
			for(; iter != aoiEntitys.end(); iter++)
			{
				Entity* entity = (*iter).get();
				if(entity == pEntity || entity->isDestroyed())
					continue;

				if(entity->hasWitness())
				{
					viewEntitys.push_back(entity);
				}
				
				if(!entity->getScriptModule()->hasClient())
					continue;

				Mercury::Bundle* pForwardBundle1 = Mercury::Bundle::ObjPool().createObject();
				Mercury::Bundle* pForwardBundle2 = Mercury::Bundle::ObjPool().createObject();

				MemoryStream* s1 = MemoryStream::ObjPool().createObject();
				entity->addPositionAndDirectionToStream(*s1);
				entity->addClientDataToStream(s1);

				(*pForwardBundle1).newMessage(ClientInterface::onUpdatePropertys);
				(*pForwardBundle1) << entity->getID();
				(*pForwardBundle1).append(*s1);
				MemoryStream::ObjPool().reclaimObject(s1);
		
				(*pForwardBundle2).newMessage(ClientInterface::onEntityEnterWorld);
				(*pForwardBundle2) << entity->getID();
				(*pForwardBundle2) << this->getID();

				MERCURY_ENTITY_MESSAGE_FORWARD_CLIENT_APPEND((*pSendBundle), (*pForwardBundle1));
				MERCURY_ENTITY_MESSAGE_FORWARD_CLIENT_APPEND((*pSendBundle), (*pForwardBundle2));
				
				Mercury::Bundle::ObjPool().reclaimObject(pForwardBundle1);
				Mercury::Bundle::ObjPool().reclaimObject(pForwardBundle2);
			}

			Mercury::Bundle::ObjPool().reclaimObject(pSendBundle);
			pEntity->getClientMailbox()->postMail(*pSendBundle);
		}
	}
	else
	{
		if(aoiEntitys.size() > 0 && pEntity->getScriptModule()->hasClient())
		{
			SPACE_ENTITIES::iterator iter = aoiEntitys.begin();
			for(; iter != aoiEntitys.end(); iter++)
			{
				Entity* entity = (*iter).get();
				if(entity == pEntity)
					continue;

				if(entity->hasWitness())
				{
					viewEntitys.push_back(entity);
				}
			}
		}
	}

	// �����е���Щentity�㲥��ǰ���������entity
	broadcastEntityToAOIEntities(pEntity, viewEntitys);
}

//-------------------------------------------------------------------------------------
void Space::broadcastEntityToAOIEntities(Entity* pEntity, SPACE_ENTITIES& aoiEntitys)
{
	if(aoiEntitys.size())
	{
		MemoryStream* s1 = MemoryStream::ObjPool().createObject();
		pEntity->addPositionAndDirectionToStream(*s1);
		pEntity->addClientDataToStream(s1);

		SPACE_ENTITIES::iterator iter = aoiEntitys.begin();
		for(; iter != aoiEntitys.end(); iter++)
		{
			Entity* entity = (*iter).get();

			if(entity == pEntity)
				continue;

			if(!entity->getScriptModule()->hasClient())
				continue;

			Mercury::Bundle* pSendBundle = Mercury::Bundle::ObjPool().createObject();
			Mercury::Bundle* pForwardBundle1 = Mercury::Bundle::ObjPool().createObject();
			Mercury::Bundle* pForwardBundle2 = Mercury::Bundle::ObjPool().createObject();

			(*pForwardBundle1).newMessage(ClientInterface::onUpdatePropertys);
			(*pForwardBundle1) << pEntity->getID();
			(*pForwardBundle1).append(*s1);

			(*pForwardBundle2).newMessage(ClientInterface::onEntityEnterWorld);
			(*pForwardBundle2) << pEntity->getID();
			(*pForwardBundle2) << this->getID();

			MERCURY_ENTITY_MESSAGE_FORWARD_CLIENT(entity->getID(), (*pSendBundle), (*pForwardBundle1));
			MERCURY_ENTITY_MESSAGE_FORWARD_CLIENT(entity->getID(), (*pSendBundle), (*pForwardBundle2));
			Mercury::Bundle::ObjPool().reclaimObject(pForwardBundle1);
			Mercury::Bundle::ObjPool().reclaimObject(pForwardBundle2);

			entity->getClientMailbox()->postMail(*pSendBundle);
			Mercury::Bundle::ObjPool().reclaimObject(pSendBundle);
		}

		MemoryStream::ObjPool().reclaimObject(s1);
	}
}

//-------------------------------------------------------------------------------------
void Space::broadcastAOIEntitiesToEntity(Entity* pEntity)
{
	if(pEntity == NULL || !pEntity->hasWitness())
		return;

	SPACE_ENTITIES aoiEntitys;
	getAOIEntities(pEntity, aoiEntitys);

	if(aoiEntitys.size() > 0)
	{
		Mercury::Bundle* pSendBundle = Mercury::Bundle::ObjPool().createObject();
		MERCURY_ENTITY_MESSAGE_FORWARD_CLIENT_START(pEntity->getID(), (*pSendBundle));

		SPACE_ENTITIES::iterator iter = aoiEntitys.begin();
		for(; iter != aoiEntitys.end(); iter++)
		{
			Entity* entity = (*iter).get();
			if(!entity->getScriptModule()->hasClient() || entity == pEntity)
				continue;

			Mercury::Bundle* pForwardBundle1 = Mercury::Bundle::ObjPool().createObject();
			Mercury::Bundle* pForwardBundle2 = Mercury::Bundle::ObjPool().createObject();

			MemoryStream* s1 = MemoryStream::ObjPool().createObject();
			entity->addPositionAndDirectionToStream(*s1);
			entity->addClientDataToStream(s1);
			
			(*pForwardBundle1).newMessage(ClientInterface::onUpdatePropertys);
			(*pForwardBundle1) << entity->getID();
			(*pForwardBundle1).append(*s1);
			MemoryStream::ObjPool().reclaimObject(s1);
	
			(*pForwardBundle2).newMessage(ClientInterface::onEntityEnterWorld);
			(*pForwardBundle2) << entity->getID();
			(*pForwardBundle2) << this->getID();

			MERCURY_ENTITY_MESSAGE_FORWARD_CLIENT_APPEND((*pSendBundle), (*pForwardBundle1));
			MERCURY_ENTITY_MESSAGE_FORWARD_CLIENT_APPEND((*pSendBundle), (*pForwardBundle2));
			
			Mercury::Bundle::ObjPool().reclaimObject(pForwardBundle1);
			Mercury::Bundle::ObjPool().reclaimObject(pForwardBundle2);
		}

		Mercury::Bundle::ObjPool().reclaimObject(pSendBundle);
		pEntity->getClientMailbox()->postMail(*pSendBundle);
	}
}

//-------------------------------------------------------------------------------------
void Space::onEntityAttachWitness(Entity* pEntity)
{
	KBE_ASSERT(pEntity != NULL && pEntity->hasWitness());
	_onEnterWorld(pEntity);
	broadcastAOIEntitiesToEntity(pEntity);
}

//-------------------------------------------------------------------------------------
void Space::onLeaveWorld(Entity* pEntity)
{
	if(!pEntity->getScriptModule()->hasClient())
		return;
	
	SPACE_ENTITIES aoiEntitys;
	getAOIEntities(pEntity, aoiEntitys);

	// �������˿ͻ��˹㲥�Լ����뿪
	if(aoiEntitys.size() > 0)
	{
		SPACE_ENTITIES::const_iterator iter = aoiEntitys.begin();
		for(; iter != aoiEntitys.end(); iter++)
		{
			const Entity* entity = (*iter).get();
			if(!entity->hasWitness() || entity == pEntity)
				continue;

			Mercury::Bundle* pSendBundle = Mercury::Bundle::ObjPool().createObject();
			Mercury::Bundle* pForwardBundle = Mercury::Bundle::ObjPool().createObject();

			(*pForwardBundle).newMessage(ClientInterface::onEntityLeaveWorld);
			(*pForwardBundle) << pEntity->getID();
			(*pForwardBundle) << this->getID();

			MERCURY_ENTITY_MESSAGE_FORWARD_CLIENT(entity->getID(), (*pSendBundle), (*pForwardBundle));
			entity->getClientMailbox()->postMail(*pSendBundle);
			Mercury::Bundle::ObjPool().reclaimObject(pSendBundle);
			Mercury::Bundle::ObjPool().reclaimObject(pForwardBundle);
		}
	}

	// ��ͻ��˷���onLeaveWorld��Ϣ
	if(pEntity->hasWitness())
	{
		Mercury::Bundle* pSendBundle = Mercury::Bundle::ObjPool().createObject();
		Mercury::Bundle* pForwardBundle = Mercury::Bundle::ObjPool().createObject();

		(*pForwardBundle).newMessage(ClientInterface::onEntityLeaveWorld);
		(*pForwardBundle) << pEntity->getID();
		(*pForwardBundle) << this->getID();

		MERCURY_ENTITY_MESSAGE_FORWARD_CLIENT(pEntity->getID(), (*pSendBundle), (*pForwardBundle));
		pEntity->getClientMailbox()->postMail(*pSendBundle);
		Mercury::Bundle::ObjPool().reclaimObject(pSendBundle);
		Mercury::Bundle::ObjPool().reclaimObject(pForwardBundle);
	}
}

//-------------------------------------------------------------------------------------
void Space::getAOIEntities(Entity* pEntity, SPACE_ENTITIES& aoiEntitys)
{
	if(pEntity->getAoiRadius() < 0.5f)
		return;

	if(this->entities().size() > 0)
	{
		SPACE_ENTITIES::const_iterator iter = this->entities().begin();
		for(; iter != this->entities().end(); iter++)
		{
			const Entity* entity = (*iter).get();
			if(entity->isDestroyed())
				continue;

			aoiEntitys.push_back((*iter));
		}
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
