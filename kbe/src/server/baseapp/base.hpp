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



#ifndef __BASE_H__
#define __BASE_H__
	
// common include	
#include "cstdkbe/cstdkbe.hpp"
#include "helper/debug_helper.hpp"
//#include "entityMailbox.hpp"
#include "pyscript/math.hpp"
#include "pyscript/scriptobject.hpp"
#include "entitydef/datatypes.hpp"	
#include "entitydef/entitydef.hpp"	
#include "entitydef/entity_macro.hpp"	
#include "server/script_timers.hpp"		

//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#include <errno.h>
#endif
	
namespace KBEngine{
class EntityMailbox;

namespace Mercury
{
class Channel;
}


class Base : public script::ScriptObject
{
	/** ���໯ ��һЩpy�������������� */
	BASE_SCRIPT_HREADER(Base, ScriptObject)	
	ENTITY_HEADER(Base)
public:
	Base(ENTITY_ID id, ScriptModule* scriptModule, PyTypeObject* pyType = getScriptType(), bool isInitialised = true);
	~Base();

	/** 
		�����������ݱ��ı��� 
	*/
	void onDefDataChanged(const PropertyDescription* propertyDescription, 
			PyObject* pyData);

	/** ����cell���ֵ�ʵ�� */
	bool destroyCellEntity(void);
	DECLARE_PY_MOTHOD_ARG0(pyDestroyCellEntity);
	
	/** �ű���������baseʵ�� */
	void destroyBase(void);
	DECLARE_PY_MOTHOD_ARG0(pyDestroyBase);
	
	/** �ű���ȡmailbox */
	DECLARE_PY_GET_MOTHOD(pyGetCellMailbox);
	EntityMailbox* getCellMailbox(void)const;
	void setCellMailbox(EntityMailbox* mailbox);
	
	/** �ű���ȡmailbox */
	DECLARE_PY_GET_MOTHOD(pyGetClientMailbox);
	EntityMailbox* getClientMailbox()const;
	void setClientMailbox(EntityMailbox* mailbox);

	/** cellData���� */
	bool installCellDataAttr(PyObject* dictData = NULL);
	void createCellData(void);
	void destroyCellData(void);
	void getCellDataByFlags(uint32 flags, MemoryStream* s);
	PyObject* createCellDataDictByFlags(uint32 flags);
	INLINE PyObject* getCellData(void)const;
	
	/** ����cellʧ�ܻص� */
	void onCreateCellFailure(void);

	/** ����cell�ɹ��ص� */
	void onGetCell(Mercury::Channel* pChannel, COMPONENT_ID componentID);

	/** ��ʧcell�˵�֪ͨ */
	void onLoseCell(PyObject* cellData);

	/** �ͻ��˻����cell */
	void onClientGetCell();

	/** �ͻ��˶�ʧ */
	void onClientDeath();

	/** ��Ҫ���浽���ݿ�֮ǰ��֪ͨ */
	void onWriteToDB(PyObject* cellData);

	/** ����ӿ�
		Զ�̺��б�entity�ķ��� 
	*/
	void onRemoteMethodCall(Mercury::Channel* pChannel, MemoryStream& s);

	/** �������entity */
	void destroy();

	/** Ϊһ��baseEntity���ƶ���cell�ϴ���һ��cellEntity */
	DECLARE_PY_MOTHOD_ARG1(createCellEntity, PyObject_ptr);

	/** ����һ��cellEntity��һ���µ�space�� */
	DECLARE_PY_MOTHOD_ARG1(createInNewSpace, PyObject_ptr);

protected:
	EntityMailbox*							clientMailbox_;			// ���entity�Ŀͻ���mailbox
	EntityMailbox*							cellMailbox_;			// ���entity��cellapp mailbox
	PyObject*								cellDataDict_;			// entity��������cell����δ����ʱ����һЩcell�������ݱ���������
};

}


#ifdef CODE_INLINE
#include "base.ipp"
#endif

#endif
