/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
#ifndef __KBENGINE_DEF_PROPERTY_H__
#define __KBENGINE_DEF_PROPERTY_H__
#include "cstdkbe/cstdkbe.hpp"
#if KBE_PLATFORM == PLATFORM_WIN32
#pragma warning (disable : 4910)
#pragma warning (disable : 4251)
#endif
// common include
#include "array.hpp"
#include "fixeddict.hpp"
#include "datatype.hpp"
#include "cstdkbe/refcountable.hpp"
#include "cstdkbe/memorystream.hpp"
//#define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>	
#include <map>	
#include <vector>
// windows include	
#if KBE_PLATFORM == PLATFORM_WIN32
#else
// linux include
#include <errno.h>
#endif
	
#define LIB_DLLAPI  __declspec(dllexport)

#ifdef __cplusplus  
extern "C" {  
#endif  

#ifdef __cplusplus  
}
#endif 

namespace KBEngine{

class RefCountable;
class PropertyDescription : public RefCountable
{
public:	
	PropertyDescription(std::string dataTypeName, std::string name, uint32 flags, bool isPersistent, 
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
	std::string& getName(void){ return name_; };
	
	/** ��ȡ�ַ������������ UINT32, BAG..*/
	std::string& getDataTypeName(void){ return dataTypeName_; }
	
	/** ���Ե�������� ���������ϴ���ʶ�� */
	uint32 getUType(void)const{ return utype_; }
	
	/** �����������Ϊ������ */
	void setIdentifier(bool isIdentifier){ isIdentifier_ = isIdentifier; }
	
	/** ����������������ݿ��еĳ��� */
	void setDatabaseLength(uint32 databaseLength){ databaseLength_ = databaseLength; }
	
	/** ��ȡ�������������def�ļ��б������Ĭ��ֵ */
	MemoryStream* getDefaultVal(void){ return defaultVal_; }
	
	/** ������������������� */
	static uint32 getDescriptionCount(void){ return propertyDescriptionCount_; }
	
	/** �������Ͳ���һ������ʵ�� */
	static PropertyDescription* createDescription(std::string& dataTypeName, std::string& name,
		uint32 flags, bool isPersistent, DataType* dataType, bool isIdentifier, uint32 databaseLength,
		std::string& defaultStr, uint8& detailLevel);
	
	/** �ű���������������Ե�ֵ */
	virtual int onSetValue(PyObject* parentObj, PyObject* value);	
protected:	
	std::string		name_;											// ������Ե�����
	std::string		dataTypeName_;									// ������Ե��ַ������������
	uint32			flags_;											// ������Ե�һЩ��־  ���� cell_public
	bool			isPersistent_;									// �Ƿ���һ���洢�����ݿ������
	DataType*		dataType_;										// ������Ե��������
	bool			isIdentifier_;									// �Ƿ���һ��������
	uint32			databaseLength_;								// ������������ݿ��еĳ���
	uint32			utype_;											// ������Ե�������� ���������ϴ���ʶ��
	MemoryStream*	defaultVal_;									// ������Ե�Ĭ��ֵ
	int8			detailLevel_;									// ������Ե�lod���鼶�� ��common�е�:���Ե�lod�㲥����Χ�Ķ���
	static uint32	propertyDescriptionCount_;						// ���е���������������	
};

class FixedDictDescription : public PropertyDescription
{
protected:	
public:	
	FixedDictDescription(std::string dataTypeName, std::string name, uint32 flags, bool isPersistent, 
		DataType* dataType, bool isIdentifier, uint32 databaseLength, std::string defaultStr, uint8 detailLevel);
	virtual ~FixedDictDescription();
	
	/** �ű���������������Ե�ֵ */
	int onSetValue(PyObject* parentObj, PyObject* value);		
};

class ArrayDescription : public PropertyDescription
{
protected:	
public:	
	ArrayDescription(std::string dataTypeName, std::string name, uint32 flags, bool isPersistent, 
		DataType* dataType, bool isIdentifier, uint32 databaseLength, std::string defaultStr, uint8 detailLevel);
	virtual ~ArrayDescription();
	
	/** �ű���������������Ե�ֵ */
	int onSetValue(PyObject* parentObj, PyObject* value);
};

class VectorDescription : public PropertyDescription
{
public:	
	VectorDescription(std::string dataTypeName, std::string name, uint32 flags, bool isPersistent, 
		DataType* dataType, bool isIdentifier, uint32 databaseLength, std::string defaultStr, uint8 detailLevel, uint8 elemCount);
	virtual ~VectorDescription();
	
	/** �ű���������������Ե�ֵ */
	int onSetValue(PyObject* parentObj, PyObject* value);
protected:	
	uint8 elemCount_;
};

}
#endif
