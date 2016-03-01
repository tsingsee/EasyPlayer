#ifndef SKIN_MENU_HEAD_FILE
#define SKIN_MENU_HEAD_FILE

#pragma once

#include "SkinUI.h"
#include "ISkinControl.h"
//////////////////////////////////////////////////////////////////////////////////

//子项类型
enum enMenuItemType
{
	MenuItemType_String,			//字符类型
	MenuItemType_Separator,			//拆分类型
};

//////////////////////////////////////////////////////////////////////////////////

//菜单子项
class CSkinMenuItem
{
	//变量定义
public:
	const enMenuItemType			m_MenuItemType;						//子项类型
	HMENU							m_hSubMenu;							//子菜单

	//函数定义
public:
	//构造函数
	CSkinMenuItem(enMenuItemType MenuItemType) : m_MenuItemType(MenuItemType) {m_hSubMenu = NULL;}
	//析构函数
	virtual ~CSkinMenuItem() {}
};

//字符菜单
class CSkinMenuString : public CSkinMenuItem
{
	//变量定义
public:
	CString							m_strString;						//菜单字符
	CImageEx						*m_pImageN;							//图像句柄
	CImageEx						*m_pImageH;							//图像句柄

	//函数定义
public:
	//构造函数
	CSkinMenuString() : CSkinMenuItem(MenuItemType_String) {m_pImageN=m_pImageH=NULL;}
	//析构函数
	virtual ~CSkinMenuString() {}
};

//拆分菜单
class CSkinMenuSeparator : public CSkinMenuItem
{
	//函数定义
public:
	//构造函数
	CSkinMenuSeparator() : CSkinMenuItem(MenuItemType_Separator) {}
	//析构函数
	virtual ~CSkinMenuSeparator() {}
};

//////////////////////////////////////////////////////////////////////////////////

//数组说明
typedef CArray<CSkinMenuItem *,CSkinMenuItem *>			CMenuItemArray;					//子项数组
typedef CArray<CSkinMenuString *,CSkinMenuString *>		CMenuStringArray;				//子项数组
typedef CArray<CSkinMenuSeparator *,CSkinMenuSeparator *>	CMenuSeparatorArray;			//子项数组

//////////////////////////////////////////////////////////////////////////////////

//菜单内核
class SKINUI_CLASS CSkinUIMenu : public CMenu,public ISkinControl
{
	//图片资源
protected:
	CImageEx						*m_pMenuBar;
	CImageEx						*m_pMenuBack;
	CImageEx						*m_pMenuHoven;
	CImageEx						*m_pSeparator;
	CImageEx						*m_pCheck;
	CImageEx						*m_pArrow;

	//内核变量
protected:
	CMenuItemArray					m_MenuItemActive;					//子项数组
	
	//静态变量
protected:
	static CMenuStringArray			m_MenuItemString;					//字符子项
	static CMenuSeparatorArray		m_MenuItemSeparator;				//拆分子项
	static HHOOK					m_hMenuHook;

protected:
	COLORREF						m_crSeparator;
	int								m_nItemHeight;

	//函数定义
public:
	//构造函数
	CSkinUIMenu();
	//析构函数
	virtual ~CSkinUIMenu();

	//重载函数
public:
	//绘画函数
	virtual VOID DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//测量位置
	virtual VOID MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	//管理函数
public:
	//创建菜单
	bool CreatePopupMenu();
	//销毁菜单
	BOOL DestroyMenu();
	//弹出菜单
	bool TrackPopupMenu(CWnd * pWnd);
	//弹出菜单
	bool TrackPopupMenu(INT nXPos, INT nYPos, CWnd * pWnd);

	//资源设置
public:
	//设置资源
	BOOL SetMenuBarImage(LPCTSTR lpszFileName,CONST LPRECT lprcNinePart=NULL);
	//设置资源
	BOOL SetMenuBackImage(LPCTSTR lpszFileName,CONST LPRECT lprcNinePart=NULL);
	//设置资源
	BOOL SetMenuHovenImage(LPCTSTR lpszFileName,CONST LPRECT lprcNinePart=NULL);
	//设置资源
	BOOL SetSeparatorImage(LPCTSTR lpszFileName,CONST LPRECT lprcNinePart=NULL);
	//设置资源
	BOOL SetCheckImage(LPCTSTR lpszFileName);
	//设置资源
	BOOL SetArrowImage(LPCTSTR lpszFileName);
	//设置高度
	void SetItemHeight(int nHeight){ m_nItemHeight = nHeight; }

	//增加函数
public:
	//插入拆分
	bool AppendSeparator();
	//插入字符
	bool AppendMenu(UINT nMenuID, LPCTSTR pszString, LPCTSTR lpszIconN=NULL, LPCTSTR lpszIconH=NULL, UINT nFlags=0);

	//插入函数
public:
	//插入拆分
	bool InsertSeparator(UINT nPosition);
	//插入字符
	bool InsertMenu(UINT nMenuID, LPCTSTR pszString, UINT nPosition,  LPCTSTR lpszIconN=NULL, LPCTSTR lpszIconH=NULL, UINT nFlags=0);

	//修改函数
public:
	//删除菜单
	bool RemoveMenu(UINT nPosition, UINT nFlags);
	//修改菜单
	bool ModifyMenu(UINT nMenuID, LPCTSTR pszString, UINT nPosition,  LPCTSTR lpszIconN=NULL, LPCTSTR lpszIconH=NULL, UINT nFlags=0);

	//内部函数
private:
	//拆分数量
	int GetSeparatorCount();
	//释放子项
	VOID FreeMenuItem(CSkinMenuItem * pSkinMenuItem);
	//获取子项
	CSkinMenuItem * AcitveMenuItem(enMenuItemType MenuItemType);

	static LRESULT CALLBACK WindowsHook(int code,WPARAM wParam,LPARAM lParam);

};

//////////////////////////////////////////////////////////////////////////////////

#endif