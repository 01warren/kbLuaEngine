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

#ifndef __KBE_RANGE_LIST_HPP__
#define __KBE_RANGE_LIST_HPP__

#include "helper/debug_helper.hpp"
#include "cstdkbe/cstdkbe.hpp"	

namespace KBEngine{

class RangeNode;

class RangeList
{
public:
	RangeList();
	~RangeList();

	/**
		��list�в���ڵ�
	*/
	bool insert(RangeNode* pNode);

	/**
		���ڵ��list���Ƴ�
	*/
	bool remove(RangeNode* pNode);

	/**
		��ĳ���ڵ��б䶯ʱ����Ҫ��������list�е�
		���λ�õ���Ϣ
	*/
	void update(RangeNode* pNode);

	INLINE RangeNode * pFirstXNode()const;
	INLINE RangeNode * pLastXNode()const;

	INLINE RangeNode * pFirstZNode()const;
	INLINE RangeNode * pLastZNode()const;
private:
	// �������βָ��
	RangeNode* first_x_rangeNode_, *last_x_rangeNode_;
	RangeNode* first_z_rangeNode_, *last_z_rangeNode_;
};

}

#ifdef CODE_INLINE
#include "range_list.ipp"
#endif
#endif
