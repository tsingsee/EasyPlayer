#pragma once

#include "SkinUI.h"
#include "ISkinControl.h"

class SKINUI_CLASS CSkinEdit : public CEdit,public ISkinControl
{
	DECLARE_DYNAMIC(CSkinEdit)
	
	//资源定义
protected:
	CImageEx 						* m_pBackImgN;			//正常资源
	CImageEx 						* m_pBackImgH;			//高亮资源
	CImageEx 						* m_pIconImg;			//图标资源

	BOOL m_bFocus, m_bPress, m_bHover, m_bMouseTracking;	//状态变量
	int								m_nIconWidth;			//图标宽度
	bool							m_bHandCursor;			//手型鼠标
	CString							m_strDefText;			//默认文本
	BOOL							m_bIsDefText;			//默认状态
	TCHAR							m_cPwdChar;				//密码字符
	CPoint							m_ptClient;				//客户区

public:
	CSkinEdit();
	virtual ~CSkinEdit();

	//资源加载
public:
	//正常背景
	BOOL SetBackNormalImage(LPCTSTR lpszFileName, CONST LPRECT lpNinePart = NULL);
	//高亮背景
	BOOL SetBackHotImage(LPCTSTR lpszFileName,CONST LPRECT lpNinePart = NULL);
	//图标资源
	BOOL SetIconImage(LPCTSTR lpszFileName,bool bHandCursor = false);

	//设置函数
public:
	//默认文本
	void SetDefaultText(LPCTSTR lpszText);
	//默认模式
	void SetDefaultTextMode(BOOL bIsDefText);
	//图标位置
	void GetIconRect( RECT &lprcIcon );
	//客户区起始坐标
	void SetClientPoint(CPoint pt);

	//重载函数
public:
	//创建窗口
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	//设置属性
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	//设置属性
	virtual void ParseItem(CXmlNode *root);
	//创建控件
	virtual BOOL CreateControl(CWnd* pParentWnd);

	//消息映射
public:
	//计算大小
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	//重绘非客户区
	afx_msg void OnNcPaint();
	//重绘背景
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//左键按下
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//左键抬起
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//鼠标移动
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//鼠标离开
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	//控件销毁
	afx_msg void OnDestroy();
	//获取焦点
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//失去焦点
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//鼠标样式
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//颜色反映射
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	//点击测试
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//非客户区左键抬起
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);

protected:
	DECLARE_MESSAGE_MAP()
};



//////////////////////////////////////////////////////////////////////////
class SKINUI_CLASS CMultiSkinEdit : public CSkinEdit 
{
	DECLARE_DYNAMIC(CMultiSkinEdit)

public:
	CMultiSkinEdit(){}
	virtual ~CMultiSkinEdit(){}

	//重载函数
public:
	//创建控件
	virtual BOOL CreateControl(CWnd* pParentWnd);

protected:
	DECLARE_MESSAGE_MAP()
};