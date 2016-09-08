// ButtonEx.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinButton.h"


// CSkinButton

IMPLEMENT_DYNAMIC(CSkinButton, CButton)

CSkinButton::CSkinButton()
{
	m_pBackImgN = m_pBackImgH = m_pBackImgD = m_pBackImgF = NULL;
	m_pCheckImgN = m_pCheckImgH = m_pCheckImgTickN = m_pCheckImgTickH = NULL;
	m_pArrowImg = m_pIconImg = NULL;
	
	m_bFocus = m_bPress = m_bHover = m_bMouseTracking = FALSE;
	m_nBtnType = en_PushButton;

	m_hMenu = NULL;
	m_hBackDC = NULL;
	m_bPushed = false;
	m_lpszUrl = NULL;
	m_bCurPushState = false;

}

CSkinButton::~CSkinButton()
{
}


BEGIN_MESSAGE_MAP(CSkinButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CSkinButton 消息处理程序
void CSkinButton::SetButtonType( UI_BUTTON_TYPE type )
{
	m_nBtnType = type;

	if ( type == en_CheckButton ) ModifyStyle(0,BS_AUTOCHECKBOX);
	else if ( type == en_RadioButton ) ModifyStyle(0,BS_AUTORADIOBUTTON);
}

//鼠标移动消息
void CSkinButton::OnMouseMove(UINT nFlags, CPoint point)
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

//鼠标离开消息
LRESULT CSkinButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bMouseTracking = FALSE;
	m_bHover = FALSE;
	RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return 0;
}

void CSkinButton::RelayEventCamMove(int nType)
{
// 	HWND hParentHwnd,hThisHwnd;
// 	hParentHwnd = GetParent()->GetSafeHwnd();
// 	hThisHwnd=GetSafeHwnd();
// 	int nThisID=GetDlgCtrlID();
// 	if(hParentHwnd && hThisHwnd)
// 	{
// 		::PostMessage(hParentHwnd,WM_BUSEDDOWNUP_BUTTON,(int)nThisID,nType);
// 	}
}

//鼠标按下消息
void CSkinButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	//向父窗口传递消息
	RelayEventCamMove(1);
	m_bPress = TRUE;
	RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	__super::OnLButtonDown(nFlags, point);
}

//鼠标抬起消息
void CSkinButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bPress)
	{

		//static bool bPushed = false;

		if( !m_bPushed && !m_bCurPushState )
			m_bPress = FALSE;

		if ( m_bPushed )
		{
			if( m_bCurPushState ) m_bPress = FALSE;
			else m_bPress = TRUE;

			m_bCurPushState = !m_bCurPushState;
		}

			
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
		
		if (m_nBtnType == en_MenuButton)
		{
			if (::IsMenu(m_hMenu))
			{
				CRect rc;
				GetClientRect(&rc);
				ClientToScreen(&rc);
				::TrackPopupMenuEx(m_hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
					rc.left, rc.bottom, ::GetParent(GetSafeHwnd()), NULL);
			}
		}

		if ( m_lpszUrl != NULL )
		{
			OpenLink(m_lpszUrl);
		}	
		
		//向父窗口传递消息
		RelayEventCamMove(2);

	}

	__super::OnLButtonUp(nFlags, point);
}

//鼠标双击消息
void CSkinButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bPress = TRUE;
	RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	__super::OnLButtonDown(nFlags, point);
}

//获取焦点消息
void CSkinButton::OnSetFocus(CWnd* pOldWnd)
{
	__super::OnSetFocus(pOldWnd);

	m_bFocus = TRUE;
	RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
}

//失去焦点消息
void CSkinButton::OnKillFocus(CWnd* pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	m_bFocus = FALSE;
	RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
}

//重画消息
BOOL CSkinButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

//设置资源
BOOL CSkinButton::SetBackImage( LPCTSTR lpNormal, LPCTSTR lpHoven, LPCTSTR lpDown, LPCTSTR lpFocus,CONST LPRECT lprcNinePart/*=NULL*/ )
{
	UIRenderEngine->RemoveImage(m_pBackImgN);
	UIRenderEngine->RemoveImage(m_pBackImgH);
	UIRenderEngine->RemoveImage(m_pBackImgD);
	UIRenderEngine->RemoveImage(m_pBackImgF);

	m_pBackImgN = UIRenderEngine->GetImage(lpNormal);
	m_pBackImgH = UIRenderEngine->GetImage(lpHoven);
	m_pBackImgD = UIRenderEngine->GetImage(lpDown);
	m_pBackImgF = UIRenderEngine->GetImage(lpFocus);

	if( lprcNinePart != NULL )
	{
		if (m_pBackImgN != NULL)
			m_pBackImgN->SetNinePart(lprcNinePart);

		if (m_pBackImgH != NULL)
			m_pBackImgH->SetNinePart(lprcNinePart);

		if (m_pBackImgD != NULL)
			m_pBackImgD->SetNinePart(lprcNinePart);

		if (m_pBackImgF != NULL)
			m_pBackImgF->SetNinePart(lprcNinePart);
	}

	if ((lpNormal != NULL && NULL == m_pBackImgN) || 
		(lpHoven  != NULL && NULL == m_pBackImgH) ||
		(lpDown   != NULL && NULL == m_pBackImgD) ||
		(lpFocus  != NULL && NULL == m_pBackImgF))
		return FALSE;
	else
		return TRUE;
}

//加载资源
BOOL CSkinButton::SetCheckImage( LPCTSTR lpNormal, LPCTSTR lpHoven, LPCTSTR lpTickNormal, LPCTSTR lpTickHoven )
{
	UIRenderEngine->RemoveImage(m_pCheckImgN);
	UIRenderEngine->RemoveImage(m_pCheckImgH);
	UIRenderEngine->RemoveImage(m_pCheckImgTickN);
	UIRenderEngine->RemoveImage(m_pCheckImgTickH);

	m_pCheckImgN = UIRenderEngine->GetImage(lpNormal);
	m_pCheckImgH = UIRenderEngine->GetImage(lpHoven);
	m_pCheckImgTickN = UIRenderEngine->GetImage(lpTickNormal);
	m_pCheckImgTickH = UIRenderEngine->GetImage(lpTickHoven);

	if (NULL == m_pCheckImgN || NULL == m_pCheckImgH
		|| NULL == m_pCheckImgTickN || NULL == m_pCheckImgTickH)
		return FALSE;
	else
		return TRUE;
}

//加载资源
BOOL CSkinButton::SetIconImage( LPCTSTR lpszFileName )
{
	UIRenderEngine->RemoveImage(m_pIconImg);

	m_pIconImg = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == m_pIconImg)
		return FALSE;
	else
		return TRUE;
}

//加载资源
BOOL CSkinButton::SetMenuImage( LPCTSTR lpszFileName )
{
	UIRenderEngine->RemoveImage(m_pArrowImg);

	m_pArrowImg = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == m_pArrowImg)
		return FALSE;
	else
		return TRUE;
}

//设置大小
void CSkinButton::SetSize( int nWidth,int nHeight )
{
	SetWindowPos(NULL,0,0,nWidth,nHeight,SWP_NOMOVE);
}

//设置菜单
void CSkinButton::SetMenu( HMENU hMenu )
{
	m_hMenu = hMenu;
}


//销毁消息
void CSkinButton::OnDestroy()
{
	__super::OnDestroy();

	UIRenderEngine->RemoveImage(m_pBackImgN);
	UIRenderEngine->RemoveImage(m_pBackImgH);
	UIRenderEngine->RemoveImage(m_pBackImgD);
	UIRenderEngine->RemoveImage(m_pBackImgF);

	UIRenderEngine->RemoveImage(m_pCheckImgN);
	UIRenderEngine->RemoveImage(m_pCheckImgH);
	UIRenderEngine->RemoveImage(m_pCheckImgTickN);
	UIRenderEngine->RemoveImage(m_pCheckImgTickH);

	UIRenderEngine->RemoveImage(m_pArrowImg);
	UIRenderEngine->RemoveImage(m_pIconImg);

	m_bFocus = m_bPress = m_bHover = m_bMouseTracking = FALSE;
	m_nBtnType = en_PushButton;
	m_hMenu = NULL;
	m_dwTextAlign = 0;
	m_hBackDC = NULL;
}

//消息循环
LRESULT CSkinButton::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//针对RadioButton组，选择其他Radio时，拦截Check消息
	if ( message == BM_SETCHECK )
	{
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}

	//鼠标移开按钮之后，会出发此消息，这里需要重新捕获，重绘按钮
	if ( message == BM_SETSTATE )
	{
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}
	
	return __super::DefWindowProc(message, wParam, lParam);
}

//绘制消息
void CSkinButton::OnPaint()
{
	CPaintDC dc(this); 

	CRect rcClient;
	GetClientRect(&rcClient);

	CMemoryDC MemDC(&dc, rcClient);

	//绘制背景
	DrawParentWndBg(GetSafeHwnd(),MemDC->GetSafeHdc());

	switch (m_nBtnType)
	{
	case en_PushButton:
		DrawPushButton(&MemDC,rcClient);
		break;

	case en_CheckButton:
	case en_RadioButton:
		DrawCheckButton(&MemDC,rcClient);
		break;

	case en_IconButton:
		DrawIConButton(&MemDC,rcClient);
		break;

	case en_MenuButton:
		DrawMenuButton(MemDC,rcClient);
		break;
	}
}

//绘制按钮
void CSkinButton::DrawPushButton(CDC* pDC,RECT &rcClient)
{
	if (m_bPress)		// 鼠标左键按下状态
	{
		if (m_pBackImgD != NULL && !m_pBackImgD->IsNull())
			m_pBackImgD->DrawImage(pDC, rcClient);
	}
	else if (m_bHover)	// 鼠标悬停状态
	{
		if (m_pBackImgH != NULL && !m_pBackImgH->IsNull())
			m_pBackImgH->DrawImage(pDC, rcClient);
	}
	else if (m_bFocus)	// 焦点状态
	{
		if (m_pBackImgF != NULL && !m_pBackImgF->IsNull())
			m_pBackImgF->DrawImage(pDC, rcClient);
	}
	else				// 普通状态
	{
		if (m_pBackImgN != NULL && !m_pBackImgN->IsNull())
			m_pBackImgN->DrawImage(pDC, rcClient);
	}

	if (m_bPress)
		::OffsetRect(&rcClient,1, 1);

	CString strText;
	// 	GetBtnText()从而可通过外部设置 [2/25/2016 Dingshuai]
	strText = GetBtnText();

	BOOL bHasText = FALSE;
	if (strText.GetLength() > 0)
		bHasText = TRUE;

	BOOL bHasIcon = FALSE;
	if (m_pIconImg != NULL && !m_pIconImg->IsNull())
		bHasIcon = TRUE;

	if (bHasIcon && bHasText)	// 带图标和文字
	{
		int cxIcon = m_pIconImg->GetWidth();
		int cyIcon = m_pIconImg->GetHeight();

		int nMode = pDC->SetBkMode(TRANSPARENT);
		
		pDC->SelectObject(GetCtrlFont());

		pDC->SetTextColor(IsWindowEnabled()?m_colNormalText:m_colDisableText);
		CRect rcText(0,0,0,0);	// 计算文字宽高
		pDC->DrawText(strText, &rcText, DT_SINGLELINE | DT_CALCRECT);

		int cx = cxIcon+3+rcText.Width();
		int cy = cyIcon;

		CRect rcCenter;
		CalcCenterRect(rcClient, cx, cy, rcCenter);

		CRect rcIcon(rcCenter.left, rcCenter.top, rcCenter.left+cxIcon, rcCenter.bottom);
		m_pIconImg->DrawImage(pDC, rcIcon);

		UINT nFormat = m_dwTextAlign ? m_dwTextAlign : DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
		rcText = CRect(rcIcon.right+3, rcClient.top, rcIcon.right+3+rcText.Width(), rcClient.bottom);
		pDC->DrawText(strText, &rcText, nFormat);

		pDC->SetBkMode(nMode);
	}
	else if (bHasIcon)	// 仅图标
	{
		int cxIcon = m_pIconImg->GetWidth();
		int cyIcon = m_pIconImg->GetHeight();

		CRect rcIcon;
		CalcCenterRect(rcClient, cxIcon, cyIcon, rcIcon);

		m_pIconImg->DrawImage(pDC, rcIcon);
	}
	else if (bHasText)	// 仅文字
	{
		UINT nFormat = m_dwTextAlign ? m_dwTextAlign : DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;

		int nMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(GetCtrlFont());
		
		if ( m_bHover ) pDC->SetTextColor(m_colSelectText);
		else  pDC->SetTextColor(IsWindowEnabled()?m_colNormalText:m_colDisableText);
		pDC->DrawText(strText, &rcClient, nFormat);
		pDC->SetBkMode(nMode);
	}
}

void CSkinButton::DrawCheckButton( CDC* pDC,RECT &rcClient )
{
	int nWidth = 15, nHeight = 15;
	if (m_pCheckImgN != NULL && !m_pCheckImgN->IsNull())
	{
		nWidth = m_pCheckImgN->GetWidth();
		nHeight = m_pCheckImgN->GetHeight();
	}

	CRect rcCheck, rcText;

	rcCheck.left = rcClient.left;
	rcCheck.right = rcCheck.left + nWidth;
	rcCheck.top = ((rcClient.bottom-rcClient.top) - nHeight) / 2;
	rcCheck.bottom = rcCheck.top + nHeight;

	rcText = rcClient;
	rcText.left = rcCheck.right + 4;

	BOOL bChecked = ((GetCheck() == BST_CHECKED) ? TRUE : FALSE);

	if (m_bHover)	// 鼠标悬停状态
	{
		if (bChecked)
		{
			if (m_pCheckImgTickH != NULL && !m_pCheckImgTickH->IsNull())
				m_pCheckImgTickH->DrawImage(pDC, rcCheck);
		}
		else
		{
			if (m_pCheckImgH != NULL && !m_pCheckImgH->IsNull())
				m_pCheckImgH->DrawImage(pDC, rcCheck);
		}
	}
	else	// 普通状态
	{
		if (bChecked)
		{
			if (m_pCheckImgTickN != NULL && !m_pCheckImgTickN->IsNull())
				m_pCheckImgTickN->DrawImage(pDC, rcCheck);
		}
		else
		{
			if (m_pCheckImgN != NULL && !m_pCheckImgN->IsNull())
				m_pCheckImgN->DrawImage(pDC, rcCheck);
		}
	}

	CString strText;
	GetWindowText(strText);

	if (strText.GetLength() > 0)
	{
		UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;

		int nMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(GetCtrlFont());

		pDC->SetTextColor(IsWindowEnabled()?m_colNormalText:m_colDisableText);

		pDC->DrawText(strText, &rcText, nFormat);
		pDC->SetBkMode(nMode);
	}
}

void CSkinButton::DrawIConButton( CDC* pDC,RECT &rcClient )
{
	if (m_bPress)	// 鼠标左键按下状态
	{
		if (m_pBackImgD != NULL && !m_pBackImgD->IsNull())
			m_pBackImgD->Draw(pDC, rcClient);
	}
	else if (m_bHover)	// 鼠标悬停状态
	{
		if (m_pBackImgH != NULL && !m_pBackImgH->IsNull())
			m_pBackImgH->Draw(pDC, rcClient);
	}

	if (m_bPress)
		::OffsetRect(&rcClient,1, 1);

	CString strText;
	GetWindowText(strText);

	BOOL bHasText = FALSE;
	if (strText.GetLength() > 0)
		bHasText = TRUE;

	BOOL bHasIcon = FALSE;
	if (m_pIconImg != NULL && !m_pIconImg->IsNull())
		bHasIcon = TRUE;

	if (bHasIcon && bHasText)	// 带图标和文字
	{
		int cxIcon = m_pIconImg->GetWidth();
		int cyIcon = m_pIconImg->GetHeight();

		int nMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(GetCtrlFont());

		CRect rcText(0,0,0,0);	// 计算文字宽高
		pDC->DrawText(strText, &rcText, DT_SINGLELINE | DT_CALCRECT);

		int cx = cxIcon+3+rcText.Width();
		int cy = cyIcon;

		CRect rcCenter;
		CalcCenterRect(rcClient, cx, cy, rcCenter);

		CRect rcIcon(rcCenter.left, rcCenter.top, rcCenter.left+cxIcon, rcCenter.bottom);
		m_pIconImg->DrawImage(pDC, rcIcon);

		UINT nFormat = m_dwTextAlign ? m_dwTextAlign : DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
		rcText = CRect(rcIcon.right+3, rcClient.top, rcIcon.right+3+rcText.Width(), rcClient.bottom);
		pDC->SetTextColor(IsWindowEnabled()?m_colNormalText:m_colDisableText);
		pDC->DrawText(strText, &rcText, nFormat);

		pDC->SetBkMode(nMode);	
	}
	else if (bHasIcon)	// 仅图标
	{
		int cxIcon = m_pIconImg->GetWidth();
		int cyIcon = m_pIconImg->GetHeight();

		CRect rcIcon;
		CalcCenterRect(rcClient, cxIcon, cyIcon, rcIcon);

		m_pIconImg->DrawImage(pDC, rcIcon);
	}
	else if (bHasText)	// 仅文字
	{
		CRect rcText(rcClient);
		rcText.left += 2;
		rcText.right -= 2;

		UINT nFormat = m_dwTextAlign ? m_dwTextAlign : DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;

		int nMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(GetCtrlFont());
		pDC->SetTextColor(IsWindowEnabled()?m_colNormalText:m_colDisableText);
		pDC->DrawText(strText, &rcText, nFormat);
		pDC->SetBkMode(nMode);	
	}
}

void CSkinButton::DrawMenuButton( CDC* pDC,RECT &rcClient )
{
	if (m_bPress)	// 鼠标左键按下状态
	{
		if (m_pBackImgD != NULL && !m_pBackImgD->IsNull())
			m_pBackImgD->DrawImage(pDC, rcClient);
	}
	else if (m_bHover)	// 鼠标悬停状态
	{
		if (m_pBackImgH != NULL && !m_pBackImgH->IsNull())
			m_pBackImgH->DrawImage(pDC, rcClient);
	}

	CRect rcArrow(0, 0, 0, 0);

	if (m_pArrowImg != NULL && !m_pArrowImg->IsNull())
	{
		int cx = m_pArrowImg->GetWidth();
		int cy = m_pArrowImg->GetHeight();
		int x = rcClient.right - 3 - cx;
		int y = (rcClient.bottom-rcClient.top - cy + 1) / 2;
		rcArrow = CRect(x, y, x+cx, y+cy);
		m_pArrowImg->DrawImage(pDC, rcArrow);
	}

	if (m_pIconImg != NULL && !m_pIconImg->IsNull())
	{
		int cx = m_pIconImg->GetWidth();
		int cy = m_pIconImg->GetHeight();

		int nRight;
		if (rcArrow.left > 0)
			nRight = rcArrow.left;
		else
			nRight = rcClient.right;

		CRect rcIcon(rcClient.left, rcClient.top, nRight, rcClient.bottom);
		CalcCenterRect(rcIcon, cx, cy, rcIcon);
		
		if (m_bPress)
			rcIcon.OffsetRect(1, 1);
		m_pIconImg->DrawImage(pDC, rcIcon);
	}
}

void CSkinButton::SetPushed( bool bPush )
{
	m_bPushed = bPush;

	RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
}

BOOL CSkinButton::CreateControl(CWnd* pParentWnd)
{
	if( !Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),pParentWnd,0) )
		return FALSE;

	m_pOwnWnd = this;

	return TRUE;
}

void CSkinButton::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	ISkinControl::SetAttribute(pstrName,pstrValue);

	if( _tcscmp(pstrName, _T("type")) == 0 ) 
	{
		if( _tcscmp(pstrValue, _T("push")) == 0 ) SetButtonType(en_PushButton);
		else if( _tcscmp(pstrValue, _T("check")) == 0 ) SetButtonType(en_CheckButton);
		else if( _tcscmp(pstrValue, _T("radio")) == 0 ) SetButtonType(en_RadioButton);
		else if( _tcscmp(pstrValue, _T("icon")) == 0 ) SetButtonType(en_IconButton);
		else if( _tcscmp(pstrValue, _T("menu")) == 0 ) SetButtonType(en_MenuButton);
	}
	//push
	else if( _tcscmp(pstrName, _T("normalimage")) == 0 ) 
	{
		UIRenderEngine->RemoveImage(m_pBackImgN);
		m_pBackImgN = UIRenderEngine->GetImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("hotimage")) == 0 ) 
	{
		UIRenderEngine->RemoveImage(m_pBackImgH);
		m_pBackImgH = UIRenderEngine->GetImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("pushedimage")) == 0 ) 
	{
		UIRenderEngine->RemoveImage(m_pBackImgD);
		m_pBackImgD = UIRenderEngine->GetImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("focusedimage")) == 0 ) 
	{
		UIRenderEngine->RemoveImage(m_pBackImgF);
		m_pBackImgF = UIRenderEngine->GetImage(pstrValue);
	}
	//check
	else if( _tcscmp(pstrName, _T("checkimgnormal")) == 0 ) 
	{
		UIRenderEngine->RemoveImage(m_pCheckImgN);
		m_pCheckImgN = UIRenderEngine->GetImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("checkimghot")) == 0 ) 
	{
		UIRenderEngine->RemoveImage(m_pCheckImgH);
		m_pCheckImgH = UIRenderEngine->GetImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("checkimgtickn")) == 0 ) 
	{
		UIRenderEngine->RemoveImage(m_pCheckImgTickN);
		m_pCheckImgTickN = UIRenderEngine->GetImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("checkimgtickh")) == 0 ) 
	{
		UIRenderEngine->RemoveImage(m_pCheckImgTickH);
		m_pCheckImgTickH = UIRenderEngine->GetImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("iconimage")) == 0 ) 
	{
		UIRenderEngine->RemoveImage(m_pIconImg);
		m_pIconImg = UIRenderEngine->GetImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("menuimage")) == 0 ) 
	{
		UIRenderEngine->RemoveImage(m_pArrowImg);
		m_pArrowImg = UIRenderEngine->GetImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("size")) == 0 )
	{ 
		LPTSTR pstr = NULL;
		int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   

		SetSize(cx,cy);
	}
	else if( _tcscmp(pstrName, _T("checked")) == 0 )
	{ 
		SetCheck((_tcscmp(pstrValue, _T("true")) == 0) ? BST_CHECKED : BST_UNCHECKED);
	}
	else if( _tcscmp(pstrName, _T("pushed")) == 0 )
	{ 
		SetPushed((_tcscmp(pstrValue, _T("true")) == 0) ? true : false);
	}
	else if( _tcscmp(pstrName, _T("url")) == 0 )
	{ 
		m_lpszUrl = pstrValue;
	}
}

void CSkinButton::SetBtnText(CString strBtnText)
{
// 	//if(!strBtnText.IsEmpty())
// 	{
// 		m_sCaption = strBtnText;
// 		m_bSetBtnText=TRUE;
// 	}

	SetWindowText(strBtnText);
}

CString CSkinButton::GetBtnText()
{
// 	if(m_bSetBtnText)
// 	{
// 		return m_sCaption;	
// 	}
// 	else
	{
		CString strBtnText(_T(""));
		GetWindowText(strBtnText);
		return strBtnText;
	}
}

void CSkinButton::SetPress(BOOL bPress)
{
	m_bPress = bPress;
	m_bCurPushState = bPress;
	if (m_hWnd&&IsWindow(m_hWnd))
	{
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}
}

//获取按钮Press状态
BOOL CSkinButton::GetPress()
{	
	return m_bPress;
}

LRESULT CSkinButton::OnBMGetCheck(WPARAM wparam, LPARAM)
{ 
	return m_bPress; 
}

LRESULT CSkinButton::OnBMSetCheck(WPARAM wparam, LPARAM)
{
	m_bPress=wparam!=0;
// 	switch (m_Style)
// 	{
// 	case BS_RADIOBUTTON:
// 		if (m_bChecked) { //uncheck the other radio buttons (in the same group)
// 			HWND hthis,hwnd2,hpwnd;
// 			hpwnd=GetParent()->GetSafeHwnd();	//get button parent handle
// 			hwnd2=hthis=GetSafeHwnd();			//get this button handle
// 			if (hthis && hpwnd){				//consistency check
// 				for( ; ; ){	//scan the buttons within the group
// 					hwnd2=::GetNextDlgGroupItem(hpwnd,hwnd2,0);
// 					//until we reach again this button
// 					if ((hwnd2==hthis)||(hwnd2==NULL)) break;
// 				}
// 			}
// 		}
// 		break;
// 	case BS_PUSHBUTTON:
// 		//m_Checked=false;
// 		//ASSERT(false); // Must be a Check or Radio button to use this function
// 		break;
// 	}

	Invalidate();
	return 0;
}
