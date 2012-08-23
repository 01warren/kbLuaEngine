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
	Base(ENTITY_ID id, ScriptModule* scriptModule, 
		PyTypeObject* pyType = getScriptType(), bool isInitialised = true);
	~Base();

	/** 
		�Ƿ�洢���ݿ� 
	*/
	INLINE bool hasDB()const ;
	INLINE void hasDB(bool has) ;

	/** 
		�����������ݱ��ı��� 
	*/
	void onDefDataChanged(const PropertyDescription* propertyDescription, 
			PyObject* pyData);

	/** 
		����cell���ֵ�ʵ�� 
	*/
	bool destroyCellEntity(void);

	DECLARE_PY_MOTHOD_ARG0(pyDestroyCellEntity);
	
	/** 
		�ű���������baseʵ�� 
	*/
	void destroyBase(void);

	DECLARE_PY_MOTHOD_ARG0(pyDestroyBase);
	
	/** 
		�ű���ȡmailbox 
	*/
	DECLARE_PY_GET_MOTHOD(pyGetCellMailbox);

	EntityMailbox* getCellMailbox(void)const;

	void setCellMailbox(EntityMailbox* mailbox);
	
	/** 
		�ű���ȡmailbox 
	*/
	DECLARE_PY_GET_MOTHOD(pyGetClientMailbox);

	EntityMailbox* getClientMailbox()const;

	void setClientMailbox(EntityMailbox* mailbox);

	/** 
		cellData���� 
	*/
	bool installCellDataAttr(PyObject* dictData = NULL);

	void createCellData(void);

	void destroyCellData(void);

	void addCellDataToStream(uint32 flags, MemoryStream* s);
	void addPersistentsDataToStream(uint32 flags, MemoryStream* s);

	PyObject* createCellDataDict(uint32 flags);

	INLINE PyObject* getCellData(void)const;
	
	/**
		����cell���ֽ�entity��celldata����һ�ݹ���
	*/
	void reqBackupCellData();
	
	/** 
		д������Ϣ����
	*/
	void writeBackupData(MemoryStream* s);
	void onBackup();

	/** 
		д�浵��Ϣ����
	*/
	void writeArchiveData(MemoryStream* s);

	/** 
		��Ҫ���浽���ݿ�֮ǰ��֪ͨ 
	*/
	void onWriteToDB();
	void onCellWriteToDBComplete();

	/**
		��ȡ���entity��client���ֵ����Էŵ�����
	*/
	void getClientPropertys(MemoryStream* s);

	/** 
		����cellʧ�ܻص� 
	*/
	void onCreateCellFailure(void);

	/** 
		����cell�ɹ��ص� 
	*/
	void onGetCell(Mercury::Channel* pChannel, COMPONENT_ID componentID);

	/** 
		��ʧcell�˵�֪ͨ 
	*/
	void onLoseCell(Mercury::Channel* pChannel, MemoryStream& s);

	/** 
		����cell����
	*/
	void onBackupCellData(Mercury::Channel* pChannel, MemoryStream& s);

	/** 
		�ͻ��˶�ʧ 
	*/
	void onClientDeath();

	/** ����ӿ�
		Զ�̺��б�entity�ķ��� 
	*/
	void onRemoteMethodCall(Mercury::Channel* pChannel, MemoryStream& s);

	/** 
		�������entity 
	*/
	void destroy();

	/** 
		Ϊһ��baseEntity���ƶ���cell�ϴ���һ��cellEntity 
	*/
	DECLARE_PY_MOTHOD_ARG1(createCellEntity, PyObject_ptr);

	/** 
		����һ��cellEntity��һ���µ�space�� 
	*/
	DECLARE_PY_MOTHOD_ARG1(createInNewSpace, PyObject_ptr);

protected:
	// ���entity�Ŀͻ���mailbox cellapp mailbox
	EntityMailbox*							clientMailbox_;			
	EntityMailbox*							cellMailbox_;

	// entity��������cell����δ����ʱ����һЩcell�������ݱ���������
	PyObject*								cellDataDict_;			

	// �Ƿ��Ǵ洢�����ݿ��е�entity
	bool									hasDB_;					

	// �Ƿ����ڻ�ȡcelldata��
	bool									isGetingCellData_;

	// �Ƿ����ڴ浵��
	bool									isArchiveing_;

	// �Ƿ����ڴ���cell��
	bool									creatingCell_;
};

}


#ifdef CODE_INLINE
#include "base.ipp"
#endif

#endif
