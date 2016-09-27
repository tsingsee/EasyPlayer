// SkinTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "Resource.h"
#include "SkinTreeCtrl.h"


// CSkinTreeCtrl

IMPLEMENT_DYNAMIC(CSkinTreeCtrl, CTreeCtrl)

CSkinTreeCtrl::CSkinTreeCtrl()
{
	m_hItemMouseHover=NULL;
	m_colBack = RGB(255,255,255);
	m_uCursorRes = 0;

	m_pImageButton = NULL;
	m_pHovenImg = m_pPressImg = NULL;
	m_MapItemInfo.clear();

	m_colHoven = RGB(255,0,0);
	m_colPress = RGB(253,231,161);
}

CSkinTreeCtrl::~CSkinTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkinTreeCtrl, CTreeCtrl)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CSkinTreeCtrl 消息处理程序



void CSkinTreeCtrl::OnPaint()
{
	CTreeCtrl::OnPaint();
	CPaintDC dc(this); // device context for painting
	
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	CMemoryDC BufferDC(&dc,rcClient);

	//剪切位置
	CRect rcClipBox;
	BufferDC.GetClipBox(&rcClipBox);

	//绘制背景
	BufferDC.FillSolidRect(rcClient,m_colBack);
	DrawParentWndBg(GetSafeHwnd(),BufferDC->GetSafeHdc());

	DrawTreeItem(&BufferDC,rcClient,rcClipBox);
}

VOID CSkinTreeCtrl::DrawTreeItem( CDC * pDC, CRect & rcClient, CRect & rcClipBox )
{
	//首项判断
	HTREEITEM hItemCurrent=GetFirstVisibleItem();
	if (hItemCurrent==NULL) return;

	//获取属性
	UINT uTreeStyte = GetStyle();

	//绘画子项
	do
	{
		//变量定义
		CRect rcItem;
		CRect rcRect;

		//获取状态
		HTREEITEM hParent=GetParentItem(hItemCurrent);
		UINT uItemState=GetItemState(hItemCurrent,TVIF_STATE);

		//获取属性
		bool bDrawChildren=(ItemHasChildren(hItemCurrent)==TRUE);
		bool bDrawSelected=(uItemState&TVIS_SELECTED)&&((this==GetFocus())||(uTreeStyte&TVS_SHOWSELALWAYS));

		//获取区域
		if (GetItemRect(hItemCurrent,rcItem,TRUE))
		{
			//绘画过滤
			if (rcItem.top>=rcClient.bottom) break;
			if (rcItem.top>=rcClipBox.bottom) continue;

			//设置位置
			rcRect.left=0;
			rcRect.top=rcItem.top+1;
			rcRect.bottom=rcItem.bottom;
			rcRect.right=rcClient.Width();

			//绘画选择
			if (bDrawSelected)
			{
				if (m_pPressImg != NULL && !m_pPressImg->IsNull())
					m_pPressImg->Draw(pDC,rcRect);
				else
					pDC->FillSolidRect(&rcRect,m_colPress);
			}

			//绘画经过
 			if ((bDrawSelected==false)&&(m_hItemMouseHover==hItemCurrent))
 			{
				if (m_pHovenImg != NULL && !m_pHovenImg->IsNull())
					m_pHovenImg->Draw(pDC,rcRect);
				else
 					pDC->FillSolidRect(&rcRect,m_colHoven);
 			}

			//绘制箭头
 			if (bDrawChildren && (uTreeStyte&TVS_HASBUTTONS) )
 			{
				if (m_pImageButton != NULL && !m_pImageButton->IsNull())
				{
					//计算位置
					INT nXPos=rcItem.left-m_pImageButton->GetWidth()/2;
					INT nYPos=rcItem.top+1+(rcItem.Height()-m_pImageButton->GetHeight())/2;

					//绘画图标
					INT nIndex=(uItemState&TVIS_EXPANDED)?1L:0L;
					m_pImageButton->DrawImage(pDC,nXPos,nYPos,m_pImageButton->GetWidth()/2,m_pImageButton->GetHeight(),nIndex*m_pImageButton->GetWidth()/2,0);
				
					rcItem.left += m_pImageButton->GetWidth();
					rcItem.right += m_pImageButton->GetWidth();;
				}

 			}

			//绘制列表
			DrawListImage(pDC,rcItem,hItemCurrent,bDrawSelected);	

			//绘制文本
			DrawItemString(pDC,rcItem,hItemCurrent,bDrawSelected);
		}
	} while ((hItemCurrent=GetNextVisibleItem(hItemCurrent))!= NULL);
}

VOID CSkinTreeCtrl::DrawListImage( CDC * pDC, CRect rcRect, HTREEITEM hTreeItem, bool bSelected )
{
	CMapItemInfo::iterator iter= m_MapItemInfo.find(hTreeItem);
	if( iter == m_MapItemInfo.end() )
	{
		CImageList *pImageList = GetImageList(TVSIL_NORMAL);
		if( pImageList == NULL ) return;

		//获取属性
		INT nImage,nSelectedImage;
		GetItemImage(hTreeItem,nImage,nSelectedImage);

		//获取信息
		IMAGEINFO ImageInfo;
		pImageList->GetImageInfo(bSelected?nSelectedImage:nImage,&ImageInfo);

		//绘画图标
		INT nImageWidth=ImageInfo.rcImage.right-ImageInfo.rcImage.left;
		INT nImageHeight=ImageInfo.rcImage.bottom-ImageInfo.rcImage.top;

		pImageList->Draw(pDC,bSelected?nSelectedImage:nImage,CPoint(rcRect.left-nImageWidth-3,rcRect.top+(rcRect.Height()-nImageHeight)/2+1),ILD_TRANSPARENT);
	}
	else
	{
		CImageEx *pImage = iter->second.pImage;
		pImage->DrawImage(pDC,rcRect.left-pImage->GetWidth()-3,rcRect.top+(rcRect.Height()-pImage->GetHeight())/2+1);
	}
}

VOID CSkinTreeCtrl::DrawItemString( CDC * pDC, CRect rcRect, HTREEITEM hTreeItem, bool bSelected )
{
	//设置环境
	pDC->SelectObject(GetCtrlFont());
	pDC->SetBkMode(TRANSPARENT);

	if( !IsWindowEnabled() ) pDC->SetTextColor(m_colDisableText);
	else pDC->SetTextColor(bSelected?m_colSelectText:m_colNormalText);

	//绘画字体
	rcRect.right += 5;

	CString strString=GetItemText(hTreeItem);
	pDC->DrawText(strString,rcRect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	return;
}

BOOL CSkinTreeCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//获取光标
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//子项测试
	HTREEITEM hItemMouseHover=HitTest(MousePoint);

	//重画判断
	if ((hItemMouseHover!=NULL)&&(hItemMouseHover!=m_hItemMouseHover))
	{
		//设置变量
		m_hItemMouseHover=hItemMouseHover;

		//重画界面
		Invalidate(FALSE);
	}

	//设置光标
	if (hItemMouseHover!=NULL)
	{
		if( m_uCursorRes )
		{
			//SetCursor(LoadCursor(GetModuleHandle(SKINUI_DLL_NAME),MAKEINTRESOURCE(m_uCursorRes)));
			SetCursor(AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(m_uCursorRes)));
		}

		return true;
	}

	return __super::OnSetCursor(pWnd, nHitTest, message);
}

void CSkinTreeCtrl::SetCursorStyle(UINT uID)
{
	m_uCursorRes = uID;
}
BOOL CSkinTreeCtrl::SetButtonImage( LPCTSTR lpszFileName )
{
	UIRenderEngine->RemoveImage(m_pImageButton);

	m_pImageButton = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == m_pImageButton)
		return FALSE;
	else
		return TRUE;
}

void CSkinTreeCtrl::OnDestroy()
{
	__super::OnDestroy();

	UIRenderEngine->RemoveImage(m_pImageButton);

	CMapItemInfo::iterator iter= m_MapItemInfo.begin();

	for (;iter!= m_MapItemInfo.end();++iter)
	{
		UIRenderEngine->RemoveImage(iter->second.pImage);
	}
}

BOOL CSkinTreeCtrl::InsertImage( HTREEITEM hTreeItem,LPCTSTR lpszFileName )
{
	CMapItemInfo::iterator iter= m_MapItemInfo.find(hTreeItem);

	if ( iter != m_MapItemInfo.end())
	{
		CImageEx *pImage = UIRenderEngine->GetImage(lpszFileName);

		if (NULL == pImage)
			return FALSE;
		else
		{
			iter->second.pImage = pImage;
			return TRUE;
		}
	}
	else
	{
		tagItemInfo _ItemInfo;
		_ItemInfo.hTreeItem = hTreeItem;
		_ItemInfo.pImage = UIRenderEngine->GetImage(lpszFileName);

		if (NULL == _ItemInfo.pImage) return FALSE;

		m_MapItemInfo.insert(pair<HTREEITEM,tagItemInfo>(hTreeItem,_ItemInfo));

		return TRUE;
	}
}

BOOL CSkinTreeCtrl::SetHovenImage( LPCTSTR lpszFileName,CONST LPRECT lprcNinePart/*=NULL*/ )
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

BOOL CSkinTreeCtrl::SetPressImage( LPCTSTR lpszFileName,CONST LPRECT lprcNinePart/*=NULL*/ )
{
	UIRenderEngine->RemoveImage(m_pPressImg);

	m_pPressImg = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == m_pPressImg)
		return FALSE;
	else
	{
		m_pPressImg->SetNinePart(lprcNinePart);
		return TRUE;
	}
}

void CSkinTreeCtrl::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	ISkinControl::SetAttribute(pstrName,pstrValue);
	if( _tcscmp(pstrName, _T("scrollimage")) == 0 ) 
	{
		SetScrollImage(this,pstrValue);
	}
	else if( _tcscmp(pstrName, _T("bkcolor")) == 0 ) 
	{
		LPTSTR pstr = NULL;
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		SetBackColor(_tcstoul(pstrValue, &pstr, 16)) ;
	}
	else if( _tcscmp(pstrName, _T("buttonimage")) == 0 ) 
	{
		SetButtonImage(pstrValue) ;
	}
	else if( _tcscmp(pstrName, _T("hoverimage")) == 0 ) 
	{
		SetHovenImage(pstrValue, CRect(2,2,2,2)) ;
	}
	else if( _tcscmp(pstrName, _T("pressimage")) == 0 ) 
	{
		SetPressImage(pstrValue, CRect(2,2,2,2)) ;
	}

}

BOOL CSkinTreeCtrl::CreateControl( CWnd* pParentWnd )
{
	if( !Create(WS_CHILD|WS_VISIBLE|TVS_HASLINES | WS_BORDER | WS_HSCROLL | WS_TABSTOP,CRect(0,0,0,0),pParentWnd,0) )
		return FALSE;

	m_pOwnWnd = this;

	return TRUE;
}

void CSkinTreeCtrl::ParseItem( CXmlNode *root )
{

}