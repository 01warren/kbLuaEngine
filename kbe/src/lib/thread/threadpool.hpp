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
#include "cstdkbe/tasks.hpp"
#include "helper/debug_helper.hpp"
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#include <windows.h>          // for HANDLE
#include <process.h>          // for _beginthread()	
#include "cstdkbe/crashhandler.hpp"
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
	TPThread(ThreadPool* threadPool, int threadWaitSecond = 0) : \
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
	
	THREAD_ID getID(void)const{
		return tidp_;
	}
	
	void setID(THREAD_ID tidp){
		tidp_ = tidp;
	}
	
	/**����һ���̣߳� �����Լ�����̰߳�*/
	THREAD_ID createThread(void);
	
	virtual void initCond(void){
		THREAD_SINGNAL_INIT(cond_);
	}

	virtual void initMutex(void){
		THREAD_MUTEX_INIT(mutex_);	
	}

	virtual void uninitCond(void){
		THREAD_SINGNAL_DELETE(cond_);
	}
	
	virtual void uninitMutex(void){
		THREAD_MUTEX_DELETE(mutex_);
	}

	virtual void lock(void){
		THREAD_MUTEX_LOCK(mutex_); 
	}
	
	virtual void unlock(void){
		THREAD_MUTEX_UNLOCK(mutex_); 
	}	

	virtual Task* tryGetTask(void);
	
	/**���������ź�*/
	int sendCondSignal(void){
		return THREAD_SINGNAL_SET(cond_);
	}
	
	/**�߳�֪ͨ �ȴ������ź�*/
	bool onWaitCondSignal(void);

	/**��ȡ���߳�Ҫ���������*/
	Task* getTask(void)const{
		return currTask_;
	}

	/**���ñ��߳�Ҫ���������*/
	void setTask(Task* tpt){
		currTask_ = tpt;
	}

	int getState(void)const{
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
			tptd->state_ = 1;
			Task * task = tptd->getTask();
			
			while(task)
			{
				task->process();									// ���������
				Task * task1 = tptd->tryGetTask();				// ���Լ��������������ȡ��һ����æ��δ���������

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
protected:
	THREAD_SINGNAL cond_;			// �߳��ź���
	THREAD_MUTEX mutex_;			// �̻߳�����
	int threadWaitSecond_;			// �߳̿���״̬��������������߳��˳��� С��0Ϊ�����߳� �뵥λ
	Task * currTask_;				// ���̵߳ĵ�ǰִ�е�����
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
	
	ThreadPool()
	{		
		extraNewAddThreadCount_ = currentThreadCount_ = currentFreeThreadCount_ = normalThreadCount_ = 0;
		THREAD_MUTEX_INIT(threadStateList_mutex_);	
		THREAD_MUTEX_INIT(busyTaskList_mutex_);
	}

	virtual ~ThreadPool()
	{
		THREAD_MUTEX_DELETE(threadStateList_mutex_);
		THREAD_MUTEX_DELETE(busyTaskList_mutex_);
		std::list<TPThread*>::iterator itr = allThreadList_.begin();
		for(; itr != allThreadList_.end(); itr++)
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
		return currentThreadCount_; 
	}
	
	/**��ȡ��ǰ�����߳�����*/		
	unsigned int getCurrentFreeThreadCount(void)const{ 
		return currentFreeThreadCount_; 
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
	bool addTask(Task* tptask);
	
	/**�߳������Ƿ񵽴�������*/
	bool isThreadCountMax(void)const{
		return currentThreadCount_ >= maxThreadCount_;	
	}
	
	/**
		�̳߳��Ƿ��ڷ�æ״̬
		δ���������Ƿ�ǳ���   ˵���̺ܷ߳�æ
	*/
	bool isBusy(void)const{
		return busyTaskList_.size() > 32;
	}	

	/**��ĳ�����񱣴浽δ�����б�*/
	void saveToBusyTaskList(Task* tptask);

	/**��δ�����б�ȡ��һ������ �����б���ɾ��*/
	Task* popBusyTaskList(void);
	
	/** �̳߳��Ƿ��Ѿ�����ʼ�� */
	bool isInitialize(void)const{ 
		return isInitialize_; 
	}
protected:
	bool isInitialize_;												// �̳߳��Ƿ񱻳�ʼ����
	std::queue<Task*> busyTaskList_;								// ϵͳ���ڷ�æʱ��δ����������б�
	THREAD_MUTEX busyTaskList_mutex_;								// ����busyTaskList_������
	THREAD_MUTEX threadStateList_mutex_;							// ����busyThreadList_ and freeThreadList_������
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
