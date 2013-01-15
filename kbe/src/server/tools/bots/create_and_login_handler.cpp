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

#include "bots.hpp"
#include "client.hpp"
#include "create_and_login_handler.hpp"
#include "network/network_interface.hpp"
#include "network/event_dispatcher.hpp"
#include "network/address.hpp"
#include "network/bundle.hpp"
#include "cstdkbe/memorystream.hpp"
#include "server/serverconfig.hpp"

namespace KBEngine { 

//-------------------------------------------------------------------------------------
CreateAndLoginHandler::CreateAndLoginHandler()
{
	timerHandle_ = Bots::getSingleton().getNetworkInterface().dispatcher().addTimer(
							1 * 1000000, this);
}

//-------------------------------------------------------------------------------------
CreateAndLoginHandler::~CreateAndLoginHandler()
{
	timerHandle_.cancel();
}

//-------------------------------------------------------------------------------------
void CreateAndLoginHandler::handleTimeout(TimerHandle handle, void * arg)
{
	KBE_ASSERT(handle == timerHandle_);
	
	Bots& bots = Bots::getSingleton();
	uint32 createCount = bots.reqCreateAndLoginTotalCount() - bots.clients().size();

	time_t t = time(NULL);
	uint64 accountID = t * 100000;

	while(createCount > 0)
	{
		Client* pClient = new Client(KBEngine::StringConv::val2str(accountID++));
		Bots::getSingleton().addClient(pClient);
	}
}

//-------------------------------------------------------------------------------------

}
