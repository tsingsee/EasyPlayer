/*
	Copyright (c) 2013-2014 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.EasyDarwin.org
*/
#include "EasyPlayerWnd.h"

#include "ConstructDefaultPluginWindows.h"
#include "precompiled_headers.h" // On windows, everything above this line in PCH

using namespace FB;

FB::PluginWindowWin* FB::createPluginWindowWin(const FB::WindowContextWin& ctx)
{
	return new EasyPlayerWnd(ctx);
}

//接口释放
#define SafeRelease(pObject) { if (pObject!=NULL) { pObject->Release(); pObject=NULL; } }

EasyPlayerWnd::PluginWindowMap EasyPlayerWnd::m_windowMap;

EasyPlayerWnd::EasyPlayerWnd(const FB::WindowContextWin& ctx) : FB::PluginWindowWin(ctx)
{
	 m_hWnd = ctx.handle;
	/**/
	Gdiplus::GdiplusStartupInput gdiInput;
	Gdiplus::GdiplusStartup(&m_gdiplusStartupToken,&gdiInput,NULL);
	/**/
	// subclass window so we can intercept window messages 
	lpOldWinProc = SubclassWindow(m_hWnd, (WNDPROC)&EasyPlayerWnd::_WinProc);
	// associate window with this object so that we can route events properly
	m_windowMap[static_cast<void*>(m_hWnd)] = this;

}

EasyPlayerWnd::~EasyPlayerWnd(void)
{
	WNDPROC current_wnd_proc = reinterpret_cast<WNDPROC>(
		GetWindowLongPtr(m_hWnd, GWLP_WNDPROC));
	if (current_wnd_proc == &PluginWindowWin::_WinProc)
		SubclassWindow(m_hWnd, lpOldWinProc);

	PluginWindowMap::iterator it = m_windowMap.find(static_cast<void*>(m_hWnd));
	if (it != m_windowMap.end()) 
		m_windowMap.erase(it);

	Gdiplus::GdiplusShutdown(m_gdiplusStartupToken);
}

void EasyPlayerWnd::DrawClientArea(HDC hdc, int nWidth, int nHeight)
{
	RECT rcClient;
	HBRUSH brash = ::CreateSolidBrush(RGB(0, 0, 0));
	::GetClientRect(m_hWnd, &rcClient);
	::FillRect(hdc, &rcClient, brash);
	::DeleteObject(brash); //释放画刷 

	int width,height;

	//加载图像
	//Gdiplus::Image image(TEXT("D:\\Dingshuai\\FireBreath-master\\build\\bin\\EasyPlayerPlugin\\Debug\\pic.png")); //

#if 1
	HINSTANCE hInstance = ::GetModuleHandle(TEXT("npEasyPlayerPlugin.dll"));
	if (!hInstance)
	{
		MessageBox(m_hWnd, TEXT("hInstance=NULL!"),TEXT("OO"),MB_OK);
		return ;
	}
	//查找资源
	//MessageBox(m_hWnd,MAKEINTRESOURCE(IDB_PNG_EASYLOGO),TEXT("OO"),MB_OK);
	HRSRC hResource=FindResource(hInstance, MAKEINTRESOURCE(IDB_PNG_EASYLOGO), TEXT("PNG"));
	if (hResource==NULL) 
	{
		MessageBox(m_hWnd, TEXT("hResource=NULL!"),TEXT("OO"),MB_OK);
		return ;
	}

	//读取资源
	DWORD dwImageSize=SizeofResource(hInstance, hResource);
	LPVOID pImageBuffer=LoadResource(hInstance, hResource);

	//创建数据
	IStream * pIStream=NULL;
	if (CreateStreamOnHGlobal(NULL,TRUE,&pIStream)!=S_OK)
	{
		MessageBox(m_hWnd, TEXT("CreateStreamOnHGlobal=NULL!"),TEXT("OO"),MB_OK);
		return ;
	}

	//写入数据
	pIStream->Write(pImageBuffer,dwImageSize,NULL);
	Gdiplus::Image image(pIStream); //(L"D:\Dingshuai\FireBreath-master\build\bin\EasyPlayerPlugin\Debug\pic.png");

#endif

	if(image.GetLastStatus() != Gdiplus::Status::Ok)
	{
		MessageBox(m_hWnd, TEXT("图片无效!"),TEXT("D:\\Dingshuai\\FireBreath-master\\buildbin\\EasyPlayerPlugin\\Debug\\pic.png"),MB_OK);
		// Release the device context
		return ;
	}
	//取得宽度和高度
	width = image.GetWidth();
	height = image.GetHeight();

	//更新窗口大小
	//set_window_size(hWnd,width,height);
	hdc = GetDC(m_hWnd);

	//绘图
	int nStartX = (rcClient.right-rcClient.left-184)/2;
	int nStartY =  (rcClient.bottom-rcClient.top-184)/2;
	Gdiplus::Graphics graphics(hdc);
	graphics.DrawImage(&image,nStartX,nStartY,width,height);
	ReleaseDC(m_hWnd,hdc);

#if 1
	//释放资源
	SafeRelease(pIStream);
#endif
}

LRESULT CALLBACK EasyPlayerWnd::_WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PluginWindowMap::iterator it = m_windowMap.find(static_cast<void*>(hWnd));
	if (it == m_windowMap.end()) 
		// This could happen if we're using this as a message-only window
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	EasyPlayerWnd *win = it->second;


	LRESULT lResult(0);
	// Try to handle the event through the plugin instace; if that doesn't work, handle it through the default winproc
	if (win->WinProc(hWnd, uMsg, wParam, lParam, lResult))
		return lResult;
	else if (win->m_callOldWinProc)
		return win->lpOldWinProc(hWnd, uMsg, wParam, lParam);
	else
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	return 0;
}
