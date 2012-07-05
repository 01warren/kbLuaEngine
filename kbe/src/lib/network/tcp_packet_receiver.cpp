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


#include "tcp_packet_receiver.hpp"
#ifndef CODE_INLINE
#include "tcp_packet_receiver.ipp"
#endif

#include "network/address.hpp"
#include "network/bundle.hpp"
#include "network/channel.hpp"
#include "network/endpoint.hpp"
#include "network/event_dispatcher.hpp"
#include "network/network_interface.hpp"
#include "network/event_poller.hpp"
#include "network/error_reporter.hpp"

namespace KBEngine { 
namespace Mercury
{
//-------------------------------------------------------------------------------------
TCPPacketReceiver::TCPPacketReceiver(EndPoint & endpoint,
	   NetworkInterface & networkInterface	) :
	PacketReceiver(endpoint, networkInterface)
{
}

//-------------------------------------------------------------------------------------
TCPPacketReceiver::~TCPPacketReceiver()
{
}


//-------------------------------------------------------------------------------------
bool TCPPacketReceiver::processSocket(bool expectingPacket)
{
	Channel* pChannel = networkInterface_.findChannel(endpoint_.addr());
	KBE_ASSERT(pChannel != NULL);
	
	Packet* pReceiveWindow = pChannel->receiveWindow();
	int len = pReceiveWindow->recvFromEndPoint(endpoint_);

	if (len < 0)
	{
		return this->checkSocketErrors(len, expectingPacket);
	}
	else if(len == 0) // �ͻ��������˳�
	{
		networkInterface_.deregisterChannel(pChannel);
		pChannel->destroy();
		return false;
	}
	
	Reason ret = this->processPacket(pChannel, pReceiveWindow);

	if(ret != REASON_SUCCESS)
		this->dispatcher().errorReporter().reportException(ret, endpoint_.addr());
	
	return true;
}

//-------------------------------------------------------------------------------------
Reason TCPPacketReceiver::processFilteredPacket(Channel* pChannel, Packet * pPacket)
{
	networkInterface_.onPacketIn(*pPacket);
	return REASON_SUCCESS;
}

//-------------------------------------------------------------------------------------
bool TCPPacketReceiver::checkSocketErrors(int len, bool expectingPacket)
{
#ifdef _WIN32
	DWORD wsaErr = WSAGetLastError();
#endif //def _WIN32

	if (
#ifdef _WIN32
		wsaErr == WSAEWOULDBLOCK && !expectingPacket// send�������ǻ���������, recv�����Ѿ������ݿɶ���
#else
		errno == EAGAIN && !expectingPacket			// recv�������Ѿ������ݿɶ���
#endif
		)
	{
		return false;
	}

#ifdef unix
	if (errno == EAGAIN ||							// �Ѿ������ݿɶ���
		errno == ECONNREFUSED ||					// ���ӱ��������ܾ�
		errno == EHOSTUNREACH)						// Ŀ�ĵ�ַ���ɵ���
	{
		this->dispatcher().errorReporter().reportException(
				REASON_NO_SUCH_PORT);
		return false;
	}
#else
	/*
	���ڵ����ӱ�Զ������ǿ�ƹرա�ͨ��ԭ��Ϊ��Զ�������϶Եȷ�Ӧ�ó���ͻȻֹͣ���У���Զ����������������
	��Զ��������Զ�̷��׽�����ʹ���ˡ�ǿ�ơ��رգ��μ�setsockopt(SO_LINGER)����
	���⣬��һ�������������ڽ���ʱ�����������keep-alive�����⵽һ��ʧ�ܶ��жϣ�Ҳ���ܵ��´˴���
	��ʱ�����ڽ��еĲ����Դ�����WSAENETRESETʧ�ܷ��أ�����������ʧ�ܷ��ش�����WSAECONNRESET
	*/
	if (wsaErr == WSAECONNRESET)
	{
		WARNING_MSG("TCPPacketReceiver::processPendingEvents: "
					"Throwing REASON_GENERAL_NETWORK - WSAECONNRESET\n");
		return false;
	}
#endif // unix

#ifdef _WIN32
	WARNING_MSG("TCPPacketReceiver::processPendingEvents: "
				"Throwing REASON_GENERAL_NETWORK - %d\n",
				wsaErr);
#else
	WARNING_MSG("TCPPacketReceiver::processPendingEvents: "
				"Throwing REASON_GENERAL_NETWORK - %s\n",
			kbe_strerror());
#endif
	this->dispatcher().errorReporter().reportException(
			REASON_GENERAL_NETWORK);

	return true;
}

//-------------------------------------------------------------------------------------
}
}