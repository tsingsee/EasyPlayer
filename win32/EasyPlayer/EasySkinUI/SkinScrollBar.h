#ifndef SCROLLBAR_HEAD_FILE
#define SCROLLBAR_HEAD_FILE

#pragma once

#include "SkinUI.h"


#define UISTATE_FOCUSED      0x00000001
#define UISTATE_SELECTED     0x00000002
#define UISTATE_DISABLED     0x00000004
#define UISTATE_HOT          0x00000008
#define UISTATE_PUSHED       0x00000010
#define UISTATE_READONLY     0x00000020
#define UISTATE_CAPTURED     0x00000040

//////////////////////////////////////////////////////////////////////////

class SKINUI_CLASS CSkinScrollBar : public CScrollBar
{
protected:
	HWND				m_hOwnerWnd;
	UINT				m_nCtrlId;
	CRect				m_rcScrollBar;
	BOOL				m_bHorizontal;
	BOOL				m_bVisible;
	BOOL				m_bEnabled;

	int					m_nRange;
	int					m_nScrollPos;
	int					m_nLineSize;

	POINT				m_ptLastMouse;
	int 				m_nLastScrollPos;
	int 				m_nLastScrollOffset;
	int 				m_nScrollRepeatDelay;
	DWORD m_dwUseTimerId, m_dwTimerId;
	BOOL 				m_bMouseHover;
	BOOL 				m_bCaptured;
	BOOL 				m_bMouseTracking;

	CImageEx 			* m_lpBgImgN;
	CImageEx 			* m_lpBgImgH;
	CImageEx 			* m_lpBgImgP;
	CImageEx 			* m_lpBgImgD;

	BOOL				m_bShowLeftUpBtn;
	CRect 				m_rcLeftUpBtn;
	DWORD 				m_dwLeftUpBtnState;
	CImageEx 			* m_lpLeftUpBtnImgN;
	CImageEx 			* m_lpLeftUpBtnImgH;
	CImageEx 			* m_lpLeftUpBtnImgP;
	CImageEx 			* m_lpLeftUpBtnImgD;

	BOOL				m_bShowRightDownBtn;
	CRect 				m_rcRightDownBtn;
	DWORD 				m_dwRightDownBtnState;
	CImageEx 			* m_lpRightDownBtnImgN;
	CImageEx 			* m_lpRightDownBtnImgH;
	CImageEx 			* m_lpRightDownBtnImgP;
	CImageEx 			* m_lpRightDownBtnImgD;

	CRect 				m_rcThumb;
	DWORD 				m_dwThumbState;
	CImageEx 			* m_lpThumbImgN;
	CImageEx 			* m_lpThumbImgH;
	CImageEx 			* m_lpThumbImgP;
	CImageEx 			* m_lpThumbImgD;

	CImageEx 			* m_lpRailImgN;
	CImageEx 			* m_lpRailImgH;
	CImageEx 			* m_lpRailImgP;
	CImageEx 			* m_lpRailImgD;


public:
	CSkinScrollBar();
	virtual ~CSkinScrollBar();

public:
	void DrawArrow(UINT uArrow,int nState);
	void SetBitmap(HBITMAP hBmp);
	BOOL SetBackImage(LPCTSTR lpszFileName);
	BOOL IsVertical();

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void DrawThumb(CDC *pDestDC, RECT *pDestRect, CDC *pSourDC, RECT *pSourRect);
	void TileBlt(CDC *pDestDC,RECT *pDestRect,CDC *pSourDC,RECT *pSourRect);
	RECT GetRect(UINT uSBCode);
	RECT GetImageRect(UINT uSBCode,int nState=0);
	UINT HitTest(CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};

#endif
