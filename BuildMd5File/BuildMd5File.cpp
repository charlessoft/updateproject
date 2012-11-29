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


CONFIGINFO g_config;

void FormatConfigInfo(string& pData)
{
	int nPos = pData.rfind('#');
	if (nPos>=0)
	{
		pData=	pData.substr(0,nPos);
	}
	trimstr(pData);

}

void GetConfigInfo(CONFIGINFO* pConfigInfoStruct )
{
	string szConfinIniPath;
	CEnvironment::Env_GetCurrentDirectory(szConfinIniPath);
	szConfinIniPath = szConfinIniPath + "\\" + UPDATECONFIGA;
	
	if (_access(szConfinIniPath.c_str(),0)==-1)
	{
		printf("没有找到配置文件,退出");
		//g_Logger.Error(__FILE__,__LINE__,"没有找到配置文件,退出");
		exit(0);
	}
	char url[MAX_PATH]={0};
	GetPrivateProfileStringA(OPTION,SERVERADDRESS,"",url,MAX_PATH,szConfinIniPath.c_str());
	pConfigInfoStruct->url=url;
	FormatConfigInfo(pConfigInfoStruct->url);
	printf("pConfigInfoStruct->url %s\r\n",pConfigInfoStruct->url.c_str());
	
}


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
		
		memset(&g_config,0,sizeof(CONFIGINFO));
		GetConfigInfo(&g_config);
		CStatMd5Dir m_md5file;
	
		string strCurDirectory;
		CEnvironment::Env_GetCurrentDirectory(strCurDirectory);
		printf("Env_GetCurrentDirectory %s\r\n",strCurDirectory.c_str());
		m_md5file.SetInitDir(strCurDirectory.c_str());
		m_md5file.BeginBrowse("*.*");
		m_md5file.BuildXmlInfo();
	}
	//system("pause");
	return nRetCode;
}
