// SkinToolBar.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinUI.h"
#include "SkinToolBar.h"


CSkinToolBarItem::CSkinToolBarItem( void )
{
	m_nID = 0;
	m_dwStyle = STBI_STYLE_BUTTON;
	m_nWidth = m_nHeight = 0;
	m_nLeftWidth = m_nRightWidth = 0;
	m_nPadding = 0;
	m_strText = _T("");
	m_strToolTipText = _T("");
	m_lpBgImgN = m_lpBgImgH = m_lpBgImgD = NULL;
	m_lpLeftH = m_lpLeftD = NULL;
	m_lpRightH = m_lpRightD = NULL;
	m_lpSepartorImg = NULL;
	m_lpArrowImg = NULL;
	m_lpIconImg = NULL;
	m_bChecked = FALSE;
}

CSkinToolBarItem::~CSkinToolBarItem( void )
{
	UIRenderEngine->RemoveImage(m_lpBgImgN);
	UIRenderEngine->RemoveImage(m_lpBgImgH);
	UIRenderEngine->RemoveImage(m_lpBgImgD);
	UIRenderEngine->RemoveImage(m_lpLeftH);
	UIRenderEngine->RemoveImage(m_lpLeftD);
	UIRenderEngine->RemoveImage(m_lpRightH);
	UIRenderEngine->RemoveImage(m_lpRightD);
	UIRenderEngine->RemoveImage(m_lpSepartorImg);
	UIRenderEngine->RemoveImage(m_lpArrowImg);
	UIRenderEngine->RemoveImage(m_lpIconImg);
}

//////////////////////////////////////////////////////////////////////////

// CSkinToolBar

IMPLEMENT_DYNAMIC(CSkinToolBar, CWnd)

CSkinToolBar::CSkinToolBar()
{
	m_lpBgImg = NULL;
	m_nPressIndex = m_nHoverIndex = -1;
	m_bPressLorR = m_bHoverLorR = FALSE;
	m_bMouseTracking = FALSE;
	m_nLeft = m_nTop = 0;
	m_bAutoSize = FALSE;
}

CSkinToolBar::~CSkinToolBar()
{
}


BEGIN_MESSAGE_MAP(CSkinToolBar, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
END_MESSAGE_MAP()



// CSkinToolBar 消息处理程序

void CSkinToolBar::SetLeftTop(int nLeft, int nTop)
{
	m_nLeft = nLeft;
	m_nTop = nTop;
}

void CSkinToolBar::SetAutoSize(BOOL bAutoSize)
{
	m_bAutoSize = bAutoSize;
}

BOOL CSkinToolBar::SetBgPic(LPCTSTR lpszFileName, CONST LPRECT lprcNinePart)
{
	UIRenderEngine->RemoveImage(m_lpBgImg);

	m_lpBgImg = UIRenderEngine->GetImage(lpszFileName);
	if (NULL == m_lpBgImg)
		return FALSE;

	m_lpBgImg->SetNinePart(lprcNinePart);
	return TRUE;
}

int CSkinToolBar::AddItem(int nID, DWORD dwStyle)
{
	CSkinToolBarItem * lpItem = new CSkinToolBarItem;
	if (NULL == lpItem)
		return -1;
	lpItem->m_nID = nID;
	lpItem->m_dwStyle = dwStyle;
	m_arrItems.push_back(lpItem);
	return m_arrItems.size() - 1;
}

void CSkinToolBar::SetItemStyle(int nIndex, int dwStyle)
{
	CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
	if (lpItem != NULL)
		lpItem->m_dwStyle = dwStyle;
}

void CSkinToolBar::SetItemID(int nIndex, int nID)
{
	CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
	if (lpItem != NULL)
		lpItem->m_nID = nID;
}

void CSkinToolBar::SetItemSize(int nIndex, int nWidth, int nHeight, 
							   int nLeftWidth/* = 0*/, int nRightWidth/* = 0*/)
{
	CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
	if (lpItem != NULL)
	{
		lpItem->m_nWidth = nWidth;
		lpItem->m_nHeight = nHeight;
		lpItem->m_nLeftWidth = nLeftWidth;
		lpItem->m_nRightWidth = nRightWidth;
	}
}

void CSkinToolBar::SetItemPadding(int nIndex, int nPadding)
{
	CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
	if (lpItem != NULL)
		lpItem->m_nPadding = nPadding;
}

void CSkinToolBar::SetItemText(int nIndex, LPCTSTR lpszText)
{
	CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
	if (lpItem != NULL)
		lpItem->m_strText = lpszText;
}

void CSkinToolBar::SetItemToolTipText(int nIndex, LPCTSTR lpszText)
{
	CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
	if (lpItem != NULL)
		lpItem->m_strToolTipText = lpszText;
}

BOOL CSkinToolBar::SetItemBgPic(int nIndex, LPCTSTR lpNormal, 
								LPCTSTR lpHighlight, LPCTSTR lpDown, CONST LPRECT lprcNinePart)
{
	CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
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

BOOL CSkinToolBar::SetItemLeftBgPic(int nIndex, LPCTSTR lpHighlight, 
									LPCTSTR lpDown, CONST LPRECT lprcNinePart)
{
	CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
	if (NULL == lpItem)
		return FALSE;

	UIRenderEngine->RemoveImage(lpItem->m_lpLeftH);
	UIRenderEngine->RemoveImage(lpItem->m_lpLeftD);

	lpItem->m_lpLeftH = UIRenderEngine->GetImage(lpHighlight);
	lpItem->m_lpLeftD = UIRenderEngine->GetImage(lpDown);

	if (lpItem->m_lpLeftH != NULL)
		lpItem->m_lpLeftH->SetNinePart(lprcNinePart);

	if (lpItem->m_lpLeftD != NULL)
		lpItem->m_lpLeftD->SetNinePart(lprcNinePart);

	if ((lpHighlight != NULL && NULL == lpItem->m_lpLeftH) ||
		(lpDown != NULL && NULL == lpItem->m_lpLeftD))
		return FALSE;
	else
		return TRUE;
}

BOOL CSkinToolBar::SetItemRightBgPic(int nIndex, LPCTSTR lpHighlight, 
									 LPCTSTR lpDown, CONST LPRECT lprcNinePart)
{
	CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
	if (NULL == lpItem)
		return FALSE;

	UIRenderEngine->RemoveImage(lpItem->m_lpRightH);
	UIRenderEngine->RemoveImage(lpItem->m_lpRightD);

	lpItem->m_lpRightH = UIRenderEngine->GetImage(lpHighlight);
	lpItem->m_lpRightD = UIRenderEngine->GetImage(lpDown);

	if (lpItem->m_lpRightH != NULL)
		lpItem->m_lpRightH->SetNinePart(lprcNinePart);

	if (lpItem->m_lpRightD != NULL)
		lpItem->m_lpRightD->SetNinePart(lprcNinePart);

	if ((lpHighlight != NULL && NULL == lpItem->m_lpRightH) ||
		(lpDown != NULL && NULL == lpItem->m_lpRightD))
		return FALSE;
	else
		return TRUE;
}

BOOL CSkinToolBar::SetItemSepartorPic(int nIndex, LPCTSTR lpszFileName)
{
	CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
	if (NULL == lpItem)
		return FALSE;

	UIRenderEngine->RemoveImage(lpItem->m_lpSepartorImg);

	lpItem->m_lpSepartorImg = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == lpItem->m_lpSepartorImg)
		return FALSE;
	else
		return TRUE;
}

BOOL CSkinToolBar::SetItemArrowPic(int nIndex, LPCTSTR lpszFileName)
{
	CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
	if (NULL == lpItem)
		return FALSE;

	UIRenderEngine->RemoveImage(lpItem->m_lpArrowImg);

	lpItem->m_lpArrowImg = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == lpItem->m_lpArrowImg)
		return FALSE;
	else
		return TRUE;
}

BOOL CSkinToolBar::SetItemIconPic(int nIndex, LPCTSTR lpszFileName)
{
	CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
	if (NULL == lpItem)
		return FALSE;

	UIRenderEngine->RemoveImage(lpItem->m_lpIconImg);

	lpItem->m_lpIconImg = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == lpItem->m_lpIconImg)
		return FALSE;
	else
		return TRUE;
}

BOOL CSkinToolBar::GetItemRectByIndex(int nIndex, CRect& rect)
{
	CSkinToolBarItem * lpItem;
	int nLeft = m_nLeft, nTop = m_nTop;

	for (int i = 0; i < (int)m_arrItems.size(); i++)
	{
		lpItem = m_arrItems[i];
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

BOOL CSkinToolBar::GetItemRectByID(int nID, CRect& rect)
{
	CSkinToolBarItem * lpItem;
	int nLeft = m_nLeft, nTop = m_nTop;

	for (int i = 0; i < (int)m_arrItems.size(); i++)
	{
		lpItem = m_arrItems[i];
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

BOOL CSkinToolBar::GetItemCheckState(int nIndex)
{
	CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
	return ((lpItem != NULL) ? lpItem->m_bChecked : FALSE);
}

void CSkinToolBar::SetItemCheckState(int nIndex, BOOL bChecked)
{
	CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
	if (lpItem != NULL && lpItem->m_bChecked != bChecked)
	{
		lpItem->m_bChecked = bChecked;
	}
}

BOOL CSkinToolBar::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return __super::OnEraseBkgnd(pDC);
}

void CSkinToolBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient;
	GetClientRect(&rcClient);

	CMemoryDC MemDC(&dc, rcClient);

	DrawParentWndBg(GetSafeHwnd(),MemDC.m_hDC);

	if ( CHECK_IMAGE(m_lpBgImg) )
		m_lpBgImg->Draw(&MemDC, rcClient);

	for (int i = 0; i < (int)m_arrItems.size(); i++)
	{
		DrawItem(&MemDC, i);
	}
}

void CSkinToolBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();

	m_nPressIndex = HitTest(point);
	if (m_nPressIndex != -1)
	{
		CSkinToolBarItem * lpItem = GetItemByIndex(m_nPressIndex);
		if (lpItem != NULL)
		{
			CRect rcItem;
			GetItemRectByIndex(m_nPressIndex, rcItem);

			CRect rcLeft, rcRight;
			if (lpItem->m_dwStyle & STBI_STYLE_DROPDOWN ||
				lpItem->m_dwStyle & STBI_STYLE_WHOLEDROPDOWN)
			{
				rcLeft = rcItem;
				rcLeft.right = rcLeft.left + lpItem->m_nLeftWidth;

				rcRight = rcItem;
				rcRight.left += lpItem->m_nLeftWidth;
				rcRight.right = rcRight.left + lpItem->m_nRightWidth;

				if (rcLeft.PtInRect(point))
					m_bPressLorR = TRUE;

				if (rcRight.PtInRect(point))
					m_bPressLorR = FALSE;
			}

			if (lpItem->m_dwStyle & STBI_STYLE_CHECK && !((lpItem->m_dwStyle & STBI_STYLE_DROPDOWN) && !m_bPressLorR))
			{
				lpItem->m_bChecked = !lpItem->m_bChecked;
				Invalidate(FALSE);

				if (lpItem->m_bChecked)
				{
					::SendMessage(::GetParent(m_hWnd), WM_COMMAND, 
						MAKEWPARAM(lpItem->m_nID, BN_PUSHED), 0);
				}
				else
				{
					::SendMessage(::GetParent(m_hWnd), WM_COMMAND, 
						MAKEWPARAM(lpItem->m_nID, BN_UNPUSHED), 0);
				}
			}
			else
			{
				if ((lpItem->m_dwStyle & STBI_STYLE_DROPDOWN && !m_bPressLorR)
					|| (lpItem->m_dwStyle & STBI_STYLE_WHOLEDROPDOWN))
				{
					Invalidate(FALSE);

					NMTOOLBAR nmtb = {0};
					nmtb.hdr.hwndFrom = m_hWnd;
					nmtb.hdr.idFrom = GetDlgCtrlID();
					nmtb.hdr.code = TBN_DROPDOWN;
					nmtb.iItem = lpItem->m_nID;
					nmtb.rcButton = rcItem;
					::SendMessage(::GetParent(m_hWnd), WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmtb);

					m_nPressIndex = -1;
					Invalidate(FALSE);
				}
				else
				{
					Invalidate(FALSE);
					::SetCapture(m_hWnd);
				}
			}
		}
	}

	__super::OnLButtonDown(nFlags, point);
}

void CSkinToolBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	HWND hWnd = ::GetCapture();
	if (m_hWnd == hWnd)
		::ReleaseCapture();

	int nIndex = HitTest(point);

	BOOL bClicked;
	if (nIndex != -1 && nIndex == m_nPressIndex)
		bClicked = TRUE;
	else
		bClicked = FALSE;

	if (m_nPressIndex != -1)
	{
		m_nPressIndex = -1;
		Invalidate(FALSE);
	}

	if (bClicked)
	{
		CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
		if (lpItem != NULL)
		{
			CRect rcItem;
			GetItemRectByIndex(m_nPressIndex, rcItem);

			CRect rcLeft, rcRight;
			BOOL bClickLorR = FALSE;
			if (lpItem->m_dwStyle & STBI_STYLE_DROPDOWN ||
				lpItem->m_dwStyle & STBI_STYLE_WHOLEDROPDOWN)
			{
				rcLeft = rcItem;
				rcLeft.right = rcLeft.left + lpItem->m_nLeftWidth;

				rcRight = rcItem;
				rcRight.left += lpItem->m_nLeftWidth;
				rcRight.right = rcRight.left + lpItem->m_nRightWidth;

				if (rcLeft.PtInRect(point))
					bClickLorR = TRUE;

				if (rcRight.PtInRect(point))
					bClickLorR = FALSE;
			}

			if ((!(lpItem->m_dwStyle & STBI_STYLE_WHOLEDROPDOWN))
				|| (lpItem->m_dwStyle & STBI_STYLE_DROPDOWN && bClickLorR && bClickLorR == m_bPressLorR))
				::SendMessage(::GetParent(m_hWnd), WM_COMMAND, MAKEWPARAM(lpItem->m_nID, BN_CLICKED), 0);
		}
	}

	__super::OnLButtonUp(nFlags, point);
}

void CSkinToolBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_nPressIndex != -1)
		return;

	if (!m_bMouseTracking)
	{
		TrackMouseLeave(GetSafeHwnd());
		m_bMouseTracking = TRUE;
	}

	int nIndex;
	BOOL bHoverLorR = FALSE;

	nIndex = HitTest(point);
	if (nIndex != -1)
	{
		CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
		if (lpItem != NULL)
		{
			CRect rcItem;
			GetItemRectByIndex(nIndex, rcItem);

			if (lpItem->m_dwStyle & STBI_STYLE_DROPDOWN ||
				lpItem->m_dwStyle & STBI_STYLE_WHOLEDROPDOWN)
			{
				CRect rcLeft(rcItem);
				rcLeft.right = rcLeft.left + lpItem->m_nLeftWidth;

				CRect rcRight(rcItem);
				rcRight.left += lpItem->m_nLeftWidth;
				rcRight.right = rcRight.left + lpItem->m_nRightWidth;

				if (rcLeft.PtInRect(point))
					bHoverLorR = TRUE;

				if (rcRight.PtInRect(point))
					bHoverLorR = FALSE;
			}

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

	if (nIndex != m_nHoverIndex || m_bHoverLorR != bHoverLorR)
	{
		m_nHoverIndex = nIndex;
		m_bHoverLorR = bHoverLorR;
		Invalidate(FALSE);
	}

	__super::OnMouseMove(nFlags, point);
}

void CSkinToolBar::OnDestroy()
{
	__super::OnDestroy();

	if (m_ToolTipCtrl.GetSafeHwnd())
		m_ToolTipCtrl.DestroyWindow();
	m_ToolTipCtrl.m_hWnd = NULL;

	CSkinToolBarItem * lpItem;
	for (int i = 0; i < (int)m_arrItems.size(); i++)
	{
		lpItem = m_arrItems[i];
		if (lpItem != NULL)
			delete lpItem;
	}
	m_arrItems.clear();

	UIRenderEngine->RemoveImage(m_lpBgImg);

	m_nPressIndex = m_nHoverIndex = -1;
	m_bPressLorR = m_bHoverLorR = FALSE;
	m_bMouseTracking = FALSE;
	m_nLeft = m_nTop = 0;
	m_bAutoSize = FALSE;
}

LRESULT CSkinToolBar::OnMouseLeave( WPARAM wparam, LPARAM lparam )
{
	m_bMouseTracking = FALSE;
	int nIndex = -1;
	if (nIndex != m_nHoverIndex)
	{
		m_nHoverIndex = nIndex;
		Invalidate(FALSE);
	}

	return TRUE;
}


BOOL CSkinToolBar::PreTranslateMessage(MSG* pMsg)
{
	if (m_ToolTipCtrl.GetSafeHwnd())
		m_ToolTipCtrl.RelayEvent(pMsg);

	return __super::PreTranslateMessage(pMsg);
}
CSkinToolBarItem * CSkinToolBar::GetItemByIndex(int nIndex)
{
	if (nIndex >= 0 && nIndex < (int)m_arrItems.size())
		return m_arrItems[nIndex];
	else
		return NULL;
}

CSkinToolBarItem * CSkinToolBar::GetItemByID(int nID)
{
	CSkinToolBarItem * lpItem;
	for (int i = 0; i < (int)m_arrItems.size(); i++)
	{
		lpItem = m_arrItems[i];
		if (lpItem != NULL && lpItem->m_nID == nID)
			return lpItem;
	}

	return NULL;
}

int CSkinToolBar::HitTest(POINT pt)
{
	CSkinToolBarItem * lpItem;
	int nLeft = m_nLeft, nTop = m_nTop;
	CRect rcItem;

	for (int i = 0; i < (int)m_arrItems.size(); i++)
	{
		lpItem = m_arrItems[i];
		if (lpItem != NULL)
		{
			rcItem = CRect(nLeft, nTop, nLeft+lpItem->m_nWidth, nTop+lpItem->m_nHeight);
			if (rcItem.PtInRect(pt))
			{
				if (lpItem->m_dwStyle & STBI_STYLE_SEPARTOR)
					return -1;
				else
					return i;
			}
			nLeft += lpItem->m_nWidth;
			nLeft += lpItem->m_nPadding;
		}
	}

	return -1;
}

void CSkinToolBar::DrawItem(CDC*pDC, int nIndex)
{
	CSkinToolBarItem * lpItem = GetItemByIndex(nIndex);
	if (NULL == lpItem)
		return;

	CRect rcItem;
	GetItemRectByIndex(nIndex, rcItem);

	CRect rcLeft, rcRight;
	if (lpItem->m_dwStyle & STBI_STYLE_DROPDOWN
		|| lpItem->m_dwStyle & STBI_STYLE_WHOLEDROPDOWN)
	{
		rcLeft = rcItem;
		rcLeft.right = rcLeft.left + lpItem->m_nLeftWidth;

		rcRight = rcItem;
		rcRight.left += lpItem->m_nLeftWidth;
		rcRight.right = rcRight.left + lpItem->m_nRightWidth;
	}

	if (lpItem->m_dwStyle & STBI_STYLE_SEPARTOR)
	{
		if (lpItem->m_lpSepartorImg != NULL && !lpItem->m_lpSepartorImg->IsNull())
			lpItem->m_lpSepartorImg->Draw(pDC, rcItem);
		return;
	}

	if ((lpItem->m_dwStyle & STBI_STYLE_CHECK) && lpItem->m_bChecked)
	{
		if (lpItem->m_lpBgImgD != NULL && !lpItem->m_lpBgImgD->IsNull())
			lpItem->m_lpBgImgD->Draw(pDC, rcItem);

		rcItem.OffsetRect(1, 1);

		if (lpItem->m_dwStyle & STBI_STYLE_DROPDOWN)
		{
			if (lpItem->m_lpLeftD != NULL && !lpItem->m_lpLeftD->IsNull())
				lpItem->m_lpLeftD->Draw(pDC, rcLeft);
			rcLeft.OffsetRect(1, 1);
		}

		if (m_nPressIndex == nIndex)
		{
			if (lpItem->m_dwStyle & STBI_STYLE_DROPDOWN)
			{
				if (!m_bPressLorR)
				{
					if (lpItem->m_lpRightD != NULL && !lpItem->m_lpRightD->IsNull())
						lpItem->m_lpRightD->Draw(pDC, rcRight);

					rcRight.OffsetRect(1, 1);
				}
			}
		}
		else if (m_nHoverIndex == nIndex)
		{
			if (lpItem->m_dwStyle & STBI_STYLE_DROPDOWN)
			{
				if (!m_bHoverLorR)
				{
					if (lpItem->m_lpRightH != NULL && !lpItem->m_lpRightH->IsNull())
						lpItem->m_lpRightH->Draw(pDC, rcRight);
				}
			}
		}
	}
	else
	{
		if (m_nPressIndex == nIndex)
		{
			if (lpItem->m_lpBgImgD != NULL && !lpItem->m_lpBgImgD->IsNull())
				lpItem->m_lpBgImgD->Draw(pDC, rcItem);

			if (lpItem->m_dwStyle & STBI_STYLE_DROPDOWN)
			{
				if (m_bPressLorR)
				{
					if (lpItem->m_lpLeftD != NULL && !lpItem->m_lpLeftD->IsNull())
						lpItem->m_lpLeftD->Draw(pDC, rcLeft);
				}
				else
				{
					if (lpItem->m_lpRightD != NULL && !lpItem->m_lpRightD->IsNull())
						lpItem->m_lpRightD->Draw(pDC, rcRight);
				}
			}

			rcItem.OffsetRect(1, 1);

			if (lpItem->m_dwStyle & STBI_STYLE_DROPDOWN
				|| lpItem->m_dwStyle & STBI_STYLE_WHOLEDROPDOWN)
			{
				if (m_bPressLorR)
					rcLeft.OffsetRect(1, 1);
				else
					rcRight.OffsetRect(1, 1);
			}
		}
		else if (m_nHoverIndex == nIndex)
		{
			if (lpItem->m_lpBgImgH != NULL && !lpItem->m_lpBgImgH->IsNull())
				lpItem->m_lpBgImgH->Draw(pDC, rcItem);

			if (lpItem->m_dwStyle & STBI_STYLE_DROPDOWN)
			{
				if (m_bHoverLorR)
				{
					if (lpItem->m_lpLeftH != NULL && !lpItem->m_lpLeftH->IsNull())
						lpItem->m_lpLeftH->Draw(pDC, rcLeft);
				}
				else
				{
					if (lpItem->m_lpRightH != NULL && !lpItem->m_lpRightH->IsNull())
						lpItem->m_lpRightH->Draw(pDC, rcRight);
				}
			}
		}
	}

	BOOL bHasText = FALSE;
	if (lpItem->m_strText.GetLength() > 0)
		bHasText = TRUE;

	BOOL bHasIcon = FALSE;
	if (lpItem->m_lpIconImg != NULL && !lpItem->m_lpIconImg->IsNull())
		bHasIcon = TRUE;

	if (!(lpItem->m_dwStyle & STBI_STYLE_DROPDOWN
		|| lpItem->m_dwStyle & STBI_STYLE_WHOLEDROPDOWN))
		rcLeft = rcItem;

	if (bHasIcon && bHasText)	// 带图标和文字
	{
		int cxIcon = lpItem->m_lpIconImg->GetWidth();
		int cyIcon = lpItem->m_lpIconImg->GetHeight();

		int nMode = pDC->SetBkMode(TRANSPARENT);
		HFONT hFont = (HFONT)SendMessage(WM_GETFONT, 0, 0);
		if (NULL == hFont)
			hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
		HFONT hOldFont = (HFONT)pDC->SelectObject(hFont);

		CRect rcText(0,0,0,0);	// 计算文字宽高
		pDC->DrawText(lpItem->m_strText, lpItem->m_strText.GetLength(), &rcText, DT_SINGLELINE | DT_CALCRECT);

		int cx = cxIcon+1+rcText.Width();
		int cy = cyIcon;

		CRect rcCenter;
		CalcCenterRect(rcLeft, cx, cy, rcCenter);

		CRect rcIcon(rcCenter.left, rcCenter.top, rcCenter.left+cxIcon, rcCenter.bottom);
		lpItem->m_lpIconImg->Draw(pDC, rcIcon);

		UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
		rcText = CRect(rcIcon.right+1, rcLeft.top, rcIcon.right+1+rcText.Width(), rcLeft.bottom);
		pDC->DrawText(lpItem->m_strText, lpItem->m_strText.GetLength(), &rcText, nFormat);

		pDC->SelectObject(hOldFont);
		pDC->SetBkMode(nMode);
	}
	else if (bHasIcon)	// 仅图标
	{
		int cxIcon = lpItem->m_lpIconImg->GetWidth();
		int cyIcon = lpItem->m_lpIconImg->GetHeight();

		CRect rcIcon;
		CalcCenterRect(rcLeft, cxIcon, cyIcon, rcIcon);

		lpItem->m_lpIconImg->Draw(pDC, rcIcon);
	}
	else if (bHasText)	// 仅文字
	{
		UINT nFormat = DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;

		int nMode = pDC->SetBkMode( TRANSPARENT);
		HFONT hFont = (HFONT)SendMessage(WM_GETFONT, 0, 0);
		if (NULL == hFont)
			hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
		HFONT hOldFont = (HFONT)pDC->SelectObject(hFont);
		pDC->DrawText(lpItem->m_strText, lpItem->m_strText.GetLength(), &rcLeft, nFormat);
		pDC->SelectObject(hOldFont);
		pDC->SetBkMode(nMode);
	}

	if (lpItem->m_dwStyle & STBI_STYLE_DROPDOWN
		|| lpItem->m_dwStyle & STBI_STYLE_WHOLEDROPDOWN)
	{
		if (lpItem->m_lpArrowImg != NULL && !lpItem->m_lpArrowImg->IsNull())
		{
			int cxArrow = lpItem->m_lpArrowImg->GetWidth();
			int cyArrow = lpItem->m_lpArrowImg->GetHeight();

			CRect rcArrow;
			CalcCenterRect(rcRight, cxArrow, cyArrow, rcArrow);
			rcArrow.left = rcRight.left;
			rcArrow.right = rcArrow.left + cxArrow;

			lpItem->m_lpArrowImg->Draw(pDC, rcArrow);
		}
	}
}
BOOL CSkinToolBar::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return __super::Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rect,pParentWnd, nID);
}

void CSkinToolBar::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	ISkinControl::SetAttribute(pstrName,pstrValue);
}

void CSkinToolBar::ParseItem( CXmlNode *root )
{

}

BOOL CSkinToolBar::CreateControl( CWnd* pParentWnd )
{
	if( !Create(WS_CHILD|WS_VISIBLE | WS_TABSTOP,CRect(0,0,0,0),pParentWnd,0) )
		return FALSE;

	m_pOwnWnd = this;

	return TRUE;
}
