/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
#ifndef _SCRIPT_MAP_H
#define _SCRIPT_MAP_H
#include "cstdkbe/cstdkbe.hpp"
#include "scriptobject.hpp"
#include "pickler.hpp"
	
namespace KBEngine{ namespace script{
class Map : public ScriptObject
{		
	/** ���໯ ��һЩpy�������������� */
	INSTANCE_SCRIPT_HREADER(Map, ScriptObject)
public:	
	static PyMappingMethods mappingMethods;

	Map(PyTypeObject* pyType, bool isInitialised = false);
	virtual ~Map();

	/** ��¶һЩ�ֵ䷽����python */
	static PyObject* __py_has_key(PyObject* self, PyObject* args);
	static PyObject* __py_keys(PyObject* self, PyObject* args);
	static PyObject* __py_values(PyObject* self, PyObject* args);
	static PyObject* __py_items(PyObject* self, PyObject* args);
	static PyObject* __py_update(PyObject* self, PyObject* args);
	
	/** map����������� */
	static PyObject* mp_subscript(PyObject* self, PyObject* key);
	static int mp_ass_subscript(PyObject* self, PyObject* key, PyObject* value);
	static int mp_length(PyObject* self);

	/** ��ȡ�ֵ���� */
	PyObject* getDictObject(void)const{ return pyDict_;}
	
	/** ���ݸı�֪ͨ */
	virtual void onDataChanged(std::string& key, std::string& value, bool isDelete = false);
protected:
	PyObject* pyDict_;											// �ֵ����ݣ� ���е����ݶ���������д
} ;

}
}
#endif