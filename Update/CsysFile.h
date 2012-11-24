#pragma once

#include "GlobalFunction.h"

class CsysFile
{
public:
	CsysFile(void);
public:
	~CsysFile(void);
	
	static BOOL CopyA(string sourceFileName, string destFileName,BOOL overwrite=TRUE);
	static BOOL CopyW(wstring sourceFileName, wstring destFileName,BOOL overwrite=TRUE);

	static BOOL DeleteA(string path);
	static BOOL ExistsA(string path);

	static BOOL MoveA(string sourceFileName, string destFileName);
	static BOOL MoveW(wstring sourceFileName, wstring destFileName);

	static DWORD SysGetlastError();
	static string ErrMsgA(DWORD dwErr);


	
};
