#ifndef CONTROL_HEAD_FILE
#define CONTROL_HEAD_FILE

#pragma once

#include "SkinUI.h"
#include "XmlManager.h"

class SKINUI_CLASS ISkinControl
{
protected:
	HDC							m_hParentDC;
	CImageEx					*m_pImageScroll;						//滚动资源
	DWORD						m_dwTextAlign;
	HFONT						m_Font;

	//文字颜色
public:
	COLORREF					m_colDefText;						//默认文字
	COLORREF					m_colNormalText;					//正常文字
	COLORREF					m_colSelectText;					//高亮文字
	COLORREF					m_colDisableText;					//失效文字
	COLORREF					m_colReadOnlyText;					//只读文字

	COLORREF					m_colBack;							//窗口背景
	COLORREF					m_colFrameNormal;					//边框颜色

public:
	BOOL						m_bTransparent;
	CWnd						* m_pOwnWnd;
	CPoint						m_ptPosition;
	CSize						m_szSize;
	int							m_FixedPostion[2];
	//标示该控件的关键字
	CString						m_sKeyName;
	//标示该控件的资源ID
	int							m_nId;
	CPoint						m_tfPosition;
	CString						m_strTextBase;
public:
	ISkinControl(void);
	virtual ~ISkinControl(void);

public:
	//设置属性
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	//设置属性
	virtual void ParseItem(CXmlNode *root){}
	//创建控件
	virtual BOOL CreateControl(CWnd* pParentWnd){ return TRUE; }

public:
	//矩形居中
	void CalcCenterRect(RECT& rcDest, int cx, int cy, RECT& rcCenter);
	//绘制背景
	void DrawParentWndBg(HWND hWnd,HDC hDC);
	//设置背景
	void SetParentBack(HDC hDC){ m_bTransparent = true; m_hParentDC = hDC;}	
	BOOL TrackMouseLeave(HWND hWnd);

	//字体设置
public:
	//设置字体
	void SetCtrlFont(HFONT hFont);
	//获取字体
	HFONT GetCtrlFont();
	//文字位置
	void SetTextAlign(WORD wTextAlign);

	//文字颜色
public:
	//默认文本
	void SetDefText(COLORREF colText);
	//正常文本
	void SetNormalText(COLORREF colText);
	//正常文本
	void SetSelectText(COLORREF colText);
	//失效文本
	void SetDisableText(COLORREF colText);

	//边框颜色
public:
	//正常状态
	void SetFrameColor(COLORREF colFrame);
	//正常状态
	void SetBackColor(COLORREF colBack);

	//设置函数
public:
	//
	void SetUnTransparent();

	//滚动条
public:
	//加载资源
	BOOL SetScrollImage(CWnd * pOwn,LPCTSTR pszFileName);
	//
	void RemoveScorll();
};

#endif