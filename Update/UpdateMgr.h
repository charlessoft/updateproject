#pragma once
#include "SingleDownload.h"
#include "MultiDownload.h"
#include "ConfigMgr.h"
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
	BOOL UpdateLocalXmlInfo();
	BOOL UpdateFiles(TCHAR* path);

	//��ȡ�����б�
	vector<MULTI_DOWNLOAD_INFO*> GetUpdateList(){return m_UpdateList;}

	void RestoreFile(wstring strRestorePath,wstring strCurDirectory);
	void GetUpdateList(CStringArray& arr);

	int GetUpdateFolderIndex(wstring pUpdateFolder);

private:
	void ParseServerXml(TiXmlNode* xmlNode);
	void ParseLocalXml(TiXmlNode* xmlNode);
	
private:
	void ClearServerXmlMap();
	void ClearLocalXmlMap();
	void ClearLocalXml_Tmp();
	void WriteLocxlXmlFile(TCHAR* path,list<LOCAL_XML_INFO*> localXmlList);
private:
	list<SERVER_XML_INFO*> m_SerXmlList;
	list<LOCAL_XML_INFO*> m_LocalXmlList;
	list<LOCAL_XML_INFO*> m_LocalXmlTmpList;
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
private:
	CConfigMgr m_ConfigMgr;


	map<wstring,int> m_mapUpdateFolder;

};
