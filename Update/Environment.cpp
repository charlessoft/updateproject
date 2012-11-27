#include "StdAfx.h"
#include "stdString.h"
#include "Environment.h"
#include "GlobalFunction.h"


CEnvironment::CEnvironment(void)
{
}

CEnvironment::~CEnvironment(void)
{
}


string CEnvironment::Env_GetCurrentDirectory(string &szCurrentDirectory)
{
	wstring wsCurrentDirectory;
	Env_GetCurrentDirectory(wsCurrentDirectory);
	szCurrentDirectory = ws2s(wsCurrentDirectory);
	return szCurrentDirectory;
}

wstring CEnvironment::Env_GetCurrentDirectory(wstring &wsCurrentDirectory)
{
	WCHAR wsbuf[MAX_PATH]={0};
	DWORD dwErr = GetCurrentDirectoryW(MAX_PATH,wsbuf);
	if (0 == dwErr)
	{
		dwErr = GetLastError();
		string strMsg;
		CGlobalFunction::AfxFormatMessageA(dwErr,strMsg);
	}
	wsCurrentDirectory = wsbuf;
//	wsCurrentDirectory += L"\\";
	return wsCurrentDirectory;
}