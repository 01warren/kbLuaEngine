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

#include "navigation.hpp"
#include "resmgr/resmgr.hpp"
#include "thread/threadguard.hpp"

namespace KBEngine{

KBE_SINGLETON_INIT(Navigation);

//-------------------------------------------------------------------------------------
Navigation::Navigation():
navhandles_(),
mutex_()
{
}

//-------------------------------------------------------------------------------------
Navigation::~Navigation()
{
	KBEngine::thread::ThreadGuard tg(&mutex_); 
	KBEUnordered_map<std::string, NavigationHandle*>::iterator iter = navhandles_.begin();
	for(; iter != navhandles_.end(); iter++)
	{
		delete iter->second;
	}

	navhandles_.clear();
}

//-------------------------------------------------------------------------------------
bool Navigation::removeNavigation(std::string name)
{
	KBEngine::thread::ThreadGuard tg(&mutex_); 
	KBEUnordered_map<std::string, NavigationHandle*>::iterator iter = navhandles_.find(name);
	if(navhandles_.find(name) != navhandles_.end())
	{
		NavMeshHandle* pNavMeshHandle = (NavMeshHandle*)iter->second;
		dtFreeNavMeshQuery(pNavMeshHandle->navmeshQuery);
		dtFreeNavMesh(pNavMeshHandle->navmesh);
		navhandles_.erase(iter);
		delete pNavMeshHandle;

		DEBUG_MSG(boost::format("Navigation::removeNavigation: (%1%) is destroyed!\n") % name);
		return true;
	}

	return false;
}

//-------------------------------------------------------------------------------------
NavigationHandle* Navigation::findNavigation(std::string name)
{
	KBEngine::thread::ThreadGuard tg(&mutex_); 
	KBEUnordered_map<std::string, NavigationHandle*>::iterator iter = navhandles_.find(name);
	if(navhandles_.find(name) != navhandles_.end())
	{
		return iter->second;
	}

	return NULL;
}

//-------------------------------------------------------------------------------------
bool Navigation::hasNavigation(std::string name)
{
	KBEngine::thread::ThreadGuard tg(&mutex_); 
	return navhandles_.find(name) != navhandles_.end();
}

//-------------------------------------------------------------------------------------
NavigationHandle* Navigation::loadNavigation(std::string name)
{
	KBEngine::thread::ThreadGuard tg(&mutex_); 
	if(name == "")
		return NULL;

	KBEUnordered_map<std::string, NavigationHandle*>::iterator iter = navhandles_.find(name);
	if(iter != navhandles_.end())
		return iter->second;

	NavigationHandle* pNavigationHandle_ = NavMeshHandle::create(name);
	navhandles_[name] = pNavigationHandle_;
	return pNavigationHandle_;
}

//-------------------------------------------------------------------------------------		
}
