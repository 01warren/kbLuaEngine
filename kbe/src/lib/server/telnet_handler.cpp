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


#include "telnet_handler.hpp"
#include "telnet_server.hpp"
#include "network/bundle.hpp"
#include "network/endpoint.hpp"
#include "pyscript/script.hpp"

#ifndef CODE_INLINE
#include "telnet_handler.ipp"
#endif

namespace KBEngine { 

char _g_state_str[][256] = {
	"password",
	"root",
	"python"
};

/*
	��ʽ: echo "\033[�ֱ�����ɫ;������ɫm�ַ���\033[0m" 

	����: 
	echo "\033[41;36m something here \033[0m"  

	����41��λ�ô����ɫ, 36��λ���Ǵ����ֵ���ɫ 


	��Щascii code �Ƕ���ɫ���õ�ʼĩ.  
	\033[ ; m ���� \033[0m  



	�ֱ�����ɫ��Χ:40----49 
	40:�� 
	41:��� 
	42:�� 
	43:��ɫ 
	44:��ɫ 
	45:��ɫ 
	46:���� 
	47:��ɫ 

	����ɫ:30-----------39 
	30:�� 
	31:�� 
	32:�� 
	33:�� 
	34:��ɫ 
	35:��ɫ 
	36:���� 
	37:��ɫ 

	\33[0m �ر���������  
	\33[1m ���ø�����  
	\33[4m �»���  
	\33[5m ��˸  
	\33[7m ����  
	\33[8m ����  
	\33[30m -- \33[37m ����ǰ��ɫ  
	\33[40m -- \33[47m ���ñ���ɫ  
	\33[nA �������n��  
	\33[nB �������n��  
	\33[nC �������n��  
	\33[nD �������n��  
	\33[y;xH���ù��λ��  
	\33[2J ����  
	\33[K ����ӹ�굽��β������  
	\33[s ������λ��  
	\33[u �ָ����λ��  
	\33[?25l ���ع��  
	\33[?25h ��ʾ���  

	ʹ�ø�ʽ�ܸ����ӣ� 
	^[[..m;..m;..m;..m
	���磺 \033[2;7;1m����\033[2;7;0m
*/

#define TELNET_CMD_LEFT							"\033[D"			// ��
#define TELNET_CMD_RIGHT						"\033[C"			// ��
#define TELNET_CMD_UP							"\033[A"			// ��
#define TELNET_CMD_DOWN							"\033[B"			// ��

#define TELNET_CMD_DEL							"\033[K"			// ɾ���ַ�
#define TELNET_CMD_NEWLINE						"\r\n"				// ����
#define TELNET_CMD_MOVE_FOCUS_LEFT_MAX			"\33[9999999999D"	// ���ƹ�굽��ǰ��
#define TELNET_CMD_MOVE_FOCUS_RIGHT_MAX			"\33[9999999999C"	// ���ƹ�굽�����

//-------------------------------------------------------------------------------------
TelnetHandler::TelnetHandler(Mercury::EndPoint* pEndPoint, TelnetServer* pTelnetServer, TELNET_STATE defstate):
buffer_(),
command_(),
historyCommand_(),
historyCommandIndex_(0),
pEndPoint_(pEndPoint),
pTelnetServer_(pTelnetServer),
state_(defstate),
currPos_(0)
{
}

//-------------------------------------------------------------------------------------
TelnetHandler::~TelnetHandler(void)
{
}

//-------------------------------------------------------------------------------------
std::string TelnetHandler::getInputStartString()
{
	return(boost::format("[%1%@%2% ~]%3% ") % COMPONENT_NAME_EX(g_componentType) % 
		_g_state_str[(int)state_] % (state_ == TELNET_STATE_PYTHON ? " >>>" : "#")).str();
}

//-------------------------------------------------------------------------------------
std::string TelnetHandler::getWelcome()
{
	return (boost::format("\033[1;32mwelcome to %1% \r\n"
			"Version: %2%. "
			"Config: %3%. "
			"Built: %4% %5%. "
			"AppUID: %6%. "
			"UID: %7%. "
			"PID: %8%"
			"\r\n---------------------------------------------"
			"%9%"
			"\r\n---------------------------------------------"
			" \033[0m\r\n%10%") %
		COMPONENT_NAME_EX(g_componentType) % KBEVersion::versionString().c_str() %
		KBE_CONFIG % __TIME__ % __DATE__ %
		g_componentID % getUserUID() % getProcessPID() % help() % getInputStartString()).str();
}

//-------------------------------------------------------------------------------------
std::string TelnetHandler::help()
{
	return 	"\033[1;32m\r\nCommand List:"
		"\r\n[:help        ]: list commands."
		"\r\n[:python      ]: python console."
		"\r\n[:root        ]: return to the root layer."
		"\r\n[:cprofile    ]: collects and reports the internal c++ profiles \r\n\t\tof a server process over a period of time."
		"\r\n\t\t usage: \":cprofile 30\""
		"\r\n[:pyprofile    ]: collects and reports the python profiles \r\n\t\tof a server process over a period of time."
		"\r\n\t\t usage: \":pyprofile 30\""
		"\r\n[:eventprofile]: a server process over a period of time, \r\n\t\tcollects and reports the all non-volatile cummunication \r\n\t\tdown to the client."
		"\r\n\t\t usage: \":eventprofile 30\""
		"\r\n\r\n\033[0m";
};

//-------------------------------------------------------------------------------------
void TelnetHandler::historyCommandCheck()
{
	if(historyCommand_.size() > 50)
		historyCommand_.pop_front();

	if(historyCommandIndex_ < 0)
		historyCommandIndex_ = historyCommand_.size() - 1;

	if(historyCommandIndex_ > (int)historyCommand_.size() - 1)
		historyCommandIndex_ = 0; 
}

//-------------------------------------------------------------------------------------
std::string TelnetHandler::getHistoryCommand(bool isNextCommand)
{
	if(isNextCommand)
		historyCommandIndex_++;
	else
		historyCommandIndex_--;

	historyCommandCheck();

	if(historyCommand_.size() == 0)
		return "";

	return historyCommand_[historyCommandIndex_];
}

//-------------------------------------------------------------------------------------
int	TelnetHandler::handleInputNotification(int fd)
{
	KBE_ASSERT((*pEndPoint_) == fd);

	char data[1024] = {0};
	int recvsize = pEndPoint_->recv(data, sizeof(data));

	if(recvsize == -1)
	{
		return 0;
	}
	else if(recvsize == 0)
	{
		onRecvInput();
		pTelnetServer_->onTelnetHandlerClosed(fd, this);
		return 0;
	}
	
	for(int i = 0; i < recvsize; i++)
	{
		buffer_.push_back(data[i]);
	}

	onRecvInput();
	return 0;
}

//-------------------------------------------------------------------------------------
void TelnetHandler::onRecvInput()
{
	while (buffer_.size() > 0)
	{
		int c = (unsigned char)buffer_.front();
		buffer_.pop_front();

		switch(c)
		{
		case '\r':
			if(buffer_.size() > 0)
			{
				int cc = (unsigned char)buffer_.front();
				if(cc == '\n')
				{
					buffer_.pop_front();
					processCommand();
					command_ = "";
					sendNewLine();
				}
			}
			break;
		case 8:		// �˸�
			sendDelChar();
			break;
		case ' ':	// �ո�
		default:
			{
				std::string s = "";
				s += c;
				command_.insert(currPos_, s);
				currPos_++;
				
				if(!checkUDLR())
				{
					if(currPos_ != command_.size())
					{
						s = command_.substr(currPos_, command_.size() - currPos_);
						s += (boost::format("\33[%1%D") % s.size()).str();
						pEndPoint_->send(s.c_str(), s.size());
					}
				}
				break;
			}
		};
	}
}

//-------------------------------------------------------------------------------------
bool TelnetHandler::checkUDLR()
{
	if(command_.find(TELNET_CMD_UP) != -1)		// �� 
	{
		pEndPoint_->send(TELNET_CMD_MOVE_FOCUS_LEFT_MAX, strlen(TELNET_CMD_MOVE_FOCUS_LEFT_MAX));
		sendDelChar();
		std::string startstr = getInputStartString();
		pEndPoint_->send(startstr.c_str(), startstr.size());
		resetStartPosition();
		std::string s = getHistoryCommand(false);
		pEndPoint_->send(s.c_str(), s.size());
		command_ = s;
		buffer_.clear();
		currPos_ = s.size();
		return true;
	}
	else if(command_.find(TELNET_CMD_DOWN) != -1)	// ��
	{
		pEndPoint_->send(TELNET_CMD_MOVE_FOCUS_LEFT_MAX, strlen(TELNET_CMD_MOVE_FOCUS_LEFT_MAX));
		sendDelChar();
		std::string startstr = getInputStartString();
		pEndPoint_->send(startstr.c_str(), startstr.size());
		resetStartPosition();
		std::string s = getHistoryCommand(true);
		pEndPoint_->send(s.c_str(), s.size());
		command_ = s;
		buffer_.clear();
		currPos_ = s.size();
		return true;
	}
	else if(command_.find(TELNET_CMD_RIGHT) != -1)	// ��
	{
		int cmdlen = strlen(TELNET_CMD_RIGHT);
		currPos_-= cmdlen;
		command_.erase(command_.find(TELNET_CMD_RIGHT), cmdlen);

		if(currPos_ < (int)command_.size())
		{
			currPos_++;
			pEndPoint_->send(TELNET_CMD_RIGHT, cmdlen);
		}
		return true;
	}
	else if(command_.find(TELNET_CMD_LEFT) != -1)	// �� 
	{
		int cmdlen = strlen(TELNET_CMD_LEFT);
		currPos_-= (int)(cmdlen + 1);
		if(currPos_ < 0)
		{
			currPos_ = 0;
		}
		else
		{
			pEndPoint_->send(TELNET_CMD_LEFT, cmdlen);
		}

		command_.erase(command_.find(TELNET_CMD_LEFT), cmdlen);
		return true;
	}

	return false;
}

//-------------------------------------------------------------------------------------
void TelnetHandler::processCommand()
{
	if(command_.size() == 0)
		return;

	bool logcmd = true;
	for(int i=0; i<(int)historyCommand_.size(); i++)
	{
		if(historyCommand_[i] == command_)
		{
			logcmd = false;
			break;
		}
	}

	if(logcmd)
	{
		historyCommand_.push_back(command_);
		historyCommandCheck();
		historyCommandIndex_ = historyCommand_.size() - 1;
	}

	if(command_ == ":python")
	{
		if(pTelnetServer_->pScript() == NULL)
			return;

		state_ = TELNET_STATE_PYTHON;
		return;
	}
	else if(command_ == ":help")
	{
		std::string str = help();
		pEndPoint_->send(str.c_str(), str.size());
		return;
	}
	else if(command_ == ":root")
	{
		state_ = TELNET_STATE_ROOT;
		return;
	}
	else if(command_ == ":cprofile")
	{
		return;
	}
	else if(command_ == ":pyprofile")
	{
		return;
	}
	else if(command_ == ":eventprofile")
	{
		return;
	}

	if(state_ == TELNET_STATE_PYTHON)
	{
		processPythonCommand();
		return;
	}
	else if(state_ == TELNET_STATE_PASSWD)
	{
		if(command_ == pTelnetServer_->passwd())
		{
			state_ = (TELNET_STATE)pTelnetServer_->deflayer();
			std::string s = getWelcome();
			pEndPoint_->send(s.c_str(), s.size());
			sendEnter();
		}

		return;
	}
}

//-------------------------------------------------------------------------------------
void TelnetHandler::processPythonCommand()
{
	if(pTelnetServer_->pScript() == NULL || command_.size() == 0)
		return;
	
	PyObject* pycmd = PyUnicode_DecodeUTF8(command_.data(), command_.size(), NULL);
	if(pycmd == NULL)
	{
		SCRIPT_ERROR_CHECK();
		return;
	}

	DEBUG_MSG(boost::format("TelnetHandler::processPythonCommand: size(%1%), command=%2%.\n") % 
		command_.size() % command_);

	std::string retbuf = "";
	PyObject* pycmd1 = PyUnicode_AsEncodedString(pycmd, "utf-8", NULL);

	if(pTelnetServer_->pScript()->run_simpleString(PyBytes_AsString(pycmd1), &retbuf) == 0)
	{
		// ��������ظ��ͻ���
		Mercury::Bundle bundle;
		bundle << retbuf;
		bundle.send(*pEndPoint_);
		sendEnter();
	}

	Py_DECREF(pycmd);
	Py_DECREF(pycmd1);
}

//-------------------------------------------------------------------------------------
void TelnetHandler::sendEnter()
{
	pEndPoint_->send(TELNET_CMD_NEWLINE, strlen(TELNET_CMD_NEWLINE));
}

//-------------------------------------------------------------------------------------
void TelnetHandler::sendDelChar()
{
	if(command_.size() > 0)
	{
		command_.erase(command_.size() - 1, 1);
		currPos_--;
		pEndPoint_->send(TELNET_CMD_DEL, strlen(TELNET_CMD_DEL));
	}
	else
	{
		resetStartPosition();
	}
}

//-------------------------------------------------------------------------------------
void TelnetHandler::sendNewLine()
{
	std::string startstr = getInputStartString();
	pEndPoint_->send(startstr.c_str(), startstr.size());
	resetStartPosition();
	currPos_ = 0;
}

//-------------------------------------------------------------------------------------
void TelnetHandler::resetStartPosition()
{
	pEndPoint_->send(TELNET_CMD_MOVE_FOCUS_LEFT_MAX, strlen(TELNET_CMD_MOVE_FOCUS_LEFT_MAX));
	std::string startstr = getInputStartString();
	std::string backcmd = (boost::format("\33[%1%C") % startstr.size()).str();
	pEndPoint_->send(backcmd.c_str(), backcmd.size());
}

//-------------------------------------------------------------------------------------
}

