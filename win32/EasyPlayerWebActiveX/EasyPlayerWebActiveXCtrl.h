#pragma once

#include "MainVideoWnd.h"
// 增加EasyPlayer管理类 [3/6/2016 dingshuai]
#include "EasyPlayerManager.h"

#define WM_TIMER_START_ID 1001
#define WM_TIMER_CONFIG_ID 1002

// EasyPlayerWebActiveXCtrl.h : CEasyPlayerWebActiveXCtrl ActiveX 控件类的声明。

// CEasyPlayerWebActiveXCtrl : 有关实现的信息，请参阅 EasyPlayerWebActiveXCtrl.cpp。

class CEasyPlayerWebActiveXCtrl : public COleControl
{
	DECLARE_DYNCREATE(CEasyPlayerWebActiveXCtrl)

// 构造函数
public:
	CEasyPlayerWebActiveXCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CEasyPlayerWebActiveXCtrl();

	BEGIN_OLEFACTORY(CEasyPlayerWebActiveXCtrl)        // 类工厂和 guid
		virtual BOOL VerifyUserLicense();
		virtual BOOL GetLicenseKey(DWORD, BSTR *);
	END_OLEFACTORY(CEasyPlayerWebActiveXCtrl)

	DECLARE_OLETYPELIB(CEasyPlayerWebActiveXCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CEasyPlayerWebActiveXCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CEasyPlayerWebActiveXCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
		dispidClose = 3L,
		dispidConfig = 2L,
		dispidStart = 1L
	};

private:
	CMainVideoWnd m_pActiveDlg; 
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
protected:
	LONG Start(LPCTSTR sURL, LPCTSTR sRenderFormat, LPCTSTR sUserName, LPCTSTR sPassword, LPCTSTR sHardDecord);
	void Config(LPCTSTR sFrameCache, LPCTSTR sPlaySound, LPCTSTR sShowToScale, LPCTSTR sShowStatisticInfo);
	void Close(void);

private:
	EasyPlayerManager m_player;
	bool m_bInit;

	char szURL[512] ;
	char szUserName[128] ;
	char szPassword[128] ;

	int nHardDecode;
    RENDER_FORMAT eRenderFormat;

	int	   nFrameCache ;
	BOOL bPlaySound ;
	BOOL bShowToScale ;
	BOOL bShowStatisticInfo ;

public:
	virtual void OnSetClientSite();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

