#pragma once
#include "SingleDownload.h"
#include "MultiDownload.h"
typedef enum ClientType
{
	SERVER,
	LOCAL
};

typedef enum ParseType 
{
	PARSE_TYPE_FILE,
	PARSE_TYPE_MEMORY
};

typedef enum UPDATE_ERROR
{
	NEEDUPDATE,//需要更新
	SERVER_DEFINE, //服务器拒绝
	DO_NOT_NEEDUPDATE //不需要更新
};
class CUpdateMgr
{
public:
	CUpdateMgr(void);
public:
	~CUpdateMgr(void);
	void ParseXml(string xml,ClientType nClientType,ParseType nParseType);
	bool DownLoadServerUpdateXmlFile();
	int IsNeedUpdate();
	string GetServerUpdateXmlBuf();

	BOOL Update();
	vector<MULTI_DOWNLOAD_INFO*> GetUpdateList(){return m_UpdateList;}

private:
	void ParseServerXml(TiXmlNode* xmlNode);
	void ParseLocalXml(TiXmlNode* xmlNode);
	
private:
	void ClearServerXmlMap();
	void ClearLocalXmlMap();
private:
	list<SERVER_XML_INFO*> m_SerXmlList;
	list<LOCAL_XML_INFO*> m_LocalXmlList;
	map<string,SERVER_XML_INFO*> m_MapServerXmlInfo;
	map<string,LOCAL_XML_INFO*> m_MapLocalXmlInfo;
	//map<string,SERVER_XML_INFO*> m_MapUpdateXml;
	
	vector<MULTI_DOWNLOAD_INFO*> m_UpdateList;

	CSingleDownload m_SingleDownLoad;
	MultiDownload	m_MultiDownLoad;


	UPDATE_ERROR m_UpdateERR;
	static UINT MyCheckUpdateThread(LPVOID lpParam);

	UINT MyCheckUpdateThreadProc();
};
