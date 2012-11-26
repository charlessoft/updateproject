#ifndef _TYPEDEFINE_H
#define _TYPEDEFINE_H

#include <string>
using namespace std;

//log 配置文件
#define UPDATE_PROPERTIES "UPDATE_properties"

//#define UPDATEXML _T("http://fjchenqian.3322.org/updateservice.xml")
#define UPDATEXMLA			"http://10.142.49.127/updateservice.xml"
#define LOCALXML			_T("update.config")

#define UPDATEFOLDER		_T("UpDateFolder_")
#define NEWLINE				_T("\r\n")

#define FILE_PATH			"path"
#define FILE_URL			"url"
#define FILE_LASTVER		"lastver"
#define FILE_SIZE			"size"
#define FILE_NEEDRESTART	"needRestart"
#define FILE_MD5			"md5"
#define FILE_UPDATETIME		"updateTime"

#define SERVER_FILES		"Server_Files"




typedef struct _STR_SERVER_XML_INFO
{
	string fileName;
	string lastVersion;
	string url;
	int size;
	string md5;
	bool needRestart;
	

}SERVER_XML_INFO,*LPSERVER_XML_INFO;


typedef struct _STR_LOCAL_XML_INFO
{
	string fileName;
	string lastVersion;
	int size;
	string md5;

}LOCAL_XML_INFO,*LPLOCAL_XML_INFO;


//自定义消息
#define UM_UPDATE WM_USER + 100
#endif