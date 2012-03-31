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
#pragma warning (disable : 4910)
#pragma warning (disable : 4251)

// common include
#include "array.hpp"
#include "fixeddict.hpp"
#include "datatype.hpp"
#include "cstdkbe/cstdkbe.hpp"
#include "cstdkbe/refcountable.hpp"
#include "memorystream.hpp"
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

class LIB_DLLAPI RefCountable;
class LIB_DLLAPI PropertyDescription : public RefCountable
{
protected:	
	std::string		m_name_;										// ������Ե�����
	std::string		m_dataTypeName_;								// ������Ե��ַ������������
	uint32			m_flags_;										// ������Ե�һЩ��־  ���� cell_public
	bool			m_isPersistent_;								// �Ƿ���һ���洢�����ݿ������
	DataType*		m_dataType_;									// ������Ե��������
	bool			m_isIdentifier_;								// �Ƿ���һ��������
	uint32			m_databaseLength_;								// ������������ݿ��еĳ���
	uint32			m_utype_;										// ������Ե�������� ���������ϴ���ʶ��
	MemoryStream*		m_defaultVal_;									// ������Ե�Ĭ��ֵ
	int8			m_detailLevel_;									// ������Ե�lod���鼶�� ��common�е�:���Ե�lod�㲥����Χ�Ķ���
	static uint32	propertyDescriptionCount_;						// ���е���������������
public:	
	PropertyDescription(std::string dataTypeName, std::string name, uint32 flags, bool isPersistent, 
		DataType* dataType, bool isIdentifier, uint32 databaseLength, std::string defaultStr, uint8 detailLevel);
	virtual ~PropertyDescription();
	
	/** ��ȡ������Ե�detailLevel */
	int8 getDetailLevel(void)const{ return m_detailLevel_; }
	
	/** �Ƿ���һ�����浽���ݿ��е����� */
	bool isPersistent(void)const{ return m_isPersistent_; };
	
	/** ��ȡ������Ե�������� */
	DataType* getDataType(void)const{ return m_dataType_; };
	
	/** ��ȡ���Եı�־ cell_public�� */
	const uint32& getFlags(void)const{ return m_flags_; };
	
	/** ��ȡ�������� */
	std::string& getName(void){ return m_name_; };
	
	/** ��ȡ�ַ������������ UINT32, BAG..*/
	std::string& getDataTypeName(void){ return m_dataTypeName_; }
	
	/** ���Ե�������� ���������ϴ���ʶ�� */
	uint32 getUType(void)const{ return m_utype_; }
	
	/** �����������Ϊ������ */
	void setIdentifier(bool isIdentifier){ m_isIdentifier_ = isIdentifier; }
	
	/** ����������������ݿ��еĳ��� */
	void setDatabaseLength(uint32 databaseLength){ m_databaseLength_ = databaseLength; }
	
	/** ��ȡ�������������def�ļ��б������Ĭ��ֵ */
	MemoryStream* getDefaultVal(void){ return m_defaultVal_; }
	
	/** ������������������� */
	static uint32 getDescriptionCount(void){ return propertyDescriptionCount_; }
	
	/** �������Ͳ���һ������ʵ�� */
	static PropertyDescription* createDescription(std::string& dataTypeName, std::string& name,
		uint32 flags, bool isPersistent, DataType* dataType, bool isIdentifier, uint32 databaseLength,
		std::string& defaultStr, uint8& detailLevel);
	
	/** �ű���������������Ե�ֵ */
	virtual int onSetValue(PyObject* parentObj, PyObject* value);		
};

class LIB_DLLAPI FixedDictDescription : public PropertyDescription
{
protected:	
public:	
	FixedDictDescription(std::string dataTypeName, std::string name, uint32 flags, bool isPersistent, 
		DataType* dataType, bool isIdentifier, uint32 databaseLength, std::string defaultStr, uint8 detailLevel);
	virtual ~FixedDictDescription();
	
	/** �ű���������������Ե�ֵ */
	int onSetValue(PyObject* parentObj, PyObject* value);		
};

class LIB_DLLAPI ArrayDescription : public PropertyDescription
{
protected:	
public:	
	ArrayDescription(std::string dataTypeName, std::string name, uint32 flags, bool isPersistent, 
		DataType* dataType, bool isIdentifier, uint32 databaseLength, std::string defaultStr, uint8 detailLevel);
	virtual ~ArrayDescription();
	
	/** �ű���������������Ե�ֵ */
	int onSetValue(PyObject* parentObj, PyObject* value);
};

class LIB_DLLAPI VectorDescription : public PropertyDescription
{
protected:	
	uint8 m_elemCount_;
public:	
	VectorDescription(std::string dataTypeName, std::string name, uint32 flags, bool isPersistent, 
		DataType* dataType, bool isIdentifier, uint32 databaseLength, std::string defaultStr, uint8 detailLevel, uint8 elemCount);
	virtual ~VectorDescription();
	
	/** �ű���������������Ե�ֵ */
	int onSetValue(PyObject* parentObj, PyObject* value);
};

}
#endif
