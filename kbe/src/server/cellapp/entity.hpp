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
#include "cstdkbe/timer.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "helper/debug_helper.hpp"
#include "entitydef/entity_mailbox.hpp"
#include "pyscript/math.hpp"
#include "pyscript/scriptobject.hpp"
#include "entitydef/datatypes.hpp"	
#include "entitydef/entitydef.hpp"	
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

namespace Mercury
{
class Channel;
}

/** �۲�����Ϣ�ṹ */
struct WitnessInfo
{
	WitnessInfo(const int8& lv, Entity* e, const float& r):
	detailLevel(lv),
	entity(e),
	range(r)
	{
		for(int i=0; i<3; i++)
			if(lv == i)
				detailLevelLog[i] = true;
			else
				detailLevelLog[i] = false;
	}
	
	int8 detailLevel;							// ��ǰ�������鼶��
	Entity* entity;								// ������entity
	float range;								// ��ǰ�����entity�ľ���
	bool detailLevelLog[3];						// ��ʾ���entity���������entity����Щ���鼶�� �ṩ���Թ㲥�Ż��õ�
												// ��û�н����ĳ����ʱ�� �Ὣ���������������Ը��¸����� ����ֻ���½���ʱ�������ı��������
	std::vector<uint32> changeDefDataLogs[3];	// entity�뿪��ĳ�����鼶��(û������witness)�� ���ڼ���ĳ�����鼶������Ըı����¼������
};

class Entity : public script::ScriptObject
{
	/** ���໯ ��һЩpy�������������� */
	BASE_SCRIPT_HREADER(Entity, ScriptObject)	
	ENTITY_HEADER(Entity)
public:
	typedef std::map<ENTITY_ID, WitnessInfo*>	WITNESSENTITY_DETAILLEVEL_MAP;
public:
	Entity(ENTITY_ID id, ScriptModule* scriptModule);
	~Entity();
	
	/** 
		�������entity 
	*/
	void onDestroy(void);
	void destroy();
	
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
		�ű���������ʵ��
	*/
	void destroyEntity(void);
	DECLARE_PY_MOTHOD_ARG0(pyDestroyEntity);

	
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
		�ű���ȡ������entity��position 
	*/
	INLINE Position3D& getPosition();
	void setPosition(Position3D& pos);
	DECLARE_PY_GETSET_MOTHOD(pyGetPosition, pySetPosition);

	/** 
		�ű���ȡ������entity�ķ��� 
	*/
	INLINE Direction3D& getDirection();
	INLINE void setDirection(Direction3D& dir);
	DECLARE_PY_GETSET_MOTHOD(pyGetDirection, pySetDirection);
	
	/** 
		����entity�����λ�� 
	*/
	void setPositionAndDirection(Position3D& position, 
		Direction3D& direction);

	/** 
		�ű�����Ϊ��ǰ����space����һ������ӳ�� 
	*/
	DECLARE_PY_MOTHOD_ARG2(pyAddSpaceGeometryMapping, SPACE_ID, const_charptr);

	/** 
		��ǰentity���������Aoi�뾶��Χ 
	*/
	int32 setAoiRadius(float radius, float hyst);
	INLINE float getAoiRadius(void)const;
	INLINE float getAoiHystArea(void)const;
	DECLARE_PY_MOTHOD_ARG2(pySetAoiRadius, float, float);
	
	/** 
		��ǰentity�Ƿ�Ϊreal 
	*/
	DECLARE_PY_MOTHOD_ARG0(pyIsReal);

	/** 
		�ű���õ�ǰentity�Ƿ�Ϊ��Ҫ���ٵ�entity 
	*/
	INLINE bool isDestroyed();
	DECLARE_PY_GET_MOTHOD(pyGetIsDestroyed);
	
	/** 
		entity�ƶ����� 
	*/
	bool navigateStep(const Position3D& destination, float velocity, 
					float maxMoveDistance, float maxDistance, 
					bool faceMovement, float girth, PyObject* userData);

	DECLARE_PY_MOTHOD_ARG7(pyNavigateStep, PyObject_ptr, float, float, float, int8, float, PyObject_ptr);
	
	/** 
		ֹͣ�κη�ʽ���ƶ���Ϊ 
	*/
	bool stopMove();
	DECLARE_PY_MOTHOD_ARG0(pyStopMove);

	/** 
		entity�ƶ���ĳ���� 
	*/
	bool moveToPoint(const Position3D& destination, float velocity, 
			PyObject* userData, bool faceMovement, bool moveVertically);
	
	DECLARE_PY_MOTHOD_ARG5(pyMoveToPoint, PyObject_ptr, float, PyObject_ptr, int32, int32);

	
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
		�ű�������һ����Χ���ĳ�����͵�entities 
	*/
	DECLARE_PY_MOTHOD_ARG3(pyEntitiesInRange, float, PyObject_ptr, PyObject_ptr);
public:
	/** 
		Զ�̺��б�entity�ķ��� 
	*/
	void onRemoteMethodCall(Mercury::Channel* pChannel, MemoryStream& s);

	/** 
		�������entity��ǰ����chunk��ID 
	*/
	//void setCurrentChunk(Chunk* chunk){ Chunk* oldchunk = currChunk_; currChunk_ = chunk; onCurrentChunkChanged(oldchunk); }

	/** 
		��ǰchunk���ı� 
	*/
	//void onCurrentChunkChanged(Chunk* oldChunk);

	/** 
		������entity��ǰ����chunk��ID 
	*/
	//Chunk* getAtChunk(void)const{ return currChunk_; }

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
	void onWitnessed(Entity* entity, float range);

	/** 
		�Ƴ�һ���۲�����Ĺ۲��� 
	*/
	void onRemoveWitness(Entity* entity);

	/** 
		���entity����˹۲������ 
	*/
	void onGetWitness(void);

	/** 
		entity��ʧ�˹۲������ 
	*/
	void onLoseWitness(void);

	/** 
		������۲쵽��entities�б� 
	*/
	INLINE std::map<ENTITY_ID, Entity*>& getViewEntities(void);

	/** 
		������й۲쵽�ҵĹ۲��� 
	*/
	WITNESSENTITY_DETAILLEVEL_MAP& getWitnessEntities(void)
	{
		return witnessEntityDetailLevelMap_; 
	}

	/** 
		����witness��״̬ 
	*/
	void onUpdateWitness(Entity* entity, float range);

	/** 
		һ���½�����Ұ��Χ��entity 
	*/
	void onViewEntity(Entity* entity);

	/** 
		һ��entity�뿪����Ұ��Χ 
	*/
	void onLoseViewEntity(Entity* entity);

	/** 
		һ��entity��һ�α�����(��һ�ν��뵽���������)detailLevel���� 
	*/
	void onEntityInitDetailLevel(Entity* entity, int8 detailLevel);

	/** 
		һ��entity��Ϊ�ƶ��ı������ڱ�entity��detailLevel�ļ��� 
	*/
	void onEntityDetailLevelChanged(const WitnessInfo* witnessInfo, 
		int8 oldDetailLevel, int8 newDetailLevel);
	

	/** 
		���һ������ 
	*/
	uint16 addProximity(float range);
	DECLARE_PY_MOTHOD_ARG1(pyAddProximity, float);

	/** 
		ɾ��һ������ 
	*/
	void delProximity(uint16 id);
	DECLARE_PY_MOTHOD_ARG1(pyDelProximity, uint16);

	/** 
		һ��entity���������entity��ĳ������ 
	*/
	void onEnterTrap(Entity* entity, float range, 
							int controllerID);

	/** 
		һ��entity�뿪�����entity��ĳ������ 
	*/
	void onLeaveTrap(Entity* entity, float range, 
							int controllerID);

	/** 
		��entity����һ���µ�space��ȥ���뿪���������¼�����������ӿ� 
	*/
	void onLeaveTrapID(ENTITY_ID entityID, 
							float range, int controllerID);

	/** 
		������������ 
	*/
//	ProximityMgr& getTrapMgr(void){ return trapMgr_; }

	/** entity��һ���ƶ���� */
	void onMove(PyObject* userData);
protected:
	EntityMailbox*							clientMailbox_;						// ���entity�Ŀͻ���mailbox
	EntityMailbox*							baseMailbox_;						// ���entity��baseapp mailbox
	Position3D								position_;							// entity�ĵ�ǰλ��
	Direction3D								direction_;							// entity�ĵ�ǰ����
//	Chunk*									currChunk_;							// �����ǰentity���ڵ�chunk
	bool									isReal_;							// �Լ��Ƿ���һ��realEntity
	bool									isDestroyed_;						// �����Ƿ�Ҫ����
	float									aoiRadius_;							// ��ǰentity��aoi�뾶
	float									aoiHysteresisArea_;					// ��ǰentityAoi��һ���ͺ�Χ
	bool									isWitnessed_;						// �Ƿ��κι۲��߼��ӵ�
	bool									hasWitness_;						// ���entity�Ƿ���һ���۲���
	std::map<ENTITY_ID, Entity*>			viewEntities_;						// ������Ұ��Χ�ڵ�entityID�� entity������һ���۲��߲���������Դ���
	WITNESSENTITY_DETAILLEVEL_MAP			witnessEntityDetailLevelMap_;		// ���������¼��һ���۲����ڵ�ǰentity�����鼶��
	std::map<ENTITY_ID, Entity*>			witnessEntities_[4];				// �۲���entity��ID�б� ��������һ�����鼶���������鼶���ܹ�4���� �֣�����Զ, ��Զ
//	ProximityMgr							trapMgr_;							// entity���������
	float									topSpeed_;							// entity x,z������ƶ��ٶ�
	float									topSpeedY_;							// entity y������ƶ��ٶ�
	Mercury::Channel *						pChannel_;							// ��entity��ͨ��Ƶ��

	};																										
																											

}

#ifdef CODE_INLINE
#include "entity.ipp"
#endif
#endif
