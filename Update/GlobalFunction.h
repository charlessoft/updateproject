#pragma once

class CGlobalFunction
{
public:
	CGlobalFunction(void);
public:
	~CGlobalFunction(void);
	static void AfxFormatMessageA(DWORD error,string& strFormatMsg);
	static void AfxFormatMessageW(DWORD error,wstring& wsFormatMsg);
};
