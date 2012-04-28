/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
#ifndef __ENTITY_CELL_BASE_CLIENT__MAILBOX_H__
#define __ENTITY_CELL_BASE_CLIENT__MAILBOX_H__
	
// common include
#include "cstdkbe/cstdkbe.hpp"
#include "entitydef/entitymailboxabstract.hpp"
#include "pyscript/scriptobject.hpp"
#include "pyscript/pickler.hpp"
#include "entitydef/method.hpp"
#include "entitydef/entitydef.hpp"
	
#ifdef KBE_SERVER
#include "server/engine_componentmgr.hpp"
#endif
//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{
	
typedef std::tr1::function<PyObject* (COMPONENT_ID componentID, ENTITY_ID& eid)> GetEntityFunc;

namespace Mercury
{
class Channel;
}

class EntityMailbox : public EntityMailboxAbstract
{
	/** ���໯ ��һЩpy�������������� */
	INSTANCE_SCRIPT_HREADER(EntityMailbox, EntityMailboxAbstract)
private:
	static GetEntityFunc					__getEntityFunc;		// ���һ��entity��ʵ��ĺ�����ַ
protected:
	ScriptModule*							m_scriptModule_;		// ��entity��ʹ�õĽű�ģ�����
public:
	EntityMailbox(Mercury::Channel* pChannel, ScriptModule* scriptModule, COMPONENT_ID componentID, ENTITY_ID& eid, ENTITY_MAILBOX_TYPE type);
	~EntityMailbox();
	
	/** �ű������ȡ���Ի��߷��� */
	PyObject* onScriptGetAttribute(PyObject* attr);						
			
	/** ��ö�������� */
	PyObject* tp_repr();
	PyObject* tp_str();

	/** unpickle���� */
	static PyObject* __unpickle__(PyObject* self, PyObject* args);

	/** �ű�����װʱ������ */
	static void onInstallScript(PyObject* mod);

	/** ����mailbox��__getEntityFunc������ַ */
	static void setGetEntityFunc(GetEntityFunc func){ __getEntityFunc = func; };
};

}
#endif
