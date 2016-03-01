#include "stdafx.h"
#include "Function.h"

 SKINUI_CLASS void WINAPI OutputString(LPCTSTR lpStr, ...)
 {
 	CString StrDes;
 
 	va_list argList;
 	va_start( argList, lpStr );
 	StrDes.FormatV( lpStr, argList );
 	va_end( argList );
 
 	OutputDebugString(StrDes);
 }
