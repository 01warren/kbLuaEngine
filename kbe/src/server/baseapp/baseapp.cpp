#include "baseapp.hpp"
#include "entity.hpp"
#include "entities.hpp"
//#include "baseapp_interface.hpp"
#include "network/tcp_packet.hpp"
#include "network/udp_packet.hpp"

namespace KBEngine{
	
ServerConfig g_serverConfig;
KBE_SINGLETON_INIT(BaseApp);

//-------------------------------------------------------------------------------------
BaseApp::BaseApp(Mercury::EventDispatcher& dispatcher, Mercury::NetworkInterface& ninterface, COMPONENT_TYPE componentType):
  EntityApp(dispatcher, ninterface, componentType),
    idClient_(NULL),
    entities_(NULL),
    gameTimer_()
{
}

//-------------------------------------------------------------------------------------
BaseApp::~BaseApp()
{
}

//-------------------------------------------------------------------------------------
bool BaseApp::installPyModules()
{
	Entities::installScript(NULL);
	Entity::installScript(getScript().getModule());

	registerScript(Entity::getScriptType());
	
	entities_ = new Entities();
	registerPyObjectToScript("entities", entities_);
	return true;
}

//-------------------------------------------------------------------------------------
bool BaseApp::uninstallPyModules()
{	
	S_RELEASE(entities_);
	Entities::uninstallScript();
	Entity::uninstallScript();
	return true;
}

//-------------------------------------------------------------------------------------
bool BaseApp::run()
{
	idClient_->onAddRange(1, 500);
	Entity* e = createEntity("Avatar", NULL);
	registerPyObjectToScript("avatar", e);
	PyRun_SimpleString("print ('888888888888888888888', KBEngine.avatar.id)");
	DEBUG_MSG("kbe:python is init successfully!!! %d\n", 88);
	SmartPointer<PyObject> testsmartpointer(::PyBytes_FromString("test"));
	testsmartpointer.clear();

	CRITICAL_MSG("hahahah %d\n", 1111);
	return ServerApp::run();
}

//-------------------------------------------------------------------------------------
void BaseApp::handleTimeout(TimerHandle handle, void * arg)
{
	switch (reinterpret_cast<uintptr>(arg))
	{
		case TIMEOUT_GAME_TICK:
			this->handleGameTick();
			break;
		case TIMEOUT_LOADING_TICK:
		{
			break;
		}
	}
}

//-------------------------------------------------------------------------------------
void BaseApp::handleGameTick()
{
	// time_t t = ::time(NULL);
	// DEBUG_MSG("BaseApp::handleGameTick[%d]:%d\n", t, time_);
	
	time_++;
	const Mercury::NetworkInterface::ChannelMap& channels = getNetworkInterface().channels();
	Mercury::NetworkInterface::ChannelMap::const_iterator iter = channels.begin();
	for(; iter != channels.end(); iter++)
	{
		Mercury::Channel* channel = iter->second;
		Mercury::EndPoint * endpoint = channel->endpoint();
		endpoint->send("hello", 5);

		Mercury::Channel::BufferedReceives& bufferedReceives = channel->bufferedReceives();
		Mercury::Channel::BufferedReceives::iterator briter = bufferedReceives.begin();
		Mercury::MessageID msgID = 0;
/*
		for(; briter != bufferedReceives.end(); briter++)
		{
			Mercury::Packet* pPacket = briter->get();
			(*pPacket) >> msgID;
			Mercury::MessageHandler* pMsgHandler = BaseAppInterface::messageHandlers.find(msgID);
			pMsgHandler->handle(*pPacket);
		}

		bufferedReceives.clear();*/
	}
}

//-------------------------------------------------------------------------------------
bool BaseApp::initializeBegin()
{
	return true;
}

//-------------------------------------------------------------------------------------
bool BaseApp::initializeEnd()
{
	idClient_ = new IDClient<ENTITY_ID>;
	
	gameTimer_ = this->getMainDispatcher().addTimer(1000000 / g_kbeSrvConfig.gameUpdateHertz(), this,
							reinterpret_cast<void *>(TIMEOUT_GAME_TICK));
	
	g_kbeSrvConfig.printCellapp();
	return true;
}

//-------------------------------------------------------------------------------------
void BaseApp::finalise()
{
	SAFE_RELEASE(idClient_);
	gameTimer_.cancel();
}

//-------------------------------------------------------------------------------------
Entity* BaseApp::createEntity(const char* entityType, PyObject* params, bool isInitializeScript, ENTITY_ID eid)
{
	// ���ID�Ƿ��㹻, ���㷵��NULL
	if(eid <= 0 && idClient_->getSize() == 0)
	{
		PyErr_SetString(PyExc_SystemError, "App::createEntity: is Failed. not enough entityIDs.");
		PyErr_PrintEx(0);
		return NULL;
	}
	
	ScriptModule* sm = EntityDef::findScriptModule(entityType);
	if(sm == NULL || !sm->hasCell())
	{
		PyErr_Format(PyExc_TypeError, "App::createEntity: entityType [%s] not found.\n", entityType);
		PyErr_PrintEx(0);
		return NULL;
	}

	PyObject* obj = sm->createObject();

	// �ж��Ƿ�Ҫ����һ���µ�id
	ENTITY_ID id = eid;
	if(id <= 0)
		id = idClient_->alloc();

	// ִ��Entity�Ĺ��캯��
	Entity* entity = new(obj) Entity(id, sm);

	// �������ֿռ�
	entity->createNamespace(params);

	// ��entity����entities
	entities_->add(id, entity); 
	
	// ���ID���㹻�ԣ�����������
	//checkEntityIDEnough();

	// ��ʼ���ű�
	if(isInitializeScript)
		entity->initializeScript();
	
	INFO_MSG("App::createEntity: new %s (%ld).\n", entityType, id);
	return entity;
}

//-------------------------------------------------------------------------------------
Entity* BaseApp::findEntity(ENTITY_ID eid)
{
	return entities_->find(eid);
}

//-------------------------------------------------------------------------------------

}
