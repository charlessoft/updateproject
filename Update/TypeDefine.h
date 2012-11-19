#ifndef _TYPEDEFINE_H
#define _TYPEDEFINE_H



//log ≈‰÷√Œƒº˛
#define UPDATE_PROPERTIES "UPDATE_properties"

//#define UPDATEXML _T("http://fjchenqian.3322.org/updateservice.xml")
#define UPDATEXML _T("http://10.142.49.127/updateservice.xml")



#define FILE_PATH			"path"
#define FILE_URL			"url"
#define FILE_LASTVER		"lastver"
#define FILE_SIZE			"size"
#define FILE_NEEDRESTART	"needRestart"
#define FILE_MD5			"md5"

#define SERVER_FILES		"Server_Files"




typedef struct _STR_SERVER_XML_INFO
{
	char fileName[MAX_PATH];
	char lastVersion[20];
	char url[MAX_PATH];
	int size;
	char md5[100];
	bool needRestart;

}SERVER_XML_INFO,*LPSERVER_XML_INFO;


typedef struct _STR_LOCAL_XML_INFO
{
	char fileName[MAX_PATH];
	char lastVersion[20];
	int size;
	char md5[100];

}LOCAL_XML_INFO,*LPLOCAL_XML_INFO;
#endif