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


void   filesearch(string path,int layer)   
{   
	struct _finddata_t filefind;
	string curr=path+"\\*.*";
	int   done=0,i,handle;   
	if((handle=_findfirst(curr.c_str(),&filefind))==-1)
		return; 

	while(!(done=_findnext(handle,&filefind)))   
	{   
		printf("--%s\n",filefind.name);
		if(!strcmp(filefind.name,"..")){

			continue;
		}

		for(i=0;i<layer;i++)
			cout<<"     "; 




		if   ((_A_SUBDIR==filefind.attrib))   
		{           
			printf("----------%s\n",filefind.name);    
			
			cout<<filefind.name<<"(dir)"<<endl;   
			curr=path+"\\"+filefind.name;   
			filesearch(curr,layer+1);   
		}   
		else     
		{   
			cout<<filefind.name<<endl;   
		}   
	}           
	_findclose(handle);               
} 

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
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
