#ifndef __TRACE_H__
#define __TRACE_H__

#ifdef _WIN32
#include <winsock2.h>
#endif



void _TRACE(char* szFormat, ...);
void _TRACE_W(const wchar_t * szFormat, ...);

bool WCharToMByte(LPCWSTR lpcwszStr, LPSTR lpszStr, DWORD dwSize);
bool MByteToWChar(LPCSTR lpcszStr, LPWSTR lpwszStr, DWORD dwSize);

#endif
