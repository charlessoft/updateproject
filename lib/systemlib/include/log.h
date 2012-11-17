#ifndef _OEGUI_LOG_H_
#define _OEGUI_LOG_H_


#define _TRACE_ 

#ifdef _TRACE_ 
#define LOGRECORD(tag) CLogRecord log(tag) 
#else 
#define LOGRECORD(tag) 
#endif 
#include "ExceptionHandler.h"

#define BeginException _se_translator_function prev_func=_set_se_translator(CExceptionLogger::translator);\
	try {

#define EndException	}catch (struct _EXCEPTION_POINTERS* p) \
{CExceptionLogger::RecordExceptionInfoByActiveX(p);}\
	catch (...) {/*AfxMessageBox("Unknown exception");*/}\
	_set_se_translator(prev_func);

//ÓÃÀ´¼ÇÂ¼WATERMARKID for jpmorgan
#define WATERMARKID (FX_LPVOID)-2


class CExceptionLogger;

#include <eh.h>
 #ifndef _GLOBAL_RECORD_CRASH_

// #define BeginException
// #define EndException
  
//#define BeginException _se_translator_function prev_func=_set_se_translator(CExceptionLogger::translator);\
//  		try {
//  
//  #define EndException	}catch (struct _EXCEPTION_POINTERS* p) \
//  		{CExceptionLogger::RecordExceptionInfoByActiveX(p);}\
//  		catch (...) {/*AfxMessageBox("Unknown exception");*/}\
//  		_set_se_translator(prev_func);
		
 #else
//  #define BeginException
//  #define EndException
 #endif

// #ifndef _GLOBAL_RECORD_CRASH_
// #define BeginException
// #define EndException
// #else
// #define BeginException
// #define EndException
// #endif


class CLogRecord
{
public:
	CLogRecord(string tag);
	CLogRecord();
	void WriteLog(LPCSTR format, ...);
	virtual ~CLogRecord();
	void SetLogFile(CHAR* filepath);

private:
	string	m_strContent;
};
#include <OCIdl.h>



#endif