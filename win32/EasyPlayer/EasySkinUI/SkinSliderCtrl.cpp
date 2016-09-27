// SkinSliderCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "SkinUI.h"
#include "SkinSliderCtrl.h"

// CSkinSliderCtrl

IMPLEMENT_DYNAMIC(CSkinSliderCtrl, CSliderCtrl)

CSkinSliderCtrl::CSkinSliderCtrl()
{
	m_pBackImg = m_pThumImg = m_pDisImg = m_pBtImg = m_pTraceImg = NULL;
	m_pAImg = NULL; 
	m_pBImg = NULL;

	m_bPress = m_bHover = m_bMouseTracking = FALSE;
	m_bDragging = false;
	m_rcThumRect.SetRect(0,0,0,0);
	m_szThum.SetSize(0,0);
	m_bLoopArrow = FALSE;
	m_nArrowLeftPos = -1;
	m_nArrowRightPos = -1;
}

CSkinSliderCtrl::~CSkinSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkinSliderCtrl, CSliderCtrl)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_SIZE()

END_MESSAGE_MAP()

BOOL CSkinSliderCtrl::SetBackImage( LPCTSTR lpszFileName,bool bFixed/* = true*/ )
{
	UIRenderEngine->RemoveImage(m_pBackImg);

	m_pBackImg = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == m_pBackImg)
		return FALSE;
	else
	{
		if(bFixed) SetWindowPos(NULL,0,0,m_pBackImg->GetWidth(),m_pBackImg->GetHeight(),SWP_NOMOVE);
		
		return TRUE;
	}
}

BOOL CSkinSliderCtrl::SetThumImage( LPCTSTR lpszFileName )
{
	UIRenderEngine->RemoveImage(m_pThumImg);

	m_pThumImg = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == m_pThumImg)
		return FALSE;
	else
	{
		m_szThum.SetSize(m_pThumImg->GetWidth(),m_pThumImg->GetHeight());
		m_rcThumRect.SetRect(0,0,m_szThum.cx,m_szThum.cy);
		SetThumbRect();
		SetChannelRect();
		return TRUE;
	}
}

BOOL CSkinSliderCtrl::SetArrowAImage( LPCTSTR lpszFileName )
{
	UIRenderEngine->RemoveImage(m_pAImg);

	m_pAImg = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == m_pAImg)
		return FALSE;
	else
	{
		return TRUE;
	}
}

BOOL CSkinSliderCtrl::SetArrowBImage( LPCTSTR lpszFileName )
{
	UIRenderEngine->RemoveImage(m_pBImg);

	m_pBImg = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == m_pBImg)
		return FALSE;
	else
	{
		return TRUE;
	}
}

BOOL CSkinSliderCtrl::SetDisImage( LPCTSTR lpszFileName )
{
	UIRenderEngine->RemoveImage(m_pDisImg);

	m_pDisImg = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == m_pDisImg)
		return FALSE;
	else
		return TRUE;
}

BOOL CSkinSliderCtrl::SetTraceImage( LPCTSTR lpszFileName,CONST LPRECT lprcNinePart/*=NULL*/ )
{
	UIRenderEngine->RemoveImage(m_pTraceImg);

	m_pTraceImg = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == m_pTraceImg)
		return FALSE;
	else
	{
		m_pTraceImg->SetNinePart(lprcNinePart);
		return TRUE;
	}
}

BOOL CSkinSliderCtrl::SetButtonImage( LPCTSTR lpszFileName )
{
	UIRenderEngine->RemoveImage(m_pBtImg);

	m_pBtImg = UIRenderEngine->GetImage(lpszFileName);

	if (NULL == m_pBtImg)
		return FALSE;
	else
	{
		m_szThum.SetSize(m_pBtImg->GetWidth()/3,m_pBtImg->GetHeight());
		m_rcThumRect.SetRect(0,0,m_szThum.cx,m_szThum.cy);
		SetThumbRect();
		SetChannelRect();
		return TRUE;
	}
}

// CSkinSliderCtrl 消息处理程序
void CSkinSliderCtrl::OnDestroy()
{
	CSliderCtrl::OnDestroy();

	UIRenderEngine->RemoveImage(m_pBackImg);
	UIRenderEngine->RemoveImage(m_pThumImg);
	UIRenderEngine->RemoveImage(m_pDisImg);
	UIRenderEngine->RemoveImage(m_pBtImg);
	UIRenderEngine->RemoveImage(m_pTraceImg);
}

void CSkinSliderCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient;
	GetClientRect(&rcClient);

	CMemoryDC MemDC(&dc, rcClient);

	//绘制背景
	DrawParentWndBg(GetSafeHwnd(),MemDC->GetSafeHdc());
	
	//  [3/9/2016 Dingshuai]
	if (m_pBackImg != NULL && !m_pBackImg->IsNull())
	{
		m_pBackImg->DrawImage(&MemDC,0,0,rcClient.Width(),rcClient.Height());
	}

	//绘制轨迹
	if ( !IsWindowEnabled() )
	{
		if (m_pDisImg != NULL && !m_pDisImg->IsNull())
		{
			m_pDisImg->DrawImage(&MemDC,0,0);
		}		
	}
	else
	{
		if (m_pBackImg != NULL && !m_pBackImg->IsNull())
		{
			m_pBackImg->DrawImage(&MemDC,0,0);
		}
	}

	//绘制痕迹
	if (m_pTraceImg != NULL && !m_pTraceImg->IsNull())
	{
		m_pTraceImg->DrawImage/*DrawExtrude*/(&MemDC,CRect(0, 0,m_rcThumRect.left,m_pTraceImg->GetHeight()));
	}

	if (m_bLoopArrow)
	{
		//绘制A,B点
		if (m_pAImg != NULL && !m_pAImg->IsNull())
		{
			m_pAImg->DrawImage(&MemDC,m_nArrowLeftPos , -1);
		}							  
		if (m_pBImg != NULL && !m_pBImg->IsNull())
		{
			m_pBImg->DrawImage(&MemDC,m_nArrowRightPos, 0);
		}
	}

	//绘制滑块
	if (m_pThumImg != NULL && !m_pThumImg->IsNull())
	{
		m_pThumImg->DrawImage(&MemDC,m_rcThumRect.left, m_rcThumRect.top);
	}

	//绘制按钮
	if (m_pBtImg != NULL && !m_pBtImg->IsNull())
	{
		if( m_bPress )
			m_pBtImg->DrawImage(&MemDC,m_rcThumRect.left, m_rcThumRect.top,m_szThum.cx,m_szThum.cy,m_szThum.cx*2,0,m_pBtImg->GetWidth()/3,m_pBtImg->GetHeight());
		else if ( m_bHover )
			m_pBtImg->DrawImage(&MemDC,m_rcThumRect.left, m_rcThumRect.top,m_szThum.cx,m_szThum.cy,m_szThum.cx,0,m_pBtImg->GetWidth()/3,m_pBtImg->GetHeight());
		else 
			m_pBtImg->DrawImage(&MemDC,m_rcThumRect.left, m_rcThumRect.top,m_szThum.cx,m_szThum.cy,0,0,m_pBtImg->GetWidth()/3,m_pBtImg->GetHeight());
	}
}

void CSkinSliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_bDragging )
	{
		int nMax = 0;
		int nMin = 0;
		GetRange(nMin,nMax);
		
		int nPos = 0;
		if ( GetStyle() & TBS_VERT )
			nPos = (nMax - nMin)*(point.y)/m_rcChannelRect.Height(); 
		else 
			nPos = (nMax - nMin)*(point.x)/m_rcChannelRect.Width(); 

		SetPos(nPos);

		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

		if (GetParent() && ::IsWindow(GetParent()->m_hWnd))
		{
			GetParent()->SendMessage(WM_HSCROLL, MAKELONG(SB_THUMBTRACK, GetPos()), 
				(LPARAM)m_hWnd);
		}
	}

	//按钮事件
	if ( PtInRect(m_rcThumRect,point) && !m_bMouseTracking )
	{
		TrackMouseLeave(GetSafeHwnd());

		m_bMouseTracking = TRUE;
		m_bHover = TRUE;
		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}

	__super::OnMouseMove(nFlags, point);
}

//鼠标离开消息
LRESULT CSkinSliderCtrl::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bMouseTracking = FALSE;
	m_bHover = FALSE;
	RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

	return 0;
}

void CSkinSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ( PtInRect(&m_rcThumRect,point) )
	{
		m_bDragging = true;
		m_bPress = TRUE;
		SetCapture();

		RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	}
 	else
 	{
		CRect rcClient;
		GetClientRect(&rcClient);
 		if ( PtInRect(&rcClient,point) )
 		{
 			int nMax = 0;  
 			int nMin = 0;  
 			GetRange(nMin,nMax);  
 
			int nPos = 0;

			if ( GetStyle() & TBS_VERT )
 				nPos = (nMax - nMin)*(point.y - m_rcChannelRect.top)/m_rcChannelRect.Height(); 
			else 
				nPos = (nMax - nMin)*(point.x - m_rcChannelRect.left)/m_rcChannelRect.Width(); 
 
 			OutputString(TEXT("Down:%d\n"),nPos);
 			nPos += nMin;
 			SetPos(nPos); 
 			RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);

			if (GetParent() && ::IsWindow(GetParent()->m_hWnd))
			{
				GetParent()->SendMessage(WM_HSCROLL, MAKELONG(SB_THUMBTRACK, GetPos()), 
					(LPARAM)m_hWnd);
			}

 			return;
 		}
 	}

	__super::OnLButtonDown(nFlags, point);
}

void CSkinSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if ( PtInRect(&m_rcThumRect,point) && m_bPress) m_bPress = FALSE;

	m_bDragging = false;
	ReleaseCapture();
	RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
	
	__super::OnLButtonUp(nFlags, point);
}

BOOL CSkinSliderCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CSkinSliderCtrl::SetThumbRect(int nPos)
{	
	CRect rcClient;
	GetClientRect(&rcClient);

	//判断是否横竖控件
	if ( GetStyle() & TBS_VERT )
	{
		m_rcThumRect.left = (rcClient.Width()-m_szThum.cx)/2;
		m_rcThumRect.top = nPos;
	}
	else
	{
		m_rcThumRect.left = nPos;
		m_rcThumRect.top = (rcClient.Height()-m_szThum.cy)/2;
	}

	m_rcThumRect.right = m_rcThumRect.left+m_szThum.cx;
	m_rcThumRect.bottom = m_rcThumRect.top+m_szThum.cy;
}

void CSkinSliderCtrl::SetPos( int nPos )
{
	int nMin,nMax;
	GetRange(nMin,nMax);

	//重新定位，防止超出客户区
	if( nPos <= nMin ) nPos = nMin;
	if ( nPos >= nMax ) nPos = nMax;
	
	__super::SetPos(nPos);

	int nPosNew = 0;

	if ( GetStyle() & TBS_VERT )

		nPosNew = m_rcChannelRect.Height()*nPos/(nMax-nMin);
	else 
		nPosNew = m_rcChannelRect.Width()*nPos/(nMax-nMin);
	
	if( nPosNew<0 ) nPosNew = 0;

	SetThumbRect(nPosNew);

	RedrawWindow(NULL,NULL,RDW_FRAME|RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW);
}

void CSkinSliderCtrl::SetChannelRect()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置轨迹区域
	if ( GetStyle() & TBS_VERT )
		m_rcChannelRect.SetRect(0,m_szThum.cy/2,rcClient.Width(),rcClient.Height()-m_szThum.cy/2);
	else 
		m_rcChannelRect.SetRect(m_szThum.cx/2,0,rcClient.Width()-m_szThum.cx/2,rcClient.Height());
}

void CSkinSliderCtrl::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	ISkinControl::SetAttribute(pstrName,pstrValue);

	if( _tcscmp(pstrName, _T("bkimage")) == 0 ) 
	{
		SetBackImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("thumimage")) == 0 ) 
	{
		SetThumImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("buttonimage")) == 0 ) 
	{
		SetButtonImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("traceimage")) == 0 ) 
	{
		SetTraceImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("disimage")) == 0 ) 
	{
		SetDisImage(pstrValue);
	}
	else if( _tcscmp(pstrName, _T("arrowAimage")) == 0 ) 
	{
		SetArrowAImage(pstrValue);
	}	
	else if( _tcscmp(pstrName, _T("arrowBimage")) == 0 ) 
	{
		SetArrowBImage(pstrValue);
	}	
	else if( _tcscmp(pstrName, _T("range")) == 0 ) 
	{
		LPTSTR pstr = NULL;
		int cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		int cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 

		SetRange(cx,cy);
	}
	else if( _tcscmp(pstrName, _T("vertical")) == 0 ) 
	{
		if( _tcscmp(pstrValue, _T("true")) == 0 ) ModifyStyle(0,TBS_VERT);
	}
	else if( _tcscmp(pstrName, _T("value")) == 0 ) 
	{
		SetPos(_ttoi(pstrValue));
	}
	else if( _tcscmp(pstrName, _T("tic")) == 0 ) 
	{
		SetTic(_ttoi(pstrValue));
	}
	else if( _tcscmp(pstrName, _T("ticfreq")) == 0 ) 
	{
		SetTicFreq(_ttoi(pstrValue));
	}
}

BOOL CSkinSliderCtrl::CreateControl( CWnd* pParentWnd )
{
	if( !Create(WS_CHILD|WS_VISIBLE|TBS_BOTH | TBS_NOTICKS | WS_TABSTOP,CRect(0,0,0,0),pParentWnd,0) )
		return FALSE;

	m_pOwnWnd = this;

	return TRUE;
}

void CSkinSliderCtrl::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	SetChannelRect();
	SetPos(GetPos());
}

void CSkinSliderCtrl::SetLeftArrowPos()
{	
	double dPixels, dMax, dInterval, dPos;

	int nMax = 0;
	int nMin = 0;
	GetRange(nMin,nMax);
	dMax = nMax-nMin;
	dPos = GetPos();
	CRect rcClient;
	GetClientRect(&rcClient);

	if (! (GetStyle() & TBS_VERT ))
	{
		// if 没有ARROW
		dPixels   = m_rcChannelRect.Width();
		dInterval = dPixels / dMax;
		double dThumbLeft = dPos * dInterval + 0.5;
		m_nArrowLeftPos =  (int)dThumbLeft ;// if no arrow (+ m_nBitmapWidth)
		if (m_nArrowRightPos==-1)
		{
			m_nArrowRightPos = rcClient.Width();
		}
		if (m_nArrowLeftPos>m_nArrowRightPos)
		{
			m_nArrowLeftPos = m_nArrowRightPos;//*+m_nBitmapWidth
			m_nArrowRightPos = (int)dThumbLeft + m_szThum.cx;
		}
		m_bLoopArrow = TRUE;
	}
	Invalidate();
}

void CSkinSliderCtrl::SetRightArrowPos()
{
	double dPixels, dMax, dInterval, dPos;

	int nMax = 0;
	int nMin = 0;
	GetRange(nMin,nMax);
	dMax = nMax-nMin;
	dPos = GetPos();
	CRect rcClient;
	GetClientRect(&rcClient);

	if ( ! (GetStyle() & TBS_VERT ) )
	{
		// if 没有ARROW
		dPixels   = m_rcChannelRect.Width();
		dInterval = dPixels / dMax;
		double dThumbLeft = dPos * dInterval + 0.5;
		m_nArrowRightPos =  (int)dThumbLeft + m_szThum.cx/2;// if no arrow (+ m_nBitmapWidth)
		if (m_nArrowLeftPos==-1)
		{
			m_nArrowLeftPos = rcClient.left;
		}
		if (m_nArrowRightPos<m_nArrowLeftPos)
		{
			//判断不准确
			m_nArrowRightPos = m_nArrowLeftPos + m_szThum.cx;
			m_nArrowLeftPos = (int)dThumbLeft;
		}
		m_bLoopArrow = TRUE;
	}
	Invalidate();
}

void CSkinSliderCtrl::StopLoopArrow()
{
	m_bLoopArrow = FALSE;
	Invalidate();
}

void CSkinSliderCtrl::StartLoopArrow()
{
	m_bLoopArrow = TRUE;
	Invalidate();
}