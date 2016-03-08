// EasyPlayerWebActiveXPropPage.cpp : CEasyPlayerWebActiveXPropPage 属性页类的实现。

#include "stdafx.h"
#include "EasyPlayerWebActiveX.h"
#include "EasyPlayerWebActiveXPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CEasyPlayerWebActiveXPropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CEasyPlayerWebActiveXPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CEasyPlayerWebActiveXPropPage, "EASYPLAYERWEBA.EasyPlayerWebAPropPage.1",
	0xf594479c, 0x1a17, 0x49c7, 0xae, 0xf8, 0x86, 0x7a, 0x93, 0x2c, 0x20, 0xa2)



// CEasyPlayerWebActiveXPropPage::CEasyPlayerWebActiveXPropPageFactory::UpdateRegistry -
// 添加或移除 CEasyPlayerWebActiveXPropPage 的系统注册表项

BOOL CEasyPlayerWebActiveXPropPage::CEasyPlayerWebActiveXPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_EASYPLAYERWEBACTIVEX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CEasyPlayerWebActiveXPropPage::CEasyPlayerWebActiveXPropPage - 构造函数

CEasyPlayerWebActiveXPropPage::CEasyPlayerWebActiveXPropPage() :
	COlePropertyPage(IDD, IDS_EASYPLAYERWEBACTIVEX_PPG_CAPTION)
{
}



// CEasyPlayerWebActiveXPropPage::DoDataExchange - 在页和属性间移动数据

void CEasyPlayerWebActiveXPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CEasyPlayerWebActiveXPropPage 消息处理程序
