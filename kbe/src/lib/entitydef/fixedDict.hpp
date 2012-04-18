/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
#ifndef __FIXED_DICT_H__
#define __FIXED_DICT_H__
#include <string>
#include "dataType.hpp"
#include "helper/debug_helper.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "pyscript/map.hpp"
#include "pyscript/pickler.hpp"

namespace KBEngine{

class FixedDict : public script::Map
{		
	/** ���໯ ��һЩpy�������������� */
	INSTANCE_SCRIPT_HREADER(FixedDict, Map)
public:	
	static PyMappingMethods mappingMethods;

	FixedDict(DataType* dataType);
	FixedDict(DataType* dataType, std::string& strDictInitData);
	virtual ~FixedDict();

	const DataType* getDataType(void){ return _dataType; }

	/** ֧��pickler ���� */
	static PyObject* __reduce_ex__(PyObject* self, PyObject* protocol);
	/** unpickle���� */
	static PyObject* __unpickle__(PyObject* self, PyObject* args);
	
	/** �ű�����װʱ������ */
	static void onInstallScript(PyObject* mod);
	
	/** map����������� */
	static PyObject* mp_subscript(PyObject* self, PyObject* key);
	static int mp_ass_subscript(PyObject* self, PyObject* key, PyObject* value);
	static int mp_length(PyObject* self);

	/** ��ʼ���̶��ֵ�*/
	void initialize(std::string strDictInitData);
	/** ������ݸı� */
	bool checkDataChanged(const char* keyName, PyObject* value, bool isDelete = false);
	
	/** �����ֵ����ݵ��Լ��������� */
	PyObject* update(PyObject* args);
protected:
	FixedDictType* _dataType;
} ;

}
#endif