#include "StdAfx.h"
#include "ConfigMgr.h"
#include "stdString.h"
#include "Environment.h"

CConfigMgr::CConfigMgr(void)
{
	memset(&m_ConfigInfo,0,sizeof(CONFIGINFO));
}

CConfigMgr::~CConfigMgr(void)
{
}

void CConfigMgr::ReadConfig()
{
	GetConfigInfo(&m_ConfigInfo);
}
void CConfigMgr::GetConfigInfo(CONFIGINFO* pConfigInfoStruct )
{
	//char szbuf[1024]={0};
	//char szConfinIniPath[1024]={0};
	//sprintf_s(szConfinIniPath,1024,"%s\\%s",m_curPath.c_str(),OEGUICONFIG/*"Config.ini"*/);

	//TCHAR szConfinIniPath[MAX_PATH]={0};
	wstring wsConfinIniPath;
	//::GetCurrentDirectory(MAX_PATH,szConfinIniPath);
	CEnvironment::Env_GetCurrentDirectory(wsConfinIniPath);
	wsConfinIniPath = wsConfinIniPath + L"\\" + UPDATECONFIG;
	/*strcat_s(szConfinIniPath,1024,"\\");
	strcat_s(szConfinIniPath,1024,OEGUICONFIGA);*/
	if (_taccess(wsConfinIniPath.c_str(),0)==-1)
	{
		g_Logger.Error(__FILE__,__LINE__,"没有找到配置文件,退出");
		exit(0);
	}
	TCHAR url[MAX_PATH]={0};
	GetPrivateProfileString(OPTION,SERVERADDRESS,L"",url,MAX_PATH,wsConfinIniPath.c_str());
	pConfigInfoStruct->url= url;
	FormatConfigInfo(pConfigInfoStruct->url);
}
void CConfigMgr::FormatConfigInfo(wstring& pData)
{
	int nPos = pData.rfind('#');
	if (nPos>=0)
	{
		pData=	pData.substr(0,nPos);
	}
	trimstr(pData);

}
void CConfigMgr::FormatConfigInfo(TCHAR* pData)
{
//  	TCHAR* pPosChar = _tcsstr(pData,"#");
//  	if(pPosChar)
//  	{
//  		pData[pPosChar-pData]='\0';
//  	}
	
	//trimws(pData);
}