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
	#undef __CELLAPP_INTERFACE_H__
#endif


#ifndef __CELLAPP_INTERFACE_H__
#define __CELLAPP_INTERFACE_H__

// common include	
#if defined(CELLAPP)
#include "entity.hpp"
#include "cellapp.hpp"
#endif
#include "cellapp_interface_macros.hpp"
#include "entity_interface_macros.hpp"
#include "network/interface_defs.hpp"
//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

/**
	cellapp������Ϣ�ӿ��ڴ˶���
*/
NETWORK_INTERFACE_DECLARE_BEGIN(CellappInterface)
	// ĳappע���Լ��Ľӿڵ�ַ����app
	CELLAPP_MESSAGE_DECLARE_ARGS8(onRegisterNewApp,						MERCURY_VARIABLE_MESSAGE,
									int32,								uid, 
									std::string,						username,
									int8,								componentType, 
									uint64,								componentID, 
									uint32,								intaddr, 
									uint16,								intport,
									uint32,								extaddr, 
									uint16,								extport)

	// consoleԶ��ִ��python��䡣
	CELLAPP_MESSAGE_DECLARE_ARGS1(onExecScriptCommand,					MERCURY_VARIABLE_MESSAGE,
									std::string,						strcommand)

	// dbmgr��֪�Ѿ�����������baseapp����cellapp�ĵ�ַ
	// ��ǰapp��Ҫ������ȥ�����ǽ�������
	CELLAPP_MESSAGE_DECLARE_ARGS8(onGetEntityAppFromDbmgr,				MERCURY_VARIABLE_MESSAGE,
									int32,								uid, 
									std::string,						username,
									int8,								componentType, 
									uint64,								componentID, 
									uint32,								intaddr, 
									uint16,								intport,
									uint32,								extaddr, 
									uint16,								extport)

	// ĳapp�����ȡһ��entityID�εĻص�
	CELLAPP_MESSAGE_DECLARE_ARGS2(onReqAllocEntityID,					MERCURY_FIXED_MESSAGE,
									ENTITY_ID,							startID,
									ENTITY_ID,							endID)

	// ĳapp�����ȡһ��entityID�εĻص�
	CELLAPP_MESSAGE_DECLARE_ARGS4(onDbmgrInitCompleted,					MERCURY_FIXED_MESSAGE,
									ENTITY_ID,							startID,
									ENTITY_ID,							endID,
									int32,								startGlobalOrder,
									int32,								startGroupOrder)

	// global���ݸı�
	CELLAPP_MESSAGE_DECLARE_STREAM(onBroadcastGlobalDataChange,			MERCURY_VARIABLE_MESSAGE)
	CELLAPP_MESSAGE_DECLARE_STREAM(onBroadcastCellAppDataChange,		MERCURY_VARIABLE_MESSAGE)

	// baseEntity���󴴽���һ���µ�space�С�
	CELLAPP_MESSAGE_DECLARE_STREAM(onCreateInNewSpaceFromBaseapp,		MERCURY_VARIABLE_MESSAGE)

	// baseapp���������cellapp�ϴ���һ��entity��
	CELLAPP_MESSAGE_DECLARE_STREAM(onCreateCellEntityFromBaseapp,		MERCURY_VARIABLE_MESSAGE)

	// ����ĳ��cellEntity��
	CELLAPP_MESSAGE_DECLARE_ARGS1(onDestroyCellEntityFromBaseapp,		MERCURY_FIXED_MESSAGE,
									ENTITY_ID,							eid)

	// ĳ��app��app��֪���ڻ״̬��
	CELLAPP_MESSAGE_DECLARE_ARGS2(onAppActiveTick,						MERCURY_FIXED_MESSAGE,
									COMPONENT_TYPE,						componentType, 
									COMPONENT_ID,						componentID)

	// entity�յ�һ��mail, ��ĳ��app�ϵ�mailbox����
	CELLAPP_MESSAGE_DECLARE_STREAM(onEntityMail,						MERCURY_VARIABLE_MESSAGE)

	// client����entity��cell����
	CELLAPP_MESSAGE_DECLARE_STREAM(onRemoteCallMethodFromClient,		MERCURY_VARIABLE_MESSAGE)

	// executeRawDatabaseCommand��dbmgr�Ļص�
	CELLAPP_MESSAGE_DECLARE_STREAM(onExecuteRawDatabaseCommandCB,		MERCURY_VARIABLE_MESSAGE)

	// base�����ȡcelldata
	CELLAPP_MESSAGE_DECLARE_STREAM(reqUpdateEntityCellData,				MERCURY_VARIABLE_MESSAGE)

	//--------------------------------------------Entity----------------------------------------------------------
	//Զ�̺���entity����
	ENTITY_MESSAGE_DECLARE_STREAM(onRemoteMethodCall,					MERCURY_VARIABLE_MESSAGE)


	
NETWORK_INTERFACE_DECLARE_END()

#ifdef DEFINE_IN_INTERFACE
	#undef DEFINE_IN_INTERFACE
#endif

}
#endif
