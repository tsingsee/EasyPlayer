// EasyPlayerWebActiveX.cpp : CEasyPlayerWebActiveXApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "EasyPlayerWebActiveX.h"
#include "comcat.h"
#include "strsafe.h"
#include "objsafe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CEasyPlayerWebActiveXApp theApp;

const GUID CDECL _tlid = { 0xE7319905, 0x7F58, 0x422D, { 0x9A, 0xF6, 0x29, 0x20, 0x78, 0xB8, 0x5D, 0x1C } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

//	0x1ee1c648, 0xf4a9, 0x42f9, 0x9a, 0xa7, 0x2c, 0x8e, 0x3a, 0xf7, 0xb7, 0xfd)
#define SAFE_CODE
const GUID CDECL SAFE_CODE CLSID_SafeItem =
{ 0x1ee1c648, 0xf4a9, 0x42f9, { 0x9a, 0xa7, 0x2c, 0x8e, 0x3a, 0xf7, 0xb7, 0xfd } };


// CEasyPlayerWebActiveXApp::InitInstance - DLL 初始化

BOOL CEasyPlayerWebActiveXApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。
	}

	return bInit;
}



// CEasyPlayerWebActiveXApp::ExitInstance - DLL 终止

int CEasyPlayerWebActiveXApp::ExitInstance()
{
	// TODO: 在此添加您自己的模块终止代码。

	return COleControlModule::ExitInstance();
}


// HRESULT CreateComponentCategory - Used to register ActiveX control as safe

HRESULT CreateComponentCategory(CATID catid, WCHAR *catDescription)
{
	ICatRegister *pcr = NULL ;
	HRESULT hr = S_OK ;

	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
	if (FAILED(hr))
		return hr;

	// Make sure the HKCR\Component Categories\{..catid...}
	// key is registered.
	CATEGORYINFO catinfo;
	catinfo.catid = catid;
	catinfo.lcid = 0x0409 ; // english
	size_t len;
	// Make sure the provided description is not too long.
	// Only copy the first 127 characters if it is.
	// The second parameter of StringCchLength is the maximum
	// number of characters that may be read into catDescription.
	// There must be room for a NULL-terminator. The third parameter
	// contains the number of characters excluding the NULL-terminator.

	USES_CONVERSION;

	hr = StringCchLength(catDescription, STRSAFE_MAX_CCH, &len);

	if (SUCCEEDED(hr))
	{
		if (len>127)
		{
			len = 127;
		}
	}   
	else
	{
		// TODO: Write an error handler;
	}
	// The second parameter of StringCchCopy is 128 because you need 
	// room for a NULL-terminator. 
	hr = StringCchCopy(catinfo.szDescription, len + 1, 
		catDescription);
	// Make sure the description is null terminated.
	catinfo.szDescription[len + 1] = '\0';

	hr = pcr->RegisterCategories(1, &catinfo);
	pcr->Release();

	return hr;
}

// HRESULT RegisterCLSIDInCategory - Register your component categories information

HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
	// Register your component categories information.
	ICatRegister *pcr = NULL ;
	HRESULT hr = S_OK ;
	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
	if (SUCCEEDED(hr))
	{
		// Register this category as being "implemented" by the class.
		CATID rgcatid[1] ;
		rgcatid[0] = catid;
		hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);
	}

	if (pcr != NULL)
		pcr->Release();

	return hr;
}

// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	HRESULT hr;    // HResult used by Safety Functions

	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	// Mark the control as safe for initializing.
	hr = CreateComponentCategory(CATID_SafeForInitializing, L"Controls safely initializable from persistent data!");
	if (FAILED(hr))
		return hr;

	hr = RegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForInitializing);
	if (FAILED(hr))
		return hr;

	// Mark the control as safe for scripting.

	hr = CreateComponentCategory(CATID_SafeForScripting, L"Controls safely scriptable!");
	if (FAILED(hr))
		return hr;

	hr = RegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForScripting);
	if (FAILED(hr))
		return hr;

	return NOERROR;
}

// HRESULT UnRegisterCLSIDInCategory - Remove entries from the registry

HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
	ICatRegister *pcr = NULL ;
	HRESULT hr = S_OK ;

	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
	if (SUCCEEDED(hr))
	{
		// Unregister this category as being "implemented" by the class.
		CATID rgcatid[1] ;
		rgcatid[0] = catid;
		hr = pcr->UnRegisterClassImplCategories(clsid, 1, rgcatid);
	}

	if (pcr != NULL)
		pcr->Release();

	return hr;
}

// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	HRESULT hr;    // HResult used by Safety Functions

	AFX_MANAGE_STATE(_afxModuleAddrThis);

	// Remove entries from the registry.
	// 
	// 先反注册掉安全组件，再反注册掉原组件，否则，当用regsvr32 /u 进行反注册时，
	// 将弹出错误提示：调用某某ocx文件的DllUnregisterServer函数出错，
	// 错误代码：0x80070002，用Error lookup工具查看，错误描述为：系统找不到指定的文件。 [4/11/2014-11:36:57 Dingshuai]
	hr=UnRegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForInitializing);
	if (FAILED(hr))
		return hr;

	hr=UnRegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForScripting);
	if (FAILED(hr))
		return hr;

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}