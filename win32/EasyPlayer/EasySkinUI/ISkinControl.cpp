#include "StdAfx.h"
#include "ISkinControl.h"

ISkinControl::ISkinControl(void)
{
	m_hParentDC = NULL;
	m_bTransparent = FALSE;
	m_colDefText = RGB(128,128,128);
	m_colNormalText = RGB(0,0,0);
	m_colDisableText = RGB(128,128,128);
	m_colReadOnlyText = RGB(0,0,0);
	m_colSelectText = RGB(255,255,255);
	m_colFrameNormal = RGB(0,0,0);
	m_colBack=RGB(255,255,255);
	m_Font = UIRenderEngine->GetDeaultFont();
	m_dwTextAlign = 0;
	m_pOwnWnd = NULL;
	m_ptPosition.SetPoint(0,0);
	m_tfPosition.SetPoint(0,0);
	m_szSize.SetSize(0,0);
	m_FixedPostion[0] = m_FixedPostion[1] = en_LTop;
	m_sKeyName = _T("");
	m_nId = 0;
	m_strTextBase=_T("");
}

ISkinControl::~ISkinControl(void)
{
	m_bTransparent = FALSE;
}

void ISkinControl::CalcCenterRect( RECT& rcDest, int cx, int cy, RECT& rcCenter )
{
	int x = ((rcDest.right-rcDest.left) - cx + 1) / 2;
	int y = ((rcDest.bottom-rcDest.top) - cy + 1) / 2;

	rcCenter.left = rcDest.left+x;
	rcCenter.top = rcDest.top+y;
	rcCenter.right = rcCenter.left+cx;
	rcCenter.bottom = rcCenter.top+cy;
}

void ISkinControl::DrawParentWndBg(HWND hWnd, HDC hDC )
{
	if( hWnd == NULL ) return;
	if ( !m_bTransparent ) return;

	HWND hParentWnd = ::GetParent(hWnd);

	CRect rcWindow;
	::GetWindowRect(hWnd,&rcWindow);
	::ScreenToClient(hParentWnd, (LPPOINT)&rcWindow); 
	::ScreenToClient(hParentWnd, ((LPPOINT)&rcWindow)+1);

	::BitBlt(hDC, 0, 0, rcWindow.Width(), rcWindow.Height(), m_hParentDC, rcWindow.left, rcWindow.top, SRCCOPY);
}

void ISkinControl::SetDefText( COLORREF colText )
{
	m_colDefText = colText;
}

void ISkinControl::SetNormalText( COLORREF colText )
{
	m_colNormalText = colText;
}

void ISkinControl::SetDisableText( COLORREF colText )
{
	m_colDisableText = colText;
}

void ISkinControl::SetSelectText( COLORREF colText )
{
	m_colSelectText = colText;
}

void ISkinControl::SetFrameColor( COLORREF colFrame )
{
	m_colFrameNormal = colFrame;
}

BOOL ISkinControl::SetScrollImage( CWnd * pOwn,LPCTSTR pszFileName )
{
	UIRenderEngine->RemoveImage(m_pImageScroll);
	m_pImageScroll = UIRenderEngine->GetImage(pszFileName);

	if ( (m_pImageScroll != NULL) && (pOwn != NULL) && (pOwn->GetSafeHwnd() != NULL) )
	{
		ASSERT(m_pImageScroll != NULL);
		if( m_pImageScroll == NULL) return FALSE;

		//设置滚动
		SkinUI_Init(pOwn->GetSafeHwnd(),m_pImageScroll->ImageToBitmap());
	}

	return TRUE;
}

void ISkinControl::RemoveScorll()
{
	UIRenderEngine->RemoveImage(m_pImageScroll);
}

BOOL ISkinControl::TrackMouseLeave( HWND hWnd )
{
	TRACKMOUSEEVENT tme = { 0 };
	tme.cbSize = sizeof(tme);
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = hWnd;

	return _TrackMouseEvent(&tme);
}

void ISkinControl::SetCtrlFont( HFONT hFont )
{
	m_Font = hFont;
}

HFONT ISkinControl::GetCtrlFont()
{
	return m_Font;
}

void ISkinControl::SetBackColor( COLORREF colBack )
{
	m_colBack = colBack;
}

void ISkinControl::SetUnTransparent()
{
	m_bTransparent  = false;
}

void ISkinControl::SetTextAlign( WORD wTextAlign )
{
	m_dwTextAlign = wTextAlign;
}

//控件自我属性解析
void ISkinControl::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	if ( m_pOwnWnd == NULL ) return;
	LPTSTR pstr = NULL;

	// 为了程序的兼容性，特地扩展关键字配置项，用以识别各控件 [2/25/2016 Dingshuai]
	if( _tcscmp(pstrName, _T("keyname")) == 0 ) m_sKeyName = pstrValue;

	if( _tcscmp(pstrName, _T("text")) == 0 )
	{
		m_strTextBase=pstrValue;
		m_pOwnWnd->SetWindowText(_CS(m_strTextBase));
		//m_pOwnWnd->SetWindowText(pstrValue);
	}
	if( _tcscmp(pstrName, _T("id")) == 0 ) 
	{
		m_nId = _ttoi(pstrValue);
		m_pOwnWnd->SetDlgCtrlID(m_nId);
	}
	else if( _tcscmp(pstrName, _T("show")) == 0 ){ m_pOwnWnd->ShowWindow((_tcscmp(pstrValue, _T("true")) == 0) ? SW_SHOW : SW_HIDE); }
	else if( _tcscmp(pstrName, _T("enable")) == 0 ){ m_pOwnWnd->EnableWindow((_tcscmp(pstrValue, _T("true")) == 0) ? TRUE : FALSE); }
	else if( _tcscmp(pstrName, _T("pos")) == 0 )
	{ 
		LPTSTR pstr = NULL;
		m_FixedPostion[0] = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
		m_FixedPostion[1] = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
		m_ptPosition.x = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
		m_ptPosition.y = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
		
// 		m_szSize.cx = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);   
// 		m_szSize.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);  

		//liuyi 2016-03-03
		int nWidth=_tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
		int nHeight= _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
		if(m_FixedPostion[0]==0&&m_FixedPostion[1]==0 || m_FixedPostion[1]==4 || m_FixedPostion[1]==5)
		{
			
			if(nWidth-m_ptPosition.x>=0&&nHeight-m_ptPosition.y>=0)
			{
				m_szSize.cx=nWidth;
				m_szSize.cy=nHeight;
			}
			else
			{
				m_szSize.cx = m_ptPosition.x + nWidth; 
				m_szSize.cy =m_ptPosition.y + nHeight;
			}

		}
		else
		{
				m_szSize.cx=nWidth;
				m_szSize.cy=nHeight;
		}
	}
	else if( _tcscmp(pstrName, _T("trans")) == 0 ){ m_bTransparent = (_tcscmp(pstrValue, _T("true")) == 0) ? TRUE : FALSE; }
	else if( _tcscmp(pstrName, _T("fontdefaultcolor")) == 0 ) 
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		m_colDefText = _tcstoul(pstrValue, &pstr, 16);
	}
	else if( _tcscmp(pstrName, _T("fontnormalcolor")) == 0 ) 
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		m_colNormalText = _tcstoul(pstrValue, &pstr, 16);
	}
	else if( _tcscmp(pstrName, _T("fontselcolor")) == 0 ) 
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		m_colSelectText = _tcstoul(pstrValue, &pstr, 16);
	}
	else if( _tcscmp(pstrName, _T("fontdiscolor")) == 0 ) 
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		m_colDisableText = _tcstoul(pstrValue, &pstr, 16);
	}
	else if( _tcscmp(pstrName, _T("fontreadonlycolor")) == 0 ) 
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		m_colReadOnlyText = _tcstoul(pstrValue, &pstr, 16);
	}
	else if(_tcscmp(pstrName, _T("textalign")) == 0)//liuyi 2016-03-04 add 字的排列位置
	{
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		m_dwTextAlign=_tcstoul(pstrValue, &pstr, 16);
	}
	else if(_tcscmp(pstrName, _T("textoffset")) == 0)//字的偏移位置
	{ 
		m_tfPosition.x = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr); 
		m_tfPosition.y = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr); 
	
	}
}