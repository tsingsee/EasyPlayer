#pragma once

// EasyPlayerWebActiveXPropPage.h : CEasyPlayerWebActiveXPropPage 属性页类的声明。


// CEasyPlayerWebActiveXPropPage : 有关实现的信息，请参阅 EasyPlayerWebActiveXPropPage.cpp。

class CEasyPlayerWebActiveXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CEasyPlayerWebActiveXPropPage)
	DECLARE_OLECREATE_EX(CEasyPlayerWebActiveXPropPage)

// 构造函数
public:
	CEasyPlayerWebActiveXPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_EASYPLAYERWEBACTIVEX };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

