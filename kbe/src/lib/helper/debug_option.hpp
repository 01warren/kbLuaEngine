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


#ifndef __KBE_DEBUG_OPTION_HPP__
#define __KBE_DEBUG_OPTION_HPP__
#include "cstdkbe/cstdkbe.hpp"

namespace KBEngine{

namespace Mercury
{

/** 
	��������������ݰ��Ƿ�����Я��������Ϣ�� ������ĳЩǰ�˽������ʱ�ṩһЩ����
	 ���Ϊfalse��һЩ�̶����ȵ����ݰ���Я��������Ϣ�� �ɶԶ����н���
*/
extern bool g_packetAlwaysContainLength;

/**
	�Ƿ���Ҫ���κν��պͷ��͵İ����ı������log���ṩ����
		0: �����
		1: 16�������
		2: �ַ������
		3: 10�������
*/
extern uint8 g_trace_packet;

}

/**
	�Ƿ����entity�Ĵ����� �ű���ȡ���ԣ� ��ʼ�����Եȵ�����Ϣ��
*/
extern bool g_debugEntity;

}

#endif // __KBE_DEBUG_OPTION_HPP__
