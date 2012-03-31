/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
#ifndef _SCRIPT_VECTOR4_H
#define _SCRIPT_VECTOR4_H
#include "cstdkbe/cstdkbe.hpp"
#include "math/math.hpp"
#include "scriptobject.hpp"
#include "pickler.hpp"
	
namespace KBEngine{ namespace script{
	
class ScriptVector4 : public ScriptObject
{		
	/** ���໯ ��һЩpy�������������� */
	INSTANCE_SCRIPT_HREADER(ScriptVector4, ScriptObject)
public:	
	static PySequenceMethods seqMethods;
	static PyNumberMethods numberMethods;
		
	ScriptVector4(Vector4& v);
	ScriptVector4(Vector4* v);
	ScriptVector4(float x, float y, float z, float w);
	virtual ~ScriptVector4();

	/** ��ö�������� */
	PyObject* tp_repr();

	/** �ű�ģ������python�д��� */
	static PyObject* tp_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

	/** seq��ز��� */
	static int seq_length(PyObject* self);
	static PyObject* seq_item(PyObject* self, int index);
	static PyObject* seq_slice(PyObject* self, int startIndex, int endIndex);
	static int seq_ass_item(PyObject* self, int index, PyObject* value);
	
	/** �Ӽ��˳���ز��� */
	static PyObject* py_add(PyObject *a, PyObject *b);
	static PyObject* py_subtract(PyObject *a, PyObject *b);
	static PyObject* py_multiply(PyObject *a, PyObject *b);
	static PyObject* py_divide(PyObject *a, PyObject *b);
	
	static PyObject* py_negative(PyObject *self);
	static PyObject* py_positive(PyObject *self);
	
	static int py_nonzero(PyObject *self);
	
	static PyObject* py_inplace_add(PyObject *self, PyObject *b);
	static PyObject* py_inplace_subtract(PyObject *self, PyObject *b);
	static PyObject* py_inplace_multiply(PyObject *self, PyObject *b);
	static PyObject* py_inplace_divide(PyObject *self, PyObject *b);
	
	/** ��©һЩ���� */
	static PyObject* pySetX(PyObject *self, PyObject *value, void *closure);
	static PyObject* pyGetX(PyObject *self, void *closure);
	static PyObject* pySetY(PyObject *self, PyObject *value, void *closure);
	static PyObject* pyGetY(PyObject *self, void *closure);
	static PyObject* pySetZ(PyObject *self, PyObject *value, void *closure);
	static PyObject* pyGetZ(PyObject *self, void *closure);
	static PyObject* pySetW(PyObject *self, PyObject *value, void *closure);
	static PyObject* pyGetW(PyObject *self, void *closure);
	static PyObject* pyDistTo(PyObject* self, PyObject* args);
	static PyObject* pyDistSqrTo(PyObject* self, PyObject* args);
	static PyObject* pyScale(PyObject* self, PyObject* args);
	static PyObject* pyDot(PyObject* self, PyObject* args);
	static PyObject* pyNormalise(PyObject* self, PyObject* args);
	static PyObject* pyTuple(PyObject* self, PyObject* args);
	static PyObject* pyList(PyObject* self, PyObject* args);
	static PyObject* pySet(PyObject* self, PyObject* args);
	static PyObject* pyGetVectorLength(PyObject *self, void *closure);
	static PyObject* pyGetVectorLengthSquared(PyObject *self, void *closure);
	
	/** ֧��pickler ���� */
	static PyObject* __reduce_ex__(PyObject* self, PyObject* protocol);
	/** unpickle���� */
	static PyObject* __unpickle__(PyObject* self, PyObject* args);

	/** �ű�����װʱ������ */
	static void onInstallScript(PyObject* mod);
	
	int length(void){ return VECTOR_SIZE; }
	Vector4& getVector(void){ return *m_val_; }
	void setVector(const Vector4& v){ *m_val_ = v; }
	
	/** ���ĳ��python�����Ƿ����ת��Ϊ������ */
	static bool check(PyObject* value, bool isPrintErr = true);
	
	/** ��ĳ������check������python����ת��Ϊvector4 */
	static void convertPyObjectToVector4(Vector4& v, PyObject* obj);
private:
	Vector4*			m_val_;
	bool				m_isCopy_;
	bool				m_isReadOnly_;
	static const int 	VECTOR_SIZE;
} ;

}
}
#endif