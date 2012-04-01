/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
/*
	IDAllocate(������)
		��������һ�����������������Ψһid�� ʹ����������������Լ���֤�� һ��Ӧ��ֻ��ʹ��
		ͬһ��id����������ȡid����Ψһ�ġ�
		
		�����һ�� unsigned int���ͣ� �����������һֱ���Ϸ��䣬 ���ﵽ���͵����ֵ֮���
		��תͷ�ִ�0��ʼ�����ۼӷ��䣬 �����list��Ѱ�ң� �����ǰҪ�����IDû����list���ҵ�
		��ô���id�������䡣

		�÷�:
		IDAllocate<ENTITY_ID>* m_IDAllocPtr = new IDAllocate<ENTITY_ID>;
		// ����һ��id 
		m_IDAllocPtr->alloc()
		// ����һ��id
		m_IDAllocPtr->reclaim()
		
	IDServer(������)
		�����Ҫ���ṩ������������֮���entityID�ķ��䣬 ����Ҫ��baseappmgrʹ�ã� ÿ��IDserver
		�����ȡID��ʱ�� ����������ͻ����һ��Ψһid�θ��ͻ��ˣ� ��ô�ͻ��˾Ϳ��Ը��������
		�������е�Ψһid���������ɵķ��ɡ�
		
		�÷�:
		IDServer<ENTITY_ID>* m_idServer = new IDServer<ENTITY_ID>(1, 400);
		// ��ȡһ��id�� �������IDClient
		std::pair< unsigned int, unsigned int > idRange = m_idServer->allocRange();
		g_socketStreamIDClient->send(idRange.first, idRange.second);
		
	IDClient(�ͻ���)
		���ģ�������IDServer����id����ͽ��յģ� ��
		
		�÷�:
		IDClient<ENTITY_ID>* m_idClient = new IDClient<ENTITY_ID>;
		// ���IDServer���͹�����id��
		m_idClient->onAddRange(idBegin, idEnd);
		// ����һ��id 
		m_idClient->alloc()
*/
#ifndef __IDAllocate_H__
#define __IDAllocate_H__

// common include	
#include "log/debug_helper.hpp"
#include "cstdkbe/cstdkbe.hpp"
//#define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>	
#include <queue>	
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#include <errno.h>
#endif
	
namespace KBEngine{

template< typename T >
class IDAllocate
{
protected:
	typename std::queue< T > m_idList_;						// id�б� ����ID����������б���
	T m_lastID_;											// ���һ�����뵽��ID
public:
	IDAllocate(): m_lastID_(0)
	{
	}

	~IDAllocate()
	{
	}	
	
	/** ����һ��id */
	T alloc(void)
	{
		if(m_idList_.size() > 0)
		{
			T n = m_idList_.front();
			m_idList_.pop();
			return n;
		}
		
		return ++m_lastID_;
	}
	
	/** ����һ��id */
	void reclaim(T id)
	{
		m_idList_.push(id);
	}
	
};


template< typename T >
class IDServer
{
protected:
	T m_lastIDRange_begin_;										// ���һ�����뵽��ID�ε���ʼλ��
	T m_rangeStep_;												// id�ε�һ���γ���
public:
	IDServer(T idBegin, T rangeStep): 
	m_lastIDRange_begin_(idBegin), 
	m_rangeStep_(rangeStep)
	{
	}

	~IDServer()
	{
	}	
	
	/** ����һ��id�� */
	std::pair< T, T > allocRange(void)
	{
		DEBUG_MSG("IDServer::allocRange: %d-%d.\n", m_lastIDRange_begin_, m_lastIDRange_begin_ + m_rangeStep_);
		std::pair< T, T > p = std::make_pair(m_lastIDRange_begin_, m_lastIDRange_begin_ + m_rangeStep_);
		m_lastIDRange_begin_ += m_rangeStep_;
		return p;
	}
};

template< typename T >
class IDClient
{
protected:
	typename std::queue< std::pair< T, T > > m_idList_;					// id�б� ����ID����������б���
	T m_lastIDRange_begin_;												// ���һ�����뵽��ID�ε���ʼλ��
	T m_lastIDRange_end_;												
public:
	IDClient():m_lastIDRange_begin_(0), m_lastIDRange_end_(0)
	{
	}
	
	/** ����ʱ����֪ͨIDServer���л��գ� ��ʹ�����Լ������ⷽ���ά�� */
	~IDClient()
	{
	}	
	
	size_t getSize()const{ return m_lastIDRange_end_ - m_lastIDRange_begin_; }
	
	/** idserver ���������һ��id�� */
	void onAddRange(T idBegin, T idEnd)
	{
		DEBUG_MSG("IDClient::onAddRange: number of ids increased from %d to %d.\n", idBegin, idEnd);
		if(getSize() <= 0)
		{
			m_lastIDRange_begin_ = idBegin;
			m_lastIDRange_end_ = idEnd;
		}
		else
		{
			m_idList_.push(std::make_pair(idBegin, idEnd));
		}
	}
	
	/** ����һ��id */
	T alloc(void)
	{
		assert(getSize() > 0 && "IDClient:: alloc:no usable of the id.\n");
		T id = m_lastIDRange_begin_;
		m_lastIDRange_begin_ ++;
		if(m_lastIDRange_begin_ > m_lastIDRange_end_)
		{
			if(m_idList_.size() > 0)
			{
				std::pair< T, T > n = m_idList_.front();
				m_lastIDRange_begin_ = n.first;
				m_lastIDRange_end_ = n.second;
				m_idList_.pop();
			}
			else
			{
				m_lastIDRange_begin_ = m_lastIDRange_end_ = 0;
			}
		}
		return id;
	}
	
	/** ����һ��id */
	void onReclaim(T id)
	{
	}
};


}
#endif
