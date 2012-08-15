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

#ifndef __ENTITY_APP_H__
#define __ENTITY_APP_H__
// common include
#include "pyscript/script.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/timer.hpp"
#include "cstdkbe/smartpointer.hpp"
#include "pyscript/pyobject_pointer.hpp"
#include "helper/debug_helper.hpp"
#include "server/script_timers.hpp"
#include "server/idallocate.hpp"
#include "server/serverconfig.hpp"
#include "server/globaldata_client.hpp"
#include "server/globaldata_server.hpp"
#include "server/callbackmgr.hpp"	
#include "entitydef/entitydef.hpp"
#include "entitydef/entities.hpp"
#include "entitydef/entity_mailbox.hpp"
#include "network/message_handler.hpp"
#include "thread/threadpool.hpp"
#include "resmgr/resmgr.hpp"
#include "helper/console_helper.hpp"

#if KBE_PLATFORM == PLATFORM_WIN32
#pragma warning (disable : 4996)
#endif
//#define NDEBUG
#include "server/serverapp.hpp"
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

template<class E>
class EntityApp : public ServerApp
{
public:
	enum TimeOutType
	{
		TIMEOUT_GAME_TICK = TIMEOUT_SERVERAPP_MAX + 1,
		TIMEOUT_ENTITYAPP_MAX
	};
public:
	EntityApp(Mercury::EventDispatcher& dispatcher, 
		Mercury::NetworkInterface& ninterface, 
		COMPONENT_TYPE componentType,
		COMPONENT_ID componentID);
	~EntityApp();
	
	/* ��ش���ӿ� */
	virtual void handleTimeout(TimerHandle handle, void * arg);
	virtual void handleGameTick();

	/* ͨ��entityIDѰ�ҵ���Ӧ��ʵ�� */
	E* findEntity(ENTITY_ID entityID);

	/* ͨ��entityID����һ��entity */
	virtual bool destroyEntity(ENTITY_ID entityID);

	/* ��mailbox�����Ի�ȡһ��entity��ʵ��
		��Ϊ�������ϲ�һ���������entity��
	*/
	PyObject* tryGetEntityByMailbox(COMPONENT_ID componentID, ENTITY_ID eid);

	KBEngine::script::Script& getScript(){ return script_; }
	PyObjectPtr getEntryScript(){ return entryScript_; }

	void registerScript(PyTypeObject*);
	int registerPyObjectToScript(const char* attrName, PyObject* pyObj);
	int unregisterPyObjectToScript(const char* attrName);

	bool installPyScript();
	virtual bool installPyModules();
	virtual void onInstallPyModules() {};
	virtual bool uninstallPyModules();
	bool uninstallPyScript();
	bool installEntityDef();
	
	virtual void finalise();
	virtual bool inInitialize();
	virtual bool initialize();
		
	virtual void onSignalled(int sigNum);
	
	PY_CALLBACKMGR& callbackMgr(){ return pyCallbackMgr_; }	

	/* ����һ��entity */
	E* createEntityCommon(const char* entityType, PyObject* params, 
		bool isInitializeScript = true, ENTITY_ID eid = 0);

	virtual E* onCreateEntityCommon(PyObject* pyEntity, ScriptModule* sm, ENTITY_ID eid);

	/** ����ӿ�
		�������һ��ENTITY_ID�εĻص�
	*/
	void onReqAllocEntityID(Mercury::Channel* pChannel, ENTITY_ID startID, ENTITY_ID endID);

	/** ����ӿ�
		dbmgr���ͳ�ʼ��Ϣ
		startID: ��ʼ����ENTITY_ID ����ʼλ��
		endID: ��ʼ����ENTITY_ID �ν���λ��
		startGlobalOrder: ȫ������˳�� �������ֲ�ͬ���
		startGroupOrder: ��������˳�� ����������baseapp�еڼ���������
	*/
	void onDbmgrInitCompleted(Mercury::Channel* pChannel, 
		ENTITY_ID startID, ENTITY_ID endID, int32 startGlobalOrder, int32 startGroupOrder);

	/** ����ӿ�
		dbmgr�㲥global���ݵĸı�
	*/
	void onBroadcastGlobalDataChange(Mercury::Channel* pChannel, KBEngine::MemoryStream& s);


	/** ����ӿ�
		����ִ��һ��pythonָ��
	*/
	void onExecScriptCommand(Mercury::Channel* pChannel, std::string& strcommand);
protected:
	KBEngine::script::Script								script_;
	std::vector<PyTypeObject*>								scriptBaseTypes_;

	PyObjectPtr												entryScript_;

	EntityIDClient											idClient_;
	Entities<E>*											pEntities_;										// �洢���е�entity������

	TimerHandle												gameTimer_;

	GlobalDataClient*										pGlobalData_;									// globalData

	PY_CALLBACKMGR											pyCallbackMgr_;
	
};


template<class E>
EntityApp<E>::EntityApp(Mercury::EventDispatcher& dispatcher, 
					 Mercury::NetworkInterface& ninterface, 
					 COMPONENT_TYPE componentType,
					 COMPONENT_ID componentID):
ServerApp(dispatcher, ninterface, componentType, componentID),
script_(),
scriptBaseTypes_(),
entryScript_(),
idClient_(),
pEntities_(NULL),
gameTimer_(),
pGlobalData_(NULL),
pyCallbackMgr_()
{
	ScriptTimers::initialize(*this);
	idClient_.pApp(this);

	// ��ʼ��mailboxģ���ȡentityʵ�庯����ַ
	EntityMailbox::setGetEntityFunc(std::tr1::bind(&EntityApp<E>::tryGetEntityByMailbox, this, 
		std::tr1::placeholders::_1, std::tr1::placeholders::_2));
}

template<class E>
EntityApp<E>::~EntityApp()
{
	ScriptTimers::finalise(*this);
}

template<class E>
bool EntityApp<E>::inInitialize()
{
	if(!installPyScript())
		return false;

	if(!installPyModules())
		return false;
	
	return installEntityDef();
}

template<class E>
bool EntityApp<E>::initialize()
{
	if(thread::ThreadPool::getSingletonPtr() && 
		!thread::ThreadPool::getSingleton().isInitialize())
		thread::ThreadPool::getSingleton().createThreadPool(16, 16, 256);

	bool ret = ServerApp::initialize();
	if(ret)
	{
		gameTimer_ = this->getMainDispatcher().addTimer(1000000 / g_kbeSrvConfig.gameUpdateHertz(), this,
								reinterpret_cast<void *>(TIMEOUT_GAME_TICK));
	}
	return ret;
}

template<class E>
void EntityApp<E>::finalise(void)
{
	gameTimer_.cancel();
	uninstallPyScript();
	ServerApp::finalise();
}

template<class E>
bool EntityApp<E>::installEntityDef()
{
	// ��ʼ���������
	// demo/res/scripts/entity_defs/alias.xml
	if(!DataTypes::initialize("scripts/entity_defs/alias.xml"))
		return false;

	// ��ʼ��������չģ��
	// demo/res/scripts/
	if(!EntityDef::initialize(Resmgr::respaths()[1] + "res/scripts/", scriptBaseTypes_, componentType_)){
		return false;
	}

	return EntityDef::installScript(NULL);
}

template<class E>
int EntityApp<E>::registerPyObjectToScript(const char* attrName, PyObject* pyObj)
{ 
	return script_.registerToModule(attrName, pyObj); 
}

template<class E>
int EntityApp<E>::unregisterPyObjectToScript(const char* attrName)
{ 
	return script_.unregisterToModule(attrName); 
}

template<class E>
bool EntityApp<E>::installPyScript()
{
	if(Resmgr::respaths().size() <= 0)
	{
		ERROR_MSG("EntityApp::installPyScript: KBE_RES_PATH is error!\n");
		return false;
	}

	std::wstring root_path = L"";
	wchar_t* tbuf = KBEngine::char2wchar(const_cast<char*>(Resmgr::respaths()[1].c_str()));
	if(tbuf != NULL)
	{
		root_path += tbuf;
		free(tbuf);
	}
	else
	{
		return false;
	}

	std::wstring pyPaths = root_path + L"res/scripts/common;";

	switch(componentType_)
	{
	case BASEAPP_TYPE:
		pyPaths += root_path + L"res/scripts/base;";
		break;
	case CELLAPP_TYPE:
		pyPaths += root_path + L"res/scripts/cell;";
		break;
	default:
		pyPaths += root_path + L"res/scripts/client;";
		break;
	};
	
	std::string kbe_res_path = Resmgr::respaths()[0].c_str();
	kbe_res_path += "script/common";

	tbuf = KBEngine::char2wchar(const_cast<char*>(kbe_res_path.c_str()));
	bool ret = getScript().install(tbuf, pyPaths, "KBEngine", componentType_);
	// �˴������Դ���python֮��python�ͷ���
	// free(tbuf);
	return ret;
}

template<class E>
void EntityApp<E>::registerScript(PyTypeObject* pto)
{
	scriptBaseTypes_.push_back(pto);
}

template<class E>
bool EntityApp<E>::uninstallPyScript()
{
	return uninstallPyModules() && getScript().uninstall();
}

template<class E>
bool EntityApp<E>::installPyModules()
{
	Entities<E>::installScript(NULL);
	//Entity::installScript(g_script.getModule());

	pEntities_ = new Entities<E>();
	registerPyObjectToScript("entities", pEntities_);

	// ��װ���ģ��
	PyObject *entryScriptFileName = NULL;
	if(componentType() == BASEAPP_TYPE)
	{
		ENGINE_COMPONENT_INFO& info = g_kbeSrvConfig.getBaseApp();
		entryScriptFileName = PyUnicode_FromString(info.entryScriptFile);
	}
	else if(componentType() == CELLAPP_TYPE)
	{
		ENGINE_COMPONENT_INFO& info = g_kbeSrvConfig.getCellApp();
		entryScriptFileName = PyUnicode_FromString(info.entryScriptFile);
	}

	if(entryScriptFileName != NULL)
	{
		entryScript_ = PyImport_Import(entryScriptFileName);
		SCRIPT_ERROR_CHECK();
		S_RELEASE(entryScriptFileName);

		if(entryScript_.get() == NULL)
		{
			return false;
		}
	}

	// ���globalData, globalBases֧��
	pGlobalData_ = new GlobalDataClient(DBMGR_TYPE, GlobalDataServer::GLOBAL_DATA);
	registerPyObjectToScript("globalData", pGlobalData_);

	return true;
}

template<class E>
bool EntityApp<E>::uninstallPyModules()
{
	S_RELEASE(pEntities_);
	unregisterPyObjectToScript("entities");

	unregisterPyObjectToScript("globalData");
	S_RELEASE(pGlobalData_); 

	Entities<E>::uninstallScript();
	//Entity::uninstallScript();
	EntityDef::uninstallScript();
	return true;
}

template<class E>
E* EntityApp<E>::createEntityCommon(const char* entityType, PyObject* params, 
										 bool isInitializeScript, ENTITY_ID eid)
{
	// ���ID�Ƿ��㹻, ���㷵��NULL
	if(eid <= 0 && idClient_.getSize() == 0)
	{
		PyErr_SetString(PyExc_SystemError, "EntityApp::createEntityCommon: is Failed. not enough entityIDs.");
		PyErr_PrintEx(0);
		return NULL;
	}
	
	ScriptModule* sm = EntityDef::findScriptModule(entityType);
	if(sm == NULL || componentType_ == CELLAPP_TYPE ? !sm->hasCell() : !sm->hasBase())
	{
		PyErr_Format(PyExc_TypeError, "EntityApp::createEntityCommon: entity [%s] not found.\n", entityType);
		PyErr_PrintEx(0);
		return NULL;
	}

	PyObject* obj = sm->createObject();

	// �ж��Ƿ�Ҫ����һ���µ�id
	ENTITY_ID id = eid;
	if(id <= 0)
		id = idClient_.alloc();
	
	E* entity = onCreateEntityCommon(obj, sm, id);

	// �������ֿռ�
	entity->createNamespace(params);

	// ��entity����entities
	pEntities_->add(id, entity); 

	// ��ʼ���ű�
	if(isInitializeScript)
		entity->initializeScript();

	SCRIPT_ERROR_CHECK();
	INFO_MSG("EntityApp::createEntityCommon: new %s (%d).\n", entityType, id);
	return entity;
}

template<class E>
E* EntityApp<E>::onCreateEntityCommon(PyObject* pyEntity, ScriptModule* sm, ENTITY_ID eid)
{
	// ִ��Entity�Ĺ��캯��
	return new(pyEntity) E(eid, sm);
}

template<class E>
void EntityApp<E>::onSignalled(int sigNum)
{
	this->ServerApp::onSignalled(sigNum);
	
	switch (sigNum)
	{
	case SIGQUIT:
		CRITICAL_MSG("Received QUIT signal. This is likely caused by the "
					"%sMgr killing this %s because it has been "
					"unresponsive for too long. Look at the callstack from "
					"the core dump to find the likely cause.\n",
				COMPONENT_NAME[componentType_], 
				COMPONENT_NAME[componentType_] );
		
		break;
	default: 
		break;
	}
}

template<class E>
PyObject* EntityApp<E>::tryGetEntityByMailbox(COMPONENT_ID componentID, ENTITY_ID eid)
{
	if(componentID != componentID_)
		return NULL;
	
	E* entity = pEntities_->find(eid);
	if(entity == NULL){
		ERROR_MSG("EntityApp::tryGetEntityByMailbox: can't found entity:%d.\n", eid);
		return NULL;
	}

	return entity;
}

template<class E>
void EntityApp<E>::handleTimeout(TimerHandle handle, void * arg)
{
	switch (reinterpret_cast<uintptr>(arg))
	{
		case TIMEOUT_GAME_TICK:
			this->handleGameTick();
			break;
		default:
			break;
	}

	ServerApp::handleTimeout(handle, arg);
}

template<class E>
void EntityApp<E>::handleGameTick()
{
	// time_t t = ::time(NULL);
	// DEBUG_MSG("EntityApp::handleGameTick[%"PRTime"]:%u\n", t, time_);
	
	time_++;
	handleTimers();
	getNetworkInterface().handleChannels(KBEngine::Mercury::MessageHandlers::pMainMessageHandlers);
}

template<class E>
bool EntityApp<E>::destroyEntity(ENTITY_ID entityID)
{
	E* entity = pEntities_->erase(entityID);
	if(entity != NULL)
	{
		entity->destroy();
		return true;
	}

	return false;
}

template<class E>
E* EntityApp<E>::findEntity(ENTITY_ID entityID)
{
	return pEntities_->find(entityID);
}

template<class E>
void EntityApp<E>::onReqAllocEntityID(Mercury::Channel* pChannel, ENTITY_ID startID, ENTITY_ID endID)
{
	// INFO_MSG("EntityApp::onReqAllocEntityID: entityID alloc(%d-%d).\n", startID, endID);
	idClient_.onAddRange(startID, endID);
}

template<class E>
void EntityApp<E>::onDbmgrInitCompleted(Mercury::Channel* pChannel, 
		ENTITY_ID startID, ENTITY_ID endID, int32 startGlobalOrder, int32 startGroupOrder)
{
	INFO_MSG("EntityApp::onDbmgrInitCompleted: entityID alloc(%d-%d), startGlobalOrder=%d, startGroupOrder=%d.\n",
		startID, endID, startGlobalOrder, startGroupOrder);

	startGlobalOrder_ = startGlobalOrder;
	startGroupOrder_ = startGroupOrder;
	g_componentOrder = startGlobalOrder;

	idClient_.onAddRange(startID, endID);

}

template<class E>
void EntityApp<E>::onBroadcastGlobalDataChange(Mercury::Channel* pChannel, KBEngine::MemoryStream& s)
{
	int32 slen;
	std::string key, value;
	bool isDelete;
	
	s >> isDelete;
	s >> slen;
	key.assign((char*)(s.data() + s.rpos()), slen);
	s.read_skip(slen);

	if(!isDelete)
	{
		s >> slen;
		value.assign((char*)(s.data() + s.rpos()), slen);
		s.read_skip(slen);
	}

	PyObject * pyKey = script::Pickler::unpickle(key);
	
	if(isDelete)
	{
		if(pGlobalData_->del(pyKey))
		{
			// ֪ͨ�ű�
			PyObject* pyResult = PyObject_CallMethod(getEntryScript().get(), 
												const_cast<char*>("onGlobalDataDel"), 
												const_cast<char*>("O"), 
												pyKey);

			if(pyResult != NULL)
				Py_DECREF(pyResult);
			else
				SCRIPT_ERROR_CHECK();
		}
	}
	else
	{
		PyObject * pyValue = script::Pickler::unpickle(value);
		if(pGlobalData_->write(pyKey, pyValue))
		{
			// ֪ͨ�ű�
			PyObject* pyResult = PyObject_CallMethod(getEntryScript().get(), 
												const_cast<char*>("onGlobalData"), 
												const_cast<char*>("OO"), 
												pyKey, pyValue);

			if(pyResult != NULL)
				Py_DECREF(pyResult);
			else
				SCRIPT_ERROR_CHECK();
		}
	}
}

template<class E>
void EntityApp<E>::onExecScriptCommand(Mercury::Channel* pChannel, std::string& strcommand)
{
	DEBUG_MSG("EntityApp::onExecScriptCommand: command size(%d).\n", strcommand.size());

	std::string retbuf = "";
	if(script_.run_simpleString(strcommand, &retbuf) == 0)
	{
		// ��������ظ��ͻ���
		Mercury::Bundle bundle;
		ConsoleInterface::ConsoleExecCommandCBMessageHandler msgHandler;
		bundle.newMessage(msgHandler);
		ConsoleInterface::ConsoleExecCommandCBMessageHandlerArgs1::staticAddToBundle(bundle, retbuf);
		bundle.send(this->getNetworkInterface(), pChannel);
	}
}

}
#endif
