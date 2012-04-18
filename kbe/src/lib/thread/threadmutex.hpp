/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
/*
	�̻߳����壺
		�����߳�֮����⾺�������������⡣
	�÷�:
		ThreadMutex tm;
		tm.lockMutex();
		....��ȫ����
		tm.unlockMutex();
		
		��������ThreadGuard��ʹ��
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
#ifndef __THREADMUTEX_H__
#define __THREADMUTEX_H__
	
// common include
#include "cstdkbe/cstdkbe.hpp"
//#define NDEBUG
#include <assert.h>

// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#include <errno.h>
#endif
	
namespace KBEngine{ namespace thread{

class ThreadMutex 
{
public:
	ThreadMutex(void)
	{
		THREAD_MUTEX_INIT(mutex_);
	}

	virtual ~ThreadMutex(void) 
	{ 
		THREAD_MUTEX_DELETE(mutex_);
	}	
	
	void lockMutex(void)
	{
		THREAD_MUTEX_LOCK(mutex_);
	}

	void unlockMutex(void)
	{
		THREAD_MUTEX_UNLOCK(mutex_);
	}
protected:
	THREAD_MUTEX mutex_;
};

}
}
#endif
