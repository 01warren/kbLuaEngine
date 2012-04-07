/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
#ifndef __ENTITY_H__
#define __ENTITY_H__
	
// common include
//#include "entitymovecontroller.hpp"
//#include "timer.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "helper/debug_helper.hpp"
//#include "entityMailbox.hpp"
#include "pyscript/math.hpp"
#include "pyscript/scriptobject.hpp"
#include "entitydef/datatypes.hpp"	
#include "entitydef/entitydef.hpp"	
//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#include <errno.h>
#endif
	
namespace KBEngine{
class Chunk;
class Entity;

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
public:
	typedef std::map<ENTITY_ID, WitnessInfo*>	WITNESSENTITY_DETAILLEVEL_MAP;
public:
	Entity(ENTITY_ID id, ScriptModule* scriptModule);
	~Entity();

	/** entity�ű���ʼ�� */
	void initializeScript();

	/** �������entity�����ֿռ��ڵ��ֵ����� */
	void createNamespace(PyObject* dictData);

	/** ������entity cell���ֶ����ָ��flags�������Ե�����, ����ֵ��һ��pyDict */
	PyObject* getCellDataByFlags(uint32 flags);
	/** ������entity cell���ֶ����ָ�����鼶����������Ե����� */
	void getCellDataByDetailLevel(const int8& detailLevel, MemoryStream* mstream);
	
	/** �������entity */
	void onDestroy(void);
	void destroy();
	
	/** �ж������Ƿ���һ��realEntity */
	inline bool isReal(void)const{ return m_isReal_; }
	
	/** �ű������������Ի��߷��� */
	int onScriptSetAttribute(PyObject* attr, PyObject * value);	
	
	/** �ű������ȡ���Ի��߷��� */
	PyObject* onScriptGetAttribute(PyObject* attr);	
			
	/** �ű�����ɾ��һ������ */
	int onScriptDelAttribute(PyObject* attr);

	/** onTimer������ */
	//void onTimer(const TIMER_ID& timerID, TimerArgsBase* args);
	
	/** ��ýű����� */
	const char* getScriptModuleName(void)const{ return m_scriptModule_->getScriptType()->tp_name; }	
	
	/** ��ȡ���entity�Ľű�ģ���װ���� */
	ScriptModule* getScriptModule(void)const{ return m_scriptModule_; }
	
	/** ֧��pickler ���� */
	static PyObject* __reduce_ex__(PyObject* self, PyObject* protocol);
	
	/** �����������ݱ��ı��� */
	void onDefDataChanged(PropertyDescription* propertyDescription, PyObject* pyData);
public:
	/** ���entity��ID */
	ENTITY_ID getID()const{ return m_id_; }
	void setID(const int& id){ m_id_ = id; }
	static PyObject* pyGetID(Entity *self, void *closure){ return PyLong_FromLong(self->getID()); }

	/** ���entity������space��ID */
	uint32 getSpaceID()const{ return m_spaceID_; }
	void setSpaceID(int id){ m_spaceID_ = id; }
	static PyObject* pyGetSpaceID(Entity *self, void *closure){ return PyLong_FromLong(self->getSpaceID()); }

	/** �ű���������ʵ�� */
	void destroyEntity(void);
	static PyObject* pyDestroyEntity(PyObject* self, PyObject* args, PyObject* kwds);
	
	/** �ű���ȡmailbox */
	/*
	EntityMailbox* getBaseMailbox()const{ return m_baseMailbox_; }
	static PyObject* pyGetBaseMailbox(Entity *self, void *closure);
	void setBaseMailbox(EntityMailbox* mailbox){ m_baseMailbox_ = mailbox; }
	*/
	/** �ű���ȡmailbox */
	/*
	EntityMailbox* getClientMailbox()const{ return m_clientMailbox_; }
	static PyObject* pyGetClientMailbox(Entity *self, void *closure);
	void setClientMailbox(EntityMailbox* mailbox){ m_clientMailbox_ = mailbox; if(m_clientMailbox_!= NULL) onGetWitness(); else onLoseWitness(); }
	*/
	/** �ű���ȡ������entity��position */
	Position3D& getPosition(){ return m_position_; }
	static PyObject* pyGetPosition(Entity *self, void *closure);
	static int pySetPosition(Entity *self, PyObject *value, void *closure);
	void setPosition(Position3D& pos);

	/** �ű���ȡ������entity�ķ��� */
	Direction3D& getDirection(){ return m_direction_; }
	static PyObject* pyGetDirection(Entity *self, void *closure);
	static int pySetDirection(Entity *self, PyObject *value, void *closure);
	void setDirection(Direction3D& dir){ m_direction_ = dir; }
	
	/** ����entity�����λ�� */
	void setPositionAndDirection(Position3D& position, Direction3D& direction);

	/** ��Ӻ�ɾ��һ��timer */
	PyObject* addTimer(uint32 startTrrigerIntervalTime, uint32 loopTrrigerIntervalTime, PyObject* args);
	PyObject* delTimer(TIMER_ID timerID);
	static PyObject* pyAddTimer(PyObject* self, PyObject* args, PyObject* kwds);
	static PyObject* pyDelTimer(PyObject* self, PyObject* args, PyObject* kwds);
	
	/** �ű�����Ϊ��ǰ����space����һ������ӳ�� */
	static PyObject* pyAddSpaceGeometryMapping(PyObject* self, PyObject* args, PyObject* kwds);

	/** ��ǰentity���������Aoi�뾶��Χ */
	PyObject* setAoiRadius(float radius, float hyst);
	float getAoiRadius(void)const{ return m_aoiRadius_; }
	float getAoiHystArea(void)const{ return m_aoiHysteresisArea_; }
	static PyObject* pySetAoiRadius(PyObject* self, PyObject* args, PyObject* kwds);
	
	/** ��ǰentity�Ƿ�Ϊreal */
	static PyObject* pyIsReal(PyObject* self, PyObject* args, PyObject* kwds);
	
	/** �ű���õ�ǰentity�Ƿ�Ϊ��Ҫ���ٵ�entity */
	bool isDestroyed(){ return m_isDestroyed_; }
	static PyObject* pyGetIsDestroyed(Entity *self, void *closure);
	
	/** entity�ƶ����� */
	bool navigateStep(Position3D& destination, float& velocity, float& maxMoveDistance, float& maxDistance, 
					bool faceMovement, float& girth, PyObject* userData);
	static PyObject* pyNavigateStep(PyObject* self, PyObject* args, PyObject* kwds);
	
	/** ֹͣ�κη�ʽ���ƶ���Ϊ */
	bool stopMove();
	static PyObject* pyStopMove(PyObject* self, PyObject* args, PyObject* kwds);

	/** entity�ƶ���ĳ���� */
	bool moveToPoint(Position3D& destination, float& velocity, PyObject* userData, bool faceMovement, bool moveVertically);
	static PyObject* pyMoveToPoint(PyObject* self, PyObject* args, PyObject* kwds);

	
	/** �ű���ȡ������entity�����xz�ƶ��ٶ� */
	float getTopSpeed()const{ return m_topSpeed_; }
	static PyObject* pyGetTopSpeed(Entity *self, void *closure){ return PyFloat_FromDouble(self->getTopSpeed()); };
	static int pySetTopSpeed(Entity *self, PyObject *value, void *closure){ self->setTopSpeed(float(PyFloat_AsDouble(PySequence_GetItem(value, 0)))); return 0; };
	void setTopSpeed(float speed){ m_topSpeed_ = speed; }
	
	/** �ű���ȡ������entity�����y�ƶ��ٶ� */
	float getTopSpeedY()const{ return m_topSpeedY_; }
	static PyObject* pyGetTopSpeedY(Entity *self, void *closure){ return PyFloat_FromDouble(self->getTopSpeedY()); };
	static int pySetTopSpeedY(Entity *self, PyObject *value, void *closure){ self->setTopSpeedY(float(PyFloat_AsDouble(PySequence_GetItem(value, 0)))); return 0; };
	void setTopSpeedY(float speed){ m_topSpeedY_ = speed; }
	
	/** �ű�������һ����Χ���ĳ�����͵�entities */
	static PyObject* pyEntitiesInRange(PyObject* self, PyObject* args, PyObject* kwds);
public:
	/** Զ�̺��б�entity�ķ��� */
	//void onRemoteMethodCall(SocketPacket& recvPacket);
	/** �յ��ʼ� */
	//void onReceiveMail(MAIL_TYPE& mailType, SocketPacket& recvPacket);

	/** �������entity��ǰ����chunk��ID */
	//void setCurrentChunk(Chunk* chunk){ Chunk* oldchunk = m_currChunk_; m_currChunk_ = chunk; onCurrentChunkChanged(oldchunk); }
	/** ��ǰchunk���ı� */
	//void onCurrentChunkChanged(Chunk* oldChunk);
	/** ������entity��ǰ����chunk��ID */
	//Chunk* getAtChunk(void)const{ return m_currChunk_; }

	/** �Ƿ��κ�proxy���ӵ�, ������entityû�пͻ��ˣ� �����ֵ��Ч */
	bool isWitnessed(void)const{ return m_isWitnessed_; }
	static PyObject* pyIsWitnessed(Entity *self, void *closure);
	/** entity�Ƿ���һ���۲��� */
	bool hasWitness(void)const{ return m_hasWitness_; }
	static PyObject* pyHasWitness(Entity *self, void *closure);
	/** ����һ���۲��߹۲쵽�� */
	void onWitnessed(Entity* entity, const float& range);
	/** �Ƴ�һ���۲�����Ĺ۲��� */
	void onRemoveWitness(Entity* entity);
	/** ���entity����˹۲������ */
	void onGetWitness(void);
	/** entity��ʧ�˹۲������ */
	void onLoseWitness(void);
	/** ������۲쵽��entities�б� */
	std::map<ENTITY_ID, Entity*>& getViewEntities(void){ return m_viewEntities_; }
	/** ������й۲쵽�ҵĹ۲��� */
	WITNESSENTITY_DETAILLEVEL_MAP& getWitnessEntities(void){ return m_witnessEntityDetailLevelMap_; }
	/** ����witness��״̬ */
	void onUpdateWitness(Entity* entity, const float& range);
	/** һ���½�����Ұ��Χ��entity */
	void onViewEntity(Entity* entity);
	/** һ��entity�뿪����Ұ��Χ */
	void onLoseViewEntity(Entity* entity);
	/** һ��entity��һ�α�����(��һ�ν��뵽���������)detailLevel���� */
	void onEntityInitDetailLevel(Entity* entity, int8 detailLevel);
	/** һ��entity��Ϊ�ƶ��ı������ڱ�entity��detailLevel�ļ��� */
	void onEntityDetailLevelChanged(WitnessInfo* witnessInfo, const int8& oldDetailLevel, const int8& newDetailLevel);
	
	/** ���һ������ */
	uint16 addProximity(float range);
	static PyObject* pyAddProximity(PyObject* self, PyObject* args, PyObject* kwds);
	/** ɾ��һ������ */
	void delProximity(const uint16& id);
	static PyObject* pyDelProximity(PyObject* self, PyObject* args, PyObject* kwds);
	/** һ��entity���������entity��ĳ������ */
	void onEnterTrap(Entity* entity, const float& range, const int& controllerID);
	/** һ��entity�뿪�����entity��ĳ������ */
	void onLeaveTrap(Entity* entity, const float& range, const int& controllerID);
	/** ��entity����һ���µ�space��ȥ���뿪���������¼�����������ӿ� */
	void onLeaveTrapID(const ENTITY_ID& entityID, const float& range, const int& controllerID);
	/** ������������ */
//	ProximityMgr& getTrapMgr(void){ return m_trapMgr_; }

	/** entity��һ���ƶ���� */
	void onMove(PyObject* userData);
protected:
	ENTITY_ID								m_id_;								// id����
	ScriptModule*							m_scriptModule_;					// ��entity��ʹ�õĽű�ģ�����
	ScriptModule::PROPERTYDESCRIPTION_MAP*	m_lpPropertyDescrs_;				// �������entity����������
	uint32									m_spaceID_;							// ���entity����space��ID
	//EntityMailbox*							m_clientMailbox_;					// ���entity�Ŀͻ���mailbox
	//EntityMailbox*							m_baseMailbox_;						// ���entity��baseapp mailbox
	Position3D								m_position_;						// entity�ĵ�ǰλ��
	Direction3D								m_direction_;						// entity�ĵ�ǰ����
//	TimerFunc								m_TimerFunc_;						// onTimer������ַ
//	Timer									m_timers_;							// timers������
//	Chunk*									m_currChunk_;						// �����ǰentity���ڵ�chunk
	bool									m_isReal_;							// �Լ��Ƿ���һ��realEntity
	bool									m_isDestroyed_;						// �����Ƿ�Ҫ����
	float									m_aoiRadius_;						// ��ǰentity��aoi�뾶
	float									m_aoiHysteresisArea_;				// ��ǰentityAoi��һ���ͺ�Χ
	bool									m_isWitnessed_;						// �Ƿ��κι۲��߼��ӵ�
	bool									m_hasWitness_;						// ���entity�Ƿ���һ���۲���
	std::map<ENTITY_ID, Entity*>			m_viewEntities_;					// ������Ұ��Χ�ڵ�entityID�� entity������һ���۲��߲���������Դ���
	WITNESSENTITY_DETAILLEVEL_MAP			m_witnessEntityDetailLevelMap_;		// ���������¼��һ���۲����ڵ�ǰentity�����鼶��
	std::map<ENTITY_ID, Entity*>			m_witnessEntities_[4];				// �۲���entity��ID�б� ��������һ�����鼶���������鼶���ܹ�4���� �֣�����Զ, ��Զ
//	ProximityMgr							m_trapMgr_;							// entity���������
	float									m_topSpeed_;						// entity x,z������ƶ��ٶ�
	float									m_topSpeedY_;						// entity y������ƶ��ٶ�
};

}
#endif
