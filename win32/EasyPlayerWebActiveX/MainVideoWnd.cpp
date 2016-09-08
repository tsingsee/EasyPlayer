// MainVideoWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "EasyPlayerWebActiveX.h"
#include "MainVideoWnd.h"
#include "afxdialogex.h"


// CMainVideoWnd 对话框

IMPLEMENT_DYNAMIC(CMainVideoWnd, CDialogEx)

CMainVideoWnd::CMainVideoWnd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainVideoWnd::IDD, pParent)
{
	m_pEasyLogo = new CImageEx;

}

CMainVideoWnd::~CMainVideoWnd()
{
	if (m_pEasyLogo)
	{
		SafeDelete(m_pEasyLogo);
	}
}

void CMainVideoWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainVideoWnd, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CMainVideoWnd 消息处理程序


void CMainVideoWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CBrush brushBkgnd; 
	CRect rcClient;
	brushBkgnd.CreateSolidBrush(RGB(0, 0, 0));
	GetClientRect(&rcClient);
	dc.FillRect(&rcClient, &brushBkgnd);
	brushBkgnd.DeleteObject(); //释放画刷 

	if ( m_pEasyLogo != NULL && !m_pEasyLogo->IsNull() )
	{
		int nStartX = (rcClient.Width()-184)/2;
		int nStartY =  (rcClient.Height()-184)/2;
		m_pEasyLogo->DrawImage(CDC::FromHandle(dc.m_hDC),nStartX,nStartY);
	}
}


BOOL CMainVideoWnd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (m_pEasyLogo)
	{
		m_pEasyLogo->LoadImage( ::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_PNG_EASYLOGO), TEXT("PNG"));
	}
		

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
