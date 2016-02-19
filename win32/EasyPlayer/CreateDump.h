#ifndef __CREATE_DUMP_H__
#define __CREATE_DUMP_H__

#include <winsock2.h>
#include <DbgHelp.h>
#pragma comment(lib,"DbgHelp.lib")




void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException);










#endif
