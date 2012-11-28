#include "StdAfx.h"
#include "StatMd5Dir.h"
#include <io.h>
CStatMd5Dir::CStatMd5Dir(void)
{


}

CStatMd5Dir::~CStatMd5Dir(void)
{

}






bool CStatMd5Dir::ProcessFile(const char *filename)
{
	WIN32_FIND_DATA ffd ;
	_finddata_t fileinfo; 
	long hFile  = _findfirst(filename,&fileinfo);
	
	//cout<<"fileinfo.name "<<fileinfo.name<<" fileinfo.size "<<fileinfo.size<<endl;
	string szfilename(filename);

	//cout<<szfilename << "  " <<GetMd5Value(szfilename.c_str())<<endl;
	cout<<"deal with "<<szfilename<<endl;
	char szbuf[MAX_PATH]={0};
	LPSERVER_XML_INFO lpServerXmlInfo = new SERVER_XML_INFO;
	memset(lpServerXmlInfo,0,sizeof(SERVER_XML_INFO));
	string szFilename(filename);
	
	lpServerXmlInfo->fileName = string_replace(szFilename,m_currDirectory,"").substr(1);
	lpServerXmlInfo->url = URL + lpServerXmlInfo->fileName;
	lpServerXmlInfo->md5 = GetMd5Value(szfilename.c_str());
	lpServerXmlInfo->size =fileinfo.size;
	lpServerXmlInfo->needRestart = true;

	m_fileLst.push_back(lpServerXmlInfo);

	
	return CBrowseDir::ProcessFile(filename);

}

//函数BrowseDir每进入一个目录,就调用ProcessDir 
//并把正在处理的目录名及上一级目录名作为参数传递过去 
//如果正在处理的是初始目录,则parentdir=NULL  
//用户可以覆写该函数,加入自己的处理代码   
//比如用户可以在这里统计子目录的个数 
void CStatMd5Dir::ProcessDir(const char *currentdir,const char *parentdir)
{
	//SetInitDir(parentdir);
	SetCurrentDirectoryA(parentdir);
	return CBrowseDir::ProcessDir(currentdir,parentdir);
}

bool CStatMd5Dir::SetInitDir(const char *dir)
{
	m_currDirectory = dir;
	return CBrowseDir::SetInitDir(dir);
}


string CStatMd5Dir::GetMd5Value(const char* file)
{
	//SetCurrentDirectoryA("E:\\yirong_Workspaces\\updateproject\\BuildMd5File");
	char szTmp[MAX_PATH]={0};
	GetCurrentDirectoryA(MAX_PATH ,szTmp);
	SetCurrentDirectoryA(m_currDirectory.c_str());
	string strCmd="fciv.exe -md5 ";
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead,&hWrite,&sa,0)) {
	//	MessageBox("Error On CreatePipe()");
		return NULL;
	}
	strCmd += file;
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfoA(&si);
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	wstring wsCmd = s2ws(strCmd);
	wsCmd +=L"\r\n";
	TRACE(wsCmd .c_str());
	char szbuf[MAX_PATH]={0};
	strcat(szbuf,strCmd.c_str());
	if (!CreateProcessA(NULL,(char*)strCmd.c_str()
		,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)) {
			DWORD dwErr=GetLastError();
			return NULL;
	}
	if(WaitForSingleObject(pi.hProcess, INFINITE)==WAIT_FAILED)
	{
//		MessageBox("Failed!");
		return NULL;
	}
	CloseHandle(hWrite);
	char buffer[4096] ={0} ;
	DWORD bytesRead;
	string strResult="";
	while(1)
	{
		if(ReadFile(hRead,buffer,4096,&bytesRead,NULL) != NULL)
		{
			strResult += buffer;
		}
		else
		{
			break;
		}
	}
	
	strResult = strResult.substr(59);
	int nPos = strResult.find(" ");
	if (nPos>=0)
	{
		strResult = strResult.substr(0,nPos);
	}
	SetCurrentDirectoryA(szTmp);	
	return strResult;
	
	return "";
}

void CStatMd5Dir::BuildXmlInfo()
{
	SetCurrentDirectoryA(m_currDirectory.c_str());
	FILE* pFile = NULL;
	pFile = fopen("updatexml.xml","wb");
	if (!pFile)
		return;
	m_Xmlbuf = m_Xmlbuf + XMLHEAD + NEWLINEA;
	m_Xmlbuf = m_Xmlbuf + SERVER_XML_BEGIN_SERVER_FILES + NEWLINEA;
	list<LPSERVER_XML_INFO>::iterator Iter;
	char szbuf[MAX_PATH]= {0};
	for (Iter = m_fileLst.begin();Iter!=m_fileLst.end();++Iter)
	{
		LPSERVER_XML_INFO lpFileInfo = (LPSERVER_XML_INFO)*Iter;
		sprintf(szbuf,SERVER_XML_SERVER_FILES_INFO,
			lpFileInfo->fileName.c_str(),
			lpFileInfo->url.c_str(),
			lpFileInfo->lastVersion.c_str(),
			lpFileInfo->size,
			lpFileInfo->needRestart,
			lpFileInfo->md5.c_str());
		m_Xmlbuf = m_Xmlbuf + szbuf + NEWLINEA;
		ZeroMemory(szbuf,MAX_PATH);
	}
	
	m_Xmlbuf = m_Xmlbuf + SERVER_XML_END_SERVER_FILES + NEWLINEA; 
	fwrite(m_Xmlbuf.c_str(),sizeof(char),m_Xmlbuf.length(),pFile);
	fclose(pFile);
	
}