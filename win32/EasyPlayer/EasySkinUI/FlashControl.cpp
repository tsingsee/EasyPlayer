#include "stdafx.h"
#include "FlashControl.h"

/////////////////////////////////////////////////////////////////////////////
// ShockwaveFlash

IMPLEMENT_DYNCREATE(CFlashControl, CWnd)

long CFlashControl::s_lCount = 0;

CFlashControl::CFlashControl()
{
#if 0
	if (0 == InterlockedExchangeAdd((volatile long *)&s_lCount, 1))
	{
		TCHAR szWorkDirectory[MAX_PATH]={0};
		UIRenderEngine->GetWorkDirectory(szWorkDirectory,MAX_PATH);
		StrCat(szWorkDirectory,TEXT("\\Flash8.ocx"));

		HMODULE	hmod=LoadLibrary (szWorkDirectory);
		ASSERT(hmod);
		if (hmod)
		{
			FARPROC  pReg =GetProcAddress (hmod,"DllRegisterServer");
			(*pReg)();
		}
	}
#endif

	m_bHasInit = FALSE;
	m_lOldProc = 0;
	m_pIFlashContrlSink = NULL;
}

CFlashControl::~CFlashControl()
{
#if 0
	if (1 == InterlockedExchangeAdd((volatile long *)&s_lCount, -1))
	{
		TCHAR szWorkDirectory[MAX_PATH]={0};
		UIRenderEngine->GetWorkDirectory(szWorkDirectory,MAX_PATH);
		StrCat(szWorkDirectory,TEXT("\\Flash8.ocx"));

		HMODULE	hmod=LoadLibrary (szWorkDirectory);
		ASSERT(hmod);
		if (hmod)
		{
			FARPROC  pReg =GetProcAddress (hmod,"DllUnregisterServer");
			(*pReg)();
		}
	}
#endif
}

LRESULT CFlashControl::FlashWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CFlashControl *pFlashCtrl = (CFlashControl *)FromHandle(hWnd);
	ASSERT(pFlashCtrl);

	switch ( msg )
	{
 	case WM_ERASEBKGND:
 		{
 			return TRUE;
 		}
	case WM_PAINT:
		{
			if ( pFlashCtrl->m_pIFlashContrlSink != NULL )
			{
				CDC*pDC = pFlashCtrl->GetDC();
				CRect rcClient;
				pFlashCtrl->GetClientRect(&rcClient);

				pFlashCtrl->m_pIFlashContrlSink->OnDraw(pDC,rcClient.Width(),rcClient.Height());
				pFlashCtrl->ReleaseDC(pDC);
			}

			return 0;
			return CallWindowProc((WNDPROC)(pFlashCtrl->m_lOldProc), hWnd, msg, wParam, lParam); 
		}
	case WM_RBUTTONDOWN:
		return 0;
	case WM_LBUTTONDOWN:
		{
			HWND hParentWnd = ::GetParent(hWnd);
			if ( hParentWnd != NULL )
			{
				UINT uID = pFlashCtrl->GetDlgCtrlID();
				::PostMessage(hParentWnd,WM_COMMAND,uID,0);
			}
			break;
		}
	}

	if ( msg == 144 || msg == 8720 || msg == 24 || msg == 2 || msg == 130)
	{
		return 0;
	}

	//调用原来的窗口过程
	return CallWindowProc((WNDPROC)(pFlashCtrl->m_lOldProc), hWnd, msg, wParam, lParam); 
}

BOOL CFlashControl::Init(IFlashContrlSink*pIFlashContrlSink)
{
	if (FALSE == m_bHasInit)
	{
		m_lOldProc = SetWindowLong(m_hWnd, GWL_WNDPROC, (long)FlashWinProc);
	}

	m_pIFlashContrlSink = pIFlashContrlSink;
	m_bHasInit = TRUE;

	return TRUE;
}

BOOL CFlashControl::CreateControl( CWnd* pParentWnd )
{
	if( !Create(NULL,WS_VISIBLE|WS_VISIBLE,CRect(0,0,0,0),pParentWnd,0) )
		return FALSE;

	m_pOwnWnd = this;

	return TRUE;
}

void CFlashControl::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	ISkinControl::SetAttribute(pstrName,pstrValue);

	if( _tcscmp(pstrName, _T("path")) == 0 ) 
	{
		TCHAR szWorkDirectory[MAX_PATH]={0};
		UIRenderEngine->GetWorkDirectory(szWorkDirectory,MAX_PATH);
		StrCat(szWorkDirectory,TEXT("\\"));
		StrCat(szWorkDirectory,pstrValue);

		LoadMovie(0,szWorkDirectory);
		Play();
	}
	else if( _tcscmp(pstrName, _T("transparent")) == 0 ) 
	{
		if( _tcscmp(pstrValue, _T("true")) == 0 ) 
			put_WMode( _bstr_t(_T("Transparent") ) );
	}
	else if( _tcscmp(pstrName, _T("pos")) == 0 )
	{ 
		//TODO:必须初始化就先设置位置，否则不显示flash，原因没查
		tagPositionData _PositionData;

		LPTSTR pstr = NULL;
		_PositionData.nFixedPostion[0] = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		_PositionData.nFixedPostion[1] = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
		_PositionData.ptPosition.x = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
		_PositionData.ptPosition.y = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
		_PositionData.szSize.cx = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
		_PositionData.szSize.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);  

		CRect rc;
		UIRenderEngine->GetRect(GetParent()->GetSafeHwnd(),&_PositionData,rc);

		SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOACTIVATE);
	}
	
}
