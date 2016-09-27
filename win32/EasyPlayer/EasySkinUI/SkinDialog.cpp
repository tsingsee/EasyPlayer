// DlgControlItem.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinDialog.h"
#include "RenderManager.h"
#include "SkinComboBox.h"
// CDlgControlItem 对话框

//边框宽度
CONST INT		BORDERWIDTH = 3;


IMPLEMENT_DYNAMIC(CSkinDialog, CDialog)

CSkinDialog::CSkinDialog(UINT nIDTemplate,LPCTSTR lpszFileName/*=NULL*/,CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	m_hMemDC = NULL;
	m_hMemBmp = m_hOldBmp = NULL;
	m_bClip = false;
	ParseWindow(lpszFileName);
	m_rcWindowSizeNow.SetRectEmpty();
}

CSkinDialog::~CSkinDialog()
{
	if (m_hMemDC != NULL && m_hMemBmp != NULL)
	{
		::SelectObject(m_hMemDC, m_hOldBmp);
		::DeleteObject(m_hMemBmp);
		::DeleteDC(m_hMemDC);
	}
	m_hMemDC = NULL;
	m_hMemBmp = m_hOldBmp = NULL;
}

void CSkinDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSkinDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_ERASEBKGND()
	ON_WM_NCCALCSIZE()
	ON_WM_NCACTIVATE()
	ON_WM_SYSCOMMAND()
	//ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

// CDlgControlItem 消息处理程序

void CSkinDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient;
	GetClientRect(&rcClient);

	CMemoryDC BufferDC(&dc,rcClient);

	int cx = 0, cy = 0;

	if (m_hMemBmp != NULL)
	{
		BITMAP bmpInfo = {0};
		::GetObject(m_hMemBmp, sizeof(BITMAP), &bmpInfo);
		cx = bmpInfo.bmWidth;
		cy = bmpInfo.bmHeight;
	}

	if (rcClient.Width() != cx || rcClient.Height() != cy)
	{
		if (m_hMemDC != NULL && m_hMemBmp != NULL)
		{
			::SelectObject(m_hMemDC, m_hOldBmp);
			::DeleteObject(m_hMemBmp);
		}

		m_hMemBmp = ::CreateCompatibleBitmap(BufferDC.GetSafeHdc(), rcClient.Width(), rcClient.Height());
		m_hOldBmp = (HBITMAP)::SelectObject(m_hMemDC, m_hMemBmp);
	}

	::SetBkMode(m_hMemDC,TRANSPARENT);
	UIRenderEngine->DrawColor(m_hMemDC,rcClient,m_colBK);

	CDC*pDC = CDC::FromHandle(m_hMemDC);

	DrawClientArea(pDC,rcClient.Width(), rcClient.Height());

	DrawImage(pDC);
	DrawString(pDC);
	
	::BitBlt(BufferDC.m_hDC, 0, 0, rcClient.Width(), rcClient.Height(), m_hMemDC, 0, 0, SRCCOPY);
}

BOOL CSkinDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetOwnHwnd(GetSafeHwnd());

	HDC hDC = ::GetDC(m_hWnd);
	m_hMemDC = ::CreateCompatibleDC(hDC);
	m_hMemBmp = ::CreateCompatibleBitmap(hDC, m_szWindowSize.cx,m_szWindowSize.cy);
	m_hOldBmp = (HBITMAP)::SelectObject(m_hMemDC, m_hMemBmp);
	::ReleaseDC(m_hWnd, hDC);

	SetExtrude(m_bExtrude);
	SetWindowPos(NULL,m_ptWindowPos.x,m_ptWindowPos.y,m_szWindowSize.cx,m_szWindowSize.cy, SWP_SHOWWINDOW);//SWP_NOMOVE

	ParseControl(this);

	RectifyControl(m_szWindowSize.cx,m_szWindowSize.cy);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

LRESULT CSkinDialog::OnNcHitTest(CPoint point)
{
	if( m_bExtrude )
	{
		CRect rcWindow;
		GetWindowRect(&rcWindow);

		if ((point.x <= rcWindow.left+BORDERWIDTH) && (point.y>BORDERWIDTH) && (point.y<rcWindow.bottom-BORDERWIDTH*2) )
			return HTLEFT;
		else if ((point.x >= rcWindow.right-BORDERWIDTH) && (point.y>BORDERWIDTH) && (point.y<rcWindow.bottom-BORDERWIDTH*2) )
			return HTRIGHT;
		else if ((point.y <= rcWindow.top+BORDERWIDTH) && (point.x>BORDERWIDTH) && (point.x<rcWindow.right-BORDERWIDTH*2))
			return HTTOP;
		else if ((point.y >= rcWindow.bottom-BORDERWIDTH) && (point.x>BORDERWIDTH) && (point.x<rcWindow.right-BORDERWIDTH*2))
			return HTBOTTOM;
		else if ((point.x <= rcWindow.left+BORDERWIDTH*2) && (point.y <= rcWindow.top+BORDERWIDTH*2))
			return HTTOPLEFT;
		else if ((point.x >= rcWindow.right-BORDERWIDTH*2) && (point.y <= rcWindow.top+BORDERWIDTH*2))
			return HTTOPRIGHT;
		else if ((point.x <= rcWindow.left+BORDERWIDTH*2) && (point.y >= rcWindow.bottom-BORDERWIDTH*2))
			return HTBOTTOMLEFT;
		else if ((point.x >= rcWindow.right-BORDERWIDTH*2) && (point.y >= rcWindow.bottom-BORDERWIDTH*2))
			return HTBOTTOMRIGHT;
		
		return CWnd::OnNcHitTest(point);
	}
	else
		return CDialog::OnNcHitTest(point);
}

void CSkinDialog::SetExtrude( bool bExtrude )
{
	m_bExtrude = bExtrude;
}

BOOL CALLBACK CSkinDialog::EnumChildProc( HWND hWndChild, LPARAM lParam )
{
	//获取位置
	CRect rcWindow;
	::GetWindowRect(hWndChild,&rcWindow);

	//创建区域
	if ((rcWindow.Width()>0)&&(rcWindow.Height()>0))
	{
		//变量定义
		ASSERT(lParam!=0L);
		CWnd * pEnumChildInfo=(CWnd *)lParam;

		//窗口判断
		HWND hWndParent=::GetParent(hWndChild);
		if (hWndParent!=pEnumChildInfo->GetSafeHwnd())
		{
			return TRUE;
		}

		static DWORD time = GetTickCount();


		if ( GetTickCount() - time > 33)
		{
			pEnumChildInfo->Invalidate(FALSE);
			time =  GetTickCount();
		}
	}

	return TRUE;
}

void CSkinDialog::SetClipChild( bool bClip )
{
	m_bClip = bClip;
}


void CSkinDialog::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if ( m_bClip )
	{
		//EnumChildWindows(GetSafeHwnd(),EnumChildProc,(LPARAM)(CWnd*)this);
	}

	//设置圆角
 	if( (m_szRcSize.cx !=0) && (m_szRcSize.cy != 0) )
 	{
#if defined(WIN32) && !defined(UNDER_CE)
		if( !IsIconic() && (m_szRcSize.cx != 0 || m_szRcSize.cy != 0) )
		{
			CRect rcWnd;
			GetWindowRect(&rcWnd);
			rcWnd.OffsetRect(-rcWnd.left, -rcWnd.top);
			rcWnd.right++; rcWnd.bottom++;
			HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, m_szRcSize.cx, m_szRcSize.cy);
			SetWindowRgn(hRgn, TRUE);
			DeleteObject(hRgn);
		}
#endif
 	}

	RectifyControl(cx, cy);

	if( !m_bDefaultSkin )
		OnNcPaint();
}

BOOL CSkinDialog::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

void CSkinDialog::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
	CRect rcWork = oMonitor.rcWork;
	CRect rcMonitor = oMonitor.rcMonitor;
	rcWork.OffsetRect(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	// 计算最大化时，正确的原点坐标
	lpMMI->ptMaxPosition.x	= rcWork.left;
	lpMMI->ptMaxPosition.y	= rcWork.top;

	lpMMI->ptMaxTrackSize.x = m_szMaxSize.cx==0 ? rcWork.Width() : m_szMaxSize.cx;
	lpMMI->ptMaxTrackSize.y = m_szMaxSize.cy==0 ? rcWork.Height() : m_szMaxSize.cy;

	lpMMI->ptMinTrackSize.x =m_szMinSize.cx;
	lpMMI->ptMinTrackSize.y =m_szMinSize.cy;


	__super::OnGetMinMaxInfo(lpMMI);
}


void CSkinDialog::RectifyControl( int cx, int cy )
{
	//移动准备
	HDWP hDwp=BeginDeferWindowPos(64);
	UINT uFlags=SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOZORDER;

	for (int i=0;i<(int)m_ControlArray.size();i++)
	{
		ISkinControl*pControl = m_ControlArray[i];

		if ( pControl != NULL )
		{
			CRect rcPos;

			//////////////////////////////位置////////////////////////////////////////////
			if (pControl->m_FixedPostion[0] == en_LTop )
			{
				rcPos.left = pControl->m_ptPosition.x;
				rcPos.top = pControl->m_ptPosition.y;
			}
			else if (pControl->m_FixedPostion[0] == en_RTop )
			{
				rcPos.left = cx-pControl->m_ptPosition.x;
				rcPos.top = pControl->m_ptPosition.y;
			}
			else if (pControl->m_FixedPostion[0] == en_LBottom )
			{
				rcPos.left = pControl->m_ptPosition.x;
				rcPos.top = cy-pControl->m_ptPosition.y;
			}
			else if (pControl->m_FixedPostion[0] == en_RBottom )
			{
				rcPos.left = cx-pControl->m_ptPosition.x;
				rcPos.top = cy-pControl->m_ptPosition.y;
			}
			else if (pControl->m_FixedPostion[0] == en_ScaleSize )//按比例缩放，只变位置
			{
				double fxScale=(double)cx/m_szWindowSize.cx;
				double fyScale=(double)cy/m_szWindowSize.cy;
				rcPos.left = fxScale*pControl->m_ptPosition.x;
				rcPos.top = fyScale*pControl->m_ptPosition.y;		

			}
			else if (pControl->m_FixedPostion[0] ==  en_ScalePos)//位置和大小，均按比例缩放
			{
				double fxScale=(double)cx/m_szWindowSize.cx;
				double fyScale=(double)cy/m_szWindowSize.cy;

				int nDesWidth = pControl->m_szSize.cx-pControl->m_ptPosition.x;
				int nDesHeight = pControl->m_szSize.cy-pControl->m_ptPosition.y;
				int nDesCenterX = (nDesWidth/2+pControl->m_ptPosition.x)*fxScale;
				int nDesCenterY = (nDesHeight/2+pControl->m_ptPosition.y)*fyScale;

				rcPos.left =  nDesCenterX - nDesWidth/2;
				rcPos.top =  nDesCenterY - nDesHeight/2;
			}

			///////////////////////////////大小///////////////////////////////////////////
			if (pControl->m_FixedPostion[1] == en_LTop )
			{
				rcPos.right = pControl->m_szSize.cx;
				rcPos.bottom = pControl->m_szSize.cy;
			}
			else if (pControl->m_FixedPostion[1] == en_RTop )
			{
				rcPos.right = cx-pControl->m_szSize.cx;
				rcPos.bottom = pControl->m_szSize.cy;
			}
			else if (pControl->m_FixedPostion[1] == en_LBottom )
			{
				rcPos.right = pControl->m_szSize.cx;
				rcPos.bottom = cy-pControl->m_szSize.cy;
			}
			else if (pControl->m_FixedPostion[1] == en_RBottom )
			{
				rcPos.right = cx-pControl->m_szSize.cx;
				rcPos.bottom = cy-pControl->m_szSize.cy;
			}
			else if (pControl->m_FixedPostion[1] == en_ScaleSize )//位置和大小，均按比例缩放
			{
				double fxScale=(double)cx/m_szWindowSize.cx;
				double fyScale=(double)cy/m_szWindowSize.cy;
				rcPos.right = fxScale*pControl->m_szSize.cx;
				rcPos.bottom = fyScale*pControl->m_szSize.cy;		

			}
			else if (pControl->m_FixedPostion[1] == en_ScalePos )//按比例缩放，只变位置
			{

				int nDesWidth = pControl->m_szSize.cx-pControl->m_ptPosition.x;
				int nDesHeight = pControl->m_szSize.cy-pControl->m_ptPosition.y;

				rcPos.right = nDesWidth+rcPos.left;
				rcPos.bottom = nDesHeight+rcPos.top;		

			}


			DeferWindowPos(hDwp,pControl->m_pOwnWnd->GetSafeHwnd(),NULL,rcPos.left,rcPos.top,rcPos.Width(),rcPos.Height(),SWP_NOACTIVATE);
		}
	}

	//结束调整
	LockWindowUpdate();
	EndDeferWindowPos(hDwp);
	UnlockWindowUpdate();

	////重新设置相应的属性，个别属性在设置位置前设置是不好使的
	for (int i=0;i<(int)m_ControlArray.size();i++)
	{
		ISkinControl*pControl = m_ControlArray[i];

		if ( pControl != NULL )
		{
			if(pControl->m_bTransparent)
				pControl->SetParentBack(m_hMemDC);

			TCHAR szClassName[MAX_PATH]={0};
			GetClassName(pControl->m_pOwnWnd->GetSafeHwnd(),szClassName,_countof(szClassName));

			if ( _tcscmp(TEXT("ComboBox"),szClassName) == 0 )
			{
				CSkinComboBox*pComboBox = (CSkinComboBox*)pControl->m_pOwnWnd;
				pComboBox->SetItemHeight(-1,pComboBox->GetEditItemHeight());
			}
		}
	}

	GetWindowRect(&m_rcWindowSizeNow);

	Invalidate(FALSE);
}

BOOL CSkinDialog::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CSkinDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch ( LOWORD(wParam) )
	{
	case 5000:
		PostQuitMessage(0);
		break;
	case 5001://最小化
		ShowWindow(SW_MINIMIZE);
		break;
	case 5002://最大化？？？
		ShowWindow(SC_MAXIMIZE);
		break;
	}

	return __super::OnCommand(wParam, lParam);
}

void CSkinDialog::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	if( m_bDefaultSkin ) return __super::OnNcCalcSize(bCalcValidRects,lpncsp);

	//最大化时，计算当前显示器最适合宽高度
	if ( IsZoomed())
	{	
		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
		CRect rcWork = oMonitor.rcWork;
		CRect rcMonitor = oMonitor.rcMonitor;
		rcWork.OffsetRect(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

		lpncsp->rgrc[0].left += GetSystemMetrics(SM_CXFRAME);
		lpncsp->rgrc[0].top += GetSystemMetrics(SM_CYSIZEFRAME);
		lpncsp->rgrc[0].right = lpncsp->rgrc[0].left + rcWork.Width();
		lpncsp->rgrc[0].bottom = lpncsp->rgrc[0].top + rcWork.Height();
	}

	return;
}

BOOL CSkinDialog::OnNcActivate(BOOL bActive)
{
	if( !m_bDefaultSkin )
	{
		//OnNcPaint(); 不能直接调用该方法，Windows采用消息队列形式进行事件处理，直接调用未能将该消息增加到消息队列，所以会出现失去焦点时，出现大粗边框的情况
		SendMessage(WM_NCPAINT);

		return TRUE;
	}

	return __super::OnNcActivate(bActive);
}

LRESULT CSkinDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if( !m_bDefaultSkin )
	{
		if(message == 0x00AE || // WM_NCUAHDRAWCAPTION
			message == 0x00AF ) // WM_NCUAHDRAWFRAME
		{
			return WM_NCPAINT;
		}
	}

	return __super::WindowProc(message, wParam, lParam);
}

void CSkinDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
	//最大化显示
	if ( nID == SC_MAXIMIZE )
	{
		SetMaxWindow();
	}
	else if ( nID == SC_RESTORE )
	{
		SetWindowPos(NULL,m_rcWindowSizeNow.left,m_rcWindowSizeNow.top,m_rcWindowSizeNow.Width(),m_rcWindowSizeNow.Height(),SWP_NOACTIVATE);
	}

	__super::OnSysCommand(nID, lParam);
}

void CSkinDialog::SetMaxWindow()
{
	//CRect rcWork(0,0,0,0);
	//::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);
	//MoveWindow(&rcWork);
 	MONITORINFO oMonitor = {};
 	oMonitor.cbSize = sizeof(oMonitor);
 	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTONEAREST), &oMonitor);
 	CRect rcWork = oMonitor.rcWork;
 	CRect rcMonitor = oMonitor.rcMonitor;
 	rcWork.OffsetRect(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

	SetWindowPos(NULL,rcWork.left,rcWork.top,rcWork.Width(),rcWork.Height(),SWP_NOACTIVATE);
}

CString CSkinDialog::GetItemNameByID(int nItemId ,CSkinButton **pSkinButton)
{
	for (int i=0;i<(int)m_ControlArray.size();i++)
	{
		ISkinControl* pControl = m_ControlArray[i];
		if ( pControl != NULL && pControl->m_nId == nItemId)
		{
			*pSkinButton = (CSkinButton*) GetDlgItem(pControl->m_nId);
			return pControl->m_sKeyName;
		}
	}
	return _T("");
}

void* CSkinDialog::GetItemByName(CString strName)
{
	for (int i=0;i<(int)m_ControlArray.size();i++)
	{
		ISkinControl* pControl = m_ControlArray[i];
		if ( pControl != NULL && pControl->m_sKeyName == strName)
		{
			return GetDlgItem(pControl->m_nId);
		}
	}
	return NULL;
}
//通过字串ID设置字串(注意：该设置只针对配置项为String有效，对于控件无效，ID为配置String ID)
void CSkinDialog::SetString( UINT uID, CString strText, BOOL bErase)
{
	CBuildDialog::SetString(uID,strText);
	Invalidate(bErase);
}
