#include "baseappmgr.hpp"
#include "baseappmgr_interface.hpp"
#include "network/common.hpp"
#include "network/tcp_packet.hpp"
#include "network/udp_packet.hpp"
#include "network/message_handler.hpp"
#include "thread/threadpool.hpp"
#include "network/broadcast_interface.hpp"

namespace KBEngine{
	
ServerConfig g_serverConfig;
KBE_SINGLETON_INIT(Baseappmgr);

//-------------------------------------------------------------------------------------
Baseappmgr::Baseappmgr(Mercury::EventDispatcher& dispatcher, Mercury::NetworkInterface& ninterface, COMPONENT_TYPE componentType):
	ServerApp(dispatcher, ninterface, componentType)
{
}

//-------------------------------------------------------------------------------------
Baseappmgr::~Baseappmgr()
{
}

//-------------------------------------------------------------------------------------
bool Baseappmgr::run()
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
void Baseappmgr::handleTimeout(TimerHandle handle, void * arg)
{
}

//-------------------------------------------------------------------------------------
bool Baseappmgr::initializeBegin()
{
	return true;
}

//-------------------------------------------------------------------------------------
bool Baseappmgr::inInitialize()
{
	return true;
}

//-------------------------------------------------------------------------------------
bool Baseappmgr::initializeEnd()
{
	// �㲥�Լ��ĵ�ַ�������ϵ�����kbemachine
	// �������new ������ͷţ� �ڲ����Զ��ͷš�
	new Mercury::BroadcastInterface(getNetworkInterface(), 
		componentType(), componentID());

	return true;
}

//-------------------------------------------------------------------------------------
void Baseappmgr::finalise()
{
}

//-------------------------------------------------------------------------------------

}
