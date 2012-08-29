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
	#undef __CLIENT_INTERFACE_H__
#endif


#ifndef __CLIENT_INTERFACE_H__
#define __CLIENT_INTERFACE_H__

// common include	
#if defined(CLIENT)
#include "clientapp.hpp"
#endif
#include "client_interface_macros.hpp"
#include "network/interface_defs.hpp"
#include "server/mercury_errors.hpp"
#include "common.hpp"

//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

/**
	CLIENT������Ϣ�ӿ��ڴ˶���
*/
NETWORK_INTERFACE_DECLARE_BEGIN(ClientInterface)
	// �����˺�ʧ�ܡ�
	CLIENT_MESSAGE_DECLARE_ARGS1(onCreateAccountResult,			MERCURY_FIXED_MESSAGE,
									MERCURY_ERROR_CODE,			failedcode)

	// ��¼�ɹ���
	CLIENT_MESSAGE_DECLARE_STREAM(onLoginSuccessfully,			MERCURY_VARIABLE_MESSAGE)

	// ��¼ʧ�ܡ�
	CLIENT_MESSAGE_DECLARE_ARGS1(onLoginFailed,					MERCURY_FIXED_MESSAGE,
									MERCURY_ERROR_CODE,			failedcode)

	// ���������Ѿ�������һ����ͻ��˹����Ĵ���Entity || ��¼���سɹ���
	CLIENT_MESSAGE_DECLARE_ARGS3(onCreatedProxies,				MERCURY_VARIABLE_MESSAGE,
									uint64,						rndUUID,
									ENTITY_ID,					eid,
									std::string,				entityType)

	// ��¼����ʧ�ܡ�
	CLIENT_MESSAGE_DECLARE_ARGS1(onLoginGatewayFailed,			MERCURY_FIXED_MESSAGE,
									MERCURY_ERROR_CODE,			failedcode)

	// �������ϵ�entity�Ѿ�����һ��cell���֡�
	CLIENT_MESSAGE_DECLARE_ARGS1(onEntityGetCell,				MERCURY_FIXED_MESSAGE,
									ENTITY_ID,					eid)

	// �������ϵ�entity�Ѿ�������Ϸ�����ˡ�
	CLIENT_MESSAGE_DECLARE_ARGS2(onEntityEnterWorld,			MERCURY_FIXED_MESSAGE,
									ENTITY_ID,					eid,
									SPACE_ID,					spaceID)

	// �������ϵ�entity�Ѿ��뿪��Ϸ�����ˡ�
	CLIENT_MESSAGE_DECLARE_ARGS2(onEntityLeaveWorld,			MERCURY_FIXED_MESSAGE,
									ENTITY_ID,					eid,
									SPACE_ID,					spaceID)

	// ���߿ͻ���ĳ��entity�����ˣ� ����entityͨ���ǻ�δonEntityEnterWorld��
	CLIENT_MESSAGE_DECLARE_ARGS1(onEntityDestroyed,				MERCURY_FIXED_MESSAGE,
									ENTITY_ID,					eid)

	// �������ϵ�entity�Ѿ�����space�ˡ�
	CLIENT_MESSAGE_DECLARE_ARGS2(onEntityEnterSpace,			MERCURY_FIXED_MESSAGE,
									ENTITY_ID,					eid,
									SPACE_ID,					spaceID)

	// �������ϵ�entity�Ѿ��뿪space�ˡ�
	CLIENT_MESSAGE_DECLARE_ARGS2(onEntityLeaveSpace,			MERCURY_FIXED_MESSAGE,
									ENTITY_ID,					eid,
									SPACE_ID,					spaceID)

	// Զ�̺���entity����
	CLIENT_MESSAGE_DECLARE_STREAM(onRemoteMethodCall,			MERCURY_VARIABLE_MESSAGE)

	// ����������entity����
	CLIENT_MESSAGE_DECLARE_STREAM(onUpdatePropertys,			MERCURY_VARIABLE_MESSAGE)

NETWORK_INTERFACE_DECLARE_END()

#ifdef DEFINE_IN_INTERFACE
	#undef DEFINE_IN_INTERFACE
#endif

}
#endif
