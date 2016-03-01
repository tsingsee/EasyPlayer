#pragma once

#include "SkinUI.h"
#include "ISkinControl.h"
// CSkinTreeCtrl
struct  tagItemInfo 
{
	HTREEITEM		hTreeItem;
	CImageEx		*pImage;
};

typedef map<HTREEITEM,tagItemInfo>	CMapItemInfo;

//////////////////////////////////////////////////////////////////////////

class SKINUI_CLASS CSkinTreeCtrl : public CTreeCtrl,public ISkinControl
{
	DECLARE_DYNAMIC(CSkinTreeCtrl)
	
	//资源变量
protected:
	CImageEx						*m_pImageButton;					//按钮资源
	CImageEx						*m_pHovenImg, *m_pPressImg;			//节点资源

	//辅助变量
protected:
	HTREEITEM						m_hItemMouseHover;					//盘旋子项
	UINT							m_uCursorRes;						//鼠标资源
	CMapItemInfo					m_MapItemInfo;						//资源容器

	//节点的颜色
public:
	COLORREF						m_colHoven,m_colPress;				//节点颜色

public:
	CSkinTreeCtrl();
	virtual ~CSkinTreeCtrl();

public:
	//设置属性
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	//创建控件
	virtual BOOL CreateControl(CWnd* pParentWnd);
	//设置属性
	virtual void ParseItem(CXmlNode *root);

public:
	//设置资源
	BOOL SetButtonImage(LPCTSTR lpszFileName);
	//设置资源
	BOOL SetHovenImage(LPCTSTR lpszFileName,CONST LPRECT lprcNinePart=NULL);
	//设置资源
	BOOL SetPressImage(LPCTSTR lpszFileName,CONST LPRECT lprcNinePart=NULL);
	//插入图标
	BOOL InsertImage(HTREEITEM hTreeItem,LPCTSTR lpszFileName);
	//设置鼠标
	void SetCursorStyle(UINT uID);

	//绘画辅助
private:
	//绘画子项
	VOID DrawTreeItem(CDC * pDC, CRect & rcClient, CRect & rcClipBox);
	//绘制图标
	VOID DrawListImage(CDC * pDC, CRect rcRect, HTREEITEM hTreeItem, bool bSelected);
	//绘制文本
	VOID DrawItemString(CDC * pDC, CRect rcRect, HTREEITEM hTreeItem, bool bSelected);

	//消息函数
public:
	//绘制消息
	afx_msg void OnPaint();
	//设置鼠标
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//销毁消息
	afx_msg void OnDestroy();

protected:
	DECLARE_MESSAGE_MAP()
};


