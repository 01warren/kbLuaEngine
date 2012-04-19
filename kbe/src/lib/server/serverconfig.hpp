/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
/*
		ServerConfig::getSingleton().loadConfig("../../res/server/KBEngine.xml");
		ENGINE_COMPONENT_INFO& ecinfo = ServerConfig::getSingleton().getCellApp();													
*/
#ifndef __SERVER_CONFIG_H__
#define __SERVER_CONFIG_H__
#define __LIB_DLLAPI__	
// common include
#include "cstdkbe/cstdkbe.hpp"
#if KBE_PLATFORM == PLATFORM_WIN32
#pragma warning (disable : 4996)
#endif
//#define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>	
#include <stdarg.h> 
#include "cstdkbe/singleton.hpp"
#include "thread/threadmutex.hpp"
#include "thread/threadguard.hpp"
#include "xmlplus/xmlplus.hpp"	
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#include <errno.h>
#endif
	
namespace KBEngine{

// ���������Ϣ�ṹ��
typedef struct EngineComponentInfo{
	uint32 port;											// ����ļ����˿�
	char ip[50];											// �����ip��ַ
	char entryScriptFile[255];								// �������ڽű��ļ�
	char dbAccountEntityScriptType[255];					// ���ݿ��ʺŽű����
	float defaultAoIRadius;									// ������cellapp�ڵ��е�player��aoi�뾶��С
	float defaultAoIHysteresisArea;							// ������cellapp�ڵ��е�player��aoi���ͺ�Χ
}ENGINE_COMPONENT_INFO;

class ServerConfig : public Singleton<ServerConfig>
{
public:
	ServerConfig();
	~ServerConfig();
	
	bool loadConfig(std::string fileName);
	
	ENGINE_COMPONENT_INFO& getCellApp(void);
	ENGINE_COMPONENT_INFO& getBaseApp(void);
	ENGINE_COMPONENT_INFO& getDBMgr(void);
	ENGINE_COMPONENT_INFO& getLoginApp(void);
	ENGINE_COMPONENT_INFO& getCellAppMgr(void);
	ENGINE_COMPONENT_INFO& getBaseAppMgr(void);
	ENGINE_COMPONENT_INFO& getKBMachine(void);
	ENGINE_COMPONENT_INFO& getKBCenter(void);

	inline ENGINE_COMPONENT_INFO& getComponent(COMPONENT_TYPE ComponentType);
 	
 	void printCellapp(void);
 	
	inline int16 gameUpdateHertz(void)const { return gameUpdateHertz_;}
private:
	ENGINE_COMPONENT_INFO _cellAppInfo;
	ENGINE_COMPONENT_INFO _baseAppInfo;
	ENGINE_COMPONENT_INFO _dbmgrInfo;
	ENGINE_COMPONENT_INFO _loginAppInfo;
	ENGINE_COMPONENT_INFO _cellAppMgrInfo;
	ENGINE_COMPONENT_INFO _baseAppMgrInfo;
	ENGINE_COMPONENT_INFO _kbMachineInfo;
	ENGINE_COMPONENT_INFO _kbCenterInfo;
public:
	KBEngine::thread::ThreadMutex myMutex_;
	int16 gameUpdateHertz_;
};

#define g_kbeSrvConfig ServerConfig::getSingleton()
}
#endif
