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
	#undef __BASEAPP_INTERFACE_H__
#endif


#ifndef __BASEAPP_INTERFACE_H__
#define __BASEAPP_INTERFACE_H__

// common include	
#if defined(BASEAPP)
#include "baseapp.hpp"
#endif
#include "baseapp_interface_macros.hpp"
#include "base_interface_macros.hpp"
#include "proxy_interface_macros.hpp"
#include "network/interface_defs.hpp"
//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

/**
	BASEAPP������Ϣ�ӿ��ڴ˶���
*/
NETWORK_INTERFACE_DECLARE_BEGIN(BaseappInterface)
	// ĳapp����������ߡ�
	BASEAPP_MESSAGE_DECLARE_ARGS0(reqClose,								MERCURY_FIXED_MESSAGE)

	// consoleԶ��ִ��python��䡣
	BASEAPP_MESSAGE_DECLARE_ARGS1(onExecScriptCommand,					MERCURY_VARIABLE_MESSAGE,
									std::string,						strcommand)

	// ĳappע���Լ��Ľӿڵ�ַ����app
	BASEAPP_MESSAGE_DECLARE_ARGS8(onRegisterNewApp,						MERCURY_VARIABLE_MESSAGE,
									int32,								uid, 
									std::string,						username,
									int8,								componentType, 
									uint64,								componentID, 
									uint32,								intaddr, 
									uint16,								intport,
									uint32,								extaddr, 
									uint16,								extport)

	// dbmgr��֪�Ѿ�����������baseapp����cellapp�ĵ�ַ
	// ��ǰapp��Ҫ������ȥ�����ǽ�������
	BASEAPP_MESSAGE_DECLARE_ARGS8(onGetEntityAppFromDbmgr,				MERCURY_VARIABLE_MESSAGE,
									int32,								uid, 
									std::string,						username,
									int8,								componentType, 
									uint64,								componentID, 
									uint32,								intaddr, 
									uint16,								intport,
									uint32,								extaddr, 
									uint16,								extport)

	// ĳapp�����ȡһ��entityID�εĻص�
	BASEAPP_MESSAGE_DECLARE_ARGS2(onReqAllocEntityID,					MERCURY_FIXED_MESSAGE,
									ENTITY_ID,							startID,
									ENTITY_ID,							endID)


	// ĳapp�����ȡһ��entityID�εĻص�
	BASEAPP_MESSAGE_DECLARE_ARGS4(onDbmgrInitCompleted,					MERCURY_FIXED_MESSAGE,
									ENTITY_ID,							startID,
									ENTITY_ID,							endID,
									int32,								startGlobalOrder,
									int32,								startGroupOrder)

	// global���ݸı�
	BASEAPP_MESSAGE_DECLARE_STREAM(onBroadcastGlobalDataChange,			MERCURY_VARIABLE_MESSAGE)
	BASEAPP_MESSAGE_DECLARE_STREAM(onBroadcastGlobalBasesChange,		MERCURY_VARIABLE_MESSAGE)

	// ĳ��app��app��֪���ڻ״̬��
	BASEAPP_MESSAGE_DECLARE_ARGS2(onAppActiveTick,						MERCURY_FIXED_MESSAGE,
									COMPONENT_TYPE,						componentType, 
									COMPONENT_ID,						componentID)

	// �յ�baseappmgr������ĳ��baseappҪ��createBaseAnywhere�������ڱ�baseapp��ִ�� 
	BASEAPP_MESSAGE_DECLARE_STREAM(onCreateBaseAnywhere,				MERCURY_VARIABLE_MESSAGE)

	// createBaseAnywhere�ɹ�֮��ص���Ϣ�������createBaseAnywhere��baseapp��entity��
	BASEAPP_MESSAGE_DECLARE_STREAM(onCreateBaseAnywhereCallback,		MERCURY_FIXED_MESSAGE)

	// createCellEntity��cellʵ�崴���ɹ��ص���
	BASEAPP_MESSAGE_DECLARE_ARGS2(onEntityGetCell,						MERCURY_FIXED_MESSAGE,
									ENTITY_ID,							id,
									COMPONENT_ID,						componentID)

	// loginapp���Լ�ע��һ����Ҫ��¼���˺�, ��baseappmgrת����
	BASEAPP_MESSAGE_DECLARE_ARGS3(registerPendingLogin,					MERCURY_VARIABLE_MESSAGE,
									std::string,						accountName,
									std::string,						password,
									ENTITY_ID,							entityID)

	// ǰ�������¼�������ϡ�
	BASEAPP_MESSAGE_DECLARE_ARGS2(loginGateway,							MERCURY_VARIABLE_MESSAGE,
									std::string,						accountName,
									std::string,						password)

	// ǰ���������µ�¼�������ϡ�
	BASEAPP_MESSAGE_DECLARE_ARGS2(reLoginGateway,						MERCURY_FIXED_MESSAGE,
									uint64,								key,
									ENTITY_ID,							entityID)

	// ��dbmgr��ȡ���˺�Entity��Ϣ
	BASEAPP_MESSAGE_DECLARE_ARGS3(onQueryAccountCBFromDbmgr,			MERCURY_VARIABLE_MESSAGE,
									std::string,						accountName,
									std::string,						password,
									std::string,						datas)

	// cellapp�ϵ�entity�Ѿ�������Ϸ�����ˡ�
	BASEAPP_MESSAGE_DECLARE_ARGS1(onEntityEnterWorldFromCellapp,		MERCURY_FIXED_MESSAGE,
									ENTITY_ID,							eid)

	// cellapp�ϵ�entity�Ѿ��뿪��Ϸ�����ˡ�
	BASEAPP_MESSAGE_DECLARE_ARGS1(onEntityLeaveWorldFromCellapp,		MERCURY_FIXED_MESSAGE,
									ENTITY_ID,							eid)

	// cellapp�ϵ�entity�Ѿ�����һ��space�ˡ�
	BASEAPP_MESSAGE_DECLARE_ARGS2(onEntityEnterSpaceFromCellapp,		MERCURY_FIXED_MESSAGE,
									ENTITY_ID,							eid,
									SPACE_ID,							spaceID)

	// cellapp�ϵ�entity�Ѿ��뿪һ��space�ˡ�
	BASEAPP_MESSAGE_DECLARE_ARGS2(onEntityLeaveSpaceFromCellapp,		MERCURY_FIXED_MESSAGE,
									ENTITY_ID,							eid,
									SPACE_ID,							spaceID)

	// entity�յ�һ��mail, ��ĳ��app�ϵ�mailbox����
	BASEAPP_MESSAGE_DECLARE_STREAM(onEntityMail,						MERCURY_VARIABLE_MESSAGE)
	
	// client����entity��cell����
	BASEAPP_MESSAGE_DECLARE_STREAM(onRemoteCellMethodCallFromClient,	MERCURY_VARIABLE_MESSAGE)

	//--------------------------------------------Base----------------------------------------------------------
	// Զ�̺���entity����
	BASE_MESSAGE_DECLARE_STREAM(onRemoteMethodCall,						MERCURY_VARIABLE_MESSAGE)

	// cellappͨ����entity��cell�������ٻ��߶�ʧ
	BASE_MESSAGE_DECLARE_STREAM(onLoseCell,								MERCURY_VARIABLE_MESSAGE)

	//--------------------------------------------Proxy---------------------------------------------------------
	/**
		Զ�̺���entity����
	*/
	PROXY_MESSAGE_DECLARE_ARGS0(onClientGetCell,						MERCURY_FIXED_MESSAGE)

NETWORK_INTERFACE_DECLARE_END()

#ifdef DEFINE_IN_INTERFACE
	#undef DEFINE_IN_INTERFACE
#endif

}
#endif
