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


#ifndef __KBE_CONSOLE_HELPER_HPP__
#define __KBE_CONSOLE_HELPER_HPP__
#include "network/message_handler.hpp"
#include "network/channel.hpp"

#define CONSOLE_COMMANDCB_MSGID 65500
#define CONSOLE_LOG_MSGID 65501

namespace KBEngine{
namespace ConsoleInterface{
	
	class ConsoleExecCommandCBMessageHandlerArgs1 : public Mercury::MessageArgs	
	{																
	public:															
		std::string strarg;										
	public:															
		ConsoleExecCommandCBMessageHandlerArgs1():Mercury::MessageArgs(){}						
		ConsoleExecCommandCBMessageHandlerArgs1(std::string init_strarg):				
		Mercury::MessageArgs(),										
		strarg(init_strarg)									
		{}															
		~ConsoleExecCommandCBMessageHandlerArgs1(){}											
																	
		static void staticAddToBundle(Mercury::Bundle& s,			
								std::string init_strarg)			
		{															
			s << init_strarg;									
		}															
		static void staticAddToStream(MemoryStream& s,			
								std::string init_strarg)			
		{															
			s << init_strarg;									
		}															
		virtual int32 msgsize(void)									
		{															
			return MERCURY_VARIABLE_MESSAGE;								
		}															
		virtual void addToStream(MemoryStream& s)					
		{															
			s << strarg;											
		}															
		virtual void createFromStream(MemoryStream& s)				
		{															
			s >> strarg;											
		}															
	};																
				
	class ConsoleExecCommandCBMessageHandler : public Mercury::MessageHandler
	{
	public:
		ConsoleExecCommandCBMessageHandler()
		{
			onInstall();
		}

		virtual void onInstall()
		{
			// ǿ������Э��ID
			msgID = CONSOLE_COMMANDCB_MSGID;
			msgLen = MERCURY_VARIABLE_MESSAGE;
		}
		virtual void handle(Mercury::Channel* pChannel, MemoryStream& s)
		{
		};
	};


	class ConsoleLogMessageHandlerArgsStream : public Mercury::MessageArgs	
	{																								
	public:															
		ConsoleLogMessageHandlerArgsStream():Mercury::MessageArgs(){}																			
		~ConsoleLogMessageHandlerArgsStream(){}											
																															
		virtual int32 msgsize(void)									
		{															
			return MERCURY_VARIABLE_MESSAGE;								
		}		
		virtual void addToStream(MemoryStream& s)					
		{																									
		}															
		virtual void createFromStream(MemoryStream& s)				
		{																										
		}	
	};																
				
	class ConsoleLogMessageHandler : public Mercury::MessageHandler
	{
	public:
		ConsoleLogMessageHandler()
		{
			onInstall();
		}

		virtual void onInstall()
		{
			// ǿ������Э��ID
			msgID = CONSOLE_LOG_MSGID;
			msgLen = MERCURY_VARIABLE_MESSAGE;
		}

		virtual void handle(Mercury::Channel* pChannel, MemoryStream& s)
		{
		};
	};
}
}

#endif // __KBE_CONSOLE_HELPER_HPP__
