#ifndef _TYPEDEFINE_H_
#define  _TYPEDEFINE_H_


typedef struct _STR_SERVER_XML_INFO
{
	char fileName[MAX_PATH];
	char lastVersion[20];
	char url[MAX_PATH];
	int size;
	char md5[MAX_PATH];
	bool needRestart;


}SERVER_XML_INFO,*LPSERVER_XML_INFO;



#define  URL		"http://10.142.49.127/MYUPDATE/"
#define	 NEWLINEA	"\r\n"
#define  XMLHEAD	"<?xml version=\"1.0\" encoding=\"utf-8\"?>"



#define  SERVER_XML_BEGIN_SERVER_FILES  "<Server_Files updateTime=\"%s\">"
#define  SERVER_XML_END_SERVER_FILES "</Server_Files>"

#define  SERVER_XML_SERVER_FILES_INFO "<file path=\"%s\"  url=\"%s\" lastver=\"%s\" size=\"%ld\" needRestart=\"%d\" md5=\"%s\" />"

#endif //_TYPEDEFINE_H_