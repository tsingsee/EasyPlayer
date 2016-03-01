// SkinTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinTabCtrl.h"


// CSkinTabCtrl

IMPLEMENT_DYNAMIC(CSkinTabCtrl, CTabCtrl)

CSkinTabCtrlItem::CSkinTabCtrlItem(void)
{
	m_nID = 0;
	m_dwStyle = STCI_STYLE_BUTTON;
	m_nWidth = m_nHeight = 0;
	m_nLeftWidth = m_nRightWidth = 0;
	m_nPadding = 0;
	m_strText = _T("");
	m_strToolTipText = _T("");
	m_lpBgImgN = m_lpBgImgH = m_lpBgImgD = NULL;
	m_lpArrowImgH = m_lpArrowImgD = NULL;
	m_lpIconImg = m_lpSelIconImg = NULL;
}

CSkinTabCtrlItem::~CSkinTabCtrlItem(void)
{
	UIRenderEngine->RemoveImage(m_lpBgImgN);
	UIRenderEngine->RemoveImage(m_lpBgImgH);
	UIRenderEngine->RemoveImage(m_lpBgImgD);
	UIRenderEngine->RemoveImage(m_lpArrowImgH);
	UIRenderEngine->RemoveImage(m_lpArrowImgD);
	UIRenderEngine->RemoveImage(m_lpIconImg);
	UIRenderEngine->RemoveImage(m_lpSelIconImg);
}

CSkinTabCtrl::CSkinTabCtrl()
{
	m_lpBack = NULL;
	m_lpItemBgImgN = m_lpItemBgImgH = m_lpItemBgImgD = NULL;
	m_lpArrowImgH = m_lpArrowImgD = NULL;
	m_nSelIndex = m_nHoverIndex = -1;
	m_bPressArrow = FALSE;
	m_bMouseTracking = FALSE;
	m_nLeft = m_nTop = 0;
	m_bTransparent = FALSE;
	m_ptPaddingText.SetPoint(0,0);
}

CSkinTabCtrl::~CSkinTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkinTabCtrl, CTabCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CSkinTabCtrl 消息处理程序

void CSkinTabCtrl::SetLeftTop(int nLeft, int nTop)
{
	m_nLeft = nLeft;
	m_nTop = nTop;
}

BOOL CSkinTabCtrl::SetBackImage(LPCTSTR lpszFileName, CONST LPRECT lprcNinePart)
{
	UIRenderEngine->RemoveImage(m_lpBack);

	m_lpBack = UIRenderEngine->GetImage(lpszFileName);

	if (m_lpBack != NULL)
		m_lpBack->SetNinePart(lprcNinePart);

	if (NULL == m_lpBack)
		return FALSE;
	else
		return TRUE;
}

BOOL CSkinTabCtrl::SetItemsImage(LPCTSTR lpNormal, LPCTSTR lpHighlight,LPCTSTR lpDown, CONST LPRECT lprcNinePart)
{
	UIRenderEngine->RemoveImage(m_lpItemBgImgN);
	UIRenderEngine->RemoveImage(m_lpItemBgImgH);
	UIRenderEngine->RemoveImage(m_lpItemBgImgD);

	m_lpItemBgImgN = UIRenderEngine->GetImage(lpNormal);
	m_lpItemBgImgH = UIRenderEngine->GetImage(lpHighlight);
	m_lpItemBgImgD = UIRenderEngine->GetImage(lpDown);

	if (m_lpItemBgImgN != NULL)
		m_lpItemBgImgN->SetNinePart(lprcNinePart);

	if (m_lpItemBgImgH != NULL)
		m_lpItemBgImgH->SetNinePart(lprcNinePart);

	if (m_lpItemBgImgD != NULL)
		m_lpItemBgImgD->SetNinePart(lprcNinePart);

	if ((lpNormal != NULL && NULL == m_lpItemBgImgN) || 
		(lpHighlight != NULL && NULL == m_lpItemBgImgH) ||
		(lpDown != NULL && NULL == m_lpItemBgImgD))
		return FALSE;
	else
		return TRUE;
}

BOOL CSkinTabCtrl::SetItemsArrowImage(LPCTSTR lpHighlight, LPCTSTR lpDown)
{
	UIRenderEngine->RemoveImage(m_lpArrowImgH);
	UIRenderEngine->RemoveImage(m_lpArrowImgD);

	m_lpArrowImgH = UIRenderEngine->GetImage(lpHighlight);
	m_lpArrowImgD = UIRenderEngine->GetImage(lpDown);

	if ((lpHighlight != NULL && NULL == m_lpArrowImgH) ||
		(lpDown != NULL && NULL == m_lpArrowImgD))
		return FALSE;
	else
		return TRUE;
}

int CSkinTabCtrl::GetItemCount()
{
	return m_ItemArray.size();
}

int CSkinTabCtrl::GetCurSel()
{
	return m_nSelIndex;
}

void CSkinTabCtrl::SetCurSel(int nCurSel)
{
	m_nSelIndex = nCurSel;

	Invalidate(FALSE);
}

int CSkinTabCtrl::AddItem(int nID, DWORD dwStyle)
{
	CSkinTabCtrlItem * lpItem = new CSkinTabCtrlItem;
	if (NULL == lpItem)
		return -1;
	lpItem->m_nID = nID;
	lpItem->m_dwStyle = dwStyle;
	m_ItemArray.push_back(lpItem);

	__super::InsertItem(nID,TEXT(""));
	
	SetItemPadding(nID,1);

	return m_ItemArray.size() - 1;
}

void CSkinTabCtrl::SetItemID(int nIndex, int nID)
{
	CSkinTabCtrlItem * lpItem = GetItemByIndex(nIndex);
	if (lpItem != NULL)
		lpItem->m_nID = nID;
}

void CSkinTabCtrl::SetItemStyle(int nIndex, int dwStyle)
{
	CSkinTabCtrlItem * lpItem = GetItemByIndex(nIndex);
	if (lpItem != NULL)
		lpItem->m_dwStyle = dwStyle;
}

CSize CSkinTabCtrl::SetItemSize(CSize size,CONST LPSIZE lpszArrow/*=NULL*/)
{
	for (int i=0;i<(int)m_ItemArray.size();i++)
	{
		CSkinTabCtrlItem * lpItem = m_ItemArray.at(i);
		lpItem->m_nWidth = size.cx;
		lpItem->m_nHeight = size.cy;
		lpItem->m_nLeftWidth = lpszArrow ? lpszArrow->cx : 0;
		lpItem->m_nRightWidth = lpszArrow ? lpszArrow->cy : 0;
	}

	return __super::SetItemSize(size);
}

void CSkinTabCtrl::SetItemPadding(int nIndex, int nPadding)
{
	CSkinTabCtrlItem * lpItem = GetItemByIndex(nIndex);
	if (lpItem != NULL)
		lpItem->m_nPadding = nPadding;
}

void CSkinTabCtrl::SetItemText(int nIndex, LPCTSTR lpszText)
{
	CSkinTabCtrlItem * lpItem = GetItemByIndex(nIndex);
	if (lpItem != NULL)
		lpItem->m_strText = lpszText;
}

void CSkinTabCtrl::SetItemToolTipText(int nIndex, LPCTSTR lpszText)
{
	CSkinTabCtrlItem * lpItem = GetItemByIndex(nIndex);
	if (lpItem != NULL)
		lpItem->m_strToolTipText = lpszText;
}

BOOL CSkinTabCtrl::SetItemBack(int nIndex, LPCTSTR lpNormal, 
								LPCTSTR lpHighlight, LPCTSTR lpDown, CONST LPRECT lprcNinePart)
{
	CSkinTabCtrlItem * lpItem = GetItemByIndex(nIndex);
	if (NULL == lpItem)
		return FALSE;

	UIRenderEngine->RemoveImage(lpItem->m_lpBgImgN);
	UIRenderEngine->RemoveImage(lpItem->m_lpBgImgH);
	UIRenderEngine->RemoveImage(lpItem->m_lpBgImgD);

	lpItem->m_lpBgImgN = UIRenderEngine->GetImage(lpNormal);
	lpItem->m_lpBgImgH = UIRenderEngine->GetImage(lpHighlight);
	lpItem->m_lpBgImgD = UIRenderEngine->GetImage(lpDown);

	if (lpItem->m_lpBgImgN != NULL)
		lpItem->m_lpBgImgN->SetNinePart(lprcNinePart);

	if (lpItem->m_lpBgImgH != NULL)
		lpItem->m_lpBgImgH->SetNinePart(lprcNinePart);

	if (lpItem->m_lpBgImgD != NULL)
		lpItem->m_lpBgImgD->SetNinePart(lprcNinePart);

	if ((lpNormal != NULL && NULL == lpItem->m_lpBgImgN) || 
		(lpHighlight != NULL && NULL == lpItem->m_lpBgImgH) ||
		(lpDown != NULL && NULL == lpItem->m_lpBgImgD))
		return FALSE;
	else
		return TRUE;
}

BOOL CSkinTabCtrl::SetItemArrowImage(int nIndex, LPCTSTR lpHighlight, LPCTSTR lpDown)
{
	CSkinTabCtrlItem * lpItem = GetItemByIndex(nIndex);
	if (NULL == lpItem)
		return FALSE;

	UIRenderEngine->RemoveImage(lpItem->m_lpArrowImgH);
	UIRenderEngine->RemoveImage(lpItem->m_lpArrowImgD);

	lpItem->m_lpArrowImgH = UIRenderEngine->GetImage(lpHighlight);
	lpItem->m_lpArrowImgD = UIRenderEngine->GetImage(lpDown);

	if ((lpHighlight != NULL && NULL == lpItem->m_lpArrowImgH) ||
		(lpDown != NULL && NULL == lpItem->m_lpArrowImgD))
		return FALSE;
	else
		return TRUE;
}

BOOL CSkinTabCtrl::SetItemIconImage(int nIndex, LPCTSTR lpIcon, LPCTSTR lpSelIcon)
{
	CSkinTabCtrlItem * lpItem = GetItemByIndex(nIndex);
	if (NULL == lpItem)
		return FALSE;

	UIRenderEngine->RemoveImage(lpItem->m_lpIconImg);
	UIRenderEngine->RemoveImage(lpItem->m_lpSelIconImg);

	lpItem->m_lpIconImg = UIRenderEngine->GetImage(lpIcon);
	lpItem->m_lpSelIconImg = UIRenderEngine->GetImage(lpSelIcon);

	if ((lpIcon != NULL && NULL == lpItem->m_lpIconImg) ||
		(lpSelIcon != NULL && NULL == lpItem->m_lpSelIconImg))
		return FALSE;
	else
		return TRUE;
}

BOOL CSkinTabCtrl::GetItemRectByIndex(int nIndex, CRect& rect)
{
	CSkinTabCtrlItem * lpItem;
	int nLeft = m_nLeft, nTop = m_nTop;

	for (int i = 0; i < (int)m_ItemArray.size(); i++)
	{
		lpItem = m_ItemArray[i];
		if (lpItem != NULL)
		{
			if (i == nIndex)
			{
				rect = CRect(nLeft, nTop, nLeft+lpItem->m_nWidth, nTop+lpItem->m_nHeight);
				return TRUE;
			}
			nLeft += lpItem->m_nWidth;
			nLeft += lpItem->m_nPadding;
		}
	}

	return FALSE;
}

BOOL CSkinTabCtrl::GetItemRectByID(int nID, CRect& rect)
{
	CSkinTabCtrlItem * lpItem;
	int nLeft = m_nLeft, nTop = m_nTop;

	for (int i = 0; i < (int)m_ItemArray.size(); i++)
	{
		lpItem = m_ItemArray[i];
		if (lpItem != NULL)
		{
			if (lpItem->m_nID == nID)
			{
				rect = CRect(nLeft, nTop, nLeft+lpItem->m_nWidth, nTop+lpItem->m_nHeight);
				return TRUE;
			}
			nLeft += lpItem->m_nWidth;
			nLeft += lpItem->m_nPadding;
		}
	}

	return FALSE;
}


void CSkinTabCtrl::DrawItem(CDC*pDC, int nIndex)
{
	CSkinTabCtrlItem * lpItem = GetItemByIndex(nIndex);
	if (NULL == lpItem) return;

	CRect rcItem;
	GetItemRectByIndex(nIndex, rcItem);

	if (m_nSelIndex == nIndex)
	{
		if (lpItem->m_lpBgImgD != NULL && !lpItem->m_lpBgImgD->IsNull())
			lpItem->m_lpBgImgD->Draw(pDC, rcItem);
		else if (m_lpItemBgImgD != NULL && !m_lpItemBgImgD->IsNull())
			m_lpItemBgImgD->Draw(pDC, rcItem);
	}
	else if (m_nHoverIndex == nIndex)
	{
		if (lpItem->m_lpBgImgH != NULL && !lpItem->m_lpBgImgH->IsNull())
			lpItem->m_lpBgImgH->Draw(pDC, rcItem);
		else if (m_lpItemBgImgH != NULL && !m_lpItemBgImgH->IsNull())
			m_lpItemBgImgH->Draw(pDC, rcItem);
	}
	else
	{
		if (lpItem->m_lpBgImgN != NULL && !lpItem->m_lpBgImgN->IsNull())
			lpItem->m_lpBgImgN->Draw(pDC, rcItem);
		else if (m_lpItemBgImgN != NULL && !m_lpItemBgImgN->IsNull())
			m_lpItemBgImgN->Draw(pDC, rcItem);
	}

	CImageEx * lpIconImg = NULL;

	if (m_nSelIndex == nIndex)
		lpIconImg = lpItem->m_lpSelIconImg;
	else
		lpIconImg = lpItem->m_lpIconImg;

	BOOL bHasText = FALSE;
	if (lpItem->m_strText.GetLength() > 0)
		bHasText = TRUE;

	BOOL bHasIcon = FALSE;
	if (lpIconImg != NULL && !lpIconImg->IsNull())
		bHasIcon = TRUE;

	if (bHasIcon && bHasText)	// 带图标和文字
	{
		int cxIcon = lpIconImg->GetWidth();
		int cyIcon = lpIconImg->GetHeight();

		int nMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(GetCtrlFont());
		pDC->SetTextColor(m_colNormalText);

		CRect rcText(0,0,0,0);	// 计算文字宽高
		CRect rcIcon(0,0,0,0);

		pDC->DrawText(lpItem->m_strText, lpItem->m_strText.GetLength(), &rcText, DT_SINGLELINE | DT_CALCRECT);

		if ( (m_ptPaddingText.x == 0) && (m_ptPaddingText.y == 0) )
		{
			int cx = cxIcon+1+rcText.Width();
			int cy = cyIcon;

			CRect rcCenter;
			CalcCenterRect(rcItem, cx, cy, rcCenter);

			rcIcon.SetRect(rcCenter.left, rcCenter.top, rcCenter.left+cxIcon, rcCenter.bottom);

			rcText = CRect(rcIcon.right+1, rcItem.top, rcIcon.right+1+rcText.Width(), rcItem.bottom);
		}
		else
		{
			CRect rcCenter;
			CalcCenterRect(rcItem, cxIcon, cyIcon, rcIcon);
			rcText = CRect(rcItem.left+m_ptPaddingText.x,rcItem.top+m_ptPaddingText.y,rcItem.left+m_ptPaddingText.x+rcText.Width(),rcItem.top+m_ptPaddingText.y+rcText.Height());
		}
		
		lpIconImg->Draw(pDC, rcIcon);

		UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
		
		pDC->DrawText(lpItem->m_strText, lpItem->m_strText.GetLength(), &rcText, nFormat);

		pDC->SetBkMode(nMode);
	}
	else if (bHasIcon)	// 仅图标
	{
		int cxIcon = lpIconImg->GetWidth();
		int cyIcon = lpIconImg->GetHeight();

		CRect rcIcon;
		CalcCenterRect(rcItem, cxIcon, cyIcon, rcIcon);

		lpIconImg->Draw(pDC, rcIcon);
	}
	else if (bHasText)	// 仅文字
	{
		UINT nFormat = DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;

		int nMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_colNormalText);
		pDC->SelectObject(GetCtrlFont());

		pDC->DrawText(lpItem->m_strText, lpItem->m_strText.GetLength(), &rcItem, nFormat);
		pDC->SetBkMode(nMode);
	}

	if (m_nSelIndex == nIndex)
	{
		if (lpItem->m_dwStyle & STCI_STYLE_DROPDOWN)
		{
			CRect rcArrow;
			rcArrow.left = rcItem.left+lpItem->m_nLeftWidth;
			rcArrow.top = rcItem.top;
			rcArrow.bottom = rcItem.bottom;
			rcArrow.right = rcArrow.left + lpItem->m_nRightWidth;

			if (m_bPressArrow)
			{
				if (lpItem->m_lpArrowImgD != NULL && !lpItem->m_lpArrowImgD->IsNull())
					lpItem->m_lpArrowImgD->Draw(pDC, rcArrow);
				else if (m_lpArrowImgD != NULL && !m_lpArrowImgD->IsNull())
					m_lpArrowImgD->Draw(pDC, rcArrow);
			}
			else if (m_nHoverIndex == nIndex)
			{
				if (lpItem->m_lpArrowImgH != NULL && !lpItem->m_lpArrowImgH->IsNull())
					lpItem->m_lpArrowImgH->Draw(pDC, rcArrow);
				else if (m_lpArrowImgH != NULL && !m_lpArrowImgH->IsNull())
					m_lpArrowImgH->Draw(pDC, rcArrow);
			}
		}
	}
}

BOOL CSkinTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CSkinTabCtrl::OnPaint()
{
	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(&rcClient);

	CMemoryDC BufferDC(&dc, rcClient);

	DrawParentWndBg(GetSafeHwnd(),BufferDC.m_hDC);

	if (m_lpBack != NULL && !m_lpBack->IsNull())
		m_lpBack->Draw(&BufferDC, rcClient);

	for (int i = 0; i < (int)m_ItemArray.size(); i++)
	{
		DrawItem(&BufferDC, i);
	}
}

void CSkinTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();

	int nIndex = HitTest(point);
	if (nIndex != -1)
	{
		if (nIndex != m_nSelIndex)
		{
			m_nSelIndex = nIndex;
			Invalidate(FALSE);

			NMHDR nmhdr = {m_hWnd, GetDlgCtrlID(), TCN_SELCHANGE};
			::SendMessage(::GetParent(m_hWnd), WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmhdr);
		}
		else
		{
			CSkinTabCtrlItem * lpItem = GetItemByIndex(nIndex);
			if (lpItem != NULL && lpItem->m_dwStyle & STCI_STYLE_DROPDOWN)
			{
				CRect rcItem;
				GetItemRectByIndex(nIndex, rcItem);

				CRect rcArrow;
				rcArrow.left = rcItem.left+lpItem->m_nLeftWidth;
				rcArrow.right = rcArrow.left + lpItem->m_nRightWidth;
				rcArrow.top = rcItem.top;
				rcArrow.bottom = rcItem.bottom;

				if (rcArrow.PtInRect(point))
					m_bPressArrow = TRUE;
				else
					m_bPressArrow = FALSE;

				if (m_bPressArrow)
				{
					Invalidate(FALSE);

					NMHDR nmhdr = {0};
					nmhdr.hwndFrom = m_hWnd;
					nmhdr.idFrom = GetDlgCtrlID();
					nmhdr.code = TCN_DROPDOWN;
					::SendMessage(::GetParent(m_hWnd), WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmhdr);

					m_bPressArrow = FALSE;
					Invalidate(FALSE);
				}
			}
		}
	}

	__super::OnLButtonDown(nFlags, point);
}

void CSkinTabCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bMouseTracking)
	{
		TrackMouseLeave(GetSafeHwnd());
		m_bMouseTracking = TRUE;
	}

	int nIndex;

	nIndex = HitTest(point);
	if (nIndex != -1)
	{
		CSkinTabCtrlItem * lpItem = GetItemByIndex(nIndex);
		if (lpItem != NULL)
		{
			CRect rcItem;
			GetItemRectByIndex(nIndex, rcItem);

			if (nIndex != m_nHoverIndex && lpItem->m_strToolTipText.GetLength() > 0)
			{
				if (!m_ToolTipCtrl.GetSafeHwnd())
				{
					m_ToolTipCtrl.Create(this);
					m_ToolTipCtrl.SetMaxTipWidth(200);
				}

				if (m_ToolTipCtrl.GetSafeHwnd())
				{
					if (m_ToolTipCtrl.GetToolCount() <= 0)
					{
						m_ToolTipCtrl.Activate(TRUE);
						m_ToolTipCtrl.AddTool(this, lpItem->m_strToolTipText.GetBuffer(), &rcItem, 1);
					}
					else
					{
						m_ToolTipCtrl.Activate(TRUE);
						m_ToolTipCtrl.UpdateTipText(lpItem->m_strToolTipText.GetBuffer(), this, 1);
						m_ToolTipCtrl.SetToolRect(this, 1, &rcItem);
					}
				}
			}
		}
	}

	if (nIndex != m_nHoverIndex)
	{
		m_nHoverIndex = nIndex;
		Invalidate();
	}

	__super::OnMouseMove(nFlags, point);
}

LRESULT CSkinTabCtrl::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bMouseTracking = FALSE;
	int nIndex = -1;
	if (nIndex != m_nHoverIndex)
	{
		m_nHoverIndex = nIndex;
		Invalidate();
	}

	return 0;
}

void CSkinTabCtrl::OnDestroy()
{
	__super::OnDestroy();

	if (m_ToolTipCtrl.GetSafeHwnd())
		m_ToolTipCtrl.DestroyWindow();

	m_ToolTipCtrl.m_hWnd = NULL;

	CSkinTabCtrlItem * lpItem;
	for (int i = 0; i < (int)m_ItemArray.size(); i++)
	{
		lpItem = m_ItemArray[i];
		if (lpItem != NULL)
			delete lpItem;
	}
	m_ItemArray.clear();

	UIRenderEngine->RemoveImage(m_lpBack);
	UIRenderEngine->RemoveImage(m_lpItemBgImgN);
	UIRenderEngine->RemoveImage(m_lpItemBgImgH);
	UIRenderEngine->RemoveImage(m_lpItemBgImgD);
	UIRenderEngine->RemoveImage(m_lpArrowImgH);
	UIRenderEngine->RemoveImage(m_lpArrowImgD);

	m_nSelIndex = m_nHoverIndex = -1;
	m_bPressArrow = FALSE;
	m_bMouseTracking = FALSE;
	m_nLeft = m_nTop = 0;
	m_bTransparent = FALSE;
}

BOOL CSkinTabCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (m_ToolTipCtrl.GetSafeHwnd())
		m_ToolTipCtrl.RelayEvent(pMsg);

	return __super::PreTranslateMessage(pMsg);
}

CSkinTabCtrlItem * CSkinTabCtrl::GetItemByIndex(int nIndex)
{
	if (nIndex >= 0 && nIndex < (int)m_ItemArray.size())
		return m_ItemArray[nIndex];
	else
		return NULL;
}

CSkinTabCtrlItem * CSkinTabCtrl::GetItemByID(int nID)
{
	CSkinTabCtrlItem * lpItem;
	for (int i = 0; i < (int)m_ItemArray.size(); i++)
	{
		lpItem = m_ItemArray[i];
		if (lpItem != NULL && lpItem->m_nID == nID)
			return lpItem;
	}

	return NULL;
}

int CSkinTabCtrl::HitTest(POINT pt)
{
	CSkinTabCtrlItem * lpItem;
	int nLeft = m_nLeft, nTop = m_nTop;
	CRect rcItem;

	for (int i = 0; i < (int)m_ItemArray.size(); i++)
	{
		lpItem = m_ItemArray[i];
		if (lpItem != NULL)
		{
			rcItem = CRect(nLeft, nTop, nLeft+lpItem->m_nWidth, nTop+lpItem->m_nHeight);
			if (rcItem.PtInRect(pt))
				return i;
			nLeft += lpItem->m_nWidth;
			nLeft += lpItem->m_nPadding;
		}
	}

	return -1;
}

BOOL CSkinTabCtrl::CreateControl( CWnd* pParentWnd )
{
	if( !Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|TCS_FIXEDWIDTH,CRect(0,0,0,0),pParentWnd,0) )
		return FALSE;

	m_pOwnWnd = this;

	return TRUE;
}

void CSkinTabCtrl::ParseItem( CXmlNode *root )
{
	if( root == NULL ) return;

	int nAttributes = root->GetAttributeCount();

	LPCTSTR pstrClass = NULL;
	LPCTSTR pstrName = NULL;
	LPCTSTR pstrValue = NULL;
	LPTSTR pstr = NULL;

	pstrClass = root->GetName();

	if( _tcscmp(pstrClass, _T("item")) == 0 ) 	
	{
		DWORD dwStyle = STCI_STYLE_BUTTON;
		LPCTSTR lpszTipText = NULL;
		LPCTSTR lpszText = NULL;
		LPCTSTR lpIcon = NULL,lpSelIcon = NULL;

		for( int i = 0; i < nAttributes; i++ ) 
		{
			pstrName = root->GetAttributeName(i);
			pstrValue = root->GetAttributeValue(i);

			if( _tcscmp(pstrName, _T("style")) == 0 )  dwStyle = _tcstol(pstrValue, &pstr, 10);
			else if( _tcscmp(pstrName, _T("tiptext")) == 0 ) lpszTipText = pstrValue;
			else if( _tcscmp(pstrName, _T("text")) == 0 ) lpszText = pstrValue;
			else if( _tcscmp(pstrName, _T("iconimage")) == 0 ) lpIcon = pstrValue;
			else if( _tcscmp(pstrName, _T("selimage")) == 0 ) lpSelIcon = pstrValue;
		}

		int nIndex = AddItem(GetItemCount(), dwStyle);
		
		if(lpszTipText != NULL)
			SetItemToolTipText(nIndex, lpszTipText);
	
		if(lpszText != NULL)
			SetItemText(nIndex, lpszText);

		SetItemIconImage(nIndex, lpIcon,lpSelIcon);
	}
	else if( _tcscmp(pstrClass, _T("backimage")) == 0 ) 	
	{
		LPCTSTR pszPath = NULL;
		CRect rcNinepart;

		for( int i = 0; i < nAttributes; i++ ) 
		{
			pstrName = root->GetAttributeName(i);
			pstrValue = root->GetAttributeValue(i);

			if( _tcscmp(pstrName, _T("path")) == 0 )  pszPath = pstrValue;
			else if( _tcscmp(pstrName, _T("ninepart")) == 0 ) 
			{
				rcNinepart.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				rcNinepart.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rcNinepart.right = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rcNinepart.bottom = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
			}
		}

		SetBackImage(pszPath, rcNinepart);
	}
	else if( _tcscmp(pstrClass, _T("itemimage")) == 0 ) 	
	{
		LPCTSTR lpNormal = NULL;
		LPCTSTR lpHighlight = NULL;
		LPCTSTR lpDown = NULL;
		CRect rcNinepart;

		for( int i = 0; i < nAttributes; i++ ) 
		{
			pstrName = root->GetAttributeName(i);
			pstrValue = root->GetAttributeValue(i);

			if( _tcscmp(pstrName, _T("normal")) == 0 )  lpNormal = pstrValue;
			else if( _tcscmp(pstrName, _T("highlight")) == 0 )  lpHighlight = pstrValue;
			else if( _tcscmp(pstrName, _T("down")) == 0 )  lpDown = pstrValue;
			else if( _tcscmp(pstrName, _T("ninepart")) == 0 ) 
			{
				rcNinepart.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				rcNinepart.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rcNinepart.right = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rcNinepart.bottom = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
			}
		}

		SetItemsImage(lpNormal, lpHighlight,lpDown, rcNinepart);
	}
	else if( _tcscmp(pstrClass, _T("itemarrowimage")) == 0 ) 	
	{
		LPCTSTR lpHighlight = NULL;
		LPCTSTR lpDown = NULL;

		for( int i = 0; i < nAttributes; i++ ) 
		{
			pstrName = root->GetAttributeName(i);
			pstrValue = root->GetAttributeValue(i);

			if( _tcscmp(pstrName, _T("highlight")) == 0 )  lpHighlight = pstrValue;
			else if( _tcscmp(pstrName, _T("down")) == 0 )  lpDown = pstrValue;
		}

		SetItemsArrowImage(lpHighlight,lpDown);
	}
	else if( _tcscmp(pstrClass, _T("itemsize")) == 0 ) 	
	{
		CSize szSzie(0,0),szArrow(0,0);

		for( int i = 0; i < nAttributes; i++ ) 
		{
			pstrName = root->GetAttributeName(i);
			pstrValue = root->GetAttributeValue(i);

			if( _tcscmp(pstrName, _T("size")) == 0 )  
			{
				szSzie.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);  
				szSzie.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
			}
			else if( _tcscmp(pstrName, _T("arrowsize")) == 0 )  
			{
				szArrow.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);  
				szArrow.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
			}
		}

		SetItemSize(szSzie,&szArrow);
	}
}

void CSkinTabCtrl::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	ISkinControl::SetAttribute(pstrName,pstrValue);

	if( _tcscmp(pstrName, _T("paddingtext")) == 0 )
	{
		LPTSTR pstr = NULL;
		m_ptPaddingText.x = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		m_ptPaddingText.y = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
	}
}
