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
	#undef __LOGINAPP_INTERFACE_H__
#endif


#ifndef __LOGINAPP_INTERFACE_H__
#define __LOGINAPP_INTERFACE_H__

// common include	
#if defined(LOGINAPP)
#include "loginapp.hpp"
#endif
#include "loginapp_interface_macros.hpp"
#include "network/interface_defs.hpp"
#include "server/mercury_errors.hpp"
//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

/**
	LOGINAPP������Ϣ�ӿ��ڴ˶���
*/
NETWORK_INTERFACE_DECLARE_BEGIN(LoginappInterface)
	// �û���¼������ 
	LOGINAPP_MESSAGE_DECLARE_STREAM(login,											MERCURY_VARIABLE_MESSAGE)

	// ĳapp�����ȡһ��entityID�εĻص�
	LOGINAPP_MESSAGE_DECLARE_ARGS2(onDbmgrInitCompleted,							MERCURY_FIXED_MESSAGE,
									int32,											startGlobalOrder,
									int32,											startGroupOrder)

	// ĳ��app��app��֪���ڻ״̬��
	LOGINAPP_MESSAGE_DECLARE_ARGS2(onAppActiveTick,									MERCURY_FIXED_MESSAGE,
									COMPONENT_TYPE,									componentType, 
									COMPONENT_ID,									componentID)

	// ��dbmgr��ѯ���û��Ϸ��Խ��
	LOGINAPP_MESSAGE_DECLARE_STREAM(onLoginAccountQueryResultFromDbmgr,				MERCURY_VARIABLE_MESSAGE)

	// baseappmgr���صĵ�¼���ص�ַ
	LOGINAPP_MESSAGE_DECLARE_ARGS3(onLoginAccountQueryBaseappAddrFromBaseappmgr,	MERCURY_VARIABLE_MESSAGE,
									std::string,									accountName,
									uint32,											addr,
									uint16,											port)

	// ��dbmgr���󴴽��˺ŷ��ؽ��
	LOGINAPP_MESSAGE_DECLARE_ARGS3(onReqCreateAccountResult,						MERCURY_VARIABLE_MESSAGE,
									MERCURY_ERROR_CODE,								failedcode,
									std::string,									accountName,
									std::string,									password)

NETWORK_INTERFACE_DECLARE_END()

#ifdef DEFINE_IN_INTERFACE
	#undef DEFINE_IN_INTERFACE
#endif

}
#endif
