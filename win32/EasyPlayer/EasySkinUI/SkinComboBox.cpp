// SkinComboBox.cpp : 实现文件
//

#include "stdafx.h"
#include "Control.h"
#include "SkinComboBox.h"

static WNDPROC m_pWndProc=0;

extern "C" LRESULT FAR PASCAL ComboBoxListBoxProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	return CallWindowProc(m_pWndProc, hWnd, nMsg, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSkinComboBox_Edit, CEdit)
 	ON_WM_ERASEBKGND()
 	ON_WM_MOUSEMOVE()
 	ON_WM_MOUSELEAVE()
 	ON_WM_SETFOCUS()
 	ON_WM_KILLFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


CSkinComboBox_Edit::CSkinComboBox_Edit( void )
{
	m_hOwnerWnd = NULL;
	m_bMouseTracking = FALSE;
	m_bIsDefText = FALSE;
}

CSkinComboBox_Edit::~CSkinComboBox_Edit( void )
{
}

void CSkinComboBox_Edit::SetOwnerWnd( HWND hWnd )
{
	m_hOwnerWnd = hWnd;
}

void CSkinComboBox_Edit::SetDefaultText( LPCTSTR lpszText )
{
	m_strDefText = lpszText;
}

BOOL CSkinComboBox_Edit::IsDefaultText()
{
	return m_bIsDefText;
}

void CSkinComboBox_Edit::SetDefaultTextMode( BOOL bIsDefText )
{
	if (bIsDefText == m_bIsDefText)
		return;

	m_bIsDefText = bIsDefText;
	if (m_bIsDefText)
	{
		SetWindowText(m_strDefText);
	}
	else
	{
		SetWindowText(_T(""));
	}
}

BOOL CSkinComboBox_Edit::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CSkinComboBox_Edit::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bMouseTracking)
	{
		m_bMouseTracking = TRUE;
		TrackMouseLeave(GetSafeHwnd());

		if (::IsWindow(m_hOwnerWnd))
			::SendMessage(m_hOwnerWnd, WM_CBO_EDIT_MOUSE_HOVER, 0, 0);
	}

	__super::OnMouseMove(nFlags, point);
}

void CSkinComboBox_Edit::OnMouseLeave()
{
	m_bMouseTracking = FALSE;

	if (::IsWindow(m_hOwnerWnd))
		::SendMessage(m_hOwnerWnd, WM_CBO_EDIT_MOUSE_LEAVE, 0, 0);

	__super::OnMouseLeave();
}

void CSkinComboBox_Edit::OnSetFocus(CWnd* pOldWnd)
{
	__super::OnSetFocus(pOldWnd);

	if (m_bIsDefText)
	{
		m_bIsDefText = FALSE;
		SetWindowText(TEXT(""));
	}

	//将焦点返回到原焦点
	if ( GetStyle() & ES_READONLY )
	{
		if( pOldWnd != NULL && pOldWnd->GetSafeHwnd() != NULL )
			pOldWnd->SetFocus();  
	}
}

void CSkinComboBox_Edit::OnLButtonDown(UINT nFlags, CPoint point)
{
	//将焦点返回到原焦点
	if ( GetStyle() & ES_READONLY )
	{
		if (::IsWindow(m_hOwnerWnd))
			::SendMessage(m_hOwnerWnd, WM_LBUTTONDOWN,0,0);

		return;
	}

	__super::OnLButtonDown(nFlags, point);
}

void CSkinComboBox_Edit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//将焦点返回到原焦点
	if ( GetStyle() & ES_READONLY )
	{
		return;
	}

	__super::OnLButtonDblClk(nFlags, point);
}

LRESULT CSkinComboBox_Edit::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//如果只读属性，且有选中消息则过滤掉
	if ( ( GetStyle() & ES_READONLY ) && (message == EM_SETSEL) )
	{
		return TRUE;
	}

	return __super::DefWindowProc(message, wParam, lParam);
}

BOOL CSkinComboBox_Edit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//将焦点返回到原焦点
	if ( GetStyle() & ES_READONLY )
	{
		SetCursor(LoadCursor(NULL,IDC_ARROW));
		return TRUE;
	}

	return __super::OnSetCursor(pWnd, nHitTest, message);
}


void CSkinComboBox_Edit::OnKillFocus(CWnd* pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	if (GetWindowTextLength() <= 0 && !m_strDefText.IsEmpty())
	{
		m_bIsDefText = TRUE;
		SetWindowText(m_strDefText);
	}
}

HBRUSH CSkinComboBox_Edit::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	bool bIsReadOnly = ((GetStyle() & ES_READONLY) != 0) ? true : false;
 
 	if (m_bIsDefText)
 	{
 		if( bIsReadOnly )
 			pDC->SetTextColor(m_colReadOnlyText);
 		else pDC->SetTextColor(m_colDefText);
 	}
 	else
 	{
 		if( bIsReadOnly )
 			pDC->SetTextColor(m_colReadOnlyText);
 		else pDC->SetTextColor(m_colNormalText);
 	}

	return (HBRUSH)NULL_BRUSH;
}

//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CSkinComboBox_ListBox, CListBox)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

CSkinComboBox_ListBox::CSkinComboBox_ListBox( void )
{
	m_hOwnerWnd = NULL;
	m_pBackImgN = NULL;
	m_pSelectImg = NULL;
}

CSkinComboBox_ListBox::~CSkinComboBox_ListBox( void )
{

}

void CSkinComboBox_ListBox::SetOwnerWnd( HWND hWnd )
{
	m_hOwnerWnd = hWnd;
}

void CSkinComboBox_ListBox::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CListBox::OnShowWindow(bShow, nStatus);

	if (!bShow)
	{
		::SendMessage(m_hOwnerWnd, WM_CBO_LIST_HIDE, 0, 0);
	}
}

BOOL CSkinComboBox_ListBox::SetBackNormalImage( LPCTSTR lpszFileName, CONST LPRECT lpNinePart /*= NULL*/ )
{
	UIRenderEngine->RemoveImage(m_pBackImgN);
	m_pBackImgN = UIRenderEngine->GetImage(lpszFileName);

	if (m_pBackImgN != NULL)
		m_pBackImgN->SetNinePart(lpNinePart);

	return (m_pBackImgN != NULL) ? TRUE : FALSE;
}

BOOL CSkinComboBox_ListBox::SetSelectImage( LPCTSTR lpszFileName, CONST LPRECT lpNinePart /*= NULL*/ )
{
	UIRenderEngine->RemoveImage(m_pSelectImg);
	m_pSelectImg = UIRenderEngine->GetImage(lpszFileName);

	if (m_pSelectImg != NULL)
		m_pSelectImg->SetNinePart(lpNinePart);

	return (m_pSelectImg != NULL) ? TRUE : FALSE;
}

void CSkinComboBox_ListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//变量定义
	CRect rcItem=lpDrawItemStruct->rcItem;
	CDC * pDCControl=CDC::FromHandle(lpDrawItemStruct->hDC);

	//创建缓冲
 	CDC BufferDC;
 	CBitmap ImageBuffer;
 	BufferDC.CreateCompatibleDC(pDCControl);
 	ImageBuffer.CreateCompatibleBitmap(pDCControl,rcItem.Width(),rcItem.Height());
 
 	//设置环境
 	BufferDC.SelectObject(&ImageBuffer);
	BufferDC.SelectObject(GetCtrlFont());

	//获取字符
	CString strString;
	GetText(lpDrawItemStruct->itemID,strString);

	//计算位置
	CRect rcString;
	rcString.SetRect(4,0,rcItem.Width()-8,rcItem.Height());

	//颜色定义
	COLORREF crTextColor=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0)?m_colSelectText:m_colNormalText;
	COLORREF crBackColor=((lpDrawItemStruct->itemState&ODS_SELECTED)!=0)?RGB(10,36,106):RGB(255,255,255);

	//绘画背景
	BufferDC.FillSolidRect(0,0,rcItem.Width(),rcItem.Height(),crBackColor);

	if ( (lpDrawItemStruct->itemState&ODS_SELECTED) != 0 )
	{
		if ( m_pSelectImg!= NULL && !m_pSelectImg->IsNull() )
		{
			rcItem.DeflateRect(1,1,1,1);
			m_pSelectImg->Draw(&BufferDC,CRect(0,0,rcItem.Width(),rcItem.Height()));
		}
	}

	//绘画字符
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SetTextColor(crTextColor);
	BufferDC.DrawText(strString,&rcString,DT_VCENTER|DT_SINGLELINE);

	//绘画界面
 	pDCControl->BitBlt(rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height(),&BufferDC,0,0,SRCCOPY);

	//清理资源
 	BufferDC.DeleteDC();
 	ImageBuffer.DeleteObject();
}

void CSkinComboBox_ListBox::OnDestroy()
{
	__super::OnDestroy();

	UIRenderEngine->RemoveImage(m_pBackImgN);
	UIRenderEngine->RemoveImage(m_pSelectImg);
}

void CSkinComboBox_ListBox::DrawListFrame()
{
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	rcWindow.OffsetRect(-rcWindow.left, -rcWindow.top);

	CDC *pDC = GetWindowDC();
	if( pDC == NULL ) pDC = GetDC();

	UIRenderEngine->DrawRect(pDC->GetSafeHdc(),rcWindow,1,m_colFrameNormal);

	ReleaseDC(pDC);
}

void CSkinComboBox_ListBox::OnRButtonUp(UINT nFlags, CPoint point)
{
	CRect rcItem(0,0,0,0);

	int nIndex = -1;

	for (int i =0; i<GetCount(); i++)
	{
		GetItemRect(i, &rcItem);

		if(rcItem.PtInRect(point))
		{
			if ( m_hOwnerWnd != NULL ) nIndex = i;
		
			break;
		}
	}

	::PostMessage(m_hOwnerWnd,WM_CBO_LIST_RBUTTONUP,nIndex,0);

	__super::OnRButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
// CSkinComboBox

IMPLEMENT_DYNAMIC(CSkinComboBox, CComboBox)

CSkinComboBox::CSkinComboBox()
{
	m_lpBgImgN = NULL;
	m_lpBgImgH = NULL;
	m_lpArrowImgN = NULL;
	m_lpArrowImgH = NULL;
	m_lpArrowImgP = NULL;
	m_bFocus = m_bPress = m_bHover = m_bMouseTracking = FALSE;
	m_bArrowPress = FALSE;
	m_bArrowHover = FALSE;
	m_nArrowWidth = 17;
	m_rcArrow.SetRectEmpty();
	m_cyItemHeight = 15;
	m_nEditHeight = 20;
}

CSkinComboBox::~CSkinComboBox()
{

}

BEGIN_MESSAGE_MAP(CSkinComboBox, CComboBox)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
 	ON_WM_MOUSEMOVE()
 	ON_WM_MOUSELEAVE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
 	ON_MESSAGE(WM_CBO_EDIT_MOUSE_HOVER, OnEditMouseHover)
 	ON_MESSAGE(WM_CBO_EDIT_MOUSE_LEAVE, OnEditMouseLeave)
	ON_MESSAGE(WM_CBO_LIST_RBUTTONUP,OnRButtonUp)
	ON_MESSAGE(WM_CBO_LIST_HIDE, OnListHide)
	ON_MESSAGE(WM_CTLCOLORLISTBOX,OnCtlColorListBox)
END_MESSAGE_MAP()

BOOL CSkinComboBox::SetBgNormalPic( LPCTSTR lpszFileName, RECT * lpNinePart /*= NULL*/ )
{
	UIRenderEngine->RemoveImage(m_lpBgImgN);
	m_lpBgImgN = UIRenderEngine->GetImage(lpszFileName);
	if (m_lpBgImgN != NULL)
		m_lpBgImgN->SetNinePart(lpNinePart);
	return (m_lpBgImgN != NULL) ? TRUE : FALSE;
}

BOOL CSkinComboBox::SetBgHotPic( LPCTSTR lpszFileName, RECT * lpNinePart /*= NULL*/ )
{
	UIRenderEngine->RemoveImage(m_lpBgImgH);
	m_lpBgImgH = UIRenderEngine->GetImage(lpszFileName);
	if (m_lpBgImgH != NULL)
		m_lpBgImgH->SetNinePart(lpNinePart);
	return (m_lpBgImgH != NULL) ? TRUE : FALSE;
}

BOOL CSkinComboBox::SetArrowNormalPic( LPCTSTR lpszFileName )
{
	UIRenderEngine->RemoveImage(m_lpArrowImgN);
	m_lpArrowImgN = UIRenderEngine->GetImage(lpszFileName);
	return (m_lpArrowImgN != NULL) ? TRUE : FALSE;
}

BOOL CSkinComboBox::SetArrowHotPic( LPCTSTR lpszFileName )
{
	UIRenderEngine->RemoveImage(m_lpArrowImgH);
	m_lpArrowImgH =UIRenderEngine->GetImage(lpszFileName);
	return (m_lpArrowImgH != NULL) ? TRUE : FALSE;
}

BOOL CSkinComboBox::SetArrowPushedPic( LPCTSTR lpszFileName )
{
	UIRenderEngine->RemoveImage(m_lpArrowImgP);
	m_lpArrowImgP = UIRenderEngine->GetImage(lpszFileName);
	return (m_lpArrowImgP != NULL) ? TRUE : FALSE;
}

void CSkinComboBox::SetDefaultText( LPCTSTR lpszText )
{
	m_SkinComboBoxEdit.SetDefaultText(lpszText);
}

BOOL CSkinComboBox::IsDefaultText()
{
	return m_SkinComboBoxEdit.IsDefaultText();
}

void CSkinComboBox::SetArrowWidth( int nWidth )
{
	m_nArrowWidth = nWidth;
}

// CSkinComboBox 消息处理程序

void CSkinComboBox::PreSubclassWindow()
{
	//变量定义
	COMBOBOXINFO ComboBoxInfo;
	ComboBoxInfo.cbSize=sizeof(ComboBoxInfo);

	//绑定控件
	if (GetComboBoxInfo(&ComboBoxInfo)==TRUE)
	{
		if (ComboBoxInfo.hwndItem!=NULL) 
		{
			m_SkinComboBoxEdit.SetOwnerWnd(GetSafeHwnd());
			m_SkinComboBoxEdit.SubclassWindow(ComboBoxInfo.hwndItem);
		}

		if (ComboBoxInfo.hwndList!=NULL) 
		{
			m_SkinComboBoxList.SetOwnerWnd(GetSafeHwnd());
			m_SkinComboBoxList.SubclassWindow(ComboBoxInfo.hwndList);
		}
	}

	CComboBox::PreSubclassWindow();
}

int CSkinComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	//变量定义
	COMBOBOXINFO ComboBoxInfo;
	ComboBoxInfo.cbSize=sizeof(ComboBoxInfo);

	//绑定控件
	if (GetComboBoxInfo(&ComboBoxInfo)==TRUE)
	{
		if (ComboBoxInfo.hwndItem!=NULL) 
		{
			m_SkinComboBoxEdit.SetOwnerWnd(GetSafeHwnd());
			m_SkinComboBoxEdit.SubclassWindow(ComboBoxInfo.hwndItem);
		}

		if (ComboBoxInfo.hwndList!=NULL) 
		{
			m_SkinComboBoxList.SetOwnerWnd(GetSafeHwnd());
			m_SkinComboBoxList.SubclassWindow(ComboBoxInfo.hwndList);
		}
	}

	//设置默认字体
	SetFont(CFont::FromHandle(UIRenderEngine->GetDeaultFont()));

	return 0;
}


BOOL CSkinComboBox::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CSkinComboBox::OnPaint()
{
	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(&rcClient);

	CMemoryDC MemDC(&dc,rcClient);

	CRect rcArrow;
	HRGN hRgn2 = NULL;
	if (m_lpArrowImgN != NULL && !m_lpArrowImgN->IsNull())
	{
		int cxIcon = m_nArrowWidth;
		int cyIcon = m_lpArrowImgN->GetHeight();

		CalcCenterRect(rcClient, cxIcon, cyIcon, rcArrow);
		rcArrow.right = rcClient.right - 2;
		rcArrow.left = rcArrow.right - cxIcon;
	}

	DrawParentWndBg(GetSafeHwnd(),MemDC->GetSafeHdc());

	if (m_bHover)
	{
		if (m_lpBgImgH != NULL && !m_lpBgImgH->IsNull())
		{
			m_lpBgImgH->Draw(&MemDC, rcClient);
		}
		else
		{
			if (m_lpBgImgN != NULL && !m_lpBgImgN->IsNull())
				m_lpBgImgN->Draw(&MemDC, rcClient);
		}
	}
	else
	{
		if (m_lpBgImgN != NULL && !m_lpBgImgN->IsNull())
			m_lpBgImgN->Draw(&MemDC, rcClient);
	}

	if (m_bArrowPress)
	{
		if (m_lpArrowImgP != NULL && !m_lpArrowImgP->IsNull())
			m_lpArrowImgP->Draw(&MemDC, rcArrow);
	}
	else if (m_bArrowHover)
	{
		if (m_lpArrowImgH != NULL && !m_lpArrowImgH->IsNull())
			m_lpArrowImgH->Draw(&MemDC, rcArrow);
	}
	else
	{
		if (m_lpArrowImgN != NULL && !m_lpArrowImgN->IsNull())
			m_lpArrowImgN->Draw(&MemDC, rcArrow);
	}

	//绘制边框
	m_SkinComboBoxList.DrawListFrame();
}

void CSkinComboBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	//设置焦点
	SetFocus();

	//效验数据
	if( m_SkinComboBoxEdit.GetSafeHwnd() == NULL ) return;

	if ( (m_SkinComboBoxEdit.GetStyle() & ES_READONLY) == 0 )
	{
		if (VerdictOverButton(point))
		{
			m_bArrowPress = TRUE;

			//显示列表
			ShowDropDown(GetDroppedState()==FALSE);
		}
	}
	else
	{
		m_bArrowPress = TRUE;

		//显示列表
		ShowDropDown(GetDroppedState()==FALSE);

		m_SkinComboBoxEdit.SetSel(0,0);
	}

	//更新按钮
	RedrawWindow(&m_rcArrow,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
	//__super::OnLButtonDown(nFlags, point);
}

void CSkinComboBox::OnMouseMove(UINT nFlags, CPoint point)
{
	BOOL bRePaint = FALSE;

	if (!m_bMouseTracking)
	{
		TrackMouseLeave(GetSafeHwnd());
		m_bMouseTracking = TRUE;
		m_bHover = TRUE;
		bRePaint = TRUE;
	}

	if (VerdictOverButton(point))
	{
		if (!m_bArrowHover)
		{
			m_bArrowHover = TRUE;
			bRePaint = TRUE;
		}
	}
	else
	{
		if (m_bArrowHover)
		{
			m_bArrowHover = FALSE;
			bRePaint = TRUE;
		}
	}

	if (bRePaint)
		RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);

	__super::OnMouseMove(nFlags, point);
}

void CSkinComboBox::OnMouseLeave()
{
	m_bMouseTracking = FALSE;

	if (!m_SkinComboBoxList.IsWindowVisible())
	{
		CPoint pt;
		GetCursorPos(&pt);

		CRect rcWindow;
		GetWindowRect(&rcWindow);

		if (!rcWindow.PtInRect(pt))
			m_bHover = FALSE;

		m_bArrowHover = FALSE;
		RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
	}

	__super::OnMouseLeave();
}

void CSkinComboBox::OnDestroy()
{
	__super::OnDestroy();

	UIRenderEngine->RemoveImage(m_lpBgImgN);
	UIRenderEngine->RemoveImage(m_lpBgImgH);
	UIRenderEngine->RemoveImage(m_lpArrowImgN);
	UIRenderEngine->RemoveImage(m_lpArrowImgH);
	UIRenderEngine->RemoveImage(m_lpArrowImgP);

	RemoveScorll();
}

void CSkinComboBox::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if ( m_SkinComboBoxEdit.GetSafeHwnd() == NULL ) return;
	
	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rcEdit;
	m_SkinComboBoxEdit.GetWindowRect(&rcEdit);
	ScreenToClient(&rcEdit);

	CDC* pDC = m_SkinComboBoxEdit.GetDC();
	TEXTMETRIC tm = {0};
	pDC->GetTextMetrics(&tm);
	int nFontHeight = tm.tmHeight + tm.tmExternalLeading;
	int nMargin = (rcEdit.Height() - nFontHeight) / 2;
	m_SkinComboBoxEdit.ReleaseDC(pDC);

	rcEdit.DeflateRect(0, nMargin);
	rcEdit.right = rcClient.right - 2 - m_nArrowWidth;

	m_SkinComboBoxEdit.MoveWindow(&rcEdit, FALSE);

	m_rcArrow.left = rcClient.right - 2 - m_nArrowWidth;
	m_rcArrow.right = m_rcArrow.left + m_nArrowWidth;
	m_rcArrow.top = rcClient.top;
	m_rcArrow.bottom = rcClient.bottom;
}

LRESULT CSkinComboBox::OnEditMouseHover( WPARAM wParam, LPARAM lParam )
{
	if (!m_bHover)
	{
		m_bHover = TRUE;
		RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
	}

	return 0;
}

LRESULT CSkinComboBox::OnEditMouseLeave( WPARAM wParam, LPARAM lParam )
{
	CPoint pt;
	GetCursorPos(&pt);

	CRect rcWindow;
	GetWindowRect(&rcWindow);

	if (!rcWindow.PtInRect(pt))
	{
		if (m_bHover)
		{
			m_bHover = FALSE;
			RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
		}
	}

	return 0;
}

LRESULT CSkinComboBox::OnListHide( WPARAM wParam, LPARAM lParam )
{
	m_bHover = FALSE;
	m_bArrowHover = FALSE;
	m_bArrowPress = FALSE;
	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);

	return 0;
}

bool CSkinComboBox::VerdictOverButton( CPoint MousePoint )
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//下拉列表
	if ((rcClient.PtInRect(MousePoint)==TRUE)&&((GetStyle()&CBS_SIMPLE)!=0)) return true;

	//坐标计算
	if ((MousePoint.y>(rcClient.Height()-1))||(MousePoint.y<1)) return false;
	if ((MousePoint.x<(rcClient.Width()-(INT)m_rcArrow.Width()-1))||(MousePoint.x>(rcClient.Width()-1))) return false;

	return true;
}

void CSkinComboBox::SetDropList()
{
	//效验数据
	if ( m_SkinComboBoxEdit.GetSafeHwnd() == NULL ) return;
	
	//m_SkinComboBoxEdit.EnableWindow(FALSE);
	m_SkinComboBoxEdit.SetReadOnly();
}

void CSkinComboBox::SetEditTextColor( COLORREF col )
{
	//效验数据
	if ( m_SkinComboBoxEdit.GetSafeHwnd() == NULL ) return;
	
	if ( (m_SkinComboBoxEdit.GetStyle() & ES_READONLY) == 0)	
		m_SkinComboBoxEdit.m_colNormalText = col;
	else m_SkinComboBoxEdit.m_colReadOnlyText = col;

	RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW);
}

CSkinComboBox_Edit * CSkinComboBox::GetEditWnd()
{
	if ( m_SkinComboBoxEdit.GetSafeHwnd() == NULL ) return NULL;
	
	return &m_SkinComboBoxEdit;
}

CSkinComboBox_ListBox * CSkinComboBox::GetListBoxWnd()
{
	if ( m_SkinComboBoxList.GetSafeHwnd() == NULL ) return NULL;

	return &m_SkinComboBoxList;
}

void CSkinComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = m_cyItemHeight;
}

void CSkinComboBox::SetAllItemHeight(UINT cyItemHeight)
{
	m_cyItemHeight = cyItemHeight;
}

LRESULT CSkinComboBox::OnCtlColorListBox( WPARAM wParam, LPARAM lParam )
{
	HWND hListBoxWnd = m_SkinComboBoxList.GetSafeHwnd();
	if ( hListBoxWnd == NULL) 
	{
		HWND hWnd = (HWND)lParam;
		if (hWnd != 0 && hWnd != m_hWnd) 
		{
			hListBoxWnd = hWnd;
			m_pWndProc = (WNDPROC)GetWindowLong(hListBoxWnd, GWL_WNDPROC);
			SetWindowLong(hListBoxWnd, GWL_WNDPROC, (LONG)ComboBoxListBoxProc);
		}
	}

	return DefWindowProc(WM_CTLCOLORLISTBOX, wParam, lParam);
}

BOOL CSkinComboBox::SetScrollImage( LPCTSTR pszFileName )
{
	if ( m_SkinComboBoxList.GetSafeHwnd() == FALSE ) return FALSE;
	
	return m_SkinComboBoxList.SetScrollImage(&m_SkinComboBoxList,pszFileName);
}

LRESULT CSkinComboBox::OnRButtonUp( WPARAM wParam, LPARAM lParam )
{
	CWnd *pWnd = GetParent();

	if ( (pWnd != NULL) && (pWnd->GetSafeHwnd() != NULL) )
	{
		pWnd->PostMessage(WM_CBO_RBUTTONUP,wParam,lParam);
	}

	return 0L;
}

BOOL CSkinComboBox::CreateControl(CWnd* pParentWnd)
{
	if( !Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWN | CBS_SORT |WS_VSCROLL | WS_TABSTOP,CRect(0,0,0,0),pParentWnd,0) )
		return FALSE;

	m_pOwnWnd = this;

	return TRUE;
}

void CSkinComboBox::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	ISkinControl::SetAttribute(pstrName,pstrValue);
	LPTSTR pstr = NULL;

	if( _tcscmp(pstrName, _T("droplist")) == 0 ) 
	{
		if( _tcscmp(pstrValue, _T("true")) == 0 )
		{
			ModifyStyle(0,CBS_OWNERDRAWVARIABLE|CBS_HASSTRINGS);
			SetDropList();
		}
	}
	else if( _tcscmp(pstrName, _T("defaulttext")) == 0 )  SetDefaultText(pstrValue);
	else if( _tcscmp(pstrName, _T("arrownormalimg")) == 0 )  SetArrowNormalPic(pstrValue);	
	else if( _tcscmp(pstrName, _T("arrowhotimg")) == 0 )  SetArrowHotPic(pstrValue);	
	else if( _tcscmp(pstrName, _T("arrowpushimg")) == 0 )  SetArrowPushedPic(pstrValue);
	else if( _tcscmp(pstrName, _T("arrowwidth")) == 0 )  SetArrowWidth(_ttoi(pstrValue));	
	else if( _tcscmp(pstrName, _T("boxheight")) == 0 )  SetEditItemHeight(_ttoi(pstrValue)); 
	else if( _tcscmp(pstrName, _T("itemheight")) == 0 )  SetAllItemHeight(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("textcolor")) == 0 ) 
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		SetEditTextColor( _tcstoul(pstrValue, &pstr, 16));
	}
}

void CSkinComboBox::ParseItem(CXmlNode *root )
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

		if( _tcscmp(pstrClass, _T("normalimage")) == 0 ) 	
		{
			if( _tcscmp(pstrName, _T("value")) == 0 ) SetBgNormalPic(pstrValue);
			else if( _tcscmp(pstrName, _T("ninepart")) == 0 )
			{
				LPTSTR pstr = NULL;
				CRect rc;
				rc.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rc.right = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rc.bottom = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   

				if (m_lpBgImgN != NULL)
					m_lpBgImgN->SetNinePart(&rc);
			}
		}
		else if( _tcscmp(pstrClass, _T("hotimage")) == 0 ) 	
		{
			if( _tcscmp(pstrName, _T("value")) == 0 ) SetBgHotPic(pstrValue);
			else if( _tcscmp(pstrName, _T("ninepart")) == 0 )
			{
				LPTSTR pstr = NULL;
				CRect rc;
				rc.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rc.right = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rc.bottom = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   

				if (m_lpBgImgH != NULL)
					m_lpBgImgH->SetNinePart(&rc);
			}
		}
		else if( _tcscmp(pstrClass, _T("item")) == 0 ) 	
		{
			if( _tcscmp(pstrName, _T("text")) == 0 ) AddString(pstrValue);
		}
		else if( _tcscmp(pstrClass, _T("ListBox")) == 0 ) 	
		{
			LPTSTR pstr = NULL;

			CSkinComboBox_ListBox *pListBox = GetListBoxWnd();
			if ( pListBox != NULL && pListBox->GetSafeHwnd() != NULL )
			{
				if( _tcscmp(pstrName, _T("selectimage")) == 0 ) pListBox->SetSelectImage(pstrValue);
				else if( _tcscmp(pstrName, _T("ninepart")) == 0 )
				{
					CRect rc;
					rc.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
					rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
					rc.right = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
					rc.bottom = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   

					if (pListBox->m_pSelectImg != NULL)
						pListBox->m_pSelectImg->SetNinePart(&rc);
				}
			}
			else if( _tcscmp(pstrName, _T("framecolor")) == 0 ) 
			{
				if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
				SetFrameColor( _tcstoul(pstrValue, &pstr, 16));
			}
		}
	}
}
