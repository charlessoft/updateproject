#include "StdAfx.h"
#include "GlobalFunction.h"
#include <string>
using namespace std;
CGlobalFunction::CGlobalFunction(void)
{
}

CGlobalFunction::~CGlobalFunction(void)
{
}



void CGlobalFunction::AfxFormatMessageA(DWORD error,string& strFormatMsg){
	
	//wstring wsFromatMsg;
	//AfxFormatMessageW(error,wsFromatMsg);
	if(error>0){
		LPVOID lpMsgBuf;
		FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,error,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPSTR) &lpMsgBuf,	0,NULL );
		//wsFormatMsg =(LPCSTR)lpMsgBuf;
		//::MessageBoxA(NULL,(LPCSTR)lpMsgBuf,"",0);
		//g_Logger.Error(__FILE__,__LINE__,"error=%ld,Msg=%s",error,(LPCTSTR)lpMsgBuf);
		strFormatMsg = (LPCSTR)lpMsgBuf;
		LocalFree( lpMsgBuf );					
	}
	return;
}

void CGlobalFunction::AfxFormatMessageW(DWORD error,wstring& wsFormatMsg)
{
	//if(error>0){
	//	LPVOID lpMsgBuf;
	//	FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
	//		NULL,error,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
	//		(LPSTR) &lpMsgBuf,	0,NULL );
	//	//wsFormatMsg =(LPCSTR)lpMsgBuf;
	//	::MessageBoxA(NULL,(LPCSTR)lpMsgBuf,"",0);
	//	//g_Logger.Error(__FILE__,__LINE__,"error=%ld,Msg=%s",error,(LPCTSTR)lpMsgBuf);
	//	LocalFree( lpMsgBuf );					
	//}
	return;
}