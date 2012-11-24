#pragma once
#include "SingleDownload.h"
#include "MultiDownload.h"

typedef enum ClientType
{
	SERVER,//����������xml
	LOCAL //��������xml
};

typedef enum ParseType 
{
	PARSE_TYPE_FILE, //��������,File
	PARSE_TYPE_MEMORY //��������,Memory
};

typedef enum UPDATE_ERROR
{
	NEEDUPDATE,//��Ҫ����
	SERVER_DEFINE, //�������ܾ�
	DO_NOT_NEEDUPDATE //����Ҫ����
};


class CUpdateMgr
{
public:
	CUpdateMgr(void);
public:
	~CUpdateMgr(void);
	//����xml 
	void ParseXml(string xml,ClientType nClientType,ParseType nParseType);
	bool DownLoadServerUpdateXmlFile();

	//�Ƿ���Ҫ����
	int IsNeedUpdate();
	//��ȡ�����xml��Ϣ buff
	string GetServerUpdateXmlBuf();

	//ִ�и��������ļ�
	BOOL Update();

	//��ȡ�����б�
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

	vector<MULTI_DOWNLOAD_INFO*> m_UpdateList;

	string m_serverUpdateTime;
	//�����ļ��Ͷ���ļ�����
	CSingleDownload m_SingleDownLoad;
	MultiDownload	m_MultiDownLoad;


	UPDATE_ERROR m_UpdateERR;
	
	
	//����Ƿ���Ҫ���µ��߳�
	static UINT MyCheckUpdateThread(LPVOID lpParam);
	UINT MyCheckUpdateThreadProc();

	//�����߳�
	static UINT MyUpdateDownLoadThread(LPVOID lpParam);
	UINT MyUpdateDownLoadThreadProc();

};
