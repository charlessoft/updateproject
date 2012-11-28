#pragma once

#include <string>
using namespace std;
std::string ws2s(const std::wstring& ws);
std::wstring s2ws(const std::string& s);
void trimstr(wstring& str);
void trimstr(string& str);
void trimstr(string& str,char value);
void trimstr(wstring& str,wchar_t value);
void string_replace(std::string& strBig, const std::string & strsrc, const std::string &strdst);
void string_replace(std::wstring& strBig, const std::wstring & strsrc, const std::wstring &strdst);
