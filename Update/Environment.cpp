#include "StdAfx.h"
#include "stdString.h"
#include "Environment.h"


CEnvironment::CEnvironment(void)
{
}

CEnvironment::~CEnvironment(void)
{
}


void CEnvironment::Env_GetCurrentDirectoryA(string &szCurrentDirectory)
{
	wstring wsCurrentDirectory;
	Env_GetCurrentDirectoryW(wsCurrentDirectory);
	szCurrentDirectory = ws2s(wsCurrentDirectory);
}

void CEnvironment::Env_GetCurrentDirectoryW(wstring &wsCurrentDirectory)
{
	WCHAR wsbuf[MAX_PATH]={0};
	GetCurrentDirectoryW(MAX_PATH,wsbuf);
	wsCurrentDirectory = wsbuf;
	
}