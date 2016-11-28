
// EasyPlayer.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

// get program path for globle use [6/12/2016 SwordTwelve]
// 通用结构定义 
struct ModuleFileInfomations
{
	CString		strFullPath;
	CString		strPath;
	CString		strDrive;
	CString		strDir;
	CString		strName;
	CString		strExt;
};
const ModuleFileInfomations&    GetModuleFileInformations();

#define GET_MODULE_FILE_INFO    (GetModuleFileInformations())

// CEasyPlayerApp:
// 有关此类的实现，请参阅 EasyPlayer.cpp
//

class CEasyPlayerApp : public CWinApp
{
public:
	CEasyPlayerApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int ExitInstance();
};

extern CEasyPlayerApp theApp;