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


#ifndef _ARRAY_TYPE_H
#define _ARRAY_TYPE_H
#include <string>
#include "datatype.hpp"
#include "pyscript/sequence.hpp"
#include "pyscript/pickler.hpp"

namespace KBEngine{

class Array : public script::Sequence
{		
	/** ���໯ ��һЩpy�������������� */
	INSTANCE_SCRIPT_HREADER(Array, Sequence)
public:	
	Array(DataType* dataType);
	Array(DataType* dataType, std::string& strInitData);
	Array(DataType* dataType, PyObject* pyInitData);
	virtual ~Array();

	const DataType* getDataType(void){ return _dataType; }
	
	/** ��ʼ���̶��ֵ�*/
	void initialize(std::string strInitData);
	void initialize(PyObject* pyInitData);

	/** ֧��pickler ���� */
	static PyObject* __py_reduce_ex__(PyObject* self, PyObject* protocol);
	/** unpickle���� */
	static PyObject* __unpickle__(PyObject* self, PyObject* args);
	
	/** �ű�����װʱ������ */
	static void onInstallScript(PyObject* mod);
	
	/** һ��Ϊһ��list����Ĳ����ӿ� */
	static PyObject* __py_append(PyObject* self, PyObject* args, PyObject* kwargs);	
	static PyObject* __py_count(PyObject* self, PyObject* args, PyObject* kwargs);
	static PyObject* __py_extend(PyObject* self, PyObject* args, PyObject* kwargs);	
	static PyObject* __py_index(PyObject* self, PyObject* args, PyObject* kwargs);
	static PyObject* __py_insert(PyObject* self, PyObject* args, PyObject* kwargs);	
	static PyObject* __py_pop(PyObject* self, PyObject* args, PyObject* kwargs);
	static PyObject* __py_remove(PyObject* self, PyObject* args, PyObject* kwargs);
	
	bool isSameType(PyObject* pyValue);

	virtual PyObject* onSetItem(PyObject* pyItem);
protected:
	ArrayType* _dataType;
} ;

}
#endif
