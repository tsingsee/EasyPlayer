#pragma once

#include "ImageEx.h"

// CMainVideoWnd 对话框

class CMainVideoWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CMainVideoWnd)

public:
	CMainVideoWnd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainVideoWnd();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	//IDB_PNG_EASYLOGO
	CImageEx	* m_pEasyLogo;

public:
	virtual BOOL OnInitDialog();
};
