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

/*
	线程互诉体：
	用法:
		ThreadMutex tm;
		tm.lockMutex();
		....安全代码
		tm.unlockMutex();
		
		最好是配合ThreadGuard来使用
		在一个类中定义互诉体成员
		ThreadMutex tm;
		在需要保护的地方:
		void XXCLASS::func(void)
		{
			ThreadGuard tg(this->tm);
			下面的代码都是安全的
			...
		}
*/
#ifndef __THREADMUTEX_H__
#define __THREADMUTEX_H__
	
#include "common/common.h"
#include <assert.h>


namespace KBEngine{ namespace thread{

class ThreadMutex 
{
public:
	ThreadMutex(void)
	{
		THREAD_MUTEX_INIT(mutex_);
	}

	virtual ~ThreadMutex(void) 
	{ 
		THREAD_MUTEX_DELETE(mutex_);
	}	
	
	void lockMutex(void)
	{
		THREAD_MUTEX_LOCK(mutex_);
	}

	void unlockMutex(void)
	{
		THREAD_MUTEX_UNLOCK(mutex_);
	}

protected:
	THREAD_MUTEX mutex_;
};

}
}
#endif
