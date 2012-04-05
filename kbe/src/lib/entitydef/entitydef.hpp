/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 kbegine Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
*/
#ifndef __ENTITYDEF_H__
#define __ENTITYDEF_H__
#include "cstdkbe/cstdkbe.hpp"
#if KBE_PLATFORM == PLATFORM_WIN32
#pragma warning (disable : 4910)
#pragma warning (disable : 4251)
#endif
// common include
#include "method.hpp"	
#include "property.hpp"
#include "math/math.hpp"
#include "pyscript/scriptobject.hpp"
#include "xmlplus/xmlplus.hpp"	
//#define NDEBUG
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>	
#include <map>	
#include <vector>
#include <algorithm>
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

class ScriptModule
{
public:
	typedef std::map<std::string, PropertyDescription*> PROPERTYDESCRIPTION_MAP;
	typedef std::map<std::string, MethodDescription*> METHODDESCRIPTION_MAP;
	typedef std::map<uint32, PropertyDescription*> PROPERTYDESCRIPTION_UIDMAP;
	typedef std::map<uint32, MethodDescription*> METHODDESCRIPTION_UIDMAP;
protected:
	PyTypeObject*						m_scriptType_;							// �ű����
	uint16								m_uType_;								// �������  ��Ҫ���ڷ�����Һ�����䴫��ʶ������ű�ģ��
	
	PROPERTYDESCRIPTION_MAP				m_cellPropertyDescr;					// ����ű�cell������ӵ�е�������������
	PROPERTYDESCRIPTION_MAP				m_cellDetailLevelPropertyDescrs[3];		// cell����Զ������������
	PROPERTYDESCRIPTION_MAP				m_basePropertyDescr;					// ����ű�base������ӵ�е���������
	PROPERTYDESCRIPTION_MAP				m_clientPropertyDescr;					// ����ű�client������ӵ�е���������
	
	PROPERTYDESCRIPTION_UIDMAP			m_cellPropertyDescr_uidmap;				// ����ű���ӵ�е���������uidӳ��
	PROPERTYDESCRIPTION_UIDMAP			m_basePropertyDescr_uidmap;				// ����ű���ӵ�е���������uidӳ��
	PROPERTYDESCRIPTION_UIDMAP			m_clientPropertyDescr_uidmap;			// ����ű���ӵ�е���������uidӳ��
	
	METHODDESCRIPTION_MAP				m_methodCellDescr;						// ����ű���ӵ�еķ�������
	METHODDESCRIPTION_MAP				m_methodBaseDescr;						// ����ű���ӵ�еķ�������
	METHODDESCRIPTION_MAP				m_methodClientDescr;					// ����ű���ӵ�еķ�������
	
	METHODDESCRIPTION_UIDMAP			m_methodCellDescr_uidmap;				// ����ű���ӵ�еķ�������uidӳ��
	METHODDESCRIPTION_UIDMAP			m_methodBaseDescr_uidmap;				// ����ű���ӵ�еķ�������uidӳ��
	METHODDESCRIPTION_UIDMAP			m_methodClientDescr_uidmap;				// ����ű���ӵ�еķ�������uidӳ��
	
	bool								m_hasCell_;								// �Ƿ���cell����
	bool								m_hasBase_;								// �Ƿ���base����
	bool								m_hasClient_;							// �Ƿ���client����
	
	DetailLevel							m_detailLevel_;							// entity�����鼶������
public:	
	ScriptModule();
	~ScriptModule();	

	uint16 getUType(void);
	void setUType(uint16 utype){ m_uType_ = utype; }
	PyTypeObject* getScriptType(void);
	void setScriptType(PyTypeObject* scriptType){ m_scriptType_ = scriptType; }

	DetailLevel& getDetailLevel(void){ return m_detailLevel_; }
	
	PyObject* createObject(void);
	PyObject* getInitDict(void);

	void setCell(bool have){ m_hasCell_ = have; }
	void setBase(bool have){ m_hasBase_ = have; }
	void setClient(bool have){ m_hasClient_ = have; }

	bool hasCell(void)const{ return m_hasCell_; }
	bool hasBase(void)const{ return m_hasBase_; }
	bool hasClient(void)const{ return m_hasClient_; }

	PropertyDescription* findCellPropertyDescription(const char* attrName);
	PropertyDescription* findBasePropertyDescription(const char* attrName);
	PropertyDescription* findClientPropertyDescription(const char* attrName);
	PropertyDescription* findCellPropertyDescription(const uint32& utype);
	PropertyDescription* findBasePropertyDescription(const uint32& utype);
	PropertyDescription* findClientPropertyDescription(const uint32& utype);

	PROPERTYDESCRIPTION_MAP& getCellPropertyDescriptions(){ return m_cellPropertyDescr; }
	PROPERTYDESCRIPTION_MAP& getCellPropertyDescriptionsByDetailLevel(const int8& detailLevel){ return m_cellDetailLevelPropertyDescrs[detailLevel]; }
	PROPERTYDESCRIPTION_MAP& getBasePropertyDescriptions(){ return m_basePropertyDescr; }
	PROPERTYDESCRIPTION_MAP& getClientPropertyDescriptions(){ return m_clientPropertyDescr; }
	PROPERTYDESCRIPTION_UIDMAP& getCellPropertyDescriptions_uidmap(){ return m_cellPropertyDescr_uidmap; }
	PROPERTYDESCRIPTION_UIDMAP& getBasePropertyDescriptions_uidmap(){ return m_basePropertyDescr_uidmap; }
	PROPERTYDESCRIPTION_UIDMAP& getClientPropertyDescriptions_uidmap(){ return m_clientPropertyDescr_uidmap; }

	bool addPropertyDescription(const char* attrName, PropertyDescription* propertyDescription, COMPONENT_TYPE propertyType);

	
	MethodDescription* findCellMethodDescription(const char* attrName);
	MethodDescription* findCellMethodDescription(uint32 utype);
	bool addCellMethodDescription(const char* attrName, MethodDescription* methodDescription);
	METHODDESCRIPTION_MAP& getCellMethodDescriptions(void){ return m_methodCellDescr; }
	
	MethodDescription* findBaseMethodDescription(const char* attrName);
	MethodDescription* findBaseMethodDescription(uint32 utype);
	bool addBaseMethodDescription(const char* attrName, MethodDescription* methodDescription);
	METHODDESCRIPTION_MAP& getBaseMethodDescriptions(void){ return m_methodBaseDescr; }
	
	MethodDescription* findClientMethodDescription(const char* attrName);
	MethodDescription* findClientMethodDescription(uint32 utype);
	bool addClientMethodDescription(const char* attrName, MethodDescription* methodDescription);
	METHODDESCRIPTION_MAP& getClientMethodDescriptions(void){ return m_methodClientDescr; }		
};

class EntityDef
{
private:
	static std::vector<ScriptModule *> __scriptModules;							// ���е���չ�ű�ģ�鶼�洢������
	static std::map<std::string, uint16> __scriptTypeMappingUType;				// �ű����ӳ��utype
	static COMPONENT_TYPE __loadComponentType;									// �����ϵ����������������							
public:
	EntityDef();
	~EntityDef();
	
	static bool initialize(const std::string entitiesPath, std::vector<PyTypeObject*>& scriptBaseTypes, COMPONENT_TYPE loadComponentType);
	static bool finish(void);

	static bool loadAllScriptModule(std::string entitiesPath, std::vector<PyTypeObject*>& scriptBaseTypes);
	static bool loadAllDefDescription(std::string& moduleName, XmlPlus* defxml, TiXmlNode* defNode, ScriptModule* scriptModule);
	static bool loadDefPropertys(XmlPlus* xml, TiXmlNode* defPropertyNode, ScriptModule* scriptModule);
	static bool loadDefCellMethods(XmlPlus* xml, TiXmlNode* defMethodNode, ScriptModule* scriptModule);
	static bool loadDefBaseMethods(XmlPlus* xml, TiXmlNode* defMethodNode, ScriptModule* scriptModule);
	static bool loadDefClientMethods(XmlPlus* xml, TiXmlNode* defMethodNode, ScriptModule* scriptModule);
	static bool loadInterfaces(std::string& defFilePath, std::string& moduleName, XmlPlus* defxml, TiXmlNode* defNode, ScriptModule* scriptModule);
	static bool loadParentClass(std::string& defFilePath, std::string& moduleName, XmlPlus* defxml, TiXmlNode* defNode, ScriptModule* scriptModule);
	static bool loadDefInfo(std::string& defFilePath, std::string& moduleName, XmlPlus* defxml, TiXmlNode* defNode, ScriptModule* scriptModule);
	static bool loadDetailLevelInfo(std::string& defFilePath, std::string& moduleName, XmlPlus* defxml, TiXmlNode* defNode, ScriptModule* scriptModule);
	
	/** �Ƿ��������ű�ģ�� */
	static bool isLoadScriptModule(ScriptModule* scriptModule);
	/** ���ű�ģ���б�����ķ����Ƿ���� */
	static bool checkDefMethod(ScriptModule* scriptModule, PyObject* moduleObj, std::string& moduleName);
	
	/** ͨ�������Ѱ�ҵ���Ӧ�Ľű�ģ����� */
	static ScriptModule* findScriptModule(uint16 utype);
	static ScriptModule* findScriptModule(const char* scriptName);
};

}
#endif
