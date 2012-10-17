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
	�̳߳�
	
	ʹ���÷�:
	if(ThreadPool::getSingletonPtr() && !ThreadPool::getSingleton().isInitialize())
		ThreadPool::getSingleton().createThreadPool(inewThreadCount, inormalMaxThreadCount, imaxThreadCount);

	@param inewThreadCount			: ��ϵͳ��æʱ�̳߳ػ���������ô���̣߳���ʱ��
	@param inormalMaxThreadCount	: �̳߳ػ�һֱ������ô��������߳�
	@param imaxThreadCount			: �̳߳����ֻ������ô����߳�
	���̳߳��̵߳����������ʱ�� һЩû�д��������ᱻ��ʱ���浽������ȴ������߳�������
*/
#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

// common include	
// #define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>	
#include <list>
#include <queue>
#include <algorithm>
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/tasks.hpp"
#include "helper/debug_helper.hpp"
#include "thread/threadtask.hpp"
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#include <windows.h>          // for HANDLE
#include <process.h>          // for _beginthread()	
#include "helper/crashhandler.hpp"
#else
// linux include
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>	
#endif
	
namespace KBEngine{ namespace thread{

/*
	�̳߳ص��̻߳���
*/
class ThreadPool;
class TPThread
{
public:
	TPThread(ThreadPool* threadPool, int threadWaitSecond = 0):
	threadWaitSecond_(threadWaitSecond), 
	currTask_(NULL), 
	threadPool_(threadPool)
	{
		state_ = 0;
		initCond();
		initMutex();
	}
		
	virtual ~TPThread()
	{
		uninitCond();
		uninitMutex();
	}
	
	THREAD_ID getID(void)const
	{
		return tidp_;
	}
	
	void setID(THREAD_ID tidp)
	{
		tidp_ = tidp;
	}
	
	/**����һ���̣߳� �����Լ�����̰߳�*/
	THREAD_ID createThread(void);
	
	virtual void initCond(void)
	{
		THREAD_SINGNAL_INIT(cond_);
	}

	virtual void initMutex(void)
	{
		THREAD_MUTEX_INIT(mutex_);	
	}

	virtual void uninitCond(void)
	{
		THREAD_SINGNAL_DELETE(cond_);
	}
	
	virtual void uninitMutex(void)
	{
		THREAD_MUTEX_DELETE(mutex_);
	}

	virtual void lock(void)
	{
		THREAD_MUTEX_LOCK(mutex_); 
	}
	
	virtual void unlock(void)
	{
		THREAD_MUTEX_UNLOCK(mutex_); 
	}	

	virtual TPTask* tryGetTask(void);
	
	void deleteFiniTask(TPTask* tpTask);
	
	/**���������ź�*/
	int sendCondSignal(void)
	{
		return THREAD_SINGNAL_SET(cond_);
	}
	
	/**�߳�֪ͨ �ȴ������ź�*/
	bool onWaitCondSignal(void);

	/**��ȡ���߳�Ҫ���������*/
	TPTask* getTask(void)const
	{
		return currTask_;
	}

	/**���ñ��߳�Ҫ���������*/
	void setTask(TPTask* tpt)
	{
		currTask_ = tpt;
	}

	int getState(void)const
	{
		return state_;
	}
	
	/**���߳�Ҫ����������Ѿ�������� ���Ǿ���ɾ���������������*/
	void onTaskComplete(void);

#if KBE_PLATFORM == PLATFORM_WIN32
	static unsigned __stdcall threadFunc(void *arg)
#else	
	static void* threadFunc(void* arg)
#endif
	{
		TPThread * tptd = static_cast<TPThread*>(arg);
		bool isRun = true;
#if KBE_PLATFORM == PLATFORM_WIN32
		THREAD_TRY_EXECUTION;
#else			
		pthread_detach(pthread_self());
#endif
		while(isRun)
		{
			isRun = tptd->onWaitCondSignal();
			tptd->state_ = 1;
			TPTask * task = tptd->getTask();
			
			while(task)
			{
				task->process();									// ���������
				TPTask * task1 = tptd->tryGetTask();				// ���Լ��������������ȡ��һ����æ��δ���������

				if(!task1)
				{
					tptd->onTaskComplete();
					break;
				}
				else
				{
					tptd->deleteFiniTask(task);
					task = task1;
					tptd->setTask(task1);
				}
			}
		}

#if KBE_PLATFORM == PLATFORM_WIN32
		THREAD_HANDLE_CRASH;
		return 0;
#else	
		pthread_exit(NULL);
		return NULL;
#endif		
	}
protected:
	THREAD_SINGNAL cond_;			// �߳��ź���
	THREAD_MUTEX mutex_;			// �̻߳�����
	int threadWaitSecond_;			// �߳̿���״̬��������������߳��˳��� С��0Ϊ�����߳� �뵥λ
	TPTask * currTask_;				// ���̵߳ĵ�ǰִ�е�����
	THREAD_ID tidp_;				// ���̵߳�ID
	ThreadPool* threadPool_;		// �̳߳�ָ��
	int state_;						// �߳�״̬ -1��δ����, 0˯�ߣ� 1��æ��
};


class ThreadPool : public Singleton<ThreadPool>
{
protected:
	/**����һ���̳߳��߳�*/
	TPThread* createThread(int threadWaitSecond = 0);
public:		
	
	ThreadPool();
	virtual ~ThreadPool();
	
	virtual void onMainThreadTick();
	
	/**
		��ȡ��ǰ�߳�����
	*/	
	unsigned int getCurrentThreadCount(void)const
	{ 
		return currentThreadCount_; 
	}
	
	/**
		��ȡ��ǰ�����߳�����
	*/		
	unsigned int getCurrentFreeThreadCount(void)const
	{ 
		return currentFreeThreadCount_; 
	}
	
	/**
		�����̳߳�
		@param inewThreadCount			: ��ϵͳ��æʱ�̳߳ػ���������ô���̣߳���ʱ��
		@param inormalMaxThreadCount	: �̳߳ػ�һֱ������ô��������߳�
		@param imaxThreadCount			: �̳߳����ֻ������ô����߳�
	*/
	bool createThreadPool(unsigned int inewThreadCount, 
	unsigned int inormalMaxThreadCount, unsigned int imaxThreadCount);

	/**
		�ƶ�һ���̵߳������б�
	*/
	bool moveThreadToFreeList(TPThread* tptd);
	
	/**
		�ƶ�һ���̵߳���æ�б�
	*/	
	bool moveThreadToBusyList(TPThread* tptd);
	
	/**
		���һ���Ѿ���ɵ������б�
	*/	
	void addFiniTask(TPTask* tptask);
	
	/**
		ɾ��һ������(��ʱ)�߳�
	*/	
	bool removeHangThread(TPThread* tptd);
	
	/**
		���̳߳����һ������
	*/		
	bool addTask(TPTask* tptask);
	
	/**
		�߳������Ƿ񵽴�������
	*/
	bool isThreadCountMax(void)const
	{
		return currentThreadCount_ >= maxThreadCount_;	
	}
	
	/**
		�̳߳��Ƿ��ڷ�æ״̬
		δ���������Ƿ�ǳ���   ˵���̺ܷ߳�æ
	*/
	bool isBusy(void)const
	{
		return busyTaskList_.size() > 32;
	}	

	/**
		��ĳ�����񱣴浽δ�����б�
	*/
	void saveToBusyTaskList(TPTask* tptask);

	/**
		��δ�����б�ȡ��һ������ �����б���ɾ��
	*/
	TPTask* popBusyTaskList(void);
	
	/** 
		�̳߳��Ƿ��Ѿ�����ʼ�� 
	*/
	bool isInitialize(void)const
	{ 
		return isInitialize_; 
	}
protected:
	bool isInitialize_;												// �̳߳��Ƿ񱻳�ʼ����
	
	std::queue<TPTask*> busyTaskList_;								// ϵͳ���ڷ�æʱ��δ����������б�
	std::vector<TPTask*> finiTaskList_;								// �Ѿ���ɵ������б�
	
	THREAD_MUTEX busyTaskList_mutex_;								// ����busyTaskList_������
	THREAD_MUTEX threadStateList_mutex_;							// ����busyThreadList_ and freeThreadList_������
	THREAD_MUTEX finiTaskList_mutex_;								// ����finiTaskList������
	
	std::list<TPThread*> busyThreadList_;							// ��æ���߳��б�
	std::list<TPThread*> freeThreadList_;							// ���õ��߳��б�
	std::list<TPThread*> allThreadList_;							// ���е��߳��б�
	
	unsigned int maxThreadCount_;									// ����߳�����
	unsigned int extraNewAddThreadCount_;							// ���normalThreadCount_���㹻ʹ������´�����ô���߳�
	unsigned int currentThreadCount_;								// ��ǰ�߳���
	unsigned int currentFreeThreadCount_;							// ��ǰ���õ��߳���
	unsigned int normalThreadCount_;								// ��׼״̬�µ��߳����� ����Ĭ�������һ�����������Ϳ�����ô���߳�
																	// ����̲߳��㹻������´���һЩ�̣߳� ����ܹ���maxThreadNum.
};

}
}
#endif
