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


#ifndef __LOGINAPP_H__
#define __LOGINAPP_H__
	
// common include	
#include "server/kbemain.hpp"
#include "server/serverapp.hpp"
#include "server/idallocate.hpp"
#include "server/serverconfig.hpp"
#include "cstdkbe/timer.hpp"
#include "network/endpoint.hpp"

//#define NDEBUG
#include <map>	
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif
	
namespace KBEngine{

class Loginapp :	public ServerApp, 
					public Singleton<Loginapp>
{
public:
	enum TimeOutType
	{
		TIMEOUT_GAME_TICK = TIMEOUT_SERVERAPP_MAX + 1
	};
	
	Loginapp(Mercury::EventDispatcher& dispatcher, 
		Mercury::NetworkInterface& ninterface, 
		COMPONENT_TYPE componentType,
		COMPONENT_ID componentID);

	~Loginapp();
	
	bool run();
	
	virtual void handleTimeout(TimerHandle handle, void * arg);

	/* ��ʼ����ؽӿ� */
	bool initializeBegin();
	bool inInitialize();
	bool initializeEnd();
	void finalise();

	/** ����ӿ�
		�û���¼������
		clientType[COMPONENT_CLIENT_TYPE]: ǰ�����(�ֻ��� web�� pcexe��)
		clientData[str]: ǰ�˸�������(����������ģ� ���總���ֻ��ͺţ� ��������͵�)
		accountName[str]: �ʺ���
		password[str]: ����
	*/
	void login(Mercury::Channel* pChannel, MemoryStream& s);

	/*
		��¼ʧ��
		failedcode: ʧ�ܷ����� -1=������û��׼����, 
		                      0=���������ع���, 
							  1=�˺Ż������벻��ȷ
	*/
	void _loginFailed(Mercury::Channel* pChannel, std::string& accountName, int8 failedcode);
	
	/** ����ӿ�
		dbmgr���صĵ�¼�˺ż����
	*/
	void onLoginAccountQueryResultFromDbmgr(Mercury::Channel* pChannel, MemoryStream& s);

	/** ����ӿ�
		baseappmgr���صĵ�¼���ص�ַ
	*/
	void onLoginAccountQueryBaseappAddrFromBaseappmgr(Mercury::Channel* pChannel, uint32 addr, uint16 port);

	/** ����ӿ�
		dbmgr���ͳ�ʼ��Ϣ
		startGlobalOrder: ȫ������˳�� �������ֲ�ͬ���
		startGroupOrder: ��������˳�� ����������baseapp�еڼ���������
	*/
	void onDbmgrInitCompleted(Mercury::Channel* pChannel, int32 startGlobalOrder, int32 startGroupOrder);
protected:
};

}
#endif
