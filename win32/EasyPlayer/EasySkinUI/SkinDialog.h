#pragma once


#include "SkinUI.h"
#include "ISkinControl.h"


class SKINUI_CLASS CSkinDialog : public CDialog,public ISkinControl,public CBuildDialog
{
	DECLARE_DYNAMIC(CSkinDialog)

protected:
	ISkinControl				m_SkinControl;
	HDC							m_hMemDC;
	HBITMAP						m_hMemBmp, m_hOldBmp;
	CRect						m_rcWindowSizeNow;

public:
	CSkinDialog(UINT nIDTemplate,LPCTSTR lpszFileName=NULL,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSkinDialog();

	//接口函数
protected:
	//初始化
	virtual BOOL OnInitDialog();
	//消息循环
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//窗口绘制
	virtual void DrawClientArea(CDC*pDC,int nWidth,int nHeight){}
	// DDX/DDV 支持
	virtual void DoDataExchange(CDataExchange* pDX);    

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	//设置拉伸
	void SetExtrude(bool bExtrude);
	//最大化窗口
	void SetMaxWindow();

	//功能函数
public:
	//获取背景
	HDC GetBackDC(){ return m_hMemDC; }
	//剪辑子窗口
	void SetClipChild(bool bClip);

	//消息函数
public:
	//绘画消息
	afx_msg void OnPaint();
	//点击消息
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//窗口尺寸
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//窗口大小
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	//刷新背景
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//裁剪边框
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	//非客户区激活
	afx_msg BOOL OnNcActivate(BOOL bActive);
	//系统消息
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	//静态函数
protected:
	//枚举函数
	static BOOL CALLBACK EnumChildProc(HWND hWndChild, LPARAM lParam);

protected:
	//调整控件
	void RectifyControl( int cx, int cy );

	DECLARE_MESSAGE_MAP()
};
