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


#include "bundle.hpp"
#include "network/network_interface.hpp"
#include "network/packet.hpp"
#include "network/channel.hpp"
#include "network/tcp_packet.hpp"
#include "network/udp_packet.hpp"

#ifndef CODE_INLINE
#include "bundle.ipp"
#endif

#define BUNDLE_SEND_OP(op)																					\
	finish();																								\
																											\
	Packets::iterator iter = packets_.begin();																\
	for (; iter != packets_.end(); iter++)																	\
	{																										\
		Packet* pPacket = (*iter);																			\
		int retries = 0;																					\
		Reason reason;																						\
																											\
		while(true)																							\
		{																									\
			retries++;																						\
			int slen = op;																					\
																											\
			if(slen != (int)pPacket->totalSize())															\
			{																								\
				reason = NetworkInterface::getSendErrorReason(&ep, slen, pPacket->totalSize());				\
				/* ������ͳ��ִ�����ô���ǿ��Լ�������һ�Σ� ����3���˳�	*/								\
				if (reason == REASON_NO_SUCH_PORT && retries <= 3)											\
				{																							\
					continue;																				\
				}																							\
																											\
				/* ���ϵͳ���ͻ����Ѿ����ˣ������ǵȴ�10ms	*/												\
				if (reason == REASON_RESOURCE_UNAVAILABLE && retries <= 3)									\
				{																							\
					fd_set	fds;																			\
					struct timeval tv = { 0, 10000 };														\
					FD_ZERO( &fds );																		\
					FD_SET(ep, &fds);																		\
																											\
					WARNING_MSG( "%s: "																		\
						"Transmit queue full, waiting for space... (%d)\n",									\
						__FUNCTION__, retries );															\
																											\
					select(ep + 1, NULL, &fds, NULL, &tv);													\
					continue;																				\
				}																							\
			}																								\
			else																							\
			{																								\
				break;																						\
			}																								\
		}																									\
																											\
		delete pPacket;																						\
	}																										\
																											\
	onSendComplete();																						\
																											\


namespace KBEngine { 
namespace Mercury
{
//-------------------------------------------------------------------------------------
Bundle::Bundle(Channel * pChannel, ProtocolType pt):
	pChannel_(pChannel),
	numMessages_(0),
	pCurrPacket_(NULL),
	currMsgID_(0),
	currMsgPacketCount_(0),
	currMsgLength_(0),
	currMsgHandlerLength_(0),
	currMsgLengthPos_(0),
	packets_(),
	isTCPPacket_(pt == PROTOCOL_TCP)
{
	 newPacket();
}

//-------------------------------------------------------------------------------------
Bundle::~Bundle()
{
	clear();
	SAFE_RELEASE(pCurrPacket_);
}

//-------------------------------------------------------------------------------------
Packet* Bundle::newPacket()
{
	if(isTCPPacket_)
		pCurrPacket_ = new TCPPacket;
	else
		pCurrPacket_ = new UDPPacket;
	
	return pCurrPacket_;
}

//-------------------------------------------------------------------------------------
void Bundle::finish(bool issend)
{
	KBE_ASSERT(pCurrPacket_ != NULL);

	if(issend)
	{
		currMsgPacketCount_++;
		packets_.push_back(pCurrPacket_);
	}

	currMsgLength_ += pCurrPacket_->totalSize();

	// �˴����ڷǹ̶����ȵ���Ϣ��˵��Ҫ�����������ճ�����Ϣ
	if(currMsgHandlerLength_ < 0 || g_packetAlwaysContainLength)
	{
		Packet* pPacket = pCurrPacket_;
		if(currMsgPacketCount_ > 0)
			pPacket = packets_[packets_.size() - currMsgPacketCount_];

		currMsgLength_ -= MERCURY_MESSAGE_ID_SIZE;
		currMsgLength_ -= MERCURY_MESSAGE_LENGTH_SIZE;

		memcpy(&pPacket->data()[currMsgLengthPos_], 
			(uint8*)&currMsgLength_, MERCURY_MESSAGE_LENGTH_SIZE);

	}
	
	if(issend)
	{
		currMsgHandlerLength_ = 0;
		pCurrPacket_ = NULL;
	}

	currMsgID_ = 0;
	currMsgPacketCount_ = 0;
	currMsgLength_ = 0;
	currMsgLengthPos_ = 0;
}

//-------------------------------------------------------------------------------------
void Bundle::clear()
{
	Packets::iterator iter = packets_.begin();
	for (; iter != packets_.end(); iter++)
		delete (*iter);
	
	packets_.clear();
	SAFE_RELEASE(pCurrPacket_);

	currMsgID_ = 0;
	currMsgPacketCount_ = 0;
	currMsgLength_ = 0;
	currMsgLengthPos_ = 0;
	currMsgHandlerLength_ = 0;
}

//-------------------------------------------------------------------------------------
void Bundle::send(NetworkInterface & networkInterface, Channel * pChannel)
{
	finish();
	networkInterface.send(*this, pChannel);
}

//-------------------------------------------------------------------------------------
void Bundle::send(EndPoint& ep)
{
	BUNDLE_SEND_OP(ep.send(pPacket->data(), pPacket->totalSize()));
}

//-------------------------------------------------------------------------------------
void Bundle::sendto(EndPoint& ep, u_int16_t networkPort, u_int32_t networkAddr)
{
	BUNDLE_SEND_OP(ep.sendto(pPacket->data(), pPacket->totalSize(), networkPort, networkAddr));
}

//-------------------------------------------------------------------------------------
void Bundle::onSendComplete()
{
	packets_.clear();
}

//-------------------------------------------------------------------------------------
void Bundle::newMessage(const MessageHandler& msgHandler)
{
	finish(false);
	KBE_ASSERT(pCurrPacket_ != NULL);
	
	(*this) << msgHandler.msgID;
	pCurrPacket_->messageID(msgHandler.msgID);

	// �˴����ڷǹ̶����ȵ���Ϣ��˵��Ҫ������������Ϣ����λΪ0�� �������Ҫ��䳤��
	if(msgHandler.msgLen == MERCURY_VARIABLE_MESSAGE)
	{
		MessageLength msglen = 0;
		currMsgLengthPos_ = pCurrPacket_->wpos();
		(*this) << msglen;
	}

	numMessages_++;
	currMsgID_ = msgHandler.msgID;
	currMsgPacketCount_ = 0;
	currMsgHandlerLength_ = msgHandler.msgLen;
}

//-------------------------------------------------------------------------------------
}
}
