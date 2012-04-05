/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
/*
	kbengine-�ű�ϵͳ��
		����ű�ϵͳ��Ҫ��װ��python/c��Ӧ�ã� ���Լ򵥵İ�װһ��pythonģ�飬 ʵ��c++��python
		��ϱ�̡�
		
		ʹ������:
				class Entity:public ScriptObject
				{
					SCRIPT_SUPERCLASS(Entity, ScriptObject)
				public:
					PyObject * id;
					void hello(){printf("��������ʾ\n");}
					static PyObject * getID(PyObject* self){ return Py_BuildValue("i",1); }
					static PyObject * helloWraper(PyObject* self){
						static_cast<Entity*>(self)->hello();
						Py_INCREF(Py_None);
						return Py_None;
					}
				};

				SCRIPT_METHOD_DECLARE_BEGIN(Entity)
				SCRIPT_METHOD_DECLARE("getID", getID, METH_NOARGS, 0)
				SCRIPT_METHOD_DECLARE("hello", helloWraper, METH_NOARGS, 0)
				SCRIPT_METHOD_DECLARE_END()
				
				SCRIPT_MEMBER_DECLARE_BEGIN(Entity)
				SCRIPT_MEMBER_DECLARE(id, T_INT, 0, 0)
				SCRIPT_MEMBER_DECLARE_END()
				
				SCRIPT_INIT(Entity)

				int _tmain(int argc, _TCHAR* argv[])
				{
					Py_Initialize();                       //python   �������ĳ�ʼ��  
					Py_IsInitialized();
					PyObject* m;
					m = PyImport_AddModule("KBEngine");
					Entity::initPyType(m);
					Py_InitModule("KBEngine", NULL);
					PyObject *pName = PyString_FromString("test1");
					PyObject * pModule = PyImport_Import(pName);
					getchar();
					Py_Finalize(); // ���
					return 0;
				}

			python:
					import KBEngine
					print dir(KBEngine.Entity)

					class Test(KBEngine.Entity):
						def __init__(self):
							pass
						def getID(self):
							print "�ű�����ʾ"
							return KBEngine.Entity.getID(self)
					t = Test()
					print t.id
					print t.getID()
					t.xx = 1
					t.hello()			
*/
#ifndef __SCRIPTOBJECT_H__
#define __SCRIPTOBJECT_H__
#include <vector>	
#include "Python.h"     
#include "log/debug_helper.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include <structmember.h>
#include <assert.h>

namespace KBEngine{ namespace script{

// python��Ĭ�Ͽշ���ֵ
#define S_Return { Py_INCREF(Py_None); return Py_None; }																		

// python�Ķ����ͷ�
#define S_RELEASE(pyObj)																	\
	if(pyObj){																				\
		Py_DECREF(pyObj);																	\
		pyObj = NULL;																		\
	}																						\

/// �����ǰ�ű������Ĵ�����Ϣ	
#define SCRIPT_ERROR_CHECK()																\
{																							\
 	if (PyErr_Occurred())																	\
 	{																						\
		PyErr_PrintEx(0);																	\
	}																						\
}

// �ű�����ͷ ��ͨ����pythonĬ�Ϸ������ʽ�����Ķ��� ��
#define SCRIPT_OBJECT_HREADER(CLASS, SUPERCLASS)											\
	SCRIPT_HREADER_BASE(CLASS, SUPERCLASS);													\
	/** python�����Ķ���������python���ͷ�
	*/																						\
	static void _tp_dealloc(PyObject* self)													\
	{																						\
		CLASS::_scriptType.tp_free(self);													\
	}																						\

// �����ű�����ͷ �����ģ��ͨ�����ṩ��python�ű��н��м̳е�һ�������� ��
#define BASE_SCRIPT_HREADER(CLASS, SUPERCLASS)												\
	SCRIPT_HREADER_BASE(CLASS, SUPERCLASS);													\
	/** python�����Ķ���������python���ͷ�
	*/																						\
	static void _tp_dealloc(PyObject* self)													\
	{																						\
		static_cast<CLASS*>(self)->~CLASS();												\
		CLASS::_scriptType.tp_free(self);													\
	}																						\

// ʵ���ű�����ͷ ������ű���������c++�н���new������ ��
#define INSTANCE_SCRIPT_HREADER(CLASS, SUPERCLASS)											\
	SCRIPT_HREADER_BASE(CLASS, SUPERCLASS);													\
	/** c++new�����Ķ��������delete����
	*/																						\
	static void _tp_dealloc(PyObject* self)													\
	{																						\
		delete static_cast<CLASS*>(self);													\
	}																						\

																							
#define SCRIPT_HREADER_BASE(CLASS, SUPERCLASS)												\
	/* ��ǰ�ű�ģ������ */																\
	static PyTypeObject _scriptType;														\
	typedef CLASS ThisClass;																\
																							\
	static PyObject* _tp_repr(PyObject* self)												\
	{																						\
		return static_cast<CLASS*>(self)->tp_repr();										\
	}																						\
																							\
	static PyObject* _tp_str(PyObject* self)												\
	{																						\
		return static_cast<CLASS*>(self)->tp_str();											\
	}																						\
																							\
	/** �ű�ģ������python�д���
	*/																						\
	static PyObject* _tp_new(PyTypeObject* type, PyObject* args, PyObject* kwds)			\
	{																						\
		return CLASS::tp_new(type, args, kwds);												\
	}																						\
																							\
	/** python �����ȡ��ģ������Ի��߷���
	*/																						\
	static PyObject* _tp_getattro(PyObject* self, PyObject* name)							\
	{																						\
		return static_cast<CLASS*>(self)->onScriptGetAttribute(PyUnicode_AS_DATA(name));	\
	}																						\
																							\
	/** python �������ñ�ģ������Ի��߷���
	*/																						\
	static int _tp_setattro(PyObject* self, PyObject* name, PyObject* value)				\
	{																						\
		const char* attr = PyUnicode_AS_DATA(name);											\
		return (value != NULL) ?															\
				static_cast<CLASS*>(self)->onScriptSetAttribute(attr, value):				\
				static_cast<CLASS*>(self)->onScriptDelAttribute(attr);						\
	}																						\
																							\
	/** python �����ʼ����ģ�����
	*/																						\
	static int _tp_init(PyObject* self, PyObject *args, PyObject* kwds)						\
	{																						\
		return static_cast<CLASS*>(self)->onScriptInit(self, args, kwds);					\
	}																						\
																							\
public:																						\
	/* ���ս�Ҫ����װ���ű�ģ���еķ����ͳ�Ա����б�*/										\
	static PyMethodDef* _##CLASS##_lpScriptmethods;											\
	static PyMemberDef* _##CLASS##_lpScriptmembers;											\
	static PyGetSetDef* _##CLASS##_lpgetseters;												\
	/* ��ģ����Ҫ��©���ű��ķ����ͳ�Ա�� ���ջᱻ���뵽�����2��ָ���б��� */				\
	static PyMethodDef _##CLASS##_scriptMethods[];											\
	static PyMemberDef _##CLASS##_scriptMembers[];											\
	static PyGetSetDef _##CLASS##_scriptGetSeters[];										\
																							\
	/** getset��ֻ������
	*/																						\
	static int __py_readonly_descr(PyObject* self, void* closure)							\
	{																						\
		PyErr_Format(PyExc_TypeError,														\
		"Sorry, this attribute in " #CLASS " is read-only");								\
		PyErr_Print();																		\
		return 0;																			\
	}																						\
																							\
	/** getset��ֻд����
	*/																						\
	static int __py_writeonly_descr(PyObject* self, PyObject* value, void* closure)			\
	{																						\
		PyErr_Format(PyExc_TypeError,														\
		"Sorry, this attribute in " #CLASS " is write-only");								\
		PyErr_Print();																		\
		return 0;																			\
	}																						\
																							\
	/** ����ӿڿ��Ի�õ�ǰģ��Ľű���� 
	*/																						\
	static PyTypeObject* getScriptType(void)												\
	{																						\
		return &_scriptType;																\
	}																						\
																							\
	/** �������м̳�ģ��ı�¶�������� 
	*/																						\
	static int calcTotalMethodCount(void)													\
	{																						\
		int nlen = 0;																		\
		while(true)																			\
		{																					\
			PyMethodDef* pmf = &_##CLASS##_scriptMethods[nlen];								\
			if(!pmf->ml_doc && !pmf->ml_flags && !pmf->ml_meth && !pmf->ml_name)			\
				break;																		\
			nlen++;																			\
		}																					\
																							\
		if(strcmp(#CLASS, #SUPERCLASS) == 0)												\
			return nlen;																	\
		return SUPERCLASS::calcTotalMethodCount() + nlen;									\
	}																						\
																							\
	/** �������м̳�ģ��ı�¶��Ա���� 
	*/																						\
	static int calcTotalMemberCount(void)													\
	{																						\
		int nlen = 0;																		\
		while(true)																			\
		{																					\
			PyMemberDef* pmd = &_##CLASS##_scriptMembers[nlen];								\
			if(!pmd->doc && !pmd->flags && !pmd->type && !pmd->name && !pmd->offset)		\
				break;																		\
			nlen++;																			\
		}																					\
																							\
		if(strcmp(#CLASS, #SUPERCLASS) == 0)												\
			return nlen;																	\
		return SUPERCLASS::calcTotalMemberCount() + nlen;									\
	}																						\
																							\
	/** �������м̳�ģ��ı�¶getset���� 
	*/																						\
	static int calcTotalGetSetCount(void)													\
	{																						\
		int nlen = 0;																		\
		while(true)																			\
		{																					\
			PyGetSetDef* pgs = &_##CLASS##_scriptGetSeters[nlen];							\
			if(!pgs->doc && !pgs->get && !pgs->set && !pgs->name && !pgs->closure)			\
				break;																		\
			nlen++;																			\
		}																					\
																							\
		if(strcmp(#CLASS, #SUPERCLASS) == 0)												\
			return nlen;																	\
		return SUPERCLASS::calcTotalGetSetCount() + nlen;									\
	}																						\
																							\
	/** �����и����Լ���ǰģ��ı�¶��Ա�ͷ�����װ������Ҫ����ű����б��� 
	*/																						\
	static void setupScriptMethodAndAttribute(PyMethodDef* lppmf, PyMemberDef* lppmd,		\
	PyGetSetDef* lppgs)																		\
	{																						\
		int i = 0;																			\
		PyMethodDef* pmf = NULL;															\
		PyMemberDef* pmd = NULL;															\
		PyGetSetDef* pgs = NULL;															\
																							\
		while(true){																		\
			pmf = &_##CLASS##_scriptMethods[i];												\
			if(!pmf->ml_doc && !pmf->ml_flags && !pmf->ml_meth && !pmf->ml_name)			\
				break;																		\
			i++;																			\
			*(lppmf++) = *pmf;																\
		}																					\
																							\
		i = 0;																				\
		while(true){																		\
			pmd = &_##CLASS##_scriptMembers[i];												\
			if(!pmd->doc && !pmd->flags && !pmd->type && !pmd->name && !pmd->offset)		\
				break;																		\
			i++;																			\
			*(lppmd++) = *pmd;																\
		}																					\
																							\
		i = 0;																				\
		while(true){																		\
			pgs = &_##CLASS##_scriptGetSeters[i];											\
			if(!pgs->doc && !pgs->get && !pgs->set && !pgs->name && !pgs->closure)			\
				break;																		\
			i++;																			\
			*(lppgs++) = *pgs;																\
		}																					\
																							\
		if(strcmp(#CLASS, #SUPERCLASS) == 0){												\
			*(lppmf) = *pmf;																\
			*(lppmd) = *pmd;																\
			*(lppgs) = *pgs;																\
			return;																			\
		}																					\
																							\
		SUPERCLASS::setupScriptMethodAndAttribute(lppmf, lppmd, lppgs);						\
	}																						\
																							\
	/** ��װ��ǰ�ű�ģ�� 
		@param mod: ��Ҫ�������ģ��
	*/																						\
	static void installScript(PyObject* mod, const char* name = #CLASS)						\
	{																						\
		int nMethodCount			= CLASS::calcTotalMethodCount();						\
		int nMemberCount			= CLASS::calcTotalMemberCount();						\
		int nGetSetCount			= CLASS::calcTotalGetSetCount();						\
																							\
		_##CLASS##_lpScriptmethods	= new PyMethodDef[nMethodCount + 2];					\
		_##CLASS##_lpScriptmembers	= new PyMemberDef[nMemberCount + 2];					\
		_##CLASS##_lpgetseters		= new PyGetSetDef[nGetSetCount + 2];					\
																							\
		setupScriptMethodAndAttribute(_##CLASS##_lpScriptmethods,							\
									  _##CLASS##_lpScriptmembers,							\
									  _##CLASS##_lpgetseters);								\
																							\
		_scriptType.tp_methods		= _##CLASS##_lpScriptmethods;							\
		_scriptType.tp_members		= _##CLASS##_lpScriptmembers;							\
		_scriptType.tp_getset		= _##CLASS##_lpgetseters;								\
																							\
		CLASS::onInstallScript(mod);														\
		if (PyType_Ready(&_scriptType) < 0)													\
			return;																			\
																							\
		Py_INCREF(&_scriptType);															\
		if(mod)																				\
			PyModule_AddObject(mod, name, (PyObject *)&_scriptType);						\
																							\
	}																						\
																							\
	/** ж�ص�ǰ�ű�ģ�� 
	*/																						\
	static void uninstallScript(void)														\
	{																						\
		SAFE_RELEASE_ARRAY(_##CLASS##_lpScriptmethods);										\
		SAFE_RELEASE_ARRAY(_##CLASS##_lpScriptmembers);										\
		SAFE_RELEASE_ARRAY(_##CLASS##_lpgetseters);											\
		CLASS::onUninstallScript();															\
		Py_DECREF(&_scriptType);															\
	}																						\




/** �������ʽ�ĳ�ʼ��һ���ű�ģ�飬 ��һЩ��Ҫ����Ϣ��䵽python��type������
*/
#define SCRIPT_INIT(CLASS, CALL, SEQ, MAP, ITER, ITERNEXT)									\
	PyMethodDef* CLASS::_##CLASS##_lpScriptmethods = NULL;									\
	PyMemberDef* CLASS::_##CLASS##_lpScriptmembers = NULL;									\
	PyGetSetDef* CLASS::_##CLASS##_lpgetseters = NULL;										\
																							\
	PyTypeObject CLASS::_scriptType =														\
	{																						\
		PyVarObject_HEAD_INIT(NULL, 0)														\
		#CLASS,													/* tp_name            */	\
		sizeof(CLASS),											/* tp_basicsize       */	\
		0,														/* tp_itemsize        */	\
		(destructor)CLASS::_tp_dealloc,							/* tp_dealloc         */	\
		0,														/* tp_print           */	\
		0,														/* tp_getattr         */	\
		0,														/* tp_setattr         */	\
		0,														/* tp_compare         */	\
		CLASS::_tp_repr,										/* tp_repr            */	\
		0,														/* tp_as_number       */	\
		SEQ,													/* tp_as_sequence     */	\
		MAP,													/* tp_as_mapping      */	\
		0,														/* tp_hash            */	\
		CALL,													/* tp_call            */	\
		0,														/* tp_str             */	\
		(getattrofunc)CLASS::_tp_getattro,						/* tp_getattro        */	\
		(setattrofunc)CLASS::_tp_setattro,						/* tp_setattro        */	\
		0,														/* tp_as_buffer       */	\
		Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,				/* tp_flags           */	\
		"KBEngine::" #CLASS " objects.",						/* tp_doc             */	\
		0,														/* tp_traverse        */	\
		0,														/* tp_clear           */	\
		0,														/* tp_richcompare     */	\
		0,														/* tp_weaklistoffset  */	\
		ITER,													/* tp_iter            */	\
		ITERNEXT,												/* tp_iternext        */	\
		0,														/* tp_methods         */	\
		0,														/* tp_members         */	\
		0,														/* tp_getset          */	\
		0,														/* tp_base            */	\
		0,														/* tp_dict            */	\
		0,														/* tp_descr_get       */	\
		0,														/* tp_descr_set       */	\
		0,														/* tp_dictoffset      */	\
		(initproc)CLASS::_tp_init,								/* tp_init            */	\
		0,														/* tp_alloc           */	\
		CLASS::_tp_new,											/* tp_new             */	\
	};																						\

// BASE_SCRIPT_HREADER������ű���ʼ��
#define BASE_SCRIPT_INIT(CLASS, CALL, SEQ, MAP, ITER, ITERNEXT)								\
	PyMethodDef* CLASS::_##CLASS##_lpScriptmethods = NULL;									\
	PyMemberDef* CLASS::_##CLASS##_lpScriptmembers = NULL;									\
	PyGetSetDef* CLASS::_##CLASS##_lpgetseters = NULL;										\
																							\
	PyTypeObject CLASS::_scriptType =														\
	{																						\
		PyObject_HEAD_INIT(&PyType_Type)													\
		#CLASS,													/* tp_name            */	\
		sizeof(CLASS),											/* tp_basicsize       */	\
		0,														/* tp_itemsize        */	\
		(destructor)CLASS::_tp_dealloc,							/* tp_dealloc         */	\
		0,														/* tp_print           */	\
		0,														/* tp_getattr         */	\
		0,														/* tp_setattr         */	\
		0,														/* void *tp_reserved  */	\
		CLASS::_tp_repr,										/* tp_repr            */	\
		0,														/* tp_as_number       */	\
		SEQ,													/* tp_as_sequence     */	\
		MAP,													/* tp_as_mapping      */	\
		0,														/* tp_hash            */	\
		CALL,													/* tp_call            */	\
		CLASS::_tp_str,											/* tp_str             */	\
		(getattrofunc)CLASS::_tp_getattro,						/* tp_getattro        */	\
		(setattrofunc)CLASS::_tp_setattro,						/* tp_setattro        */	\
		0,														/* tp_as_buffer       */	\
		Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,				/* tp_flags           */	\
		0,														/* tp_doc             */	\
		0,														/* tp_traverse        */	\
		0,														/* tp_clear           */	\
		0,														/* tp_richcompare     */	\
		0,														/* tp_weaklistoffset  */	\
		ITER,													/* tp_iter            */	\
		ITERNEXT,												/* tp_iternext        */	\
		0,														/* tp_methods         */	\
		0,														/* tp_members         */	\
		0,														/* tp_getset          */	\
		0,														/* tp_base            */	\
		0,														/* tp_dict            */	\
		0,														/* tp_descr_get       */	\
		0,														/* tp_descr_set       */	\
		0,														/* tp_dictoffset      */	\
		0,														/* tp_init            */	\
		0,														/* tp_alloc           */	\
		0,														/* tp_new             */	\
		PyObject_GC_Del,										/* tp_free            */	\
		0,														/* tp_is_gc           */	\
		0,														/* tp_bases           */	\
		0,														/* tp_mro             */	\
		0,														/* tp_cache           */	\
		0,														/* tp_subclasses      */	\
		0,														/* tp_weaklist        */	\
		0,														/* tp_del			  */	\
	};																						\
			
/** ���屩¶���ű��ķ�����
*/
#define SCRIPT_METHOD_DECLARE_BEGIN(CLASS)											PyMethodDef CLASS::_##CLASS##_scriptMethods[] = {			
#define SCRIPT_METHOD_DECLARE(METHOD_NAME, METHOD_FUNC, FLAGS, DOC)					{METHOD_NAME, (PyCFunction)&METHOD_FUNC, FLAGS, DOC},
#define SCRIPT_METHOD_DECLARE_END()													{NULL, NULL, NULL, NULL}};

// ��ģ��׷�ӷ���
#define APPEND_SCRIPT_MODULE_METHOD(MODULE, NAME, FUNC, FLAGS, SELF)						\
	static PyMethodDef __pymethod_NAME = {#NAME, (PyCFunction) FUNC, FLAGS, NULL};			\
	PyModule_AddObject(MODULE, #NAME, PyCFunction_New(&__pymethod_##NAME, SELF));

/** ���屩¶���ű������Ժ�
*/
#define SCRIPT_MEMBER_DECLARE_BEGIN(CLASS)											PyMemberDef CLASS::_##CLASS##_scriptMembers[] =	{
#define SCRIPT_MEMBER_DECLARE(MEMBER_NAME, MEMBER_REF, MEMBER_TYPE, FLAGS, DOC)		{MEMBER_NAME, MEMBER_TYPE, offsetof(ThisClass, MEMBER_REF), FLAGS, DOC},
#define SCRIPT_MEMBER_DECLARE_END()													{NULL, NULL, NULL, NULL, NULL}};

/** ���屩¶���ű���getset���Ժ�
*/
#define SCRIPT_GETSET_DECLARE_BEGIN(CLASS)											PyGetSetDef CLASS::_##CLASS##_scriptGetSeters[] =	{
#define SCRIPT_GETSET_DECLARE(NAME, GET, SET, DOC, CLOSURE)							{NAME, (getter)GET, (setter)SET, DOC, CLOSURE},
#define SCRIPT_GET_DECLARE(NAME, GET, DOC, CLOSURE)									{NAME, (getter)GET, (setter)__py_readonly_descr, DOC, CLOSURE},
#define SCRIPT_SET_DECLARE(NAME, GET, DOC, CLOSURE)									{NAME, (getter)GET, (setter)__py_writeonly_descr, DOC, CLOSURE},
#define SCRIPT_GETSET_DECLARE_END()													{NULL, NULL, NULL, NULL, NULL}};


class ScriptObject: public PyObject
{
	/** ���໯ ��һЩpy�������������� */
	SCRIPT_OBJECT_HREADER(ScriptObject, ScriptObject)							
public:	
	ScriptObject(PyTypeObject* pyType, bool isInitialised = false);
	~ScriptObject();

	/** �ű��������ü��� */
	void incRef() const				{ Py_INCREF((PyObject*)this); }
	void decRef() const				{ Py_DECREF((PyObject*)this); }
	int refCount() const			{ return int(((PyObject*)this)->ob_refcnt); }
	
	/** ��ö�������� */
	PyObject* tp_repr();
	PyObject* tp_str();

	/** �ű����󴴽�һ���ö��� */
	static PyObject* tp_new(PyTypeObject* type, PyObject* args, PyObject* kwds){ return type->tp_alloc(type, 0); };

	/** �ű������ȡ���Ի��߷��� */
	PyObject* onScriptGetAttribute(const char* attr);						

	/** �ű������������Ի��߷��� */
	int onScriptSetAttribute(const char* attr, PyObject* value);			

	/** �ű�����ɾ��һ������ */
	int onScriptDelAttribute(const char* attr);

	/** �ű������ʼ�� */
	int onScriptInit(PyObject* self, PyObject *args, PyObject* kwds);

	/** ��ȡ����������� */
	const char* getObjTypeName() const{ return ob_type->tp_name; }

	/** �ű�����װʱ������ */
	static void onInstallScript(PyObject* mod){}

	/** �ű���ж��ʱ������ */
	static void onUninstallScript(){}
} ;

}
}
#endif