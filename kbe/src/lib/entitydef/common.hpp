/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
#ifndef __KBENGINE_DEF_COMMON_H__
#define __KBENGINE_DEF_COMMON_H__
#pragma warning (disable : 4910)
#pragma warning (disable : 4251)

// common include	
#include "cstdkbe/cstdkbe.hpp"
//#define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>	
#include <map>	
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#include <errno.h>
#endif
	
#define LIB_DLLAPI  __declspec(dllexport)

#ifdef __cplusplus  
extern "C" {  
#endif  

#ifdef __cplusplus  
}
#endif 

namespace KBEngine{

/** entity�����ݴ������Ա�� */
enum EntityDataFlags
{
	ED_FLAG_CELL_PUBLIC												= 0x00000000, // �������cell�㲥
	ED_FLAG_CELL_PRIVATE											= 0x00000001, // ��ǰcell
	ED_FLAG_ALL_CLIENTS												= 0x00000002, // cell�㲥�����пͻ���
	ED_FLAG_CELL_PUBLIC_AND_OWN										= 0x00000004, // cell�㲥���Լ��Ŀͻ���
	ED_FLAG_OWN_CLIENT												= 0x00000008, // ��ǰcell�Ϳͻ���
	ED_FLAG_BASE_AND_CLIENT											= 0x00000010, // base�Ϳͻ���
	ED_FLAG_BASE													= 0x00000020, // ��ǰbase
	ED_FLAG_OTHER_CLIENTS											= 0x00000040, // cell�㲥�������ͻ���
};

/** �൱�ڶ�entity���ݴ�������һ������Ķ��� */
enum EntityDataFlagRelation
{
	// ������baseapp�й�ϵ�ı�־
	ENTITY_BASE_DATA_FLAGS											= ED_FLAG_BASE | ED_FLAG_BASE_AND_CLIENT,
	// ������cellapp�й�ϵ�ı�־
	ENTITY_CELL_DATA_FLAGS											= ED_FLAG_CELL_PUBLIC | ED_FLAG_CELL_PRIVATE | ED_FLAG_ALL_CLIENTS | ED_FLAG_CELL_PUBLIC_AND_OWN | ED_FLAG_OTHER_CLIENTS | ED_FLAG_OWN_CLIENT,
	// ������client�й�ϵ�ı�־
	ENTITY_CLIENT_DATA_FLAGS										= ED_FLAG_BASE_AND_CLIENT | ED_FLAG_ALL_CLIENTS | ED_FLAG_CELL_PUBLIC_AND_OWN | ED_FLAG_OTHER_CLIENTS | ED_FLAG_OWN_CLIENT,
	// ������Ҫ�㲥������cellapp�ı�־
	ENTITY_BROADCAST_CELL_FLAGS										= ED_FLAG_CELL_PUBLIC | ED_FLAG_ALL_CLIENTS | ED_FLAG_CELL_PUBLIC_AND_OWN | ED_FLAG_OTHER_CLIENTS,
	// ������Ҫ�㲥�������ͻ���(�������Լ���)�ı�־
	ENTITY_BROADCAST_OTHER_CLIENT_FLAGS								= ED_FLAG_OTHER_CLIENTS | ED_FLAG_ALL_CLIENTS,
	// ������Ҫ�㲥���Լ��Ŀͻ��˵ı�־
	ENTITY_BROADCAST_OWN_CLIENT_FLAGS								= ED_FLAG_ALL_CLIENTS | ED_FLAG_CELL_PUBLIC_AND_OWN | ED_FLAG_OWN_CLIENT | ED_FLAG_BASE_AND_CLIENT,
};

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

/** mailbox�������Ӧ��������ӳ�䣬  �������������ϸ�ƥ��ENTITY_MAILBOX_TYPE��ֵ */
const COMPONENT_TYPE ENTITY_MAILBOX_COMPONENT_TYPE_MAPPING[] = 
{
	CELLAPP_TYPE,
	BASEAPP_TYPE,
	CLIENT_TYPE,
	BASEAPP_TYPE,
	CELLAPP_TYPE,
	CELLAPP_TYPE,
	BASEAPP_TYPE,
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

/** mailbox ��Ͷ�ݵ�mail��� */
#define MAIL_TYPE_REMOTE_CALL										0x00000001	// Զ�̺���һ������
#define MAIL_TYPE_LOST_VIEW_ENTITY									0x00000002	// ��ǰ���entity��Ұ��Χ�ڵ�һ��entity��ʧ��
#define MAIL_TYPE_ENTITY_MOVE_TO_POINT								0x00000003	// ��ǰ���entity��Ұ��Χ�ڵ�һ��entity�ƶ���ĳ����
#define MAIL_TYPE_ENTER_SPACE										0x00000004	// entity����һ���µ�space
#define MAIL_TYPE_LEAVE_SPACE										0x00000005	// entity��Ҫ�뿪��ǰspace
#define MAIL_TYPE_UPDATE_PROPERTYS									0x00000006	// ����ĳ��entity��һЩ���Ե��ͻ���
#define MAIL_TYPE_UPDATE_PROPERTY									0x00000007	// ����ĳ��entity��ĳ�����Ե��ͻ���
#define MAIL_TYPE_SEEK												0x00000008	// ĳ��player�Ŀͻ��������ƶ�player


typedef std::map<std::string, EntityDataFlags> ENTITYFLAGMAP;
extern ENTITYFLAGMAP g_entityFlagMapping;										// entity ��flag�ַ���ӳ���
}
#endif
