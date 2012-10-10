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


#ifndef __KBENGINE_DEF_PROPERTY_H__
#define __KBENGINE_DEF_PROPERTY_H__
#include "cstdkbe/cstdkbe.hpp"
#if KBE_PLATFORM == PLATFORM_WIN32
#pragma warning (disable : 4910)
#pragma warning (disable : 4251)
#endif
// common include
#include "fixedarray.hpp"
#include "fixeddict.hpp"
#include "datatype.hpp"
#include "cstdkbe/refcountable.hpp"
#include "cstdkbe/memorystream.hpp"
//#define NDEBUG
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#endif

namespace KBEngine{

class RefCountable;
class PropertyDescription : public RefCountable
{
public:	
	PropertyDescription(ENTITY_PROPERTY_UID utype, std::string dataTypeName, std::string name, uint32 flags, bool isPersistent, 
		DataType* dataType, bool isIdentifier, uint32 databaseLength, std::string defaultStr, uint8 detailLevel);
	virtual ~PropertyDescription();
	
	/** ��ȡ������Ե�detailLevel */
	int8 getDetailLevel(void)const{ return detailLevel_; }
	
	/** �Ƿ���һ�����浽���ݿ��е����� */
	bool isPersistent(void)const{ return isPersistent_; };
	
	/** ��ȡ������Ե�������� */
	DataType* getDataType(void)const{ return dataType_; };
	
	/** ��ȡ���Եı�־ cell_public�� */
	const uint32& getFlags(void)const{ return flags_; };
	
	/** ��ȡ�������� */
	const char* getName(void)const{ return name_.c_str(); };
	
	/** ��ȡ�ַ������������ UINT32, BAG..*/
	const char* getDataTypeName(void)const{ return dataTypeName_.c_str(); }
	
	/** ���Ե�������� ���������ϴ���ʶ�� */
	ENTITY_PROPERTY_UID getUType(void)const{ return utype_; }
	
	/** �����������Ϊ������ */
	void setIdentifier(bool isIdentifier){ isIdentifier_ = isIdentifier; }
	
	/** ����������������ݿ��еĳ��� */
	void setDatabaseLength(uint32 databaseLength){ databaseLength_ = databaseLength; }
	uint32 getDatabaseLength()const { return databaseLength_; }

	/** ��ȡ�������������def�ļ��б������Ĭ��ֵ */
	PyObject* newDefaultVal(void);
	
	/** ������������������� */
	static uint32 getDescriptionCount(void){ return propertyDescriptionCount_; }
	
	/** �������Ͳ���һ������ʵ�� */
	static PropertyDescription* createDescription(ENTITY_PROPERTY_UID utype, std::string& dataTypeName, std::string& name,
		uint32 flags, bool isPersistent, DataType* dataType, bool isIdentifier, uint32 databaseLength,
		std::string& defaultStr, uint8 detailLevel);
	
	/** �ű���������������Ե�ֵ */
	virtual int onSetValue(PyObject* parentObj, PyObject* value);	
protected:	
	std::string					name_;											// ������Ե�����
	std::string					dataTypeName_;									// ������Ե��ַ������������
	uint32						flags_;											// ������Ե�һЩ��־  ���� cell_public
	bool						isPersistent_;									// �Ƿ���һ���洢�����ݿ������
	DataType*					dataType_;										// ������Ե��������
	bool						isIdentifier_;									// �Ƿ���һ��������
	uint32						databaseLength_;								// ������������ݿ��еĳ���
	ENTITY_PROPERTY_UID			utype_;											// ������Ե�������� ���������ϴ���ʶ��
	PyObject*					defaultVal_;									// ������Ե�Ĭ��ֵ
	int8						detailLevel_;									// ������Ե�lod���鼶�� ��common�е�:���Ե�lod�㲥����Χ�Ķ���
	static uint32				propertyDescriptionCount_;						// ���е���������������	
};

class FixedDictDescription : public PropertyDescription
{
protected:	
public:	
	FixedDictDescription(ENTITY_PROPERTY_UID utype, std::string dataTypeName, std::string name, uint32 flags, bool isPersistent, 
		DataType* dataType, bool isIdentifier, uint32 databaseLength, std::string defaultStr, uint8 detailLevel);
	virtual ~FixedDictDescription();
	
	/** �ű���������������Ե�ֵ */
	int onSetValue(PyObject* parentObj, PyObject* value);		
};

class ArrayDescription : public PropertyDescription
{
protected:	
public:	
	ArrayDescription(ENTITY_PROPERTY_UID utype, std::string dataTypeName, std::string name, uint32 flags, bool isPersistent, 
		DataType* dataType, bool isIdentifier, uint32 databaseLength, std::string defaultStr, uint8 detailLevel);
	virtual ~ArrayDescription();
	
	/** �ű���������������Ե�ֵ */
	int onSetValue(PyObject* parentObj, PyObject* value);
};

class VectorDescription : public PropertyDescription
{
public:	
	VectorDescription(ENTITY_PROPERTY_UID utype, std::string dataTypeName, std::string name, uint32 flags, bool isPersistent, 
		DataType* dataType, bool isIdentifier, uint32 databaseLength, std::string defaultStr, uint8 detailLevel, uint8 elemCount);
	virtual ~VectorDescription();
	
	/** �ű���������������Ե�ֵ */
	int onSetValue(PyObject* parentObj, PyObject* value);
protected:	
	uint8 elemCount_;
};

}
#endif
