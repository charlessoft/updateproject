#include "StdAfx.h"
#include "stdString.h"
#include "Environment.h"


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
	GetCurrentDirectoryW(MAX_PATH,wsbuf);
	wsCurrentDirectory = wsbuf;
//	wsCurrentDirectory += L"\\";
	return wsCurrentDirectory;
}