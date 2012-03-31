/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
/*
	�߳��ػ��壺
		�����߳�֮����⾺�������������⡣
	�÷�:
		��һ�����ж��廥�����Ա
		ThreadMutex tm;
		����Ҫ�����ĵط�:
		void XXCLASS::foo(void)
		{
			ThreadGuard tg(this->tm);
			����Ĵ��붼�ǰ�ȫ��
			...
		}
*/
#ifndef __THREADGUARD_H__
#define __THREADGUARD_H__
	
// common include
#include "thread/threadMutex.hpp"
//#define NDEBUG
#include <assert.h>

// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#include <errno.h>
#endif
	
namespace KBEngine{ namespace thread{

class ThreadGuard
{
public:
	explicit ThreadGuard(ThreadMutex* mutexPtr):m_mutexPtr_(mutexPtr)
	{
		m_mutexPtr_->lockMutex();
	}

	virtual ~ThreadGuard(void) 
	{ 
		m_mutexPtr_->unlockMutex();
	}	
	
protected:
	ThreadMutex* m_mutexPtr_;
};

}
}
#endif
