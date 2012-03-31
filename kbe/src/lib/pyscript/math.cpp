#include "math.hpp"
namespace KBEngine{ namespace script{ namespace math {

//-------------------------------------------------------------------------------------
bool installModule(const char* moduleName)
{
	// ��ʼ��һ����ѧ��ص�ģ��
	PyObject *mathModule = PyImport_AddModule(moduleName);
	static struct PyModuleDef moduleDesc =   
	{  
			 PyModuleDef_HEAD_INIT,  
			 "Math",  
			 "This module is created by KBEngine!",  
			 -1,  
			 NULL  
	};  

	PyModule_Create(&moduleDesc);			
	
	// ��ʼ��ScriptVector2
	script::ScriptVector2::installScript(mathModule, "Vector2");
	// ��ʼ��ScriptVector3
	script::ScriptVector3::installScript(mathModule, "Vector3");
	// ��ʼ��ScriptVector4
	script::ScriptVector4::installScript(mathModule, "Vector4");
	return true;
}

//-------------------------------------------------------------------------------------
bool uninstallModule()
{
	script::ScriptVector2::uninstallScript();
	script::ScriptVector3::uninstallScript();
	script::ScriptVector4::uninstallScript();
	return true;
}

}
}
}
