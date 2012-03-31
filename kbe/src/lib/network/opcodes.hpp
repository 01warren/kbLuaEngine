/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
#ifndef __OPCODES_H__
#define __OPCODES_H__

// common include
#include "cstdkbe/cstdkbe.hpp"
// #define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <iostream>	
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif

namespace KBEngine{
	
/** ��������� */
typedef	uint16 OPCODE_TYPE;

/** �������־ */
#define OP_FLAG_UNKNOWN											0x00000001						// δ֪���߲�֧�ֵĲ�����
#define OP_FLAG_NORMAL											0x00000002						// ����ʹ�õĲ�����
#define OP_FLAG_URGENT											0x00000004						// �������� ��Ҫ������ִ�У�

/** ��������� */
enum Opcodes
{
	OP_UNKNOWN													= 0x000,						// δ֪���߲�֧�ֲ���
	OP_ENGINE_COMPONENT_HANDSHAKE								= 0x001,						// �������ֲ���
	OP_SCRIPT_EXEC_COMMAND										= 0x002,						// ִ�нű�����
	OP_BROADCAST_GLOBALDATA_CHANGE								= 0x003,						// �㲥globalData�ĸı�
	OP_BROADCAST_GLOBALBASES_CHANGE								= 0x004,						// �㲥globalBases�ĸı�
	OP_BROADCAST_CELLAPPDATA_CHANGE								= 0x005,						// �㲥cellAppData�ĸı�
	OP_CELLAPP_BROADCAST_MSG									= 0x006,						// cellapp��Ϣ�㲥
	OP_REQUEST_ID_RANGE_ALLOC									= 0x007,						// ����ID���ݶεķ���
	OP_INIT_APP													= 0x008,						// baeappmgr��ʼ��baseapp ���� cellapp
	OP_CREATE_IN_NEW_SPACE										= 0x009,						// һ��entity���󴴽���һ���µ�space��
	OP_ENTITY_CELL_CREATE_COMPLETE								= 0x010,						// һ��entity��cell����ʵ�崴�����
	OP_CELLAPP_OR_BASEAPP_STARTUP								= 0x011,						// һ���µ�cellapp����baseapp��������� �㲥������baseapp��cellapp
	OP_CREATE_BASE_ANY_WHERE									= 0x012,						// baseapp��createBaseAnywhere��ز���
	OP_CREATE_BASE_ANY_WHERE_CALLBACK							= 0x013,						// baseapp��createBaseAnywhere�Ļص�
	OP_CREATE_CELL_ENTITY										= 0x014,						// createCellEntity�Ĳ�����
	OP_CLIENT_LOGIN												= 0x015,						// �ʺŵ�½����
	OP_QUERY_ACCOUNT_PASSWORD									= 0x016,						// dbmgr��ѯ�������뷵�ظ�loginapp
	OP_UPDATE_COMPONENT_STATE									= 0x017,						// ĳ���������һ����������������״̬��Ϣ (���磺�������ϵ�����)
	OP_ALLOC_CLIENT_TO_BASEAPP									= 0x018,						// ����һ���ͻ��˸�baseapp
	OP_CREATE_CLIENT_ENTITY										= 0x019,						// ��ĳ���ͻ����ϴ���һ��entity
	OP_CREATE_CLIENT_PROXY										= 0x020,						// ��ĳ���ͻ����ϴ���һ��ProxyEntity
	OP_DESTROY_CLIENT_ENTITY									= 0x021,						// ��ĳ���ͻ���������һ��Entity
	OP_ENTITY_ENTER_WORLD										= 0x022,						// һ��entity��������Ϸ(cellʵ�����������)
	OP_ENTITY_LOSE_CELL											= 0x023,						// һ��entity��cell���ֶ�ʧ(����˵�Ǳ������� ֪ͨbase����)
	OP_ENTITY_DESTROY_CELL_ENTITY								= 0x024,						// ����һ��entity��cell
	OP_ENTITY_CLIENT_GET_CELL									= 0x025,						// ����baseentity,entity�Ŀͻ��˻����cell
	OP_ENTITY_MAIL												= 0x026,						// һ���ʼ����ݰ�, ������һ���ʼĵ�socketPacket ��Ҫ��mailbox����ʹ��
	OP_NTP														= 0x027,						// ��Ϸʱ��ͬ��Э��
	OP_DELTATIME_TEST											= 0x028,						// ������ʱ����
	OP_CLIENT_SET_DELTATIME										= 0x029,						// �ͻ��������Լ���������ʱ(�ͻ����Ѿ��ͷ�������������ز���)
	OP_TYPE_TOTAL_NUMBER										= 0x030							// ע��:���ʼ��ȷ�������һ��
};

/** �ʺŵ�½��ز����� */
#define ALOPC_SUCCESS											0x00000000						// �ʺŵ�½�ɹ�
#define ALOPC_ACCOUNT_OR_PASSWORD_ERROR							0x00000001						// �ʺŻ����������




}
#endif
