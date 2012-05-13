/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
#ifndef __COMPONENT_BRIDEGE__
#define __COMPONENT_BRIDEGE__

#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/tasks.hpp"
#include "cstdkbe/singleton.hpp"
#include "helper/debug_helper.hpp"
#include "network/common.hpp"
#include "network/interfaces.hpp"
#include "network/endpoint.hpp"
#include "network/udp_packet_receiver.hpp"
#include "server/components.hpp"

namespace KBEngine { 
namespace Mercury
{
class Address;
class NetworkInterface;
class EventDispatcher;
class UDPPacket;
}

/*
	���ģ�鱻����Ϊ����������š��� �������cellapp����dbmgr�ȡ�
	����ְ����Ҫ���õ�ǰapp����ܹ��ȽϷ����������������н�����
*/
class Componentbridge : public Task, 
						public Mercury::UDPPacketReceiver,
						public Singleton<Componentbridge>
{
public:
	Componentbridge(Mercury::NetworkInterface & networkInterface, 
			COMPONENT_TYPE componentType, COMPONENT_ID componentID);
	~Componentbridge();

	static Components& getComponents();
private:
	virtual bool process();
	Mercury:: EventDispatcher & dispatcher();

	bool findInterfaces();
private:
	Mercury::EndPoint epBroadcast_;
	Mercury::NetworkInterface & networkInterface_;
	COMPONENT_TYPE componentType_;
	COMPONENT_ID componentID_;									// �������ID
	int8 broadcastCount_;
};

}

#endif // __COMPONENT_BRIDEGE__