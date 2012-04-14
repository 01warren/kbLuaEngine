/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
#ifndef __CSTDKBE__
#define __CSTDKBE__
#include "cstdkbe/platform.hpp"
#include "cstdkbe/singleton.hpp"
#include "cstdkbe/kbeversion.hpp"
	
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

/** ����������������� */
enum COMPONENT_TYPE
{
	UNKNOWN_COMPONENT_TYPE	= 0,
	DBMGR_TYPE		= 1,
	LOGINAPP_TYPE	= 2,
	BASEAPPMGR_TYPE	= 3,
	CELLAPPMGR_TYPE	= 4,
	CELLAPP_TYPE	= 5,
	BASEAPP_TYPE	= 6,
	CLIENT_TYPE		= 7,
	MACHINE_TYPE	= 8,
	CENTER_TYPE		= 9
};

/** ��ǰ������������ */
extern COMPONENT_TYPE g_componentType;
	
/** ������������������ */
const char COMPONENT_NAME[][12] = {
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
};
/** һ��cell�߽��ܹ�������һ��cell�߽�����Χ */
#define CELL_BORDER_WIDTH 500
	
/** һ���ռ��һ��chunk��С */
#define SPACE_CHUNK_SIZE 100

/** ���Ե�lod�㲥����Χ�Ķ��� */
#define DETAIL_LEVEL_NEAR													0	// lod���𣺽�						
#define DETAIL_LEVEL_MEDIUM													1	// lod������
#define DETAIL_LEVEL_FAR													2	// lod����Զ	
#define DETAIL_LEVEL_UNKNOW													3	// lod���𣺷ǳ���Զ (ͨ������������ڵ�entity��������㲥�κ����ԣ�������λ�÷���ȣ�)	
}
#endif // __CSTDKBE__