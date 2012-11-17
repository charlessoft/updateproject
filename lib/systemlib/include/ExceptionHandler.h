// ExceptionHandler.h  


#ifndef	EXCEPTIONHANDLER_H
#define	EXCEPTIONHANDLER_H

#define WUNKNOWNSTR	_T("unknown Windows version")

#define W95STR			_T("Windows 95")
#define W95SP1STR		_T("Windows 95 SP1")
#define W95OSR2STR		_T("Windows 95 OSR2")
#define W98STR			_T("Windows 98")
#define W98SP1STR		_T("Windows 98 SP1")
#define W98SESTR		_T("Windows 98 SE")
#define WMESTR			_T("Windows ME")

#define WNT351STR		_T("Windows NT 3.51")
#define WNT4STR			_T("Windows NT 4")
#define W2KSTR			_T("Windows 2000")
#define WXPSTR			_T("Windows XP")
#define W2003SERVERSTR	_T("Windows 2003 Server")
#define WVISTASTR		_T("Windows Vista")

#define WCESTR			_T("Windows CE")


#define WUNKNOWN	0

#define W9XFIRST	1
#define W95			1
#define W95SP1		2
#define W95OSR2		3
#define W98			4
#define W98SP1		5
#define W98SE		6
#define WME			7
#define W9XLAST		99

#define WNTFIRST	101
#define WNT351		101
#define WNT4		102
#define W2K			103
#define WXP			104
#define W2003SERVER	105
#define WVISTA		106
#define WNTLAST		199

#define WCEFIRST	201
#define WCE			201
#define WCELAST		299

#define XCRASHREPORT_MINI_DUMP_FILE		_T("CRASH.DMP")

//#define XCRASHREPORT_REGISTRY_DUMP_FILE	_T("REGISTRY.TXT")
//#define XCRASHREPORT_CRASH_REPORT_APP	_T("CrashReport.exe")


// We forward declare PEXCEPTION_POINTERS so that the function
// prototype doesn't needlessly require windows.h.
#define  CRITICAL_ERROR_SUCCESS	1
#define  CRITICAL_ERROR_FAIL	0

#include <string>
using namespace std;
class CExceptionLogger
{
public: 
	CExceptionLogger();
	~CExceptionLogger();
	static	long WINAPI RecordExceptionInfoByActiveX(PEXCEPTION_POINTERS data);
	static void		WriteEventLog();
	static BOOL		syslog(DWORD dwID,char*str,int strLen,WORD wType);//参数:事件id;事件附加信息;事件类型
	static BOOL     writeToFile(char* str,int strlen);
	static BOOL		syslogPrintError(DWORD dwID,char*str,int strLen,WORD wType);//参数:事件id;事件附加信息;事件类型
	static BOOL		FireEventError(char*str,int strLen,short nFlag);
	static BOOL		FireJsEvent(char*str,int strLen);
	static void		SetCrashLogPath(char* crashLogPath);
	#if 0
		static void		SplitString(char*str,CStringArray& rArrString);
	#endif

	static void		translator(unsigned int code, struct _EXCEPTION_POINTERS* p);

	static void		DumpSystemInformation();
	static bool		DumpModuleInfo(HINSTANCE ModuleHandle, int nModuleNo);
	static void		DumpModuleList();
	static void		DumpStack(DWORD *pStack);
	static void		DumpRegisters( PCONTEXT Context);
	static UINT		ThreadProc(  LPVOID pParam );
	#if 0
		static CString	GetCurrentPIDMemory();
	#endif

	
	static LPTOP_LEVEL_EXCEPTION_FILTER m_pPreviousFilter;
	static HANDLE			m_ReportHandle;
	static string			m_StrCrashLog;
#ifdef _UNICODE
	static  wstringstream	m_phEventLogBuf;
#else
	static  stringstream	m_phEventLogBuf;
#endif 
	static HANDLE			m_Handle;
	static BOOL				m_bWriteFlag;
	static string			m_Version;
	
protected:  
private:
};



BOOL GetWinVer(LPTSTR pszVersion, int *nVersion, LPTSTR pszMajorMinorBuild);
#endif
