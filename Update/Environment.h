#pragma once
#include <string>
using namespace std;
class CEnvironment
{
public:
	CEnvironment(void);
	static	string Env_GetCurrentDirectoryA(string &szCurrentDirectory);
	static	wstring Env_GetCurrentDirectoryW(wstring &wsCurrentDirectory);
public:
	~CEnvironment(void);
};
