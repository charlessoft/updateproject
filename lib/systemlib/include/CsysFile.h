#pragma once

#include "GlobalFunction.h"

class CsysFile
{
public:
	CsysFile(void);
public:
	~CsysFile(void);
	
	static BOOL Copy(string sourceFileName, string destFileName,BOOL overwrite=TRUE);
	static BOOL Copy(wstring sourceFileName, wstring destFileName,BOOL overwrite=TRUE);

	static BOOL Delete(string path);
	static BOOL Exists(string path);

	static BOOL Move(string sourceFileName, string destFileName);
	static BOOL Move(wstring sourceFileName, wstring destFileName);

	static DWORD SysGetlastError();
	static string ErrMsgA(DWORD dwErr);


	
};
