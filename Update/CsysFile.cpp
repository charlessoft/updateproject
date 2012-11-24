#include "StdAfx.h"
#include "stdString.h"
#include "CsysFile.h"
#include <io.h>

CsysFile::CsysFile(void)
{
}

CsysFile::~CsysFile(void)
{

}

BOOL CsysFile::CopyW(wstring sourceFileName, wstring destFileName,BOOL overwrite/*=TRUE*/)
{
	return CopyFileW(sourceFileName.c_str(),destFileName.c_str(),!overwrite);
}

BOOL CsysFile::CopyA(string sourceFileName, string destFileName,BOOL overwrite/*=TRUE*/)
{
	wstring wsSourceFileName = s2ws(sourceFileName);
	wstring wsDestFileName= s2ws(destFileName);
	return CopyW(wsSourceFileName,wsDestFileName,overwrite);
}

BOOL CsysFile::DeleteA(string path)
{
	return DeleteFileA(path.c_str());
}

BOOL CsysFile::ExistsA(string path)
{
	if (_access(path.c_str(),0)!=-1)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CsysFile::MoveA(string sourceFileName, string destFileName)
{
	return MoveFileExA(sourceFileName.c_str(), destFileName.c_str(),MOVEFILE_REPLACE_EXISTING |MOVEFILE_COPY_ALLOWED);
}

BOOL CsysFile::MoveW(wstring sourceFileName, wstring destFileName)
{
	return MoveFileExW(sourceFileName.c_str(), destFileName.c_str(),MOVEFILE_REPLACE_EXISTING |MOVEFILE_COPY_ALLOWED);
}

DWORD CsysFile::SysGetlastError()
{
	return GetLastError();
}

string CsysFile::ErrMsgA(DWORD dwErr)
{
	string strMsg;
	CGlobalFunction::AfxFormatMessageA(dwErr,strMsg);
	return strMsg;
}