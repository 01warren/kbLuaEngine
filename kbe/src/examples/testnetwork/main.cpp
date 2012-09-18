/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/

#include "cstdkbe/cstdkbe.hpp"
#include "network/address.hpp"
#include "network/endpoint.hpp"
#include "network/event_poller.hpp"
#include "helper/debug_helper.hpp"
#include "network/event_dispatcher.hpp"
#include "network/interfaces.hpp"
#include "network/tcp_packet.hpp"
#include "network/error_reporter.hpp"
#include "network/bundle.hpp"
#include "network/fixed_messages.hpp"
#include "network/common.hpp"

#undef DEFINE_IN_INTERFACE
#include "baseappmgr/baseappmgr_interface.hpp"
#define DEFINE_IN_INTERFACE
#include "baseappmgr/baseappmgr_interface.hpp"

#undef DEFINE_IN_INTERFACE
#include "cellappmgr/cellappmgr_interface.hpp"
#define DEFINE_IN_INTERFACE
#include "cellappmgr/cellappmgr_interface.hpp"

#undef DEFINE_IN_INTERFACE
#include "cellapp/cellapp_interface.hpp"
#define DEFINE_IN_INTERFACE
#include "cellapp/cellapp_interface.hpp"

#undef DEFINE_IN_INTERFACE
#include "baseapp/baseapp_interface.hpp"
#define DEFINE_IN_INTERFACE
#include "baseapp/baseapp_interface.hpp"

#undef DEFINE_IN_INTERFACE
#include "dbmgr/dbmgr_interface.hpp"
#define DEFINE_IN_INTERFACE
#include "dbmgr/dbmgr_interface.hpp"

#undef DEFINE_IN_INTERFACE
#include "loginapp/loginapp_interface.hpp"
#define DEFINE_IN_INTERFACE
#include "loginapp/loginapp_interface.hpp"

#include "log4cxx/logger.h"
#include "log4cxx/propertyconfigurator.h"

using namespace KBEngine;
using namespace KBEngine::Mercury;
Address address;
EndPoint mysocket;
EventDispatcher gdispatcher;

class MyPacketReceiver : public InputNotificationHandler
{
public:
	MyPacketReceiver(EndPoint & mysocket):
	socket_(mysocket),
	pNextPacket_(new TCPPacket())
	{
	}
	
	~MyPacketReceiver()
	{
	}
	
	EventDispatcher& dispatcher(){return gdispatcher;}
private:
	virtual int handleInputNotification(int fd)
	{
		if (this->processSocket(/*expectingPacket:*/true))
		{
			while (this->processSocket(/*expectingPacket:*/false))
			{
				/* pass */;
			}
		}

		return 0;
	}
	
	bool processSocket(bool expectingPacket)
	{
		int len = pNextPacket_->recvFromEndPoint(socket_);
		if (len <= 0)
		{
			return this->checkSocketErrors(len, expectingPacket);
		}

		PacketPtr curPacket = pNextPacket_;
		pNextPacket_ = new TCPPacket();
		Address srcAddr = socket_.getRemoteAddress();
		Reason ret = this->processPacket(srcAddr, curPacket.get());

		if (ret != REASON_SUCCESS)
		{
			this->dispatcher().errorReporter().reportException(ret, srcAddr);
		}
		return true;
	}

	Reason processPacket(const Address & addr, Packet * p)
	{
		return REASON_SUCCESS;
	}
	
	bool checkSocketErrors(int len, bool expectingPacket)
	{
		// is len weird?
		if (len == 0)
		{
			WARNING_MSG("PacketReceiver::processPendingEvents: "
				"Throwing REASON_GENERAL_NETWORK (1)- %s\n",
				kbe_strerror());

			this->dispatcher().errorReporter().reportException(
					REASON_GENERAL_NETWORK);

			return true;
		}
			// I'm not quite sure what it means if len is 0
			// (0 => 'end of file', but with dgram sockets?)

	#ifdef _WIN32
		DWORD wsaErr = WSAGetLastError();
	#endif //def _WIN32

		// is the buffer empty?
		if (
	#ifdef _WIN32
			wsaErr == WSAEWOULDBLOCK
	#else
			errno == EAGAIN && !expectingPacket
	#endif
			)
		{
			return false;
		}

	#ifdef unix
		// is it telling us there's an error?
		if (errno == EAGAIN ||
			errno == ECONNREFUSED ||
			errno == EHOSTUNREACH)
		{
	#if defined(PLAYSTATION3)
			this->dispatcher().errorReporter().reportException(
					REASON_NO_SUCH_PORT);
			return true;
	#else
			Mercury::Address offender;

			if (socket_.getClosedPort(offender))
			{
				// If we got a NO_SUCH_PORT error and there is an internal
				// channel to this address, mark it as remote failed.  The logic
				// for dropping external channels that get NO_SUCH_PORT
				// exceptions is built into BaseApp::onClientNoSuchPort().
				if (errno == ECONNREFUSED)
				{
				}

				this->dispatcher().errorReporter().reportException(
						REASON_NO_SUCH_PORT, offender);

				return true;
			}
			else
			{
				WARNING_MSG("PacketReceiver::processPendingEvents: "
					"getClosedPort() failed\n");
			}
	#endif
		}
	#else
		if (wsaErr == WSAECONNRESET)
		{
			return true;
		}
	#endif // unix

		// ok, I give up, something's wrong
	#ifdef _WIN32
		WARNING_MSG("PacketReceiver::processPendingEvents: "
					"Throwing REASON_GENERAL_NETWORK - %d\n",
					wsaErr);
	#else
		WARNING_MSG("PacketReceiver::processPendingEvents: "
					"Throwing REASON_GENERAL_NETWORK - %s\n",
				kbe_strerror());
	#endif
		this->dispatcher().errorReporter().reportException(
				REASON_GENERAL_NETWORK);

		return true;
	}
private:
	EndPoint & socket_;
	PacketPtr pNextPacket_;
};

MyPacketReceiver* packetReceiver;

struct AvatarInfos
{
	uint32 dbid;
	std::string name;
	uint8 roleType;
	uint16 level;
};

struct vector3
{
	int32 x, y, z;
};

struct EntityInfos
{
	int32 entityID;  // �����������entityΨһID
	uint32 modelID;  // ģ��ID
	vector3 spawnPos; // �����ڵ�ͼλ��
	uint32 utype;    // �߻���д��ʵ��ID
	uint32 dialogID;    // �߻���д�ĶԻ�ID
};

// �Ի�ѡ��
struct DialogOption
{
	uint8 dialogType; // �Ի����
	uint32 dialogKey; // �Ի�Э��key
	std::string title; // ѡ�����
	int32 extraData; // ��չ��(����)
};

void init_network(void)
{
	Mercury::g_trace_packet = 3;

	mysocket.close();
	mysocket.socket(SOCK_DGRAM);
	mysocket.setbroadcast(true);
	if (!mysocket.good())
	{
		ERROR_MSG("NetworkInterface::recreateListeningSocket: couldn't create a socket\n");
		return;
	}
	
	srand(getSystemTime());
	std::string accountname = "kebiao";
	char ttt1[256];
	memset(ttt1, 0, 256);
	int nnn = rand() % 65535;
	nnn = 0;
	sprintf(ttt1, "%d", nnn);
	accountname += ttt1;

	while(1)
	{
		mysocket.close();
		mysocket.socket(SOCK_STREAM);
		if (!mysocket.good())
		{
			ERROR_MSG("NetworkInterface::recreateListeningSocket: couldn't create a socket\n");
			return;
		}
		
		packetReceiver = new MyPacketReceiver(mysocket);
		gdispatcher.registerFileDescriptor(mysocket, packetReceiver);

		printf("������������˿ں�:\n>>>");
		static int port = 0;
		if(port == 0)
			std::cin >> port;
		
		// ������Ϸ��½����
		printf("������Ϸ��½����\n");
		u_int32_t address;
		std::string ip = "192.168.4.205";
		mysocket.convertAddress(ip.c_str(), address );
		if(mysocket.connect(htons(port), address) == -1)
		{
			ERROR_MSG("NetworkInterface::recreateListeningSocket: connect server is error(%s)!\n", kbe_strerror());
			port = 0;
			continue;
		}
		
		// ���󴴽��˺�
		printf("���󴴽��˺�\n"); 
	//	mysocket.setnodelay(false);
		mysocket.setnonblocking(false);
		MessageID msgID = 0;
		MessageLength msgLength = 0;
		Mercury::Bundle bundle1;
		bundle1.newMessage(LoginappInterface::reqCreateAccount);

		std::string avatarname = "kebiao";
		char ttt[256];
		memset(ttt, 0, 256);
		sprintf(ttt, "%d", nnn++);
		avatarname += ttt;
		bundle1 << accountname;
		bundle1 << "123456";
		bundle1.send(mysocket);
		//::sleep(300);

		// �����˺ųɹ� failedcode == 0
		TCPPacket packet1;
		packet1.resize(65535);
		int len = mysocket.recv(packet1.data(), 65535);
		packet1.wpos(len);
		uint16 failedcode = 0;
		packet1 >> msgID;
		packet1 >> failedcode;
		printf("Client::onCreateAccountResult: �����˺�[%s]%s size(%d) failedcode=%u.\n", 
			accountname.c_str(), failedcode == 0 ? "�ɹ�" : "ʧ��",len, failedcode);

		// �ύ�˺����������¼
		printf("�ύ�˺����������¼\n");
		Mercury::Bundle bundle2;
		bundle2.newMessage(LoginappInterface::login);
		int8 tclient = 1;
		bundle2 << tclient;
		bundle2 << "phone";
		bundle2 << accountname;
		bundle2 << "123456";
		bundle2.send(mysocket);


		// ��ȡ���ص�����ip��ַ
		TCPPacket packet2;
		packet2.resize(65535);
		
		len = -1;
		len = mysocket.recv(packet2.data(), 65535);
		packet2.wpos(len);
		packet2.print_storage();
		uint16 iport;
		packet2 >> msgID;
		packet2 >> msgLength;
		packet2 >> ip;
		packet2 >> iport;
		printf("Client::onLoginSuccessfully: ��ȡ���ص�����ip��ַ size(%d) msgID=%u, ip:%s, port=%u.\n", len, msgID, ip.c_str(), iport);

		// ��������
		printf("��������\n");
		mysocket.close();
		mysocket.socket(SOCK_STREAM);
		mysocket.convertAddress(ip.c_str(), address );
		if(mysocket.connect(htons(iport), address) == -1)
		{
			printf("NetworkInterface::recreateListeningSocket: connect server is error(%s)!\n", kbe_strerror());
			port = 0;
			continue;
		}
		
		mysocket.setnonblocking(false);

		// �����¼����
		Mercury::Bundle bundle3;
		bundle3.newMessage(BaseappInterface::loginGateway);
		bundle3 << accountname;
		bundle3 << "123456";
		bundle3.send(mysocket);
		//::sleep(300);

		// ���������� Client::onCreatedProxies:���������Ѿ�������һ����ͻ��˹����Ĵ���Entity
		TCPPacket packet33;
		packet33.resize(65535);
		len = mysocket.recv(packet33.data(), 65535);
		packet33.wpos(len);

		uint64 uuid;
		ENTITY_ID eid;
		std::string entityType;
		MessageLength msgLen;
		packet33 >> msgID;

		if(msgID == 505) // ��¼ʧ��
		{
			packet33 >> failedcode;
			printf("��¼����ʧ��:msgID=%u, err=%u\n", msgID, failedcode);
			continue;
		}
		else
		{
			packet33 >> msgLen;
			packet33 >> uuid;
			packet33 >> eid;
			packet33 >> entityType;
			printf("Client::onCreatedProxies: size(%d) : msgID=%u, uuid:%"PRIu64", eid=%d, entityType=%s.\n", 
				len, msgID, uuid, eid, entityType.c_str());
		}
		
		printf("������������ѯ��ɫ�б�\n");
		// ������������ѯ��ɫ�б�
		Mercury::Bundle bundle44;
		bundle44.newMessage(BaseappInterface::onRemoteMethodCall);
		uint16 methodID = 10001;
		bundle44 << eid;
		bundle44 << methodID;
		bundle44.send(mysocket);
		//::sleep(300);

		// ��ʼ�����б�
		TCPPacket packet444;
		packet444.resize(65535);
		len = mysocket.recv(packet444.data(), 65535);
		packet444.wpos(len);
		packet444 >> msgID;
		packet444 >> msgLen;
		packet444 >> eid;
		packet444 >> methodID;
		uint32 size;

		std::vector<AvatarInfos> vargs;

		packet444 >> size;
		for(uint32 i=0; i<size; i++)
		{
			AvatarInfos ainfo;
			packet444 >> ainfo.dbid;
			packet444 >> ainfo.name;
			packet444 >> ainfo.roleType;
			packet444 >> ainfo.level;
			vargs.push_back(ainfo);
			printf("���ս�ɫ�б�:dbid=%u,name=%s,roleType=%u,level=%u\n", ainfo.dbid, ainfo.name.c_str(),ainfo.roleType, ainfo.level);
		}

		printf("����������󴴽���ɫ:%s\n", avatarname.c_str());

		// ����������󴴽���ɫ
		Mercury::Bundle bundle55;
		bundle55.newMessage(BaseappInterface::onRemoteMethodCall);
		methodID = 10002;
		bundle55 << eid;
		bundle55 << methodID;
		uint8 createType = 4;
		bundle55 << createType;
		bundle55 << avatarname;
		bundle55.send(mysocket);
		//::sleep(3000);

		// ��ʼ���մ������
		TCPPacket packet555;
		packet555.resize(65535);
		len = mysocket.recv(packet555.data(), 65535);
		packet555.wpos(len);
		packet555 >> msgID;
		packet555 >> msgLen;
		packet555 >> eid;
		packet555 >> methodID;
		// ������
		uint8 errorcode = 0;
		packet555 >> errorcode;
		AvatarInfos retainfo;
		packet555 >> retainfo.dbid;
		packet555 >> retainfo.name;
		packet555 >> retainfo.roleType;
		packet555 >> retainfo.level;
		printf("������ɫ���:������:%u,dbid=%u,name=%s,roleType=%u,level=%u\n", 
			errorcode, retainfo.dbid, retainfo.name.c_str(),retainfo.roleType, retainfo.level);

		printf("�����������ѡ��ĳ����ɫ������Ϸ\n");
		// �����������ѡ��ĳ����ɫ������Ϸ
		Mercury::Bundle bundle66;
		bundle66.newMessage(BaseappInterface::onRemoteMethodCall);
		methodID = 10004;
		bundle66 << eid;
		bundle66 << methodID;
		bundle66 << retainfo.dbid;
		bundle66.send(mysocket);
		//::sleep(3000);

		// �������˸�֪�����ٿͻ����˺�entity��
		TCPPacket packet77;
		packet77.resize(65535);
		len = mysocket.recv(packet77.data(), 65535);
		packet77.wpos(len);
		//packet77 >> msgID;
		//packet77 >> eid;
		//printf("Client::onEntityDestroyed: �������˸�֪�����ٿͻ����˺�entity�� size(%d) : msgID=%u, eid=%d.\n", 
		//	len, msgID, eid);
		 
		//::sleep(3000);

		// ���������� Client::onCreatedProxies:���������Ѿ�������һ����ͻ��˹����Ĵ���player
		//TCPPacket packet88;
		//len = mysocket.recv(packet88.data(), 65535);
		packet77 >> msgID;
		packet77 >> msgLen;
		packet77 >> uuid;
		packet77 >> eid;
		packet77 >> entityType;
		printf("Client::onCreatedProxies: size(%d) : msgID=%u, uuid:%"PRIu64", eid=%d, entityType=%s.\n", 
			len, msgID, uuid, eid, entityType.c_str());

		// ��ʼ��������
		TCPPacket packet88;
		packet88.resize(65535);
		len = mysocket.recv(packet88.data(), 65535);
		packet88.wpos(len);
		packet88 >> msgID;
		packet88 >> msgLen;
		packet88 >> eid;

		uint16 propertyID = 0;
		uint32 spaceUType;
		uint16 level;
		std::string name;
		SPACE_ID spaceID;

		while(packet88.opsize() > 0)
		{
			packet88 >> propertyID;

			if(41001 == propertyID)
			{
				packet88 >> spaceUType;
			}
			else if(41002 == propertyID)
			{
				packet88 >> level;
			}
			else if(41003 == propertyID)
			{
				packet88 >> name;
			}
			else if(40000 == propertyID)
			{
				int32 x, y, z;
				uint32 listlen;
				
				packet88 >> listlen;
				packet88 >> x;
				packet88 >> y ;
				packet88 >> z;
				z = 0;
			}
			else if(40001 == propertyID)
			{
				int32 x, y, z;
				uint32 listlen;
				
				packet88 >> listlen;
				packet88 >> x;
				packet88 >> y ;
				packet88 >> z;
			}
			else if(40002 == propertyID)
			{
				packet88 >> spaceID;
			}
			else if(40005 == propertyID)
			{
				uint32 listlen;
				packet88 >> listlen;
			}
			else if(41006 == propertyID)
			{
				uint32 model;
				packet88 >> model;
			}
			else if(41008 == propertyID)
			{
				uint32 headmodel;
				packet88 >> headmodel;
			}
		}
		
		printf("�������·�����:spaceUType=%u, level=%u.\n", spaceUType, level);

		// ��ʼ���ս���������Ϣ
		TCPPacket packet99;
		packet99.resize(65535);
		len = mysocket.recv(packet99.data(), 65535);
		packet99.wpos(len);
		packet99 >> msgID;
		packet99 >> eid >> spaceID;
		printf("!!!��ҽ�������:spaceUType=%u, level=%u.\n", spaceUType, level);
		uint32 dialogID = 0;
		// �������·�entity
		if(packet99.opsize() == 0)
		{
			packet99.clear(false);
			len = mysocket.recv(packet99.data(), 65535);
			packet99.wpos(len);
		}

		ENTITY_ID targetID = 0;

		while(packet99.opsize() > 0)
		{
			ENTITY_ID eid1 = 0;
			// ��ʼ��������
			packet99 >> msgID;
			packet99 >> msgLen;
			packet99 >> eid1;
			

			uint16 propertyID = 0;
			uint32 spaceUType;
			uint16 level;
			std::string name;
			SPACE_ID spaceID;
			uint32 utype = 0;
			uint32 dialogID1;
			uint32 model;
			uint32 endpos = msgLen + packet99.rpos() - 4;
			if(endpos > packet99.wpos())
			{
				packet99.clear(false);
				len = mysocket.recv(packet99.data(), 65535);
				break;
			}

			while(packet99.rpos() < endpos)
			{
				packet99 >> propertyID;

				if(41001 == propertyID)
				{
					packet99 >> spaceUType;
				}
				else if(41002 == propertyID)
				{
					packet99 >> level;
				}
				else if(41003 == propertyID)
				{
					packet99 >> name;
				}
				else if(40000 == propertyID)
				{
					int32 x, y, z;
					uint32 listlen;
					
					packet99 >> listlen;
					packet99 >> x;
					packet99 >> y ;
					packet99 >> z;
					z = 0;
				}
				else if(40001 == propertyID)
				{
					int32 x, y, z;
					uint32 listlen;
					
					packet99 >> listlen;
					packet99 >> x;
					packet99 >> y ;
					packet99 >> z;
				}
				else if(40002 == propertyID)
				{
					packet99 >> spaceID;
				}
				else if(41004 == propertyID)
				{
					packet99 >> utype;
					if(utype > 0)
						targetID = eid1;
				}
				else if(41005 == propertyID)
				{
					uint32 listlen;
					packet99 >> listlen;
				}
				else if(41006 == propertyID)
				{
					packet99 >> model;
				}
				else if(41007 == propertyID)
				{
					packet99 >> dialogID1;

					if(dialogID == 0)
						dialogID = dialogID1;
				}
				else if(41008 == propertyID)
				{
					uint32 headmodel;
					packet88 >> headmodel;
				}
			}
			printf("�������·�����:name=%s, utype=%u. dialogID=%u\n", name.c_str(), utype, dialogID);

			packet99 >> msgID;
			packet99 >> eid1;
			SPACE_ID spaceID1;
			packet99 >> spaceID1;
			printf("!!!entity��������:id=%d.\n", eid1);
		};

		
		// ������������NPC�Ի�
		Mercury::Bundle bundle9999;
		bundle9999.newMessage(BaseappInterface::onRemoteCallCellMethodFromClient);
		methodID = 11003;
		bundle9999 << eid;
		bundle9999 << methodID;
		bundle9999 << targetID;
		
		bundle9999 << dialogID;
		bundle9999.send(mysocket);
		
		bool readover = false;
		std::wcout.imbue(std::locale("chs"));
		while(!readover)
		{
			
			TCPPacket packet999;
			packet999.resize(65535);
			len = mysocket.recv(packet999.data(), 65535);
			while(packet999.opsize() > 0)
			{
				packet999.wpos(len);
				packet999 >> msgID;
				packet999 >> msgLen;
				packet999 >> eid;
				packet999 >> methodID;
				
				if(methodID == 10102)
				{
					std::string body;
					packet999.readBlob(body);
					std::wstring outstr;
					utf82wchar(body, outstr);
					std::wcout << "�Ի�����:" << outstr << std::endl;
					readover = true;
					if(false)
					{
						// ��������ʼ���͵�ĳ���� ��ʼ���ս���������Ϣ
						TCPPacket packet99;
						packet99.resize(65535);
						len = mysocket.recv(packet99.data(), 65535);
						packet99.wpos(len);
						packet99 >> msgID;
						packet99 >> eid >> spaceID;

						printf("!!!����뿪����:spaceUType=%u, level=%u.\n", spaceUType, level);
					}
					
					if(false)
					{
						// ��������ʼ���͵�ĳ���� ��ʼ���ս���������Ϣ
						TCPPacket packet99;
						packet99.resize(65535);
						len = mysocket.recv(packet99.data(), 65535);
						packet99.wpos(len);
						packet99 >> msgID;
						packet99 >> eid >> spaceID;
						printf("!!!��ҽ�������:spaceUType=%u, level=%u.\n", spaceUType, level);
					}
					break;
				}

				DialogOption doption;
				
				packet999 >> doption.dialogType;
				packet999 >> doption.dialogKey;
				// ������ʵ�ǴӰ�����ȡ��uint32��size�� Ȼ��ȡ��size���ȵ�bytedatas
				packet999.readBlob(doption.title);
				packet999 >> doption.extraData;
				std::wstring outstr;
				utf82wchar(doption.title, outstr);
				std::wcout << "�Ի�ѡ��(" << doption.dialogKey << "):" << outstr  << std::endl;
			}
		};


		// �����������ʩ�ż���
		Mercury::Bundle bundle100;
		bundle100.newMessage(BaseappInterface::onRemoteCallCellMethodFromClient);
		methodID = 11001;
		bundle100 << eid;
		bundle100 << methodID;
		int32 skillID = 10011001;
		// ����ID
		bundle100 << skillID;
		// Ŀ��ID
		bundle100 << targetID;
		bundle100.send(mysocket);
	};
}

int main(int argc, char* argv[])
{
	Mercury::FixedMessages::getSingleton().loadConfig("../../res/server/fixed_mercury_messages.xml");
	DebugHelper::initHelper(UNKNOWN_COMPONENT_TYPE);
    INFO_MSG("��ã�log4cxx---%d!---%s", 1, __FUNCTION__);
	//LOG4CXX_INFO("Attempted to " << " in MemoryStream (pos:" << 111 <<  "size: " << 222 << ").\n");
	init_network();
	gdispatcher.processUntilBreak();
	getchar();
	return 0; 
}
