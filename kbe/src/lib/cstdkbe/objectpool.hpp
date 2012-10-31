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
/*
	ʹ�÷���:
	ObjectPool<TimerData>* objPool = new ObjectPool<TimerData>();
	struct TimerData * td = objPool->createObject();
	objPool->reclaimObject(td);		// �黹����
*/
#ifndef __OBJECTPOOL_H__
#define __OBJECTPOOL_H__
	
// common include	
//#define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>	
#include <map>	
#include <list>	
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#include <errno.h>
#endif
	
namespace KBEngine{

#define OBJECT_POOL_INIT_SIZE	16

template< typename T >
class SmartPoolObject;

template< typename T >
class ObjectPool
{
public:
	typedef std::list<T*> OBJECTS;

	ObjectPool()
	{
		for(unsigned int i=0; i<OBJECT_POOL_INIT_SIZE; i++){
			objects_.push_back(new T);
		}
	}

	ObjectPool(unsigned int preAssignVal)
	{
		for(unsigned int i=0; i<preAssignVal; i++){
			objects_.push_back(new T);
		}
	}

	~ObjectPool()
	{
		typename OBJECTS::iterator iter = objects_.begin();
		for(; iter!=objects_.end(); iter++)
		{
			if(!(*iter)->destructorPoolObject())
			{
				delete (*iter);
			}
		}
				
		objects_.clear();	
	}	
	
	const OBJECTS& objects(void)const { return objects_; }

	/** 
		ǿ�ƴ���һ��ָ�����͵Ķ��� ����������Ѿ������򷵻����еģ�����
		����һ���µģ� �����������Ǽ̳���T�ġ�
	*/
	template<typename T1>
	T* createObject(void)
	{
		if(objects_.size() > 0){
			T* t = static_cast<T1*>(*objects_.begin());
			objects_.pop_front();
			return t;
		}
		
		// INFO_MSG("ObjectPool:create new object! total:%d\n", m_totalCount_);
		return new T1;
	}

	/** 
		����һ������ ����������Ѿ������򷵻����еģ�����
		����һ���µġ�
	*/
	T* createObject(void)
	{
		if(objects_.size() > 0){
			T* t = static_cast<T*>(*objects_.begin());
			objects_.pop_front();

			// ������״̬
			t->onReclaimObject();
			return t;
		}
		
		// INFO_MSG("ObjectPool:create new object! total:%d\n", m_totalCount_);
		return new T;
	}

	/**
		����һ������
	*/
	void reclaimObject(T* obj)
	{
		if(obj != NULL)
			objects_.push_back(obj);
	}

	size_t size(void)const{ return objects_.size(); }
	
protected:
	OBJECTS objects_;							// ���󻺳���
};

/*
	�ض��� ����ʹ�óصĶ������ʵ�ֻ��չ��ܡ�
*/
class PoolObject
{
public:
	virtual void onReclaimObject() = 0;
	
	/**
		�ض�������ǰ��֪ͨ
		ĳЩ��������ڴ���һЩ����
	*/
	virtual bool destructorPoolObject()
	{
		return false;
	}
};

template< typename T >
class SmartObjectPool : public ObjectPool<T>
{
public:
};

template< typename T >
class SmartPoolObject
{
public:
	SmartPoolObject(T* pPoolObject, ObjectPool<T>& objectPool):
	  pPoolObject_(pPoolObject),
	  objectPool_(objectPool)
	{
	}

	~SmartPoolObject()
	{
		onReclaimObject();
	}

	void onReclaimObject()
	{
		if(pPoolObject_ != NULL)
		{
			objectPool_.reclaimObject(pPoolObject_);
			pPoolObject_ = NULL;
		}
	}

	T* get()
	{
		return pPoolObject_;
	}

	T* operator->()
	{
		return pPoolObject_;
	}
private:
	T* pPoolObject_;
	ObjectPool<T>& objectPool_;
};

}
#endif
