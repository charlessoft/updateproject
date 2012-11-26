#include "StdAfx.h"
#include "stdString.h"
#include "CsysPath.h"

CsysPath::CsysPath(void)
{
}

CsysPath::~CsysPath(void)
{
}

string CsysPath::GetDirectoryName(string path)
{
	wstring wsPath = s2ws(path);
	return ws2s(GetDirectoryName(wsPath));
	//int nPos = path.rfind('\\');
	//return path.substr(0,nPos);
}
wstring CsysPath::GetDirectoryName(wstring path)
{
	int nPos = path.rfind('\\');
	return path.substr(0,nPos);
}


string CsysPath::ChangeExtension(string path, string extension)
{
	return NULL;
}

wstring CsysPath::ChangeExtension(wstring path, wstring extension)
{
	return NULL;
}

string CsysPath::Combine(string path1, string path2)
{
	return NULL;
}

wstring CsysPath::Combine(wstring path1, wstring path2)
{
	return NULL;
}

string CsysPath::GetExtension(string path)
{
	wstring wsPath = s2ws(path);
	return ws2s(GetExtension(wsPath));
}

wstring CsysPath::GetExtension(wstring path)
{
	int nPos = path.rfind('.');
	return path.substr(nPos);
}



string CsysPath::GetFileName(string path)
{
	wstring wsPath = s2ws(path);
	return ws2s(GetFileName(wsPath));
}

wstring CsysPath::GetFileName(wstring path)
{
	int nPos = path.rfind('\\');
	return path.substr(nPos+1 );
}

string CsysPath::GetFileNameWithoutExtension(string path)
{
	wstring wsPath = s2ws(path);
	return ws2s(GetFileNameWithoutExtension(wsPath));
}

wstring CsysPath::GetFileNameWithoutExtension(wstring path)
{
	int nPos = path.rfind('\\');
	int nPosEntension = path.rfind('.');
	return path.substr(nPos+1, nPosEntension-nPos-1);
}


string CsysPath::GetPathRoot(string path)
{
	wstring wsPath = s2ws(path);
	return ws2s(GetPathRoot(wsPath));
}

wstring CsysPath::GetPathRoot(wstring path)
{
	int nPos = path.find('\\');
	return path.substr(0,nPos+1);
}

string CsysPath::GetRandomFileNameA()
{
	return ws2s(GetRandomFileNameW());

}

wstring CsysPath::GetRandomFileNameW()
{
	WCHAR szTempPath[_MAX_PATH]={0},szTempfile[_MAX_PATH]={0}; 
	::GetTempFileNameW(szTempPath,L"",0,szTempfile); 
	wstring wsRandomFileName(szTempfile+1);
	return wsRandomFileName;
}

string CsysPath::GetTempFileNameA(char* headInfo)
{
	string szHeadInfo(headInfo);
	wstring wsHeadInfo = s2ws(szHeadInfo);
	return ws2s(GetTempFileNameW((wchar_t*)wsHeadInfo.c_str()));
}

wstring CsysPath::GetTempFileNameW(wchar_t* headInfo)
{
	WCHAR szTempPath[_MAX_PATH]={0},szTempfile[_MAX_PATH]={0}; 
	::GetTempPathW(_MAX_PATH, szTempPath); 
	::GetTempFileNameW(szTempPath,headInfo==NULL?L"":headInfo,0,szTempfile); 
	wstring wsRandomFileName(szTempfile);
	return wsRandomFileName;
}

BOOL CsysPath::HasExtension(string path)
{
	wstring wsPath = s2ws(path);
	return HasExtension(wsPath);
}

BOOL CsysPath::HasExtension(wstring path)
{
	//if (path[path.length()-1]=='.' ||)
	int nDosPos = path.rfind('.');
	int nPos = path.rfind('\\');
	if(nDosPos == path.length()-1 || nDosPos < nPos)
	{
		return FALSE;
	}
	return TRUE;
}