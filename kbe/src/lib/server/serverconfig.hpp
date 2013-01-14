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
namespace Mercury
{
class Address;
}

struct Profiles_Config
{
	Profiles_Config():
		open_pyprofile(false),
		open_cprofile(false),
		open_eventprofile(false),
		open_mercuryprofile(false)
	{
	}

	bool open_pyprofile;
	bool open_cprofile;
	bool open_eventprofile;
	bool open_mercuryprofile;
};

struct ChannelCommon
{
	float channelInternalTimeout;
	float channelExternalTimeout;
	uint32 extReadBufferSize;
	uint32 extWriteBufferSize;
	uint32 intReadBufferSize;
	uint32 intWriteBufferSize;
};

// ���������Ϣ�ṹ��
typedef struct EngineComponentInfo
{
	uint32 port;											// ��������к�����Ķ˿�
	char ip[MAX_BUF];										// �����������ip��ַ

	char entryScriptFile[MAX_NAME];							// �������ڽű��ļ�
	char dbAccountEntityScriptType[MAX_NAME];				// ���ݿ��ʺŽű����
	float defaultAoIRadius;									// ������cellapp�ڵ��е�player��aoi�뾶��С
	float defaultAoIHysteresisArea;							// ������cellapp�ڵ��е�player��aoi���ͺ�Χ
	const Mercury::Address* externalAddr;					// �ⲿ��ַ
	const Mercury::Address* internalAddr;					// �ڲ���ַ
	COMPONENT_ID componentID;

	char internalInterface[MAX_NAME];						// �ڲ������ӿ�����
	char externalInterface[MAX_NAME];						// �ⲿ�����ӿ�����
	int32 externalPorts_min;								// ����socket�˿�ʹ��ָ����Χ
	int32 externalPorts_max;

	char db_type[MAX_BUF];									// ���ݿ�����
	uint32 db_port;											// ���ݿ�Ķ˿�
	char db_ip[MAX_BUF];									// ���ݿ��ip��ַ
	char db_username[MAX_BUF];								// ���ݿ���û���
	char db_password[MAX_BUF];								// ���ݿ������
	char db_name[MAX_BUF];									// ���ݿ���
	uint16 db_numConnections;								// ���ݿ��������
	std::string db_unicodeString_characterSet;				// �������ݿ��ַ���
	std::string db_unicodeString_collation;

	float archivePeriod;									// entity�洢���ݿ�����
	float backupPeriod;										// entity��������
	bool backUpUndefinedProperties;							// entity�Ƿ񱸷�δ��������

	float loadSmoothingBias;								// baseapp������ƽ�����ֵ�� 
	uint32 login_port;										// ��������¼�˿� Ŀǰbots����
	char login_ip[MAX_BUF];									// ��������¼ip��ַ

	ENTITY_ID criticallyLowSize;							// idʣ����ô���ʱ��dbmgr�����µ�id��Դ

	uint32 downloadBitsPerSecondTotal;						// ���пͻ���ÿ�����ش���������
	uint32 downloadBitsPerSecondPerClient;					// ÿ���ͻ���ÿ������ش���

	Profiles_Config profiles;
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
	ENGINE_COMPONENT_INFO& getBots(void);
	ENGINE_COMPONENT_INFO& getResourcemgr(void);
	ENGINE_COMPONENT_INFO& getMessagelog(void);

	inline ENGINE_COMPONENT_INFO& getComponent(COMPONENT_TYPE ComponentType);
 	
 	void updateInfos(bool isPrint, COMPONENT_TYPE componentType, COMPONENT_ID componentID, 
 				const Mercury::Address& internalAddr, const Mercury::Address& externalAddr);
 	
	inline int16 gameUpdateHertz(void)const { return gameUpdateHertz_;}

	inline Mercury::Address billingSystemAddr(void)const { return billingSystemAddr_;}
	
	inline const char* billingSystemType()const { return billingSystem_type_.c_str(); }

	inline const char* billingSystemThirdpartyAccountServiceAddr()const { return billingSystem_thirdpartyAccountServiceAddr_.c_str(); }
	inline uint16 billingSystemThirdpartyAccountServicePort()const { return billingSystem_thirdpartyAccountServicePort_; }

	inline const char* billingSystemThirdpartyChargeServiceAddr()const { return billingSystem_thirdpartyChargeServiceAddr_.c_str(); }
	inline uint16 billingSystemThirdpartyChargeServicePort()const { return billingSystem_thirdpartyChargeServicePort_; }

	inline uint16 billingSystemThirdpartyServiceCBPort()const { return billingSystem_thirdpartyServiceCBPort_; }

	const ChannelCommon& channelCommon(){ return channelCommon_; }
private:
	ENGINE_COMPONENT_INFO _cellAppInfo;
	ENGINE_COMPONENT_INFO _baseAppInfo;
	ENGINE_COMPONENT_INFO _dbmgrInfo;
	ENGINE_COMPONENT_INFO _loginAppInfo;
	ENGINE_COMPONENT_INFO _cellAppMgrInfo;
	ENGINE_COMPONENT_INFO _baseAppMgrInfo;
	ENGINE_COMPONENT_INFO _kbMachineInfo;
	ENGINE_COMPONENT_INFO _kbCenterInfo;
	ENGINE_COMPONENT_INFO _botsInfo;
	ENGINE_COMPONENT_INFO _resourcemgrInfo;
	ENGINE_COMPONENT_INFO _messagelogInfo;
public:
	int16 gameUpdateHertz_;

	ChannelCommon channelCommon_;

	// ÿ���ͻ���ÿ��ռ�õ�������
	uint32 bitsPerSecondToClient_;		

	Mercury::Address billingSystemAddr_;
	std::string billingSystem_type_;								// �Ʒ�ϵͳ���
	std::string billingSystem_thirdpartyAccountServiceAddr_;		// ��������Ӫ�˺ŷ����ַ(��type��thirdpartyʱ��Ч)
	uint16	billingSystem_thirdpartyAccountServicePort_;			
	std::string billingSystem_thirdpartyChargeServiceAddr_;			// ��������Ӫ��ֵ�����ַ(��type��thirdpartyʱ��Ч)
	uint16	billingSystem_thirdpartyChargeServicePort_;	
	uint16	billingSystem_thirdpartyServiceCBPort_;	
};

#define g_kbeSrvConfig ServerConfig::getSingleton()
}
#endif
