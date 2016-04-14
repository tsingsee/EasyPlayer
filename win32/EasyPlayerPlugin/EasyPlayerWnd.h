/*
	Copyright (c) 2013-2014 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
// 播放窗口界面类 [11/8/2015 Dingshuai]
// Add by SwordTwelve

#pragma once


#include "Win/PluginWindowWin.h"
#include <gdiplus.h>
#include "../../build\projects\EasyPlayerPlugin\gen\global\resource.h"

FB_FORWARD_PTR(EasyPlayerWnd)

class EasyPlayerWnd : public FB::PluginWindowWin
{
public:
	EasyPlayerWnd(const FB::WindowContextWin& ctx);
	~EasyPlayerWnd(void);
	static LRESULT CALLBACK _WinProc(HWND hWnd, UINT uMsg, WPARAM wParam,
		LPARAM lParam);
	typedef std::map<void*,EasyPlayerWnd*> PluginWindowMap;


	//Member Function
protected:
	static PluginWindowMap m_windowMap;
	//窗口绘制函数
	virtual void DrawClientArea(HDC hdc,int nWidth,int nHeight);

};

