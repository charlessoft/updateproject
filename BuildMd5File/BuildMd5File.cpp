// BuildMd5File.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BuildMd5File.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object
#include<iostream>   
#include<string>
#include<io.h> 
#include "StatMd5Dir.h"
CWinApp theApp;

using namespace std;



int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	//_CrtSetBreakAlloc(3158);
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		//filesearch("E:\\yirong_Workspaces\\updateproject\\SystemLib",2);
		CStatMd5Dir m_md5file;
		//m_md5file.SetInitDir("");
		string strCurDirectory;
		CEnvironment::Env_GetCurrentDirectory(strCurDirectory);
		m_md5file.SetInitDir(strCurDirectory.c_str());
		m_md5file.BeginBrowse("*.*");
		m_md5file.BuildXmlInfo();
	}

	return nRetCode;
}
