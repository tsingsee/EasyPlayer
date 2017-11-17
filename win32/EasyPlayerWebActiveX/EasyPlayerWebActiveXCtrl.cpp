// EasyPlayerWebActiveXCtrl.cpp : CEasyPlayerWebActiveXCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "EasyPlayerWebActiveX.h"
#include "EasyPlayerWebActiveXCtrl.h"
#include "EasyPlayerWebActiveXPropPage.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CEasyPlayerWebActiveXCtrl, COleControl)



// 消息映射

BEGIN_MESSAGE_MAP(CEasyPlayerWebActiveXCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP(CEasyPlayerWebActiveXCtrl, COleControl)
	DISP_FUNCTION_ID(CEasyPlayerWebActiveXCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CEasyPlayerWebActiveXCtrl, "Start", dispidStart, Start, VT_I4, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CEasyPlayerWebActiveXCtrl, "Config", dispidConfig, Config, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CEasyPlayerWebActiveXCtrl, "Close", dispidClose, Close, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP(CEasyPlayerWebActiveXCtrl, COleControl)
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CEasyPlayerWebActiveXCtrl, 1)
	PROPPAGEID(CEasyPlayerWebActiveXPropPage::guid)
END_PROPPAGEIDS(CEasyPlayerWebActiveXCtrl)



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CEasyPlayerWebActiveXCtrl, "EASYPLAYERWEBACT.EasyPlayerWebActCtrl.1",
	0x1ee1c648, 0xf4a9, 0x42f9, 0x9a, 0xa7, 0x2c, 0x8e, 0x3a, 0xf7, 0xb7, 0xfd)



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CEasyPlayerWebActiveXCtrl, _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID IID_DEasyPlayerWebActiveX = { 0xFB5E35A6, 0x2538, 0x47C4, { 0x9B, 0x40, 0xB0, 0xFB, 0x6A, 0x0, 0xDE, 0xA0 } };
const IID IID_DEasyPlayerWebActiveXEvents = { 0x66887A6D, 0xC75B, 0x48B8, { 0xB5, 0xE1, 0xBD, 0x72, 0x2D, 0x87, 0x59, 0x23 } };


// 控件类型信息

static const DWORD _dwEasyPlayerWebActiveXOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CEasyPlayerWebActiveXCtrl, IDS_EASYPLAYERWEBACTIVEX, _dwEasyPlayerWebActiveXOleMisc)



// CEasyPlayerWebActiveXCtrl::CEasyPlayerWebActiveXCtrlFactory::UpdateRegistry -
// 添加或移除 CEasyPlayerWebActiveXCtrl 的系统注册表项

BOOL CEasyPlayerWebActiveXCtrl::CEasyPlayerWebActiveXCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegInsertable | afxRegApartmentThreading 改为 afxRegInsertable。

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_EASYPLAYERWEBACTIVEX,
			IDB_EASYPLAYERWEBACTIVEX,
			afxRegInsertable | afxRegApartmentThreading,
			_dwEasyPlayerWebActiveXOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// 授权字符串

static const TCHAR _szLicFileName[] = _T("EasyPlayerWebActiveX.lic");

static const WCHAR _szLicString[] = L"Copyright (c) 2016 ";



// CEasyPlayerWebActiveXCtrl::CEasyPlayerWebActiveXCtrlFactory::VerifyUserLicense -
// 检查是否存在用户许可证

BOOL CEasyPlayerWebActiveXCtrl::CEasyPlayerWebActiveXCtrlFactory::VerifyUserLicense()
{
	return AfxVerifyLicFile(AfxGetInstanceHandle(), _szLicFileName,
		_szLicString);
}



// CEasyPlayerWebActiveXCtrl::CEasyPlayerWebActiveXCtrlFactory::GetLicenseKey -
// 返回运行时授权密钥

BOOL CEasyPlayerWebActiveXCtrl::CEasyPlayerWebActiveXCtrlFactory::GetLicenseKey(DWORD dwReserved,
	BSTR *pbstrKey)
{
	if (pbstrKey == NULL)
		return FALSE;

	*pbstrKey = SysAllocString(_szLicString);
	return (*pbstrKey != NULL);
}



// CEasyPlayerWebActiveXCtrl::CEasyPlayerWebActiveXCtrl - 构造函数

CEasyPlayerWebActiveXCtrl::CEasyPlayerWebActiveXCtrl()
{
	InitializeIIDs(&IID_DEasyPlayerWebActiveX, &IID_DEasyPlayerWebActiveXEvents);
	// TODO: 在此初始化控件的实例数据。
	m_bInit = false;

	memset(szURL, 0x00, 512);
	memset(szUserName, 0x00, 128);
	memset(szPassword, 0x00, 128);
	nHardDecode = 0;
	eRenderFormat = DISPLAY_FORMAT_YV12;

	nFrameCache = 3;
	bPlaySound = TRUE;
	bShowToScale = FALSE;
	bShowStatisticInfo = TRUE;
}



// CEasyPlayerWebActiveXCtrl::~CEasyPlayerWebActiveXCtrl - 析构函数

CEasyPlayerWebActiveXCtrl::~CEasyPlayerWebActiveXCtrl()
{
	// TODO: 在此清理控件的实例数据。

}



// CE	AfxMessageBox(_T("页面已经关闭，重新加载OCX"));


void CEasyPlayerWebActiveXCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: 用您自己的绘图代码替换下面的代码。
// 	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
// 	pdc->Ellipse(rcBounds);

	DoSuperclassPaint(pdc, rcBounds);

	//if (!m_pActiveDlg.IsFullScreen())
	{
		m_pActiveDlg.MoveWindow(rcBounds, TRUE);
	}

	CBrush brBackGnd(TranslateColor(AmbientBackColor()));
	pdc->FillRect(rcBounds, &brBackGnd);

}



// CEasyPlayerWebActiveXCtrl::DoPropExchange - 持久性支持

void CEasyPlayerWebActiveXCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}



// CEasyPlayerWebActiveXCtrl::OnResetState - 将控件重置为默认状态

void CEasyPlayerWebActiveXCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}



// CEasyPlayerWebActiveXCtrl::AboutBox - 向用户显示“关于”框

void CEasyPlayerWebActiveXCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_EASYPLAYERWEBACTIVEX);
	dlgAbout.DoModal();
}



// CEasyPlayerWebActiveXCtrl 消息处理程序


int CEasyPlayerWebActiveXCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	// OCX内置窗口必须以OCX控件为父类窗口的子窗口，否则界面显示会有问题 [4/14/2014-10:12:38 Dingshuai]
	m_pActiveDlg.Create(CMainVideoWnd::IDD, this);

	return 0;
}


void CEasyPlayerWebActiveXCtrl::OnDestroy()
{
	COleControl::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
	m_pActiveDlg.DestroyWindow();

}

int StartStream();

LONG CEasyPlayerWebActiveXCtrl::Start(LPCTSTR sURL, LPCTSTR sRenderFormat, LPCTSTR sUserName, LPCTSTR sPassword, LPCTSTR sHardDecord)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: 在此添加调度处理程序代码
	int nRet = 0;
	if (m_bInit)
	{
		char szRenderFormat[128] ;
		char szHardDecord[128] ;
		if (wcslen(sURL) < 1)		
			return -1;
		__WCharToMByte(sURL, szURL, sizeof(szURL)/sizeof(szURL[0]));
		if (wcslen(sRenderFormat) > 0)
		{
			__WCharToMByte(sRenderFormat, szRenderFormat, sizeof(szRenderFormat)/sizeof(szRenderFormat[0]));
		}
		if (wcslen(sUserName) > 0)
		{
			__WCharToMByte(sUserName, szUserName, sizeof(szUserName)/sizeof(szUserName[0]));
		}
		if (wcslen(sPassword) > 0)
		{
			__WCharToMByte(sPassword, szPassword, sizeof(szPassword)/sizeof(szPassword[0]));
		}
		if (wcslen(sHardDecord) > 0)
		{
			__WCharToMByte(sHardDecord, szHardDecord, sizeof(szHardDecord)/sizeof(szHardDecord[0]));
		}

		nHardDecode = atoi(szHardDecord);
		int nRenderType = atoi(szRenderFormat);
		eRenderFormat = DISPLAY_FORMAT_YV12;
		switch (nRenderType)
		{
		case 0:
			eRenderFormat = DISPLAY_FORMAT_YV12;
			break;
		case 1:
			eRenderFormat = DISPLAY_FORMAT_YUY2;
			break;
		case 2:
			eRenderFormat = DISPLAY_FORMAT_UYVY;
			break;
		case 3:
			eRenderFormat = DISPLAY_FORMAT_A8R8G8B8;
			break;
		case 4:
			eRenderFormat = DISPLAY_FORMAT_X8R8G8B8;
			break;
		case 5:
			eRenderFormat = DISPLAY_FORMAT_RGB565;
			break;
		case 6:
			eRenderFormat = DISPLAY_FORMAT_RGB555;
			break;
		case 7:
			eRenderFormat = DISPLAY_FORMAT_RGB24_GDI;
			break;
		}

		nRet = m_player.Start(szURL, m_pActiveDlg.GetSafeHwnd(), eRenderFormat , 1, szUserName , szPassword, nHardDecode);
	}
	else
	{
		SetTimer( WM_TIMER_START_ID, 500, NULL );
	}

	return nRet;
}


void CEasyPlayerWebActiveXCtrl::Config(LPCTSTR sFrameCache, LPCTSTR sPlaySound, LPCTSTR sShowToScale, LPCTSTR sShowStatisticInfo )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: 在此添加调度处理程序代码
	// 
	if (m_bInit)
	{
		char szFrameCache[128] = {0,};
		char szPlaySound[128] = {0,};
		char szShowToScale[128] = {0,};
		char szShowStatisticInfo[128] = {0,};


		if (wcslen(sFrameCache) > 0)
		{
			__WCharToMByte(sFrameCache, szFrameCache, sizeof(szFrameCache)/sizeof(szFrameCache[0]));
			nFrameCache = atoi(szFrameCache);
		}
		if (wcslen(sPlaySound) > 0)
		{
			__WCharToMByte(sPlaySound, szPlaySound, sizeof(szPlaySound)/sizeof(szPlaySound[0]));
			bPlaySound = atoi(szPlaySound);
		}
		if (wcslen(sShowToScale) > 0)
		{
			__WCharToMByte(sShowToScale, szShowToScale, sizeof(szShowToScale)/sizeof(szShowToScale[0]));
			bShowToScale = atoi(szShowToScale);
		}
		if (wcslen(sShowStatisticInfo) > 0)
		{
			__WCharToMByte(sShowStatisticInfo, szShowStatisticInfo, sizeof(szShowStatisticInfo)/sizeof(szShowStatisticInfo[0]));
			bShowStatisticInfo = atoi(szShowStatisticInfo);
		}

		m_player.Config(nFrameCache, bPlaySound, bShowToScale, bShowStatisticInfo );
	}
	else
	{
		SetTimer( WM_TIMER_CONFIG_ID, 500, NULL );
	}
}

void CEasyPlayerWebActiveXCtrl::Close(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	m_player.Close();
}

void CEasyPlayerWebActiveXCtrl::OnSetClientSite()
{
	// TODO: 
	if (m_pClientSite) 
	{
		int ret = EasyPlayerManager::Init();
#if 0
		CString str = _T("");
		str.Format(_T("Init = %d"), ret);
		AfxMessageBox(str);
#endif
		//父窗口及其大小并不重要，因为控件在本地激活时会自动重画和重新定位。
		VERIFY (CreateControlWindow (::GetDesktopWindow(), CRect(0,0,0,0), CRect(0,0,0,0)));
		m_bInit = true;
	}
	else
	{
		Close();
		// 调用刷新会报错 [10/12/2017 dingshuai]
		//EasyPlayerManager::UnInit();
		//AfxMessageBox(_T("页面已经关闭，重新加载OCX"));
		DestroyWindow();
		m_bInit = false;

	}
	COleControl::OnSetClientSite();
}

void CEasyPlayerWebActiveXCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 
	switch (nIDEvent)
	{
	case WM_TIMER_START_ID:
		{
			if (m_bInit)
			{
				int ret = m_player.Start(szURL, m_pActiveDlg.GetSafeHwnd(), eRenderFormat , 1, szUserName , szPassword, nHardDecode);
				KillTimer(WM_TIMER_START_ID);
			}
		}
		break;
	case WM_TIMER_CONFIG_ID:
		{
			if (m_bInit)
			{
				m_player.Config(nFrameCache, bPlaySound, bShowToScale, bShowStatisticInfo );
				KillTimer(WM_TIMER_CONFIG_ID);
			}
		}
		break;
	}

	COleControl::OnTimer(nIDEvent);
}
