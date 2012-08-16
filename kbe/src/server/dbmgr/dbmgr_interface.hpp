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


#if defined(DEFINE_IN_INTERFACE)
	#undef __DBMGR_INTERFACE_H__
#endif


#ifndef __DBMGR_INTERFACE_H__
#define __DBMGR_INTERFACE_H__

// common include	
#if defined(DBMGR)
#include "dbmgr.hpp"
#endif
#include "dbmgr_interface_macros.hpp"
#include "network/interface_defs.hpp"
//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

/**
	Dbmgr��Ϣ�꣬  ����Ϊ���� ��Ҫ�Լ��⿪
*/

/**
	DBMGR������Ϣ�ӿ��ڴ˶���
*/
NETWORK_INTERFACE_DECLARE_BEGIN(DbmgrInterface)
	// ĳappע���Լ��Ľӿڵ�ַ����app
	DBMGR_MESSAGE_DECLARE_ARGS8(onRegisterNewApp,					MERCURY_VARIABLE_MESSAGE,
									int32,							uid, 
									std::string,					username,
									int8,							componentType, 
									uint64,							componentID, 
									uint32,							intaddr, 
									uint16,							intport,
									uint32,							extaddr, 
									uint16,							extport)

	// ĳapp�����ȡһ��entityID�� 
	DBMGR_MESSAGE_DECLARE_ARGS2(onReqAllocEntityID,					MERCURY_FIXED_MESSAGE,
								int8,								componentType,
								COMPONENT_ID,						componentID)

	// global���ݸı�
	DBMGR_MESSAGE_DECLARE_STREAM(onBroadcastGlobalDataChange,		MERCURY_VARIABLE_MESSAGE)

	// ĳ��app��app��֪���ڻ״̬��
	DBMGR_MESSAGE_DECLARE_ARGS2(onAppActiveTick,					MERCURY_FIXED_MESSAGE,
									COMPONENT_TYPE,					componentType, 
									COMPONENT_ID,					componentID)

	// loginapp���󴴽��˺š�
	DBMGR_MESSAGE_DECLARE_ARGS2(reqCreateAccount,					MERCURY_VARIABLE_MESSAGE,
									std::string,					accountName,
									std::string,					password)

	// ĳ��app��app��֪���ڻ״̬��
	DBMGR_MESSAGE_DECLARE_ARGS2(onAccountLogin,						MERCURY_VARIABLE_MESSAGE,
									std::string,					accountName,
									std::string,					password)

	// baseapp��ѯ�˺���Ϣ��
	DBMGR_MESSAGE_DECLARE_ARGS2(queryAccount,						MERCURY_VARIABLE_MESSAGE,
									std::string,					accountName,
									std::string,					password)

	// baseapp���˺����ߡ�
	DBMGR_MESSAGE_DECLARE_ARGS3(onAccountOnline,					MERCURY_VARIABLE_MESSAGE,
									std::string,					accountName,
									COMPONENT_ID,					componentID,
									ENTITY_ID,						entityID)
		
	// baseapp���˺����ߡ�
	DBMGR_MESSAGE_DECLARE_ARGS1(onAccountOffline,					MERCURY_VARIABLE_MESSAGE,
									std::string,					accountName)
NETWORK_INTERFACE_DECLARE_END()

#ifdef DEFINE_IN_INTERFACE
	#undef DEFINE_IN_INTERFACE
#endif

}
#endif
