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
	��Դ��������
*/
#ifndef __RESMGR_H__
#define __RESMGR_H__

// common include
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/singleton.hpp"
#include "xmlplus/xmlplus.hpp"	
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
#endif
	
namespace KBEngine{


class Resmgr : public Singleton<Resmgr>
{
public:
	// ���滷������
	struct KBEEnv
	{
		std::string root;
		std::string res_path;
		std::string hybrid_path;
	};
public:
	Resmgr();
	~Resmgr();
	
	bool initialize();

	const Resmgr::KBEEnv& getEnv() { return kb_env_; }

	// ����Դ·����(����������ָ����)ƥ�䵽��������Դ��ַ
	std::string matchRes(std::string path);

	const std::vector<std::string>& respaths() { return respaths_; }

	void pirnt(void);

	bool isInit(){ return isInit_; }

	std::string getPySysResPath();
private:
	KBEEnv kb_env_;
	std::vector<std::string> respaths_;
	bool isInit_;
};

}
#endif
