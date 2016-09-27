#include "Stdafx.h"
#include "SkinListCtrl.h"

//////////////////////////////////////////////////////////////////////////////////

//排序信息
struct tagSortInfo
{
	bool							bAscendSort;						//升序标志
	WORD							wColumnIndex;						//列表索引
	CSkinListCtrl *					pSkinListCtrl;						//列表控件
};

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSkinHeaderCtrl, CHeaderCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CSkinListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//默认属性
#define LIST_STYTE					LVS_EX_SUBITEMIMAGES|LVS_EX_FULLROWSELECT|LVS_EX_INFOTIP|LVS_EX_ONECLICKACTIVATE

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinHeaderCtrl::CSkinHeaderCtrl()
{
	//设置变量
	m_uLockCount=0L;
	m_uItemHeight=130;

	m_pBackImg = m_pPressImg = m_pGridImg = NULL;
	m_bPress = false;

	return;
}

//析构函数
CSkinHeaderCtrl::~CSkinHeaderCtrl()
{
}

//控件绑定
VOID CSkinHeaderCtrl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	////创建字体
	//CFont Font;
	//Font.CreatePointFont(m_uItemHeight,TEXT("宋体"));

	////设置字体
	//SetFont(&Font);

	return;
}

//控件消息
BOOL CSkinHeaderCtrl::OnChildNotify(UINT uMessage, WPARAM wParam, LPARAM lParam, LRESULT * pLResult)
{
	//变量定义
	NMHEADER * pNMHearder=(NMHEADER*)lParam;

	//拖动消息
	if ((pNMHearder->hdr.code==HDN_BEGINTRACKA)||(pNMHearder->hdr.code==HDN_BEGINTRACKW))
	{
		//禁止拖动
		if (pNMHearder->iItem<(INT)m_uLockCount)
		{
			*pLResult=TRUE;
			return TRUE;
		}
	}

	return __super::OnChildNotify(uMessage,wParam,lParam,pLResult);
}

//设置锁定
VOID CSkinHeaderCtrl::SetLockCount(UINT uLockCount)
{
	//设置变量
	m_uLockCount=uLockCount;

	return;
}

//设置列高
VOID CSkinHeaderCtrl::SetItemHeight(UINT uItemHeight)
{
	//设置变量
	m_uItemHeight=uItemHeight;

	////设置控件
	//if (m_hWnd!=NULL)
	//{
	//	//创建字体
	//	CFont Font;
	//	Font.CreatePointFont(m_uItemHeight,TEXT("宋体"));

	//	//设置字体
	//	SetFont(&Font);
	//}

	return;
}

//重画函数
VOID CSkinHeaderCtrl::OnPaint() 
{
	CPaintDC dc(this);

	//获取位置
	CRect rcRect;
	GetClientRect(&rcRect);

	CMemoryDC BufferDC(&dc,rcRect);

	//设置 DC
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SetTextColor(m_colNormalText);
	BufferDC.SelectObject(GetCtrlFont());

	//绘画背景
	if (m_pBackImg != NULL && !m_pBackImg->IsNull())
		m_pBackImg->Draw(&BufferDC,rcRect);

	if (m_pPressImg != NULL && !m_pPressImg->IsNull() && m_bPress)
	{
		CRect rcItem;
		GetItemRect(m_uActiveItem,&rcItem);

		m_pPressImg->Draw(&BufferDC,rcItem);
	}

	//绘画子项
	CRect rcItem;
	HDITEM HDItem;
	TCHAR szBuffer[64];
	for (INT i=0;i<GetItemCount();i++)
	{
		//构造变量
		HDItem.mask=HDI_TEXT;
		HDItem.pszText=szBuffer;
		HDItem.cchTextMax=CountArray(szBuffer);

		//获取信息
		GetItem(i,&HDItem);
		GetItemRect(i,&rcItem);

		if (m_pGridImg != NULL && !m_pGridImg->IsNull())
			m_pGridImg->DrawImage(&BufferDC,(rcItem.right-m_pGridImg->GetWidth()),(rcItem.Height()-m_pGridImg->GetHeight())/2);
	
		//绘画标题
		rcItem.DeflateRect(3,1,3,1);
		BufferDC.DrawText(szBuffer,lstrlen(szBuffer),&rcItem,DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	}

	return;
}

//背景函数
BOOL CSkinHeaderCtrl::OnEraseBkgnd(CDC * pDC) 
{
	//刷新界面
// 	Invalidate(FALSE);
// 	UpdateWindow();

	return TRUE;
}

BOOL CSkinHeaderCtrl::SetBackImage( LPCTSTR lpNormal,CONST LPRECT lprcNinePart/*=NULL*/ )
{
	UIRenderEngine->RemoveImage(m_pBackImg);

	m_pBackImg = UIRenderEngine->GetImage(lpNormal);

	if (NULL == m_pBackImg)
		return FALSE;
	else
	{
		m_pBackImg->SetNinePart(lprcNinePart);
		return TRUE;
	}
}

BOOL CSkinHeaderCtrl::SetPressImage( LPCTSTR lpNormal,CONST LPRECT lprcNinePart/*=NULL*/ )
{
	UIRenderEngine->RemoveImage(m_pPressImg);

	m_pPressImg = UIRenderEngine->GetImage(lpNormal);

	if (NULL == m_pPressImg)
		return FALSE;
	else
	{
		m_pPressImg->SetNinePart(lprcNinePart);
		return TRUE;
	}
}

BOOL CSkinHeaderCtrl::SetGridImage( LPCTSTR lpNormal)
{
	UIRenderEngine->RemoveImage(m_pGridImg);

	m_pGridImg = UIRenderEngine->GetImage(lpNormal);

	if (NULL == m_pGridImg)
		return FALSE;
	else
	{
		return TRUE;
	}
}

void CSkinHeaderCtrl::OnDestroy()
{
	__super::OnDestroy();

	UIRenderEngine->RemoveImage(m_pBackImg);
	UIRenderEngine->RemoveImage(m_pPressImg);
	UIRenderEngine->RemoveImage(m_pGridImg);
}

LRESULT CSkinHeaderCtrl::OnLayout( WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam); 
	HD_LAYOUT &hdl = *( HD_LAYOUT * ) lParam; 
	RECT *prc = hdl.prc; 
	WINDOWPOS *pwpos = hdl.pwpos; 

	int nHeight = 28;
	pwpos->cy = nHeight; 
	prc->top = nHeight; 

	return lResult; 
}

void CSkinHeaderCtrl::OnLButtonDown( UINT nFlags, CPoint point )
{
	CRect rcItem;

	for (INT i=0;i<GetItemCount();i++)
	{
		GetItemRect(i,&rcItem);

		if ( PtInRect(&rcItem,point) )
		{
			m_bPress = true;
			m_uActiveItem = i;
			break;
		}
	}

	RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	__super::OnLButtonDown(nFlags, point);
}

void CSkinHeaderCtrl::OnLButtonUp( UINT nFlags, CPoint point )
{
	m_bPress = false;

	RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	__super::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinListCtrl::CSkinListCtrl()
{
	//设置变量
	m_bAscendSort=false;
	m_nHeightItem = 0;
	m_uActiveItem = 0;
	m_pHovenImg = m_pSelectImg = NULL;
	m_pCheckImg = m_pUnCheckImg = NULL;

	m_colBack=RGB(255,255,255);
	m_iNumColumns = 0;
	return;
}

//析构函数
CSkinListCtrl::~CSkinListCtrl()
{
}

//控件绑定
VOID CSkinListCtrl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	m_SkinHeaderCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());

	return;
}

//绘画函数
VOID CSkinListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//变量定义
	CRect rcItem=lpDrawItemStruct->rcItem;
	CDC * pDC=CDC::FromHandle(lpDrawItemStruct->hDC);

	CMemoryDC BufferDC(pDC,rcItem);

	//获取属性
	INT nItemID=lpDrawItemStruct->itemID;
	INT nColumnCount=m_SkinHeaderCtrl.GetItemCount();

	//绘画区域
	CRect rcClipBox;
	BufferDC.GetClipBox(&rcClipBox);

	//设置环境
	BufferDC.SetBkMode(TRANSPARENT);
	BufferDC.SetTextColor(m_colNormalText);
	BufferDC.SelectObject(GetCtrlFont());
	BufferDC->FillSolidRect(&rcItem,m_colBack);

	//绘画焦点
	if (lpDrawItemStruct->itemState&ODS_SELECTED)
	{
		if (m_pSelectImg != NULL && !m_pSelectImg->IsNull())
			m_pSelectImg->Draw(&BufferDC,rcItem);
	}
	else if ( m_uActiveItem == nItemID )
	{
		if (m_pHovenImg != NULL && !m_pHovenImg->IsNull())
			m_pHovenImg->Draw(&BufferDC,rcItem);
	}

	//绘画子项
	for (INT i=0;i<nColumnCount;i++)
	{
		//获取位置
		CRect rcSubItem;
		GetSubItemRect(nItemID,i,LVIR_BOUNDS,rcSubItem);

		//绘画判断
		if (rcSubItem.left>rcClipBox.right) break;
		if (rcSubItem.right<rcClipBox.left) continue;

		//绘画数据
		DrawReportItem(&BufferDC,nItemID,rcSubItem,i);
	}
	
	return;
}

//建立消息
INT CSkinListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	m_SkinHeaderCtrl.SubclassWindow(GetHeaderCtrl()->GetSafeHwnd());

	return 0;
}

//绘画数据
VOID CSkinListCtrl::DrawReportItem(CDC * pDC, INT nItem, CRect & rcSubItem, INT nColumnIndex)
{
	//获取文字
	TCHAR szString[256]=TEXT("");
	GetItemText(nItem,nColumnIndex,szString,CountArray(szString));

	//绘画文字
	rcSubItem.left+=5;

	//绘制CheckButton
	if( nColumnIndex == 0 )
	{
		if ((m_pCheckImg != NULL && !m_pCheckImg->IsNull()) && (m_pUnCheckImg != NULL && !m_pUnCheckImg->IsNull()))
		{
			if( GetCheck(nItem) )
				m_pCheckImg->DrawImage(pDC,rcSubItem.left+2,rcSubItem.top+(rcSubItem.Height()-m_pCheckImg->GetHeight())/2);
			else
				m_pUnCheckImg->DrawImage(pDC,rcSubItem.left+2,rcSubItem.top+(rcSubItem.Height()-m_pUnCheckImg->GetHeight())/2);

			rcSubItem.left+=(8+m_pCheckImg->GetWidth());
		}

		CItemImgArray::iterator iter = m_ItemImgArray.begin();

		for (;iter != m_ItemImgArray.end(); ++iter )
		{
			if ( iter->nItem == nItem )
			{
				CImageEx *pImage = iter->pImage;

				if (pImage != NULL && !pImage->IsNull())
				{
					pImage->DrawImage(pDC,rcSubItem.left+2,rcSubItem.top+(rcSubItem.Height()-pImage->GetHeight())/2);
					rcSubItem.left+=(8+pImage->GetWidth());
				}
				break;
			}
		}
	}

	pDC->DrawText(szString,lstrlen(szString),&rcSubItem,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	return;
}

BOOL CSkinListCtrl::OnEraseBkgnd( CDC* pDC )
{
	CRect rcClient;
	GetClientRect(&rcClient);

	//只绘制客户区内item之外的区域
	int nWidth = 0;
	int nHeight = 0;

	DWORD dwStyle = GetWindowLong(this->m_hWnd, GWL_STYLE);
	DWORD dwSytleRE=(dwStyle&LVS_REPORT);
	DWORD dwSytleLI=(dwStyle&LVS_LIST);
	if(dwSytleRE==LVS_REPORT&&dwSytleLI!=LVS_LIST)	
	{
		int nCount = 0;
		if(m_SkinHeaderCtrl.GetSafeHwnd())
		{
			nCount= m_SkinHeaderCtrl.GetItemCount();
		}
		CRect rcHead;
		for (int i=0;i<nCount;i++)
		{
			m_SkinHeaderCtrl.GetItemRect(i,&rcHead);

			nWidth+= rcHead.Width();
			nHeight = rcHead.Height();
		}
	}

	rcClient.left = 0;
	rcClient.top = nHeight;
	
	SetBkColor(RGB(/*40,40,40*/24,36,44));//m_colorBK(RGB(/*40,40,40*/24,36,44))

	pDC->FillSolidRect(&rcClient,m_colBack);

	return TRUE;
}

void CSkinListCtrl::SetItemHeight( int nHeight )
{
	m_nHeightItem = nHeight;

	CRect rcWin;
	GetWindowRect(&rcWin);

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;

	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}

void CSkinListCtrl::MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
	if (m_nHeightItem>0)
	{
		lpMeasureItemStruct->itemHeight = m_nHeightItem;
	}
}

void CSkinListCtrl::OnDestroy()
{
	__super::OnDestroy();

	UIRenderEngine->RemoveImage(m_pHovenImg);
	UIRenderEngine->RemoveImage(m_pSelectImg);
	UIRenderEngine->RemoveImage(m_pCheckImg);
	UIRenderEngine->RemoveImage(m_pUnCheckImg);

	CItemImgArray::iterator iter = m_ItemImgArray.begin();

	for (;iter != m_ItemImgArray.end(); ++iter)
	{
		UIRenderEngine->RemoveImage(iter->pImage);
	}

	m_ItemImgArray.clear();

	RemoveScorll();
}

BOOL CSkinListCtrl::SetHovenImage( LPCTSTR lpszFileName,CONST LPRECT lprcNinePart/*=NULL*/ )
{
	UIRenderEngine->RemoveImage(m_pHovenImg);

	m_pHovenImg = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == m_pHovenImg)
		return FALSE;
	else
	{
		m_pHovenImg->SetNinePart(lprcNinePart);
		return TRUE;
	}
}

BOOL CSkinListCtrl::SetSelectImage( LPCTSTR lpszFileName,CONST LPRECT lprcNinePart/*=NULL*/ )
{
	UIRenderEngine->RemoveImage(m_pSelectImg);

	m_pSelectImg = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == m_pSelectImg)
		return FALSE;
	else
	{
		m_pSelectImg->SetNinePart(lprcNinePart);
		return TRUE;
	}
}

BOOL CSkinListCtrl::SetCheckImage( LPCTSTR lpszCheckName,LPCTSTR lpszUnCheckName )
{
	UIRenderEngine->RemoveImage(m_pCheckImg);
	UIRenderEngine->RemoveImage(m_pUnCheckImg);

	m_pCheckImg = UIRenderEngine->GetImage(lpszCheckName);
	m_pUnCheckImg = UIRenderEngine->GetImage(lpszUnCheckName);

	if (NULL == m_pCheckImg || NULL == m_pUnCheckImg)
		return FALSE;
	else
		return TRUE;
}

BOOL CSkinListCtrl::InsertImage( int nItem,LPCTSTR lpszFileName )
{
	CItemImgArray::iterator iter = m_ItemImgArray.begin();

	for (;iter != m_ItemImgArray.end(); ++iter )
	{
		if ( iter->nItem == nItem )
		{
			//当该节点存在图片时，我们更新新的图片资源
			if( iter->pImage != NULL )
			{
				UIRenderEngine->RemoveImage(iter->pImage);

				iter->pImage = UIRenderEngine->GetImage(lpszFileName);
				
				return TRUE;
			}
		}
	}

	tagItemImage ItemImage;

	//设置数据
	ItemImage.nItem = nItem;
	ItemImage.pImage = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == ItemImage.pImage)
		return FALSE;
	else
	{
		m_ItemImgArray.push_back(ItemImage);

		return TRUE;
	}
}


void CSkinListCtrl::OnMouseMove( UINT nFlags, CPoint point )
{
	CRect rcItem;

	static UINT uOldActiveItem = -1;

	for (int i=0;i<GetItemCount();i++)
	{
		GetItemRect(i,rcItem,LVIR_BOUNDS);

		if ( PtInRect(&rcItem,point) )
		{
			m_uActiveItem = i;

			if( uOldActiveItem != m_uActiveItem )
			{
				uOldActiveItem = m_uActiveItem;

				Invalidate(FALSE);
			}

			break;
		}
	}

	__super::OnMouseMove(nFlags, point);
}

void CSkinListCtrl::OnLButtonDown( UINT nFlags, CPoint point )
{
	if (m_pCheckImg != NULL && !m_pCheckImg->IsNull())
	{
		CRect rcSubItem,rcIcon;
		for (int i=0;i<GetItemCount();i++)
		{
			GetItemRect(i,rcSubItem,LVIR_BOUNDS);

			rcIcon.left = rcSubItem.left+7;
			rcIcon.top = rcSubItem.top+(rcSubItem.Height()-m_pCheckImg->GetHeight())/2;
			rcIcon.right = rcIcon.left + m_pCheckImg->GetWidth();
			rcIcon.bottom = rcIcon.top + m_pCheckImg->GetHeight();

			if ( PtInRect(&rcIcon,point) )
			{
 				SetCheck(i,!GetCheck(i));

				SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);
				SetSelectionMark(i);

				Invalidate(FALSE);
				break;
			}
		}
	}

	__super::OnLButtonDown(nFlags, point);
}

int CSkinListCtrl::InsertItem( const LVITEM* pItem )
{
	int nResult = __super::InsertItem(pItem);

	SetCheck(pItem->iItem,FALSE);

	return nResult;
}

int CSkinListCtrl::InsertItem( int nItem, LPCTSTR lpszItem )
{
	int nResult = __super::InsertItem(nItem,lpszItem);

	SetCheck(nItem,FALSE);

	return nResult;
}

int CSkinListCtrl::InsertItem( int nItem, LPCTSTR lpszItem, int nImage )
{
	int nResult = __super::InsertItem(nItem,lpszItem,nImage);

	SetCheck(nItem,FALSE);

	return nResult;
}

int CSkinListCtrl::InsertItem( UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState,UINT nStateMask, int nImage, LPARAM lParam )
{
	int nResult = __super::InsertItem(nMask,nItem,lpszItem,nState,nStateMask,nImage,lParam);

	SetCheck(nItem,FALSE);

	return nResult;
}

DWORD CSkinListCtrl::SetExtendedStyle( DWORD dwNewStyle )
{
	if ( dwNewStyle & LVS_EX_CHECKBOXES )
	{
		dwNewStyle &=~LVS_EX_CHECKBOXES;
		dwNewStyle &=~LVS_EX_GRIDLINES;
	}
	
	return __super::SetExtendedStyle(dwNewStyle);
}

void CSkinListCtrl::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	ISkinControl::SetAttribute(pstrName,pstrValue);

	if( _tcscmp(pstrName, _T("scrollimage")) == 0 )  
	{
		SetScrollImage(this,pstrValue);
	}
	else if( _tcscmp(pstrName, _T("itemheight")) == 0 )  
	{
		SetItemHeight(_ttoi(pstrValue));
	}
}

void CSkinListCtrl::ParseItem( CXmlNode *root )
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

		if( _tcscmp(pstrClass, _T("headbkimage")) == 0 ) 	
		{
			if( _tcscmp(pstrName, _T("value")) == 0 ) m_SkinHeaderCtrl.SetBackImage(pstrValue);
			else if( _tcscmp(pstrName, _T("ninepart")) == 0 )
			{
				LPTSTR pstr = NULL;
				CRect rc;
				rc.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rc.right = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rc.bottom = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   

				// 			if (m_pBackImgN != NULL)
				// 				m_pBackImgN->SetNinePart(&rc);
				if (m_SkinHeaderCtrl.m_pBackImg != NULL)
					m_SkinHeaderCtrl.m_pBackImg->SetNinePart(&rc);
			}
		}
		else if( _tcscmp(pstrClass, _T("headpressimage")) == 0 ) 	
		{
			if( _tcscmp(pstrName, _T("value")) == 0 ) m_SkinHeaderCtrl.SetPressImage(pstrValue);
			else if( _tcscmp(pstrName, _T("ninepart")) == 0 )
			{
				LPTSTR pstr = NULL;
				CRect rc;
				rc.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rc.right = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rc.bottom = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   

				if (m_SkinHeaderCtrl.m_pPressImg != NULL)
					m_SkinHeaderCtrl.m_pPressImg->SetNinePart(&rc);
			}
		}
		else if( _tcscmp(pstrClass, _T("headgirdimage")) == 0 ) 	
		{
			if( _tcscmp(pstrName, _T("value")) == 0 ) m_SkinHeaderCtrl.SetGridImage(pstrValue);
		}
		else if( _tcscmp(pstrClass, _T("hovenimage")) == 0 ) 	
		{
			if( _tcscmp(pstrName, _T("value")) == 0 ) SetHovenImage(pstrValue);
			else if( _tcscmp(pstrName, _T("ninepart")) == 0 )
			{
				LPTSTR pstr = NULL;
				CRect rc;
				rc.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rc.right = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rc.bottom = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   

				if (m_pHovenImg != NULL)
					m_pHovenImg->SetNinePart(&rc);
			}
		}
		else if( _tcscmp(pstrClass, _T("selectimage")) == 0 ) 	
		{
			if( _tcscmp(pstrName, _T("value")) == 0 ) SetSelectImage(pstrValue);
			else if( _tcscmp(pstrName, _T("ninepart")) == 0 )
			{
				LPTSTR pstr = NULL;
				CRect rc;
				rc.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
				rc.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rc.right = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
				rc.bottom = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   

				if (m_pSelectImg != NULL)
					m_pSelectImg->SetNinePart(&rc);
			}
		}
		else if( _tcscmp(pstrClass, _T("checkimage")) == 0 ) 	
		{
			if( _tcscmp(pstrName, _T("value1")) == 0 ) 
			{
				UIRenderEngine->RemoveImage(m_pCheckImg);
				m_pCheckImg = UIRenderEngine->GetImage(pstrValue);
			}
			else if( _tcscmp(pstrName, _T("value2")) == 0 )
			{
				UIRenderEngine->RemoveImage(m_pUnCheckImg);
				m_pUnCheckImg = UIRenderEngine->GetImage(pstrValue);
			}
		}
		else if( _tcscmp(pstrClass, _T("headstring")) == 0 ) 	
		{
			if( _tcscmp(pstrName, _T("value")) == 0 )
			{
				SetHeadings(pstrValue);
			}

		}
	}
}

BOOL CSkinListCtrl::CreateControl( CWnd* pParentWnd )
{
	if( !Create(WS_CHILD|WS_VISIBLE|LVS_REPORT | LVS_OWNERDRAWFIXED | LVS_ALIGNLEFT | WS_BORDER | WS_TABSTOP ,CRect(0,0,0,0),pParentWnd,0) )
		return FALSE;

	SetFont(CFont::FromHandle(UIRenderEngine->GetDeaultFont()));

	m_pOwnWnd = this;

	return TRUE;
}

BOOL CSkinListCtrl::SetHeadings(const CString& strHeadings)
{
	int iStart = 0;

	if(strHeadings.IsEmpty())
		return false;
	for( ;; )
	{
		const int iComma = strHeadings.Find( _T(','), iStart );

		if( iComma == -1 )
			break;

		const CString strHeading = strHeadings.Mid( iStart, iComma - iStart );

		iStart = iComma + 1;

		int iSemiColon = strHeadings.Find( _T(';'), iStart );

		if( iSemiColon == -1 )
			iSemiColon = strHeadings.GetLength();

		const int iWidth = _ttoi( strHeadings.Mid( iStart, iSemiColon - iStart ) );

		iStart = iSemiColon + 1;

		if( InsertColumn( m_iNumColumns++, strHeading, LVCFMT_CENTER, iWidth ) == -1 )
			return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
