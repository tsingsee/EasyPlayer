#pragma once

#include "SkinUI.h"

struct tagString 
{
	UINT				uID;
	UINT				uFontIndex;
	COLORREF			colStatic;
	tagPositionData		*pPositionData;
	CString				strText;
	UINT				uFormat;
	bool				bVisible;
	CString				strTextBase;
};

struct tagImage
{
	tagPositionData		*pPositionData;
	UINT				uID;
	CImageEx			*pImage;
	BYTE				cbAlpha;
	DWORD				dwMask;
	int					nType;
	int					nRotation;
	bool				bGray;
	bool				bVisible;
};

//////////////////////////////////////////////////////////////////////////

class SKINUI_CLASS CBuildDialog
{
	friend class CXmlManager;
	friend class ISkinControl;

protected:
	HWND						m_hOwnWnd;
	CXmlManager					*m_pXml;

public:
	vector<ISkinControl*>		m_ControlArray;//控件
	vector<tagString*>			m_StringArray; //文本
	vector<tagImage*>			m_ImageArray;  //图像
	vector<CBuildDialog*>		m_DialogArray;//子窗口指针

	//窗口变量
public:
	CPoint						m_ptWindowPos;				//窗口初始化位置
	CSize						m_szWindowSize;				//窗口大小
	CSize						m_szRcSize;					//圆角大小
	CSize						m_szMinSize;				//最小尺寸
	CSize						m_szMaxSize;				//最大尺寸
	bool						m_bExtrude;					//是否可以拉伸
	bool						m_bClip;
	COLORREF					m_colBK;
	bool						m_bDefaultSkin;		
	
	bool						m_bDragMoving;				//是否可以拖拽移动 标志

public:
	CBuildDialog(void);
	virtual ~CBuildDialog(void);

public:
	//设置句柄
	inline void SetOwnHwnd(HWND hWnd){ m_hOwnWnd = hWnd; }
	//解析窗口
	bool ParseWindow(LPCTSTR lpszFileName);
	//解析控件
	bool ParseControl(CWnd* pParentWnd);
	//解析图形
	bool ParseGraphics();
	//默认皮肤
	void SetDefaultSkin(bool bDefault){ m_bDefaultSkin = bDefault; }

	//String函数
public:
	int AddString(LPCTSTR lpszText,UINT uFontIndex,UINT uFormat,COLORREF col,bool bVisible,tagPositionData *pPositionData,UINT uID);
	tagPositionData *GetStringRect(UINT uID);
	tagString *GetString(UINT uID);
	// 增加配置字串可编辑编辑 [3/4/2016 Dingshuai]
	void SetString( UINT uID, CString strText);

	//Image函数
public:
	int AddImage(LPCTSTR lpszFileName,BYTE dwAlpha,DWORD dwMask,int nType,int nRotation,bool bGray,bool bVisible,tagPositionData *pPositionData,UINT uID);
	tagPositionData *GetImageRect(UINT uID);
	tagImage *GetImage(UINT uID);

	//绘制函数
public:
	//绘制文字
	void DrawString(CDC*pDC);
	//绘制图像
	void DrawImage(CDC*pDC);

public:
	//用户自定义控件
	virtual bool CreateUserControl(){ return false; }
	void AddDialog(CBuildDialog *pDialog);
	void DeleteDialog(CBuildDialog *pDialog);
};
