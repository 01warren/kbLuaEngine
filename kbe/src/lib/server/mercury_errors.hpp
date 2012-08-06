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

#ifndef __MERCURY_ERRORS_H__
#define __MERCURY_ERRORS_H__

// common include
#include "cstdkbe/cstdkbe.hpp"
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif

namespace KBEngine { 

#define MERCURY_ERR_SRV_NO_READY					-1			// ������û��׼���á�
#define MERCURY_ERR_SRV_OVERLOAD					-2			// ���������ع��ء�
#define MERCURY_ERR_ILLEGAL_LOGIN					-3			// �Ƿ���¼��

#define MERCURY_SUCCESS								0			// �Ƿ���¼��

#define MERCURY_ERR_NAME_PASSWORD					1			// �û����������벻��ȷ��
#define MERCURY_ERR_ACCOUNT_CREATE					2			// �����˺�ʧ�ܣ��Ѿ�����һ����ͬ���˺ţ���

}
#endif // __MERCURY_ERRORS_H__
