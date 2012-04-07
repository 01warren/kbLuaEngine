/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
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
#include "helper/debug_helper.hpp"
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#include <windows.h>          // for HANDLE
#include <process.h>          // for _beginthread()	
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
	�̳߳ص����������
*/
class TPTask
{
protected:	
public:	
	TPTask(){}
	virtual ~TPTask(){}

	/*
		����������ӿڣ�  ��Ҫ�̳�����ɾ���ʵ��
	*/
	virtual void onHandle(void) = 0;
};

/*
	�̳߳ص��̻߳���
*/
class ThreadPool;
class TPThread
{
protected:
	THREAD_SINGNAL m_cond;			// �߳��ź���
	THREAD_MUTEX m_mutex;			// �̻߳�����
	int m_threadWaitSecond;			// �߳̿���״̬��������������߳��˳��� С��0Ϊ�����߳� �뵥λ
	TPTask * m_currTask;			// ���̵߳ĵ�ǰִ�е�����
	THREAD_ID m_tidp;				// ���̵߳�ID
	ThreadPool* m_threadPool;		// �̳߳�ָ��
	int m_state;					// �߳�״̬ -1��δ����, 0˯�ߣ� 1��æ��
public:
	TPThread(ThreadPool* threadPool, int threadWaitSecond = 0) : \
	m_threadWaitSecond(threadWaitSecond), 
	m_currTask(NULL), 
	m_threadPool(threadPool)
	{
		m_state = 0;
		initCond();
		initMutex();
	}
		
	virtual ~TPThread()
	{
		uninitCond();
		uninitMutex();
	}
	
	THREAD_ID getID(void)const{
		return m_tidp;
	}
	
	void setID(THREAD_ID tidp){
		m_tidp = tidp;
	}
	
	/**����һ���̣߳� �����Լ�����̰߳�*/
	THREAD_ID createThread(void);
	
	virtual void initCond(void){
		THREAD_SINGNAL_INIT(m_cond);
	}

	virtual void initMutex(void){
		THREAD_MUTEX_INIT(m_mutex);	
	}

	virtual void uninitCond(void){
		THREAD_SINGNAL_DELETE(m_cond);
	}
	
	virtual void uninitMutex(void){
		THREAD_MUTEX_DELETE(m_mutex);
	}

	virtual void lock(void){
		THREAD_MUTEX_LOCK(m_mutex); 
	}
	
	virtual void unlock(void){
		THREAD_MUTEX_UNLOCK(m_mutex); 
	}	

	virtual TPTask* tryGetTask(void);
	
	/**���������ź�*/
	int sendCondSignal(void){
		return THREAD_SINGNAL_SET(m_cond);
	}
	
	/**�߳�֪ͨ �ȴ������ź�*/
	bool onWaitCondSignal(void);

	/**��ȡ���߳�Ҫ���������*/
	TPTask* getTask(void)const{
		return m_currTask;
	}

	/**���ñ��߳�Ҫ���������*/
	void setTask(TPTask* tpt){
		m_currTask = tpt;
	}

	int getState(void)const{
		return m_state;
	}
	
	/**���߳�Ҫ����������Ѿ�������� ���Ǿ���ɾ���������������*/
	void onTaskComplete(void);

#if KBE_PLATFORM == PLATFORM_WIN32
	static unsigned __stdcall threadFunc(void *arg)
#else	
	static void* threadFunc(void* arg)
#endif
	{
		TPThread * tptd = (TPThread*) arg;
		bool isRun = true;
#if KBE_PLATFORM == PLATFORM_WIN32
		THREAD_TRY_EXECUTION;
#else			
		pthread_detach(pthread_self());
#endif
		while(isRun)
		{
			isRun = tptd->onWaitCondSignal();
			tptd->m_state = 1;
			TPTask * task = tptd->getTask();
			
			while(task)
			{
				task->onHandle();									// ���������
				TPTask * task1 = tptd->tryGetTask();				// ���Լ��������������ȡ��һ����æ��δ���������

				if(!task1){
					tptd->onTaskComplete();
					break;
				}
				else
				{
					delete task;
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
	
};


class ThreadPool : public Singleton<ThreadPool>
{
protected:
	bool m_isInitialize;											// �̳߳��Ƿ񱻳�ʼ����
	std::queue<TPTask*> m_busyTaskList;								// ϵͳ���ڷ�æʱ��δ����������б�
	THREAD_MUTEX busyTaskList_mutex;								// ����m_busyTaskList������
	THREAD_MUTEX threadStateList_mutex;								// ����m_busyThreadList and m_freeThreadList������
	std::list<TPThread*> m_busyThreadList;							// ��æ���߳��б�
	std::list<TPThread*> m_freeThreadList;							// ���õ��߳��б�
	std::list<TPThread*> m_allThreadList;							// ���е��߳��б�
	unsigned int m_maxThreadCount;									// ����߳�����
	unsigned int m_extraNewAddThreadCount;							// ���m_normalThreadCount���㹻ʹ������´�����ô���߳�
	unsigned int m_currentThreadCount;								// ��ǰ�߳���
	unsigned int m_currentFreeThreadCount;							// ��ǰ���õ��߳���
	unsigned int m_normalThreadCount;								// ��׼״̬�µ��߳����� ����Ĭ�������һ�����������Ϳ�����ô���߳�
																	// ����̲߳��㹻������´���һЩ�̣߳� ����ܹ���maxThreadNum.
																	
protected:
	/**����һ���̳߳��߳�*/														
	TPThread* createThread(int threadWaitSecond = 0);													
public:		
	
	ThreadPool()
	{		
		m_extraNewAddThreadCount = m_currentThreadCount = m_currentFreeThreadCount = m_normalThreadCount = 0;
		THREAD_MUTEX_INIT(threadStateList_mutex);	
		THREAD_MUTEX_INIT(busyTaskList_mutex);
	}

	virtual ~ThreadPool()
	{
		THREAD_MUTEX_DELETE(threadStateList_mutex);
		THREAD_MUTEX_DELETE(busyTaskList_mutex);
		std::list<TPThread*>::iterator itr = m_allThreadList.begin();
		for(; itr != m_allThreadList.end(); itr++)
		{
			if((*itr))
			{
				delete (*itr);
				(*itr) = NULL;
			}
		}
	}
	
	/**��ȡ��ǰ�߳�����*/	
	unsigned int getCurrentThreadCount(void)const{ 
		return m_currentThreadCount; 
	}
	
	/**��ȡ��ǰ�����߳�����*/		
	unsigned int getCurrentFreeThreadCount(void)const{ 
		return m_currentFreeThreadCount; 
	}
	
	/**
		�����̳߳�
		@param inewThreadCount			: ��ϵͳ��æʱ�̳߳ػ���������ô���̣߳���ʱ��
		@param inormalMaxThreadCount	: �̳߳ػ�һֱ������ô��������߳�
		@param imaxThreadCount			: �̳߳����ֻ������ô����߳�
	*/
	bool createThreadPool(unsigned int inewThreadCount, unsigned int inormalMaxThreadCount, unsigned int imaxThreadCount);

	/**�ƶ�һ���̵߳������б�*/
	bool moveThreadToFreeList(TPThread* tptd);
	
	/**�ƶ�һ���̵߳���æ�б�*/	
	bool moveThreadToBusyList(TPThread* tptd);
	
	/**ɾ��һ������(��ʱ)�߳�*/	
	bool removeHangThread(TPThread* tptd);
	
	/**���̳߳����һ������*/		
	bool addTask(TPTask* tptask);
	
	/**�߳������Ƿ񵽴�������*/
	bool isThreadCountMax(void)const{
		return m_currentThreadCount >= m_maxThreadCount;	
	}
	
	/**
		�̳߳��Ƿ��ڷ�æ״̬
		δ���������Ƿ�ǳ���   ˵���̺ܷ߳�æ
	*/
	bool isBusy(void)const{
		return m_busyTaskList.size() > 32;
	}	

	/**��ĳ�����񱣴浽δ�����б�*/
	void saveToBusyTaskList(TPTask* tptask);

	/**��δ�����б�ȡ��һ������ �����б���ɾ��*/
	TPTask* popBusyTaskList(void);
	
	/** �̳߳��Ƿ��Ѿ�����ʼ�� */
	bool isInitialize(void)const{ 
		return m_isInitialize; 
	}
};

}
}
#endif
