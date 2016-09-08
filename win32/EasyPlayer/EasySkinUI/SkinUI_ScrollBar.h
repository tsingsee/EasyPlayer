/////////////////////////////////////////////////////////////////
//
// Header file : SkinUI.h
//
// Descrption  : Skin scrollbar library Version 1.0
//
/////////////////////////////////////////////////////////////////

#pragma once
#include <windows.h>

// #ifdef __cplusplus
// extern "C"{
// #endif


//----------------------------------------------------------------
// Name : SkinUI_Init()
// Desc : Initialize and load the scrollbar skin
//----------------------------------------------------------------
BOOL WINAPI SkinUI_Init(HWND hwnd, HBITMAP hBmp);


//----------------------------------------------------------------
// Name : SkinUI_Uninit()
// Desc : Remove the scrollbar skin
//----------------------------------------------------------------
BOOL WINAPI SkinUI_Uninit(HWND hwnd);


//----------------------------------------------------------------
// Name : SkinUI_IsValid()
// Desc : Whether initialied ?
//----------------------------------------------------------------
BOOL WINAPI SkinUI_IsValid(HWND hwnd);


//----------------------------------------------------------------
// Name : SkinUI_GetScrollInfo()
// Desc : Gets the information for a skin scroll bar
//----------------------------------------------------------------
BOOL WINAPI SkinUI_GetScrollInfo(HWND hwnd, int fnBar, LPSCROLLINFO lpsi);


//----------------------------------------------------------------
// Name : SkinUI_SetScrollInfo()
// Desc : Sets the information for a skin scroll bar
//---------------------------------------------------------------- 
int  WINAPI SkinUI_SetScrollInfo(HWND hwnd, int fnBar, LPCSCROLLINFO psi, BOOL fRedraw);


//----------------------------------------------------------------
// Name : SkinUI_GetScrollPos()
// Desc : Gets the thumb position in a skin scroll bar
//----------------------------------------------------------------
int  WINAPI SkinUI_GetScrollPos(HWND hwnd, int fnBar);


//----------------------------------------------------------------
// Name : SkinUI_SetScrollPos()
// Desc : Sets the current position of the thumb in a skin scroll bar
//----------------------------------------------------------------
int  WINAPI SkinUI_SetScrollPos(HWND hwnd, int nBar, int nPos, BOOL fRedraw);


//----------------------------------------------------------------
// Name : SkinUI_GetScrollRange()
// Desc : Gets the scroll range for a skin scroll bar
//----------------------------------------------------------------
BOOL WINAPI SkinUI_GetScrollRange(HWND hwnd, int nBar, LPINT lpMinPos, LPINT lpMaxPos);


//----------------------------------------------------------------
// Name : SkinUI_SetScrollRange()
// Desc : Sets the scroll range of a skin scroll bar
//----------------------------------------------------------------
BOOL WINAPI SkinUI_SetScrollRange(HWND hwnd, int nBar, int nMinPos, int nMaxPos, BOOL fRedraw);


//----------------------------------------------------------------
// Name : SkinUI_ShowScrollBar()
// Desc : Shows or hides a skin scroll bar
//----------------------------------------------------------------
BOOL WINAPI SkinUI_ShowScrollBar(HWND hwnd, int wBar, BOOL fShow);


//----------------------------------------------------------------
// Name : SkinUI_EnableScrollBar()
// Desc : Enables or disables one or both skin scroll bar direction buttons
//----------------------------------------------------------------
BOOL WINAPI SkinUI_EnableScrollBar(HWND hwnd, UINT wSBflags, UINT wArrows);


// #ifdef __cplusplus
// }
// #endif
