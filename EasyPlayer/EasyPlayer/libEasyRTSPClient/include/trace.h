/*
	Copyright (c) 2013-2015 EasyDarwin.ORG.  All rights reserved.
	Github: https://github.com/EasyDarwin
	WEChat: EasyDarwin
	Website: http://www.easydarwin.org
*/
#ifndef __TRACE_H__
#define __TRACE_H__

#ifdef _WIN32
#include <winsock2.h>
#else
#define MAX_PATH 260
#endif


void _TRACE_OpenLogFile(char *filenameprefix);
void _TRACE_CloseFile();

void _TRACE(char* szFormat, ...);
void _TRACE_W(const wchar_t * szFormat, ...);
#ifdef _WIN32
bool WCharToMByte(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize);
bool MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize);
#endif
#endif
