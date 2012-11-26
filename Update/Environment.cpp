#include "StdAfx.h"
#include "stdString.h"
#include "Environment.h"


CEnvironment::CEnvironment(void)
{
}

CEnvironment::~CEnvironment(void)
{
}


string CEnvironment::Env_GetCurrentDirectoryA(string &szCurrentDirectory)
{
	wstring wsCurrentDirectory;
	Env_GetCurrentDirectoryW(wsCurrentDirectory);
	szCurrentDirectory = ws2s(wsCurrentDirectory);
	return szCurrentDirectory;
}

wstring CEnvironment::Env_GetCurrentDirectoryW(wstring &wsCurrentDirectory)
{
	WCHAR wsbuf[MAX_PATH]={0};
	GetCurrentDirectoryW(MAX_PATH,wsbuf);
	wsCurrentDirectory = wsbuf;
	return wsCurrentDirectory;
}