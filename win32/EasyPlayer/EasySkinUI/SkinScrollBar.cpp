//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkinScrollBar.h"

#define TIMERID_NOTIFY	100
#define TIMERID_DELAY	200
#define TIME_DELAY		500
#define TIME_INTER		100

#define THUMB_BORDER	3
#define THUMB_MINSIZE	(THUMB_BORDER*2)
/////////////////////////////////////////////////////////////////////////////
// CSkinScrollBar

CSkinScrollBar::CSkinScrollBar()
{
	m_hBmp=NULL;
	m_bDrag=FALSE;
	memset(&m_si,0,sizeof(SCROLLINFO));
	m_si.nTrackPos=-1;
	m_uClicked=-1;
	m_bNotify=FALSE;
	m_uHtPrev=-1;
	m_bPause=FALSE;
	m_bTrace=FALSE;
}

CSkinScrollBar::~CSkinScrollBar()
{
}


BEGIN_MESSAGE_MAP(CSkinScrollBar, CScrollBar)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CSkinScrollBar::IsVertical()
{
	DWORD dwStyle=GetStyle();
	return dwStyle&SBS_VERT;
}

UINT CSkinScrollBar::HitTest(CPoint pt)
{
	int nTestPos=pt.y;
	if(!IsVertical()) nTestPos=pt.x;
	if(nTestPos<0) return -1;
	SCROLLINFO si=m_si;
	int nInterHei=m_nHei-2*m_nWid;
	if(nInterHei<0) nInterHei=0;
	int	nSlideHei=si.nPage*nInterHei/(si.nMax-si.nMin+1);
	if(nSlideHei<THUMB_MINSIZE) nSlideHei=THUMB_MINSIZE;
	if(nInterHei<THUMB_MINSIZE) nSlideHei=0;
	int nEmptyHei=nInterHei-nSlideHei;

	int nArrowHei=(nInterHei==0)?(m_nHei/2):m_nWid;
	int nBottom=0;
	int nSegLen=nArrowHei;
	nBottom+=nSegLen;
	UINT uHit=SB_LINEUP;
	if(nTestPos<nBottom) goto end;
	if(si.nTrackPos==-1) si.nTrackPos=si.nPos;
	uHit=SB_PAGEUP;
	if((si.nMax-si.nMin-si.nPage+1)==0)
		nSegLen=nEmptyHei/2;
	else
		nSegLen=nEmptyHei*si.nTrackPos/(si.nMax-si.nMin-si.nPage+1);
	nBottom+=nSegLen;
	if(nTestPos<nBottom) goto end;
	nSegLen=nSlideHei;
	nBottom+=nSegLen;
	uHit=SB_THUMBTRACK;
	if(nTestPos<nBottom) goto end;
	nBottom=m_nHei-nArrowHei;
	uHit=SB_PAGEDOWN;
	if(nTestPos<nBottom) goto end;
	uHit=SB_LINEDOWN;
end:
	return uHit;
}

void CSkinScrollBar::SetBitmap(HBITMAP hBmp)
{
	ASSERT(m_hWnd);
	m_hBmp=hBmp;
	BITMAP bm;
	GetObject(hBmp,sizeof(bm),&bm);
	m_nWid=bm.bmWidth/9;
	m_nFrmHei=bm.bmHeight/3;
	CRect rc;
	GetWindowRect(&rc);
	GetParent()->ScreenToClient(&rc);
	if(IsVertical())
	{
		rc.right=rc.left+m_nWid;
	}else
	{
		rc.bottom=rc.top+m_nWid;
	}
	MoveWindow(&rc);
}

void CSkinScrollBar::OnSize(UINT nType, int cx, int cy) 
{
	CScrollBar::OnSize(nType, cx, cy);
	m_nHei=IsVertical()?cy:cx;
}

void CSkinScrollBar::OnPaint() 
{
	CPaintDC dc(this);
	if(!m_hBmp) return;
	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	HGDIOBJ hOldBmp=::SelectObject(memdc,m_hBmp);

	RECT rcSour={0,0,m_nWid,m_nWid};
	if(!IsVertical()) OffsetRect(&rcSour,m_nWid*4,0);
	RECT rcDest;
	rcDest=GetRect(SB_LINEUP);
	if((rcDest.right-rcDest.left != rcSour.right-rcSour.left)
		||(rcDest.bottom-rcDest.top != rcSour.bottom-rcSour.top))
		dc.StretchBlt(rcDest.left,rcDest.top,rcDest.right-rcDest.left,rcDest.bottom-rcDest.top,
		&memdc,
		rcSour.left,rcSour.top,rcSour.right-rcSour.left,rcSour.bottom-rcSour.top,
		SRCCOPY);
	else
		dc.BitBlt(rcDest.left,rcDest.top,m_nWid,m_nWid,&memdc,rcSour.left,rcSour.top,SRCCOPY);
	rcDest=GetRect(SB_LINEDOWN);
	OffsetRect(&rcSour,m_nWid,0);
	if((rcDest.right-rcDest.left != rcSour.right-rcSour.left)
		||(rcDest.bottom-rcDest.top != rcSour.bottom-rcSour.top))
		dc.StretchBlt(rcDest.left,rcDest.top,rcDest.right-rcDest.left,rcDest.bottom-rcDest.top,
		&memdc,
		rcSour.left,rcSour.top,rcSour.right-rcSour.left,rcSour.bottom-rcSour.top,
		SRCCOPY);
	else
		dc.BitBlt(rcDest.left,rcDest.top,m_nWid,m_nWid,&memdc,rcSour.left,rcSour.top,SRCCOPY);
	rcDest=GetRect(SB_THUMBTRACK);
	OffsetRect(&rcSour,m_nWid,0);
	DrawThumb(&dc,&rcDest,&memdc,&rcSour);
	OffsetRect(&rcSour,m_nWid,0);
	rcDest=GetRect(SB_PAGEUP);
	TileBlt(&dc,&rcDest,&memdc,&rcSour);
	rcDest=GetRect(SB_PAGEDOWN);
	TileBlt(&dc,&rcDest,&memdc,&rcSour);

	::SelectObject(memdc,hOldBmp);
}

RECT CSkinScrollBar::GetImageRect(UINT uSBCode,int nState)
{
	int nIndex=0;
	switch(uSBCode)
	{
	case SB_LINEUP:nIndex=0;break;
	case SB_PAGEUP:nIndex=3;break;
	case SB_THUMBTRACK:nIndex=2;break;
	case SB_PAGEDOWN:nIndex=3;break;
	case SB_LINEDOWN:nIndex=1;break;
	}
	if(!IsVertical())nIndex+=4;
	RECT rcRet={m_nWid*nIndex,m_nWid*nState,m_nWid*(nIndex+1),m_nWid*(nState+1)};
	return rcRet;
}

RECT CSkinScrollBar::GetRect(UINT uSBCode)
{
	SCROLLINFO si=m_si;
	if(si.nTrackPos==-1) si.nTrackPos=si.nPos;
	int nInterHei=m_nHei-2*m_nWid;
	if(nInterHei<0) nInterHei=0;
	int	nSlideHei=si.nPage*nInterHei/(si.nMax-si.nMin+1);
	if(nSlideHei<THUMB_MINSIZE) nSlideHei=THUMB_MINSIZE;
	if(nInterHei<THUMB_MINSIZE) nSlideHei=0;
	if((si.nMax-si.nMin-si.nPage+1)==0) nSlideHei=0;
	int nEmptyHei=nInterHei-nSlideHei;
	int nArrowHei=m_nWid;
	if(nInterHei==0) nArrowHei=m_nHei/2;
	RECT rcRet={0,0,m_nWid,nArrowHei};
	if(uSBCode==SB_LINEUP) goto end;
	rcRet.top=rcRet.bottom;
	if((si.nMax-si.nMin-si.nPage+1)==0)
		rcRet.bottom+=nEmptyHei/2;
	else
		rcRet.bottom+=nEmptyHei*si.nTrackPos/(si.nMax-si.nMin-si.nPage+1);
	if(uSBCode==SB_PAGEUP) goto end;
	rcRet.top=rcRet.bottom;
	rcRet.bottom+=nSlideHei;
	if(uSBCode==SB_THUMBTRACK) goto end;
	rcRet.top=rcRet.bottom;
	rcRet.bottom=m_nHei-nArrowHei;
	if(uSBCode==SB_PAGEDOWN) goto end;
	rcRet.top=rcRet.bottom;
	rcRet.bottom=m_nHei;
	if(uSBCode==SB_LINEDOWN) goto end;
end:
	if(!IsVertical())
	{
		int t=rcRet.left;
		rcRet.left=rcRet.top;
		rcRet.top=t;
		t=rcRet.right;
		rcRet.right=rcRet.bottom;
		rcRet.bottom=t;
	}
	return rcRet;
}

void CSkinScrollBar::TileBlt(CDC *pDestDC, RECT *pDestRect, CDC *pSourDC, RECT *pSourRect)
{
	int nSourHei=pSourRect->bottom-pSourRect->top;
	int nSourWid=pSourRect->right-pSourRect->left;

	int y=pDestRect->top;
	while(y<pDestRect->bottom)
	{
		int nHei=nSourHei;
		if(y+nHei>pDestRect->bottom) nHei=pDestRect->bottom-y;

		int x=pDestRect->left;
		while(x<pDestRect->right)
		{
			int nWid=nSourWid;
			if(x+nWid>pDestRect->right) nWid=pDestRect->right-x;
			pDestDC->BitBlt(x,y,nWid,nHei,pSourDC,pSourRect->left,pSourRect->top,SRCCOPY);
			x+=nWid;
		}
		y+=nHei;
	}
}

void CSkinScrollBar::DrawThumb(CDC *pDestDC, RECT *pDestRect, CDC *pSourDC, RECT *pSourRect)
{
	if(IsRectEmpty(pDestRect)) return;
	RECT rcDest=*pDestRect,rcSour=*pSourRect;
	if(IsVertical())
	{
		ASSERT(pDestRect->bottom-pDestRect->top>=THUMB_MINSIZE);
		pDestDC->BitBlt(pDestRect->left,pDestRect->top,m_nWid,THUMB_BORDER,pSourDC,pSourRect->left,pSourRect->top,SRCCOPY);
		pDestDC->BitBlt(pDestRect->left,pDestRect->bottom-THUMB_BORDER,m_nWid,THUMB_BORDER,pSourDC,pSourRect->left,pSourRect->bottom-THUMB_BORDER,SRCCOPY);
		rcDest.top+=THUMB_BORDER,rcDest.bottom-=THUMB_BORDER;
		rcSour.top+=THUMB_BORDER,rcSour.bottom-=THUMB_BORDER;
		TileBlt(pDestDC,&rcDest,pSourDC,&rcSour);
	}else
	{
		ASSERT(pDestRect->right-pDestRect->left>=THUMB_MINSIZE);
		pDestDC->BitBlt(pDestRect->left,pDestRect->top,THUMB_BORDER,m_nWid,pSourDC,pSourRect->left,pSourRect->top,SRCCOPY);
		pDestDC->BitBlt(pDestRect->right-THUMB_BORDER,pDestRect->top,THUMB_BORDER,m_nWid,pSourDC,pSourRect->right-THUMB_BORDER,pSourRect->top,SRCCOPY);
		rcDest.left+=THUMB_BORDER,rcDest.right-=THUMB_BORDER;
		rcSour.left+=THUMB_BORDER,rcSour.right-=THUMB_BORDER;
		TileBlt(pDestDC,&rcDest,pSourDC,&rcSour);
	}
}


void CSkinScrollBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	UINT uHit=HitTest(point);
	if(uHit==SB_THUMBTRACK)
	{
		m_bDrag=TRUE;
		m_ptDrag=point;
		m_si.nTrackPos=m_si.nPos;
		m_nDragPos=m_si.nPos;
	}else
	{
		m_uClicked=uHit;
		GetParent()->SendMessage(IsVertical()?WM_VSCROLL:WM_HSCROLL,MAKELONG(m_uClicked,0),(LPARAM)m_hWnd);
		SetTimer(TIMERID_DELAY,TIME_DELAY,NULL);
		m_bPause=FALSE;
		if(uHit==SB_LINEUP||uHit==SB_LINEDOWN) DrawArrow(uHit,2);
	}
}

void CSkinScrollBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!m_bTrace && nFlags!=-1)
	{
		m_bTrace=TRUE;
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		m_bTrace = _TrackMouseEvent(&tme);
	}

	if(m_bDrag)
	{
		int nInterHei=m_nHei-2*m_nWid;
		int	nSlideHei=m_si.nPage*nInterHei/(m_si.nMax-m_si.nMin+1);
		if(nSlideHei<THUMB_MINSIZE) nSlideHei=THUMB_MINSIZE;
		if(nInterHei<THUMB_MINSIZE) nSlideHei=0;
		int nEmptyHei=nInterHei-nSlideHei;
		int nDragLen=IsVertical()?point.y-m_ptDrag.y:point.x-m_ptDrag.x;
		int nSlide=(nEmptyHei==0)?0:(nDragLen*(int)(m_si.nMax-m_si.nMin-m_si.nPage+1)/nEmptyHei);
		int nNewTrackPos=m_nDragPos+nSlide;
		if(nNewTrackPos<m_si.nMin)
		{
			nNewTrackPos=m_si.nMin;
		}else if(nNewTrackPos>(int)(m_si.nMax-m_si.nMin-m_si.nPage+1))
		{
			nNewTrackPos=m_si.nMax-m_si.nMin-m_si.nPage+1;
		}
		if(nNewTrackPos!=m_si.nTrackPos)
		{
			CDC *pDC=GetDC();
			CDC memdc;
			memdc.CreateCompatibleDC(pDC);
			HGDIOBJ hOldBmp=SelectObject(memdc,m_hBmp);
			RECT rcThumb1=GetRect(SB_THUMBTRACK);
			m_si.nTrackPos=nNewTrackPos;
			RECT rcThumb2=GetRect(SB_THUMBTRACK);

			RECT rcSourSlide=GetImageRect(SB_PAGEUP,0);
			RECT rcSourThumb=GetImageRect(SB_THUMBTRACK,2);
			RECT rcOld;
			if(IsVertical())
			{
				rcOld.left=0,rcOld.right=m_nWid;
				if(rcThumb2.bottom>rcThumb1.bottom)
				{
					rcOld.top=rcThumb1.top;
					rcOld.bottom=rcThumb2.top;
				}else
				{
					rcOld.top=rcThumb2.bottom;
					rcOld.bottom=rcThumb1.bottom;
				}
			}else
			{
				rcOld.top=0,rcOld.bottom=m_nWid;
				if(rcThumb2.right>rcThumb1.right)
				{
					rcOld.left=rcThumb1.left;
					rcOld.right=rcThumb2.left;
				}else
				{
					rcOld.left=rcThumb2.right;
					rcOld.right=rcThumb1.right;
				}
			}
			TileBlt(pDC,&rcOld,&memdc,&rcSourSlide);
			DrawThumb(pDC,&rcThumb2,&memdc,&rcSourThumb);
			SelectObject(memdc,hOldBmp);
			ReleaseDC(pDC);

			GetParent()->SendMessage(IsVertical()?WM_VSCROLL:WM_HSCROLL,MAKELONG(SB_THUMBTRACK,m_si.nTrackPos),(LPARAM)m_hWnd);
		}
	}else if(m_uClicked!=-1)
	{
		CRect rc=GetRect(m_uClicked);
		m_bPause=!rc.PtInRect(point);
		if(m_uClicked==SB_LINEUP||m_uClicked==SB_LINEDOWN)
		{
			DrawArrow(m_uClicked,m_bPause?0:2);
		}
	}else
	{
		UINT uHit=HitTest(point);
		if(uHit!=m_uHtPrev)
		{
			if(m_uHtPrev!=-1)
			{
				if(m_uHtPrev==SB_THUMBTRACK)
				{
					CDC *pDC=GetDC();
					CDC memdc;
					memdc.CreateCompatibleDC(pDC);
					HGDIOBJ hOldBmp=SelectObject(memdc,m_hBmp);
					RECT rcDest=GetRect(SB_THUMBTRACK);
					RECT rcSour=GetImageRect(SB_THUMBTRACK,0);
					DrawThumb(pDC,&rcDest,&memdc,&rcSour);
					ReleaseDC(pDC);
					SelectObject(memdc,hOldBmp);
				}else if(m_uHtPrev==SB_LINEUP||m_uHtPrev==SB_LINEDOWN)
				{
					DrawArrow(m_uHtPrev,0);
				}
			}
			if(uHit!=-1)
			{
				if(uHit==SB_THUMBTRACK)
				{
					CDC *pDC=GetDC();
					CDC memdc;
					memdc.CreateCompatibleDC(pDC);
					HGDIOBJ hOldBmp=SelectObject(memdc,m_hBmp);
					RECT rcDest=GetRect(SB_THUMBTRACK);
					RECT rcSour=GetImageRect(SB_THUMBTRACK,1);
					DrawThumb(pDC,&rcDest,&memdc,&rcSour);
					ReleaseDC(pDC);
					SelectObject(memdc,hOldBmp);
				}else if(uHit==SB_LINEUP||uHit==SB_LINEDOWN)
				{
					DrawArrow(uHit,1);
				}
			}
			m_uHtPrev=uHit;
		}
	}
}

void CSkinScrollBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	if(m_bDrag)
	{
		m_bDrag=FALSE;
		GetParent()->SendMessage(IsVertical()?WM_VSCROLL:WM_HSCROLL,MAKELONG(SB_THUMBPOSITION,m_si.nTrackPos),(LPARAM)m_hWnd);
		CDC *pDC=GetDC();
		CDC memdc;
		memdc.CreateCompatibleDC(pDC);
		HGDIOBJ hOldBmp=SelectObject(memdc,m_hBmp);
		if(m_si.nTrackPos != m_si.nPos)
		{
			RECT rcThumb=GetRect(SB_THUMBTRACK);
			RECT rcSour={m_nWid*3,0,m_nWid*4,m_nWid};
			if(!IsVertical())  OffsetRect(&rcSour,m_nWid*4,0);
			TileBlt(pDC,&rcThumb,&memdc,&rcSour);
		}
		m_si.nTrackPos=-1;

		RECT rcThumb=GetRect(SB_THUMBTRACK);
		RECT rcSour={m_nWid*2,0,m_nWid*3,m_nWid};
		if(PtInRect(&rcThumb,point)) OffsetRect(&rcSour,0,m_nWid);
		if(!IsVertical()) OffsetRect(&rcSour,m_nWid*4,0);
		DrawThumb(pDC,&rcThumb,&memdc,&rcSour);
		SelectObject(memdc,hOldBmp);
		ReleaseDC(pDC);
	}else if(m_uClicked!=-1)
	{
		if(m_bNotify)
		{
			KillTimer(TIMERID_NOTIFY);
			m_bNotify=FALSE;
		}else
		{
			KillTimer(TIMERID_DELAY);
		}
		if(m_uClicked==SB_LINEUP||m_uClicked==SB_LINEDOWN) DrawArrow(m_uClicked,0);
		m_uClicked=-1;
	}
}

void CSkinScrollBar::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==TIMERID_DELAY)
	{
		m_bNotify=TRUE;
		nIDEvent=TIMERID_NOTIFY;
		KillTimer(TIMERID_DELAY);
		SetTimer(TIMERID_NOTIFY,TIME_INTER,NULL);
	}
	if(nIDEvent==TIMERID_NOTIFY && !m_bPause)
	{
		ASSERT(m_uClicked!=-1 && m_uClicked!=SB_THUMBTRACK);

		switch(m_uClicked)
		{
		case SB_LINEUP:
			if(m_si.nPos==m_si.nMin)
			{
				KillTimer(TIMERID_NOTIFY);
				break;
			}
			GetParent()->SendMessage(IsVertical()?WM_VSCROLL:WM_HSCROLL,MAKELONG(SB_LINEUP,0),(LPARAM)m_hWnd);
			break;
		case SB_LINEDOWN:
			if(m_si.nPos==m_si.nMax)
			{
				KillTimer(TIMERID_NOTIFY);
				break;
			}
			GetParent()->SendMessage(IsVertical()?WM_VSCROLL:WM_HSCROLL,MAKELONG(SB_LINEDOWN,0),(LPARAM)m_hWnd);
			break;
		case SB_PAGEUP:
		case SB_PAGEDOWN:
			{
				CPoint pt;
				GetCursorPos(&pt);
				ScreenToClient(&pt);
				CRect rc=GetRect(SB_THUMBTRACK);
				if(rc.PtInRect(pt))
				{
					KillTimer(TIMERID_NOTIFY);
					break;
				}
				GetParent()->SendMessage(IsVertical()?WM_VSCROLL:WM_HSCROLL,MAKELONG(m_uClicked,0),(LPARAM)m_hWnd);
			}
			break;
		default:
			ASSERT(FALSE);
			break;
		}
	}
}

void CSkinScrollBar::DrawArrow(UINT uArrow, int nState)
{
	ASSERT(uArrow==SB_LINEUP||uArrow==SB_LINEDOWN);
	CDC *pDC=GetDC();
	CDC memdc ;
	memdc.CreateCompatibleDC(pDC);
	HGDIOBJ hOldBmp=::SelectObject(memdc,m_hBmp);

	RECT rcDest=GetRect(uArrow);
	RECT rcSour=GetImageRect(uArrow,nState);
	if((rcDest.right-rcDest.left != rcSour.right-rcSour.left)
		||(rcDest.bottom-rcDest.top != rcSour.bottom-rcSour.top))
		pDC->StretchBlt(rcDest.left,rcDest.top,rcDest.right-rcDest.left,rcDest.bottom-rcDest.top,
		&memdc,
		rcSour.left,rcSour.top,rcSour.right-rcSour.left,rcSour.bottom-rcSour.top,
		SRCCOPY);
	else
		pDC->BitBlt(rcDest.left,rcDest.top,m_nWid,m_nWid,&memdc,rcSour.left,rcSour.top,SRCCOPY);

	ReleaseDC(pDC);
	::SelectObject(memdc,hOldBmp);
}


LRESULT CSkinScrollBar::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTrace = FALSE;
	OnMouseMove(-1,CPoint(-1,-1));
	return 0;
}


LRESULT CSkinScrollBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message==WM_LBUTTONDBLCLK) return 1;
	if(message==SBM_SETSCROLLINFO)
	{
		BOOL bRedraw=(BOOL)wParam;
		LPSCROLLINFO lpScrollInfo=(LPSCROLLINFO)lParam;
		if(lpScrollInfo->fMask&SIF_PAGE) m_si.nPage=lpScrollInfo->nPage;
		if(lpScrollInfo->fMask&SIF_POS) m_si.nPos=lpScrollInfo->nPos;
		if(lpScrollInfo->fMask&SIF_RANGE)
		{
			m_si.nMin=lpScrollInfo->nMin;
			m_si.nMax=lpScrollInfo->nMax;
		}
		if(bRedraw)
		{
			CDC *pDC=GetDC();
			CDC memdc;
			memdc.CreateCompatibleDC(pDC);
			HGDIOBJ hOldBmp=::SelectObject(memdc,m_hBmp);

			RECT rcSour=GetImageRect(SB_PAGEUP);
			RECT rcDest=GetRect(SB_PAGEUP);
			TileBlt(pDC,&rcDest,&memdc,&rcSour);
			rcDest=GetRect(SB_THUMBTRACK);
			rcSour=GetImageRect(SB_THUMBTRACK);
			DrawThumb(pDC,&rcDest,&memdc,&rcSour);
			rcDest=GetRect(SB_PAGEDOWN);
			rcSour=GetImageRect(SB_PAGEDOWN);
			TileBlt(pDC,&rcDest,&memdc,&rcSour);
			::SelectObject(memdc,hOldBmp);
			ReleaseDC(pDC);
		}
		return TRUE;
	}
	if(message==SBM_GETSCROLLINFO)
	{
		LPSCROLLINFO lpScrollInfo=(LPSCROLLINFO)lParam;
		int nMask=lpScrollInfo->fMask;
		if(nMask&SIF_PAGE) lpScrollInfo->nPage=m_si.nPage;
		if(nMask&SIF_POS) lpScrollInfo->nPos=m_si.nPos;
		if(nMask&SIF_TRACKPOS) lpScrollInfo->nTrackPos=m_si.nTrackPos;
		if(nMask&SIF_RANGE)
		{
			lpScrollInfo->nMin=m_si.nMin;
			lpScrollInfo->nMax=m_si.nMax;
		}
		return TRUE;

	}
	return CScrollBar::WindowProc(message, wParam, lParam);
}
void CSkinScrollBar::OnDestroy()
{
	CScrollBar::OnDestroy();

	RenderEngine->RemoveImage(m_pBackImg);
}

BOOL CSkinScrollBar::SetBackImage( LPCTSTR lpszFileName )
{
	RenderEngine->RemoveImage(m_pBackImg);

	m_pBackImg = RenderEngine->GetImage(lpszFileName);

	if (NULL == m_pBackImg)
		return FALSE;
	else
	{
		SetBitmap(m_pBackImg->ImageToBitmap());
		return TRUE;
	}
}
