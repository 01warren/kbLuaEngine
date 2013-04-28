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

#ifndef __ENTITY_H__
#define __ENTITY_H__
	
// common include
//#include "entitymovecontroller.hpp"
#include "profile.hpp"
#include "cstdkbe/timer.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/smartpointer.hpp"
#include "helper/debug_helper.hpp"
#include "entitydef/entity_mailbox.hpp"
#include "pyscript/math.hpp"
#include "pyscript/scriptobject.hpp"
#include "entitydef/datatypes.hpp"	
#include "entitydef/entitydef.hpp"	
#include "entitydef/scriptdef_module.hpp"
#include "entitydef/entity_macro.hpp"	
#include "server/script_timers.hpp"	

//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

class Chunk;
class Entity;
class EntityMailbox;
class Cellapp;
class Witness;
class AllClients;
class EntityRangeNode;
class Controller;
class Controllers;

namespace Mercury
{
class Channel;
class Bundle;
}

typedef SmartPointer<Entity> EntityPtr;
typedef std::vector<EntityPtr> SPACE_ENTITIES;

class Entity : public script::ScriptObject
{
	/** ���໯ ��һЩpy�������������� */
	BASE_SCRIPT_HREADER(Entity, ScriptObject)	
	ENTITY_HEADER(Entity)
public:
	Entity(ENTITY_ID id, const ScriptDefModule* scriptModule);
	~Entity();
	
	/** 
		�������entity 
	*/
	void onDestroy(void);
	
	/**
		���ٳ���
	*/
	DECLARE_PY_MOTHOD_ARG0(pyDestroySpace);
	void destroySpace(void);

	/** 
		�ж������Ƿ���һ��realEntity 
	*/
	INLINE bool isReal(void)const;

	/** 
		�����������ݱ��ı��� 
	*/
	void onDefDataChanged(const PropertyDescription* propertyDescription, 
			PyObject* pyData);
	
	/** 
		��entityͨ��ͨ��
	*/
	INLINE void pChannel(Mercury::Channel* pchannel);
	INLINE Mercury::Channel* pChannel(void)const ;
public:
	/** 
		mailbox section
	*/
	INLINE EntityMailbox* getBaseMailbox()const;
	DECLARE_PY_GET_MOTHOD(pyGetBaseMailbox);
	INLINE void setBaseMailbox(EntityMailbox* mailbox);
	
	INLINE EntityMailbox* getClientMailbox()const;
	DECLARE_PY_GET_MOTHOD(pyGetClientMailbox);
	INLINE void setClientMailbox(EntityMailbox* mailbox);

	/**
		all_clients
	*/
	INLINE AllClients* getAllClients()const;
	DECLARE_PY_GET_MOTHOD(pyGetAllClients);
	INLINE void setAllClients(AllClients* clients);

	/**
		other_clients
	*/
	INLINE AllClients* getOtherClients()const;
	DECLARE_PY_GET_MOTHOD(pyGetOtherClients);
	INLINE void setOtherClients(AllClients* clients);

	/** 
		�ű���ȡ������entity��position 
	*/
	INLINE Position3D& getPosition();
	INLINE void setPosition(const Position3D& pos);
	DECLARE_PY_GETSET_MOTHOD(pyGetPosition, pySetPosition);

	/** 
		�ű���ȡ������entity�ķ��� 
	*/
	INLINE Direction3D& getDirection();
	INLINE void setDirection(const Direction3D& dir);
	DECLARE_PY_GETSET_MOTHOD(pyGetDirection, pySetDirection);
	
	/**
		�Ƿ��ڵ�����
	*/
	INLINE void isOnGround(bool v);
	INLINE bool isOnGround()const;

	/** 
		����entity�����λ�� 
	*/
	void setPositionAndDirection(const Position3D& position, 
		const Direction3D& direction);
	
	void onPositionChanged();
	void onDirectionChanged();
	
	bool checkMoveForTopSpeed(const Position3D& position);

	/** ����ӿ�
		�ͻ���������λ��
	*/
	void setPosition_XZ_int(Mercury::Channel* pChannel, int32 x, int32 z);

	/** ����ӿ�
		�ͻ���������λ��
	*/
	void setPosition_XYZ_int(Mercury::Channel* pChannel, int32 x, int32 y, int32 z);

	/** ����ӿ�
		�ͻ�������λ��
	*/
	void setPosition_XZ_float(Mercury::Channel* pChannel, float x, float z);

	/** ����ӿ�
		�ͻ�������λ��
	*/
	void setPosition_XYZ_float(Mercury::Channel* pChannel, float x, float y, float z);

	/** 
		�ű�����Ϊ��ǰ����space����һ������ӳ�� 
	*/
	DECLARE_PY_MOTHOD_ARG2(pyAddSpaceGeometryMapping, SPACE_ID, const_charptr);

	/** ����ӿ�
		entity����
		@cellAppID: Ҫ���͵���Ŀ��cellappID
		@targetEntityID��Ҫ���͵����entity��space��
		@sourceBaseAppID: �п�������ĳ��baseapp�ϵ�base����teleport�ģ� ���Ϊ0��ΪcellEntity����
	*/
	void teleportFromBaseapp(Mercury::Channel* pChannel, COMPONENT_ID cellAppID, ENTITY_ID targetEntityID, COMPONENT_ID sourceBaseAppID);

	/**
		cell�ϵĴ��ͷ���
	*/
	DECLARE_PY_MOTHOD_ARG3(pyTeleport, PyObject_ptr, PyObject_ptr, PyObject_ptr);
	void teleport(PyObject_ptr nearbyMBRef, Position3D& pos, Direction3D& dir);

	/**
		���ͳɹ���ʧ����ػص�
	*/
	void onTeleport();
	void onTeleportFailure();
	void onTeleportSuccess(PyObject* nearbyEntity, SPACE_ID lastSpaceID);

	/**
		�����뿪cell�Ȼص�
	*/
	void onEnteredCell();
	void onEnteringCell();
	void onLeavingCell();
	void onLeftCell();
	
	/** 
		��cellapp������ֹ�� baseapp������ҵ����ʵ�cellapp����ָ���
		����ô˷���
	*/
	void onRestore();

	/**
		�ű�����aoi
	*/
	void debugAOI();
	DECLARE_PY_MOTHOD_ARG0(pyDebugAOI);

	/** 
		��ǰentity���������Aoi�뾶��Χ 
	*/
	int32 setAoiRadius(float radius, float hyst);
	float getAoiRadius(void)const;
	float getAoiHystArea(void)const;
	DECLARE_PY_MOTHOD_ARG2(pySetAoiRadius, float, float);
	
	/** 
		��ǰentity�Ƿ�Ϊreal 
	*/
	DECLARE_PY_MOTHOD_ARG0(pyIsReal);
	
	/** 
		��baseapp���ͱ�������
	*/
	void backupCellData();

	/** 
		��Ҫ���浽���ݿ�֮ǰ��֪ͨ 
	*/
	void onWriteToDB();

	/** 
		entity�ƶ����� 
	*/
	bool navigateStep(const Position3D& destination, float velocity, 
					float maxMoveDistance, float maxDistance, 
					bool faceMovement, float girth, PyObject* userData);

	DECLARE_PY_MOTHOD_ARG7(pyNavigateStep, PyObject_ptr, float, float, float, int8, float, PyObject_ptr);

	/** 
		entity�ƶ���ĳ���� 
	*/
	uint32 moveToPoint(const Position3D& destination, float velocity, 
			PyObject* userData, bool faceMovement, bool moveVertically);
	
	DECLARE_PY_MOTHOD_ARG5(pyMoveToPoint, PyObject_ptr, float, PyObject_ptr, int32, int32);

	/** 
		entity�ƶ���ĳ��entity 
	*/
	uint32 moveToEntity(ENTITY_ID targetID, float velocity, float range,
			PyObject* userData, bool faceMovement, bool moveVertically);
	
	DECLARE_PY_MOTHOD_ARG6(pyMoveToEntity, int32, float, float, PyObject_ptr, int32, int32);

	/** 
		�ű���ȡ������entity�����xz�ƶ��ٶ� 
	*/
	float getTopSpeed()const{ return topSpeed_; }
	INLINE void setTopSpeed(float speed);
	DECLARE_PY_GETSET_MOTHOD(pyGetTopSpeed, pySetTopSpeed);
	
	/** 
		�ű���ȡ������entity�����y�ƶ��ٶ� 
	*/
	INLINE float getTopSpeedY()const;
	INLINE void setTopSpeedY(float speed);
	DECLARE_PY_GETSET_MOTHOD(pyGetTopSpeedY, pySetTopSpeedY);
	
	/** 
		�ű�������һ����Χ�ڵ�ĳ�����͵�entities 
	*/
	static PyObject* __py_pyEntitiesInRange(PyObject* self, PyObject* args);

	/** 
		�ű�������AOI��Χ�ڵ�entities 
	*/
	DECLARE_PY_MOTHOD_ARG0(pyEntitiesInAOI);
public:
	/** ����ӿ�
		Զ�̺��б�entity�ķ��� 
	*/
	void onRemoteMethodCall(Mercury::Channel* pChannel, MemoryStream& s);

	/**
		�۲���
	*/
	INLINE Witness* pWitness()const;
	INLINE void pWitness(Witness* w);

	/** 
		�Ƿ��κ�proxy���ӵ�, ������entityû�пͻ��ˣ� �����ֵ��Ч 
	*/
	INLINE bool isWitnessed(void)const;
	DECLARE_PY_GET_MOTHOD(pyIsWitnessed);

	/** 
		entity�Ƿ���һ���۲��� 
	*/
	INLINE bool hasWitness(void)const;
	DECLARE_PY_GET_MOTHOD(pyHasWitness);

	/** 
		����һ���۲��߹۲쵽�� 
	*/
	void addWitnessed(Entity* entity);

	/** 
		�Ƴ�һ���۲�����Ĺ۲��� 
	*/
	void delWitnessed(Entity* entity);

	/** ����ӿ�
		entity����һ���۲���(�ͻ���)

	*/
	void onGetWitness(Mercury::Channel* pChannel);

	/** ����ӿ�
		entity��ʧ��һ���۲���(�ͻ���)

	*/
	void onLoseWitness(Mercury::Channel* pChannel);

	/** ����ӿ�
		entity��ʧ��һ���۲���(�ͻ���)

	*/
	void onResetWitness(Mercury::Channel* pChannel);

	/** 
		client��������
	*/
	void onUpdateDataFromClient(KBEngine::MemoryStream& s);

	/** 
		���һ������ 
	*/
	uint32 addProximity(float range_xz, float range_y, int32 userarg);
	DECLARE_PY_MOTHOD_ARG3(pyAddProximity, float, float, int32);

	/** 
		ɾ��һ������ 
	*/
	void cancelController(uint32 id);
	static PyObject* __py_pyCancelController(PyObject* self, PyObject* args);

	/** 
		һ��entity���������entity��ĳ������ 
	*/
	void onEnterTrap(Entity* entity, float range_xz, float range_y, 
							uint32 controllerID, int32 userarg);

	/** 
		һ��entity�뿪�����entity��ĳ������ 
	*/
	void onLeaveTrap(Entity* entity, float range_xz, float range_y, 
							uint32 controllerID, int32 userarg);

	/** 
		��entity����һ���µ�space��ȥ���뿪���������¼�����������ӿ� 
	*/
	void onLeaveTrapID(ENTITY_ID entityID, 
							float range_xz, float range_y, 
							uint32 controllerID, int32 userarg);

	/** 
		ֹͣ�κ��ƶ���Ϊ
	*/
	bool stopMove();

	/** 
		entity��һ���ƶ���� 
	*/
	void onMove(uint32 controllerId, PyObject* userarg);

	/** 
		entity���ƶ���� 
	*/
	void onMoveOver(uint32 controllerId, PyObject* userarg);

	/** 
		entity�ƶ�ʧ��
	*/
	void onMoveFailure(uint32 controllerId, PyObject* userarg);

	/**
		��ȡ������space��entities�е�λ��
	*/
	INLINE SPACE_ENTITIES::size_type spaceEntityIdx()const;
	INLINE void spaceEntityIdx(SPACE_ENTITIES::size_type idx);

	/**
		��ȡentity���ڽڵ�
	*/
	INLINE EntityRangeNode* pEntityRangeNode()const;
private:
	/** 
		����teleport�����base��
	*/
	void _sendBaseTeleportResult(ENTITY_ID sourceEntityID, COMPONENT_ID sourceBaseAppID, SPACE_ID spaceID, SPACE_ID lastSpaceID);
protected:
	EntityMailbox*							clientMailbox_;						// ���entity�Ŀͻ���mailbox
	EntityMailbox*							baseMailbox_;						// ���entity��baseapp mailbox

	Position3D								position_;							// entity�ĵ�ǰλ��
	Direction3D								direction_;							// entity�ĵ�ǰ����

	bool									isReal_;							// �Լ��Ƿ���һ��realEntity
	bool									isOnGround_;						// �Ƿ��ڵ�����

	float									topSpeed_;							// entity x,z������ƶ��ٶ�
	float									topSpeedY_;							// entity y������ƶ��ٶ�

	SPACE_ENTITIES::size_type				spaceEntityIdx_;					// ������space��entities�е�λ��

	uint16									witnessedNum_;						// �Ƿ��κι۲��߼��ӵ�
	Witness*								pWitness_;							// �۲��߶���

	AllClients*								allClients_;
	AllClients*								otherClients_;

	EntityRangeNode*						pEntityRangeNode_;					// entity�ڵ�

	Controllers*							pControllers_;						// ������������
	Controller*								pMoveController_;
};																										
																											

}

#ifdef CODE_INLINE
#include "entity.ipp"
#endif
#endif
