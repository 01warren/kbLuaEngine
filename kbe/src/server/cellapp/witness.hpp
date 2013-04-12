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

#ifndef __KBE_WITNESS_HPP__
#define __KBE_WITNESS_HPP__

// common include
#include "updatable.hpp"
#include "entityref.hpp"
#include "helper/debug_helper.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/objectpool.hpp"

// #define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32	
#else
// linux include
#endif

namespace KBEngine{

namespace Mercury
{
	class Bundle;
	class MessageHandler;
}

class Entity;
class MemoryStream;
class AOITrigger;
class Space;

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

/**
	���ģ�������������Ǹ���Ȥ��entity���ݣ� �磺aoi�� ���Ը��£� ����entity�ķ���
	�����䴫��������ߡ�
*/
class Witness : public PoolObject, public Updatable
{
public:
	Witness();
	~Witness();

	typedef KBEShared_ptr< SmartPoolObject< Witness > > SmartPoolObjectPtr;
	static SmartPoolObjectPtr createSmartPoolObj();

	static ObjectPool<Witness>& ObjPool();
	void onReclaimObject();

	INLINE Entity* pEntity();

	void attach(Entity* pEntity);
	void detach(Entity* pEntity);

	void setAoiRadius(float radius, float hyst = 5.0f);
	
	INLINE float aoiRadius()const;
	INLINE float aoiHysteresisArea()const;

	typedef std::vector<Mercury::Bundle*> Bundles;
	Bundles* pBundles();

	bool update();
	
	void onEnterSpace(Space* pSpace);
	void onLeaveSpace(Space* pSpace);

	void onEnterAOI(Entity* pEntity);
	void onLeaveAOI(Entity* pEntity);

	/**
		��witness�ͻ�������һ����Ϣ
	*/
	bool sendToClient(const Mercury::MessageHandler& msgHandler, Mercury::Bundle* pBundle);

	typedef std::vector<EntityRef*> AOI_ENTITIES;
private:
	Entity*									pEntity_;

	float									aoiRadius_;							// ��ǰentity��aoi�뾶
	float									aoiHysteresisArea_;					// ��ǰentityAoi��һ���ͺ�Χ

	AOITrigger*								pAOITrigger_;

	AOI_ENTITIES							aoiEntities_;

};

}

#ifdef CODE_INLINE
#include "witness.ipp"
#endif
#endif
