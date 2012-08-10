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


#include "bots.hpp"
#include "entity.hpp"
#include "entitydef/entity_mailbox.hpp"
#include "network/channel.hpp"	

#include "../../../server/baseapp/baseapp_interface.hpp"
#include "../../../server/cellapp/cellapp_interface.hpp"

#ifdef CODE_INLINE
//#include "entity.ipp"
#endif

namespace KBEngine{

//-------------------------------------------------------------------------------------
ENTITY_METHOD_DECLARE_BEGIN(Entity)
ENTITY_METHOD_DECLARE_END()

SCRIPT_MEMBER_DECLARE_BEGIN(Entity)
SCRIPT_MEMBER_DECLARE_END()

ENTITY_GETSET_DECLARE_BEGIN(Entity)
SCRIPT_GET_DECLARE("base",							pyGetBaseMailbox,				0,					0)
SCRIPT_GET_DECLARE("cell",							pyGetCellMailbox,				0,					0)
SCRIPT_GETSET_DECLARE("position",					pyGetPosition,					pySetPosition,		0,		0)
SCRIPT_GETSET_DECLARE("direction",					pyGetDirection,					pySetDirection,		0,		0)
ENTITY_GETSET_DECLARE_END()
BASE_SCRIPT_INIT(Entity, 0, 0, 0, 0, 0)	
	
//-------------------------------------------------------------------------------------
Entity::Entity(ENTITY_ID id, ScriptModule* scriptModule):
ScriptObject(getScriptType(), true),
ENTITY_CONSTRUCTION(Entity),
cellMailbox_(NULL),
baseMailbox_(NULL),
pChannel_(NULL)
{
	ENTITY_INIT_PROPERTYS(Entity);
}

//-------------------------------------------------------------------------------------
Entity::~Entity()
{
	ENTITY_DECONSTRUCTION(Entity);
}	

//-------------------------------------------------------------------------------------
PyObject* Entity::pyGetBaseMailbox()
{ 
	EntityMailbox* mailbox = getBaseMailbox();
	if(mailbox == NULL)
		S_Return;

	Py_INCREF(mailbox);
	return mailbox; 
}

//-------------------------------------------------------------------------------------
PyObject* Entity::pyGetCellMailbox()
{ 
	EntityMailbox* mailbox = getCellMailbox();
	if(mailbox == NULL)
		S_Return;

	Py_INCREF(mailbox);
	return mailbox; 
}

//-------------------------------------------------------------------------------------
void Entity::onDefDataChanged(const PropertyDescription* propertyDescription, PyObject* pyData)
{
	/*
	// �������һ��realEntity�����
	if(!isReal())
		return;
	
	const uint32& flags = propertyDescription->getFlags();
	ENTITY_PROPERTY_UID utype = propertyDescription->getUType();

	// ���ȴ���һ����Ҫ�㲥��ģ����
	MemoryStream* mstream = new MemoryStream();
	(*mstream) << utype;
	propertyDescription->getDataType()->addToStream(mstream, pyData);

	// �ж��Ƿ���Ҫ�㲥��������cellapp, �������һ��ǰ����entity����ӵ��ghostʵ��
	// ֻ����cell�߽�һ����Χ�ڵ�entity��ӵ��ghostʵ��
	if((flags & ENTITY_BROADCAST_CELL_FLAGS) > 0)
	{
	}
	
	// �ж���������Ƿ���Ҫ�㲥�������ͻ���
	if((flags & ENTITY_BROADCAST_OTHER_CLIENT_FLAGS) > 0)
	{
		int8 detailLevel = propertyDescription->getDetailLevel();
		for(int8 i=DETAIL_LEVEL_NEAR; i<=detailLevel; i++)
		{
			std::map<ENTITY_ID, Entity*>::iterator iter = witnessEntities_[i].begin();
			for(; iter != witnessEntities_[i].end(); iter++)
			{
				Entity* entity = iter->second;
				EntityMailbox* clientMailbox = entity->getClientMailbox();
				if(clientMailbox != NULL)
				{
					Packet* sp = clientMailbox->createMail(MAIL_TYPE_UPDATE_PROPERTY);
					(*sp) << id_;
					sp->append(mstream->contents(), mstream->size());
					clientMailbox->post(sp);
				}
			}
		}

		// ��������Ѿ����¹��� ����Щ��Ϣ��ӵ������������������entity�� �����ڿ�����Զ��һ�㣬 �����������½������detaillevel
		// ʱ������½����е����Զ����µ����Ŀͻ��˿��ܲ����ʣ� ���Ǽ�¼������Եĸı䣬 �´������½�������ֻ��Ҫ�������ڼ��й��ı��
		// ���ݷ��͵����Ŀͻ��˸���
		for(int8 i=detailLevel; i<=DETAIL_LEVEL_FAR; i++)
		{
			std::map<ENTITY_ID, Entity*>::iterator iter = witnessEntities_[i].begin();
			for(; iter != witnessEntities_[i].end(); iter++)
			{
				Entity* entity = iter->second;
				EntityMailbox* clientMailbox = entity->getClientMailbox();
				if(clientMailbox != NULL)
				{
					WitnessInfo* witnessInfo = witnessEntityDetailLevelMap_.find(iter->first)->second;
					if(witnessInfo->detailLevelLog[detailLevel])
					{
						std::vector<uint32>& cddlog = witnessInfo->changeDefDataLogs[detailLevel];
						std::vector<uint32>::iterator fiter = std::find(cddlog.begin(), cddlog.end(), utype);
						if(fiter == cddlog.end())
							witnessInfo->changeDefDataLogs[detailLevel].push_back(utype);
					}
				}
			}
		}
	}

	// �ж���������Ƿ���Ҫ�㲥���Լ��Ŀͻ���
	if((flags & ENTITY_BROADCAST_OWN_CLIENT_FLAGS) > 0 && clientMailbox_ != NULL)
	{
		SocketPacket* sp = clientMailbox_->createMail(MAIL_TYPE_UPDATE_PROPERTY);
		(*sp) << id_;
		sp->append(bytestream->contents(), bytestream->size());
		clientMailbox_->post(sp);
	}

	SAFE_RELEASE(bytestream);
	*/
}

//-------------------------------------------------------------------------------------
void Entity::onRemoteMethodCall(Mercury::Channel* pChannel, MemoryStream& s)
{
	ENTITY_METHOD_UID utype = 0;
	s >> utype;
	
	DEBUG_MSG("Entity::onRemoteMethodCall: entityID %d, methodType %d.\n", 
				id_, utype);
	
	MethodDescription* md = scriptModule_->findCellMethodDescription(utype);
	
	if(md == NULL)
	{
		ERROR_MSG("Entity::onRemoteMethodCall: can't found method. utype=%u, callerID:%d.\n", utype, id_);
		return;
	}

	md->currCallerID(this->getID());

	PyObject* pyFunc = PyObject_GetAttrString(this, const_cast<char*>
						(md->getName().c_str()));

	if(md != NULL)
		md->call(pyFunc, md->createFromStream(&s));
	
	Py_XDECREF(pyFunc);
}

//-------------------------------------------------------------------------------------
int Entity::pySetPosition(PyObject *value)
{
	if(!script::ScriptVector3::check(value))
		return -1;

	script::ScriptVector3::convertPyObjectToVector3(getPosition(), value);
	return 0;
}

//-------------------------------------------------------------------------------------
PyObject* Entity::pyGetPosition()
{
	return new script::ScriptVector3(&getPosition());
}

//-------------------------------------------------------------------------------------
int Entity::pySetDirection(PyObject *value)
{
	if(PySequence_Check(value) <= 0)
	{
		PyErr_Format(PyExc_TypeError, "args of direction is must a sequence.");
		PyErr_PrintEx(0);
		return -1;
	}

	Py_ssize_t size = PySequence_Size(value);
	if(size != 3)
	{
		PyErr_Format(PyExc_TypeError, "len(direction) != 3. can't set.");
		PyErr_PrintEx(0);
		return -1;
	}

	Direction3D& dir = getDirection();
	dir.roll	= float(PyFloat_AsDouble(PySequence_GetItem(value, 0)));
	dir.pitch	= float(PyFloat_AsDouble(PySequence_GetItem(value, 1)));
	dir.yaw		= float(PyFloat_AsDouble(PySequence_GetItem(value, 2)));
	return 0;
}

//-------------------------------------------------------------------------------------
PyObject* Entity::pyGetDirection()
{
	return new script::ScriptVector3(getDirection().asVector3());
}

//-------------------------------------------------------------------------------------
void Entity::setPosition(Position3D& pos)
{ 
	position_ = pos; 
//	if(currChunk_ != NULL)
//		currChunk_->getSpace()->onEntityPositionChanged(this, currChunk_, position_);
}

//-------------------------------------------------------------------------------------
}
