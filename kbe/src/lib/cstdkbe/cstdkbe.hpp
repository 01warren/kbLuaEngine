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
#ifndef __CSTDKBE__
#define __CSTDKBE__
#include "cstdkbe/platform.hpp"
#include "cstdkbe/singleton.hpp"
#include "cstdkbe/kbeversion.hpp"
#include "cstdkbe/kbemalloc.hpp"

namespace KBEngine{
/** ��ȫ���ͷ�һ��ָ���ڴ� */
#define SAFE_RELEASE(i)										\
	if (i)													\
		{													\
			delete i;										\
			i = NULL;										\
		}

/** ��ȫ���ͷ�һ��ָ�������ڴ� */
#define SAFE_RELEASE_ARRAY(i)								\
	if (i)													\
		{													\
			delete[] i;										\
			i = NULL;										\
		}

#ifdef CODE_INLINE
    #define INLINE    inline
#else
    #define INLINE
#endif

/** entity��mailbox��� */
enum ENTITY_MAILBOX_TYPE
{
	MAILBOX_TYPE_CELL												= 0,
	MAILBOX_TYPE_BASE												= 1,
	MAILBOX_TYPE_CLIENT												= 2,
	MAILBOX_TYPE_CELL_VIA_BASE										= 3,
	MAILBOX_TYPE_BASE_VIA_CELL										= 4,
	MAILBOX_TYPE_CLIENT_VIA_CELL									= 5,
	MAILBOX_TYPE_CLIENT_VIA_BASE									= 6,
};

/** mailbox�����Ի�Ϊ�ַ������� �ϸ��ENTITY_MAILBOX_TYPE����ƥ�� */
const char ENTITY_MAILBOX_TYPE_TO_NAME_TABLE[][8] = 
{
	"cell",
	"base",
	"client",
	"cell",
	"base",
	"client",
	"client",
};

/** ����������������� */
enum COMPONENT_TYPE
{
	UNKNOWN_COMPONENT_TYPE	= 0,
	DBMGR_TYPE				= 1,
	LOGINAPP_TYPE			= 2,
	BASEAPPMGR_TYPE			= 3,
	CELLAPPMGR_TYPE			= 4,
	CELLAPP_TYPE			= 5,
	BASEAPP_TYPE			= 6,
	CLIENT_TYPE				= 7,
	MACHINE_TYPE			= 8,
	CENTER_TYPE				= 9,
	CONSOLE_TYPE			= 10,
	MESSAGELOG_TYPE			= 11,
	RESOURCEMGR_TYPE		= 12
};

/** ��ǰ�������������ID */
extern COMPONENT_TYPE g_componentType;
extern COMPONENT_ID g_componentID;

/** ������������������ */
const char COMPONENT_NAME[][255] = {
	"unknown",
	"dbmgr",
	"loginapp",
	"baseappmgr",
	"cellappmgr",
	"cellapp",
	"baseapp",
	"client",
	"kbmachine",
	"kbcenter",
	"console",
	"messagelog",
	"resourcemgr",
};

// ���е�����б�
const COMPONENT_TYPE ALL_COMPONENT_TYPES[] = {BASEAPPMGR_TYPE, CELLAPPMGR_TYPE, DBMGR_TYPE, CELLAPP_TYPE, 
						BASEAPP_TYPE, LOGINAPP_TYPE, MACHINE_TYPE, UNKNOWN_COMPONENT_TYPE};

// ǰ��Ӧ�õ����
enum COMPONENT_CLIENT_TYPE
{
	UNKNOWN_CLIENT_COMPONENT_TYPE	= 0,
	CLIENT_TYPE_PHONE				= 1,	// �ֻ���
	CLIENT_TYPE_PC					= 2,	// pc�� һ�㶼��exe�ͻ���
	CLIENT_TYPE_WEB					= 3		// webӦ�ã� html5��flash
};

/** ����ǰ��Ӧ�õ�������� */
const char COMPONENT_CLIENT_NAME[][255] = {
	"UNKNOWN_CLIENT_COMPONENT_TYPE",
	"CLIENT_TYPE_PHONE",
	"CLIENT_TYPE_PC",
	"CLIENT_TYPE_WEB",
};

// ����ǰ��Ӧ�õ����
const COMPONENT_CLIENT_TYPE ALL_CLIENT_TYPES[] = {CLIENT_TYPE_PHONE, CLIENT_TYPE_PC, CLIENT_TYPE_WEB, UNKNOWN_CLIENT_COMPONENT_TYPE};

// ǰ���Ƿ�֧�ָ�����
#define CLIENT_NO_FLOAT

/** һ��cell�߽��ܹ�������һ��cell�߽�����Χ */
#define CELL_BORDER_WIDTH					500
	
/** һ���ռ��һ��chunk��С */
#define SPACE_CHUNK_SIZE					100

}
#endif // __CSTDKBE__
