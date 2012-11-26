#pragma once
#include <string>
using namespace std;
class CEnvironment
{
public:
	CEnvironment(void);
	static	string Env_GetCurrentDirectory(string &szCurrentDirectory);
	static	wstring Env_GetCurrentDirectory(wstring &wsCurrentDirectory);
public:
	~CEnvironment(void);
};
