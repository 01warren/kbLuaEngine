#include "dbmgr.hpp"
#include "dbmgr_interface.hpp"
#include "network/common.hpp"
#include "network/tcp_packet.hpp"
#include "network/udp_packet.hpp"
#include "network/message_handler.hpp"
#include "thread/threadpool.hpp"
#include "server/componentbridge.hpp"

namespace KBEngine{
	
ServerConfig g_serverConfig;
KBE_SINGLETON_INIT(Dbmgr);

//-------------------------------------------------------------------------------------
Dbmgr::Dbmgr(Mercury::EventDispatcher& dispatcher, Mercury::NetworkInterface& ninterface, COMPONENT_TYPE componentType):
	ServerApp(dispatcher, ninterface, componentType)
{
}

//-------------------------------------------------------------------------------------
Dbmgr::~Dbmgr()
{
}

//-------------------------------------------------------------------------------------
bool Dbmgr::run()
{
	bool ret = true;

	while(!this->getMainDispatcher().isBreakProcessing())
	{
		this->getMainDispatcher().processOnce(false);
		KBEngine::sleep(100);
	};

	return ret;
}

//-------------------------------------------------------------------------------------
void Dbmgr::handleTimeout(TimerHandle handle, void * arg)
{
}

//-------------------------------------------------------------------------------------
bool Dbmgr::initializeBegin()
{
	return true;
}

//-------------------------------------------------------------------------------------
bool Dbmgr::inInitialize()
{
	return true;
}

//-------------------------------------------------------------------------------------
bool Dbmgr::initializeEnd()
{
	// �㲥�Լ��ĵ�ַ�������ϵ�����kbemachine
	// �������new ������ͷţ� �ڲ����Զ��ͷš�
	this->getMainDispatcher().addFrequentTask(&Componentbridge::getSingleton());

	return true;
}

//-------------------------------------------------------------------------------------
void Dbmgr::finalise()
{
}

//-------------------------------------------------------------------------------------

}
