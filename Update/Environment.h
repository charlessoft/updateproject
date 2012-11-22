#pragma once
#include <string>
using namespace std;
class CEnvironment
{
public:
	CEnvironment(void);
	static	void Env_GetCurrentDirectoryA(string &szCurrentDirectory);
	static	void Env_GetCurrentDirectoryW(wstring &wsCurrentDirectory);
public:
	~CEnvironment(void);
};
