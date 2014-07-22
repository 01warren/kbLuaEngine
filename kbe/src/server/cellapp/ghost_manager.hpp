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

#ifndef __GHOST_MANAGER_HANDLER__
#define __GHOST_MANAGER_HANDLER__

// common include
#include "helper/debug_helper.hpp"
#include "cstdkbe/cstdkbe.hpp"

namespace KBEngine{

/*
	1: cell1: entity(1) is real, ����GhostManager�д����entityIDs_���м��  (������ghost����)

	2: cell2: entity(1) is ghost�����cell2������Ǩ���ߣ�����Ҫ��ghost_route_��ʱ����һ��·�ɵ�ַ�� ·�������һ���հ�����һ��ʱ�������
	                    ����ڼ���һЩ����ת�������� ��ô�Ҳ���entity�Ͳ�ѯ·�ɱ�������ת����ghostEntity(����real������Ҫ����������ghost)��

	3: cell1: entity(1) �������Ǩ�Ƶ�cell3�� ����Ҫ��ghost_route_��ʱ����һ��·�ɵ�ַ�� ·�������һ���հ�����һ��ʱ�������
	                    ����ڼ���һЩ����ת�������� ��ô�Ҳ���entity�Ͳ�ѯ·�ɱ�������ת����realEntity��
*/
class GhostManager : public Task
{
public:
	GhostManager(Mercury::NetworkInterface & networkInterface);
	~GhostManager();
	
	bool process();
private:
	Mercury::NetworkInterface & networkInterface_;
	
	// ���д���ghost����real���ֵ����entity
	std::vector<ENTITY_ID> 	entityIDs_;
	
	// ghost·�ɣ� �ֲ�ʽ����ĳЩʱ���޷���֤ͬ���� ��ô�ڱ����ϵ�ĳЩentity��Ǩ�����˵�
	// ʱ����ܻỹ���յ�һЩ������Ϣ�� ��Ϊ����app���ܻ��޷������õ�Ǩ�Ƶ�ַ�� ��ʱ����
	// �����ڵ�ǰapp�Ͻ�Ǩ���ߵ�entityָ�򻺴�һ�£� ��������Ϣ�������ǿ��Լ���ת�����µĵ�ַ
	std::map<ENTITY_ID, COMPONENT_ID> ghost_route_;

};


}
#endif
