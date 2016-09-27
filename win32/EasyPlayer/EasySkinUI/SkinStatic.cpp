#include "stdafx.h"
#include "Control.h"
#include "SkinStatic.h"

//IMPLEMENT_DYNAMIC(CSkinStatic,CStatic)
CSkinStatic::CSkinStatic()
{

	
}

CSkinStatic::~CSkinStatic()
{
}

BEGIN_MESSAGE_MAP(CSkinStatic, CStatic)
	//{{AFX_MSG_MAP(CSkinStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


void CSkinStatic::DrawText(CDC* pDC,RECT &rcClient)
{
	CString strText;
	strText = GetWindowText();

	pDC->SelectObject(GetCtrlFont());
	pDC->SetTextColor(IsWindowEnabled()?m_colNormalText:m_colDisableText);
	pDC->SetBkMode(TRANSPARENT);

	UINT nFormat = m_dwTextAlign ? m_dwTextAlign : DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;
	pDC->DrawText(strText, &rcClient, nFormat);
}
void CSkinStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(&rcClient);
	CMemoryDC MemDC(&dc, rcClient);

	//»æÖÆ±³¾°
	//DrawParentWndBg(GetSafeHwnd(),MemDC->GetSafeHdc());

	//DrawText(&MemDC,rcClient);
}




BOOL CSkinStatic::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CSkinStatic::SetWindowText(LPCSTR lpcString)
{
	m_strCaption=lpcString;
}
CString CSkinStatic::GetWindowText()
{
	return m_strCaption;

}

BOOL CSkinStatic::CreateControl(CWnd* pParentWnd)
{
	if( !Create(NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),pParentWnd,0) )
		return FALSE;

	m_pOwnWnd = this;

	return TRUE;
}

int CSkinStatic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	// TODO: Add your specialized creation code here
	return 0;
}

void CSkinStatic::PreSubclassWindow()
{
	//TRACE(_T("in CxStatic::PreSubclassWindow\n"));
	
	CStatic::PreSubclassWindow();
	ModifyStyle(0,SS_OWNERDRAW);
}

void CSkinStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct != NULL);
}

int CSkinStatic::GetStringSize(CString strMsg)
{
	SIZE size;
	size.cx = 0;
	size.cy = 0;
	::GetTextExtentPoint32(GetDC()->GetSafeHdc(), strMsg, strMsg.GetLength(), &size);

	return size.cx;
}

void CSkinStatic::SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue )
{
	ISkinControl::SetAttribute(pstrName,pstrValue);

	if( _tcscmp(pstrName, _T("normalimage")) == 0 ) 
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

}











