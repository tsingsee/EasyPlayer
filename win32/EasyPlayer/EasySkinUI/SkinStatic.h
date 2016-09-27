#pragma once

#include "SkinUI.h"
#include "ISkinControl.h"

class SKINUI_CLASS CSkinStatic : public CStatic,public ISkinControl
{
// Construction
public:
	CSkinStatic();

// Attributes
public:
private:
	CImageEx * m_pBackImgN, * m_pBackImgH, * m_pBackImgD, * m_pBackImgF;
	CString m_strCaption;
// Operations
public:

	void SetDrawRectangle(bool bDraw);
	int DrawRectangle(CPaintDC *dc);

	void SetWindowText(LPCSTR lpcString);
	CString GetWindowText();
	int GetStringSize(CString strMsg);
	void SetTextFont(CFont& font,BOOL bBold=FALSE);
	void SetBackFlush(bool bValide);
	
	void DrawText(CDC* pDC,RECT &rcClient);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSkinStatic();

	// Generated message map functions
protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

	//{{AFX_MSG(CStaticEx)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	virtual BOOL CreateControl(CWnd* pParentWnd);
};
