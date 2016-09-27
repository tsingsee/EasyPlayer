// SkinEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "Control.h"
#include "SkinEdit.h"


// CSkinEdit

IMPLEMENT_DYNAMIC(CSkinEdit, CEdit)

CSkinEdit::CSkinEdit()
{
	m_pBackImgN = NULL;
	m_pBackImgH = NULL;
	m_pIconImg = NULL;

	m_bFocus = m_bPress = m_bHover = m_bMouseTracking = FALSE;
	m_nIconWidth = 0;
	m_bHandCursor = false;

	m_bIsDefText = FALSE;
	m_cPwdChar = 0;
	m_ptClient.SetPoint(0,0);
	m_colBack=RGB(255,255,255);
	m_hBrush = CreateSolidBrush(m_colBack) ;
}

CSkinEdit::~CSkinEdit()
{
}


BEGIN_MESSAGE_MAP(CSkinEdit, CEdit)
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONUP()
END_MESSAGE_MAP()

BOOL CSkinEdit::SetBackNormalImage( LPCTSTR lpszFileName, CONST LPRECT lpNinePart /*= NULL*/ )
{
	UIRenderEngine->RemoveImage(m_pBackImgN);
	m_pBackImgN = UIRenderEngine->GetImage(lpszFileName);

	if (m_pBackImgN != NULL)
		m_pBackImgN->SetNinePart(lpNinePart);

	return (m_pBackImgN != NULL) ? TRUE : FALSE;
}

BOOL CSkinEdit::SetBackHotImage( LPCTSTR lpszFileName,CONST LPRECT lpNinePart /*= NULL*/ )
{
	UIRenderEngine->RemoveImage(m_pBackImgH);
	m_pBackImgH = UIRenderEngine->GetImage(lpszFileName);

	if (m_pBackImgH != NULL)
		m_pBackImgH->SetNinePart(lpNinePart);

	return (m_pBackImgH != NULL) ? TRUE : FALSE;
}

BOOL CSkinEdit::SetIconImage( LPCTSTR lpszFileName,bool bHandCursor /*= false*/ )
{
	UIRenderEngine->RemoveImage(m_pIconImg);
	m_pIconImg = UIRenderEngine->GetImage(lpszFileName);

	if (m_pIconImg != NULL)
	{
		m_nIconWidth = m_pIconImg->GetWidth();

		// 		CRect rcClient;
		// 		GetClientRect(&rcClient);
		// 		rcClient.right-=m_nIconWidth;
		// 
		// 		SetRect(&rcClient);

		//CRect rcWindow;
		//GetWindowRect(&rcWindow);
		//GetParent()->ScreenToClient(&rcWindow);
		//rcWindow.right+=1;
		//MoveWindow(rcWindow,TRUE);
	}

	m_bHandCursor = bHandCursor;

	return (m_pIconImg != NULL) ? TRUE : FALSE;
}

void CSkinEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	if (NULL != m_pIconImg && !m_pIconImg->IsNull())
	{
		if (bCalcValidRects)
		{
			lpncsp->rgrc[0].left += m_ptClient.x;
			lpncsp->rgrc[0].top += m_ptClient.y;
			lpncsp->rgrc[0].right -= m_nIconWidth;
			lpncsp->rgrc[0].bottom -= m_ptClient.y;
			lpncsp->rgrc[1] = lpncsp->rgrc[0];
		}
	}
	else
	{
		if (bCalcValidRects)
		{
			lpncsp->rgrc[0].left += 3;
			lpncsp->rgrc[0].top += 3;
			lpncsp->rgrc[0].right -= 3;
			lpncsp->rgrc[0].bottom -= 1;
			//lpncsp->rgrc[1] = lpncsp->rgrc[0];
		}
	}

	CEdit::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CSkinEdit::OnNcPaint()
{
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	rcWindow.OffsetRect(-rcWindow.left, -rcWindow.top);

	CDC *pWindowDC = GetWindowDC();

	CRect rcIcon;
	if (m_pIconImg != NULL && !m_pIconImg->IsNull())
	{
		int cxIcon = m_pIconImg->GetWidth();
		int cyIcon = m_pIconImg->GetHeight();

		CalcCenterRect(rcWindow, cxIcon, cyIcon, rcIcon);
		rcIcon.right = rcWindow.right - 2;
		rcIcon.left = rcIcon.right - cxIcon;
	}

	pWindowDC->FillSolidRect(&rcWindow,m_colBack);
	DrawParentWndBg(GetSafeHwnd(),pWindowDC->GetSafeHdc());

	//不要刷新整个客户区，客户区因为有字符串的缘故，当拉伸窗口时，非客户区和客户区两者因为刷新不同步，会造成客户区文字闪烁
	UIRenderEngine->DrawRect(pWindowDC->GetSafeHdc(),rcWindow,3,m_colBack);

	if (m_bHover)
	{
		if (m_pBackImgH != NULL && !m_pBackImgH->IsNull())
		{
			m_pBackImgH->DrawFrame(pWindowDC, rcWindow);
		}
		else
		{
			if (m_pBackImgN != NULL && !m_pBackImgN->IsNull())
				m_pBackImgN->DrawFrame(pWindowDC, rcWindow);
		}
	}
	else
	{
 		if (m_pBackImgN != NULL && !m_pBackImgN->IsNull())
		{
			m_pBackImgN->DrawFrame(pWindowDC, rcWindow);
		}
	}

	if (m_pIconImg != NULL && !m_pIconImg->IsNull())
	{
		//图标填充一下背景，防止未能刷新留下尾巴
		pWindowDC->FillSolidRect(&rcIcon,m_colBack);
		m_pIconImg->Draw(pWindowDC, rcIcon);
	}

	ReleaseDC(pWindowDC);
}

BOOL CSkinEdit::OnEraseBkgnd(CDC* pDC)
{
	CEdit::OnEraseBkgnd(pDC);
	return TRUE;
}

void CSkinEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rcIcon;
	CPoint pt;
	GetCursorPos(&pt);
	GetIconRect(rcIcon);

	if( PtInRect(&rcIcon,pt)) return;

	m_bPress = TRUE;

	__super::OnLButtonDown(nFlags, point);
}

void CSkinEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRect rcIcon;
	CPoint pt;
	GetCursorPos(&pt);
	GetIconRect(rcIcon);

	if( PtInRect(&rcIcon,pt)) 
	{
		return;
	}
	else
	{
		if (m_bPress)
		{
			m_bPress = FALSE;
			RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
		}
	}

	__super::OnLButtonUp(nFlags, point);
}

void CSkinEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bMouseTracking)
	{
		TrackMouseLeave(GetSafeHwnd());

		m_bMouseTracking = TRUE;
		m_bHover = TRUE;
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}

	__super::OnMouseMove(nFlags, point);
}

LRESULT CSkinEdit::OnMouseLeave( WPARAM wparam, LPARAM lparam )
{
	m_bMouseTracking = FALSE;
	m_bHover = FALSE;
	RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return TRUE;
}

void CSkinEdit::OnDestroy()
{
	__super::OnDestroy();

	UIRenderEngine->RemoveImage(m_pBackImgN);
	UIRenderEngine->RemoveImage(m_pBackImgH);
	UIRenderEngine->RemoveImage(m_pIconImg);
	RemoveScorll();
}

void CSkinEdit::OnSetFocus(CWnd* pOldWnd)
{
	__super::OnSetFocus(pOldWnd);

	if (m_bIsDefText)
	{
		m_bIsDefText = FALSE;
		SetPasswordChar(m_cPwdChar);
		SetWindowText(_T(""));
	}

	m_bFocus = TRUE;
	//Invalidate(FALSE);

	OnNcPaint();
}

void CSkinEdit::OnKillFocus(CWnd* pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	if (GetWindowTextLength() <= 0 && !m_strDefText.IsEmpty())
	{
		m_bIsDefText = TRUE;
		m_cPwdChar = GetPasswordChar();
		SetPasswordChar(0);
		SetWindowText(m_strDefText);
	}

	m_bFocus = FALSE;
	//Invalidate(FALSE);

	OnNcPaint();
}

BOOL CSkinEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CRect rcIcon;
	CPoint pt;
	GetCursorPos(&pt);
	GetIconRect(rcIcon);

	if( PtInRect(&rcIcon,pt))
	{
		if( m_bHandCursor )
			SetCursor(LoadCursor(NULL,IDC_HAND));
		else
			SetCursor(LoadCursor(NULL,IDC_ARROW));

		return TRUE;
	}

	return __super::OnSetCursor(pWnd, nHitTest, message);
}

void CSkinEdit::GetIconRect( RECT &rcIcon )
{
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	CalcCenterRect(rcWindow, m_nIconWidth, rcWindow.Height(), rcIcon);
	rcIcon.right = rcWindow.right - 2;
	rcIcon.left = rcIcon.right - m_nIconWidth;
}

BOOL CSkinEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return 	__super::CreateEx(
		WS_EX_LEFT|WS_EX_LTRREADING|WS_EX_RIGHTSCROLLBAR|WS_EX_NOPARENTNOTIFY|WS_EX_CLIENTEDGE,
		TEXT("EDIT"),
		NULL,
		WS_VISIBLE|WS_CHILDWINDOW|WS_TABSTOP|ES_LEFT|ES_AUTOHSCROLL|dwStyle,
		rect,pParentWnd,nID);


	return __super::Create(dwStyle, rect, pParentWnd, nID);
}

void CSkinEdit::SetDefaultText( LPCTSTR lpszText )
{
	m_strDefText = lpszText;
}

void CSkinEdit::SetDefaultTextMode( BOOL bIsDefText )
{
	if (bIsDefText == m_bIsDefText)
		return;

	m_bIsDefText = bIsDefText;
	if (m_bIsDefText)
	{
		m_cPwdChar = GetPasswordChar();
		SetPasswordChar(0);
		SetWindowText(m_strDefText);
	}
	else
	{
		SetPasswordChar(m_cPwdChar);
		SetWindowText(_T(""));
	}
}

HBRUSH CSkinEdit::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	if (m_bIsDefText)
		pDC->SetTextColor(m_colDefText);
	else
		pDC->SetTextColor(m_colNormalText);

	if (m_hBrush)
	{
		return m_hBrush ;
	}
	return (HBRUSH)NULL_BRUSH;
}

LRESULT CSkinEdit::OnNcHitTest(CPoint point)
{
	CRect rcIcon;
	GetIconRect(rcIcon);

	if( PtInRect(&rcIcon,point))
	{
		return HTBORDER;
	}

	return __super::OnNcHitTest(point);
}

void CSkinEdit::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	CWnd *pWnd = GetParent();

	if ( pWnd != NULL && pWnd->GetSafeHwnd() != NULL )
	{
		CRect rcIcon;
		CPoint pt;
		GetCursorPos(&pt);
		GetIconRect(rcIcon);

		if( PtInRect(&rcIcon,pt)) 
		{
			//m_bSendMsg = true;
			pWnd->PostMessage(WM_EDIT_CLICK,GetDlgCtrlID());
			//m_bSendMsg = false;
		}
	}

	__super::OnNcLButtonUp(nHitTest, point);
}

void CSkinEdit::SetClientPoint( CPoint pt )
{
	m_ptClient = pt;
}

void CSkinEdit::ParseItem( CXmlNode *root )
{
	if( root == NULL ) return;

	int nAttributes = root->GetAttributeCount();

	LPCTSTR pstrClass = NULL;
	LPCTSTR pstrName = NULL;
	LPCTSTR pstrValue = NULL;

	for( int i = 0; i < nAttributes; i++ ) 
	{
		pstrClass = root->GetName();
		pstrName = root->GetAttributeName(i);
		pstrValue = root->GetAttributeValue(i);

		if( _tcscmp(pstrClass, _T("bknormalimg")) == 0 ) 	
		{
			if( _tcscmp(pstrName, _T("value")) == 0 ) SetBackNormalImage(pstrValue);
			else if( _tcscmp(pstrName, _T("ninepart")) == 0 )
			{
				LPTSTR pstr = NULL;
				CRect rc;
				rc.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rc.right = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rc.bottom = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   

				if (m_pBackImgN != NULL)
					m_pBackImgN->SetNinePart(&rc);
			}
		}
		else if( _tcscmp(pstrClass, _T("bkhotimg")) == 0 ) 	
		{
			if( _tcscmp(pstrName, _T("value")) == 0 ) SetBackHotImage(pstrValue);
			else if( _tcscmp(pstrName, _T("ninepart")) == 0 )
			{
				LPTSTR pstr = NULL;
				CRect rc;
				rc.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rc.right = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rc.bottom = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   

				if (m_pBackImgH != NULL)
					m_pBackImgH->SetNinePart(&rc);
			}
		}
	}
}

void CSkinEdit::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	ISkinControl::SetAttribute(pstrName,pstrValue);

	if( _tcscmp(pstrName, _T("defaulttext")) == 0 )  
	{
		SetDefaultText(pstrValue);
		SetDefaultTextMode(TRUE);
	}
	else if( _tcscmp(pstrName, _T("iconimage")) == 0 )  
	{
		SetIconImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("passwordchar")) == 0 )  
	{
		SetPasswordChar(pstrValue[0]);
	}
	else if( _tcscmp(pstrName, _T("clientpos")) == 0 )  
	{
		LPTSTR pstr = NULL;
		CPoint pt;
		pt.x = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		pt.y = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 

		SetClientPoint(pt);
	}
	else if( _tcscmp(pstrName, _T("handcur")) == 0 )  
	{
		m_bHandCursor = _tcscmp(pstrValue, _T("true")) == 0;
	}
	else if( _tcscmp(pstrName, _T("scrollimage")) == 0 )  
	{
		SetScrollImage(this,pstrValue);
	}
	//else if( _tcscmp(pstrName, _T("multiline")) == 0 )  
	//{
	//	if( _tcscmp(pstrValue, _T("true")) == 0 )  
	//	//	//SetWindowLong(GetSafeHwnd(),GWL_STYLE,GetWindowLong(GetSafeHwnd(),GWL_STYLE)| ES_MULTILINE|ES_WANTRETURN|ES_AUTOVSCROLL| WS_VSCROLL);
	//		ModifyStyle(ES_AUTOHSCROLL,ES_MULTILINE|ES_WANTRETURN|ES_AUTOVSCROLL| WS_VSCROLL );
	//	//ModifyStyle(ES_MULTILINE|ES_WANTRETURN|ES_AUTOVSCROLL| WS_VSCROLL,0);
	//}
}

BOOL CSkinEdit::CreateControl( CWnd* pParentWnd )
{
	if( !CEdit::Create(WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),pParentWnd,0) )
		return FALSE;

	SetFont(CFont::FromHandle(UIRenderEngine->GetDeaultFont()));

	m_pOwnWnd = this;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CMultiSkinEdit, CSkinEdit)

BEGIN_MESSAGE_MAP(CMultiSkinEdit,CSkinEdit)
END_MESSAGE_MAP()

BOOL CMultiSkinEdit::CreateControl( CWnd* pParentWnd )
{
	if( !CEdit::Create(WS_VISIBLE|WS_CHILD|ES_MULTILINE|ES_WANTRETURN|ES_AUTOVSCROLL| WS_VSCROLL,CRect(0,0,0,0),pParentWnd,0) )
		return FALSE;

	SetFont(CFont::FromHandle(UIRenderEngine->GetDeaultFont()));

	m_pOwnWnd = this;

	return TRUE;
}
