#pragma once
#include "SingleDownload.h"
#include "MultiDownload.h"
#include "ConfigMgr.h"
typedef enum ClientType
{
	SERVER,//解析服务器xml
	LOCAL //解析本地xml
};

typedef enum ParseType 
{
	PARSE_TYPE_FILE, //解析类型,File
	PARSE_TYPE_MEMORY //解析类型,Memory
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
	//解析xml 
	bool ParseXml(string xml,ClientType nClientType,ParseType nParseType);
	bool DownLoadServerUpdateXmlFile();

	//是否需要更新
	int IsNeedUpdate();
	//获取服务端xml信息 buff
	string GetServerUpdateXmlBuf();

	//执行更新下载文件
	BOOL Update();
	BOOL UpdateLocalXmlInfo();
	BOOL UpdateFiles(TCHAR* path);

	//获取更新列表
	vector<MULTI_DOWNLOAD_INFO*> GetUpdateList(){return m_UpdateList;}
	
	//返回备份的文件
	void RestoreFile(wstring strRestorePath,wstring strCurDirectory);

	void GetUpdateList(CStringArray& arr);
	
	//获取更新文件夹索引
	int GetUpdateFolderIndex(wstring pUpdateFolder);

private:
	void ParseServerXml(TiXmlNode* xmlNode);
	bool ParseLocalXml(TiXmlNode* xmlNode);
	
private:
	void ClearServerXmlMap();
	void ClearLocalXmlMap();
	void ClearLocalXml_Tmp();
	//把更新的信息写入到local xml中
	void WriteLocxlXmlFile(TCHAR* path,list<LOCAL_XML_INFO*> localXmlList);
private:
	list<SERVER_XML_INFO*> m_SerXmlList;
	list<LOCAL_XML_INFO*> m_LocalXmlList;
	list<LOCAL_XML_INFO*> m_LocalXmlTmpList;			//记录本地更新的列表,
	map<string,SERVER_XML_INFO*> m_MapServerXmlInfo;	//保存服务端xml信息.
	map<string,LOCAL_XML_INFO*> m_MapLocalXmlInfo;		//保存本地xml信息,与服务器进行匹配

	vector<MULTI_DOWNLOAD_INFO*> m_UpdateList;			//待更新的文件

	string m_serverUpdateTime;	//记录服务器更新时间

	//单个文件和多个文件下载
	CSingleDownload m_SingleDownLoad;
	MultiDownload	m_MultiDownLoad;


	
	UPDATE_ERROR m_UpdateERR;
	
	
	//检查是否需要更新的线程
	static UINT MyCheckUpdateThread(LPVOID lpParam);
	UINT MyCheckUpdateThreadProc();

	//下载线程
	static UINT MyUpdateDownLoadThread(LPVOID lpParam);
	UINT MyUpdateDownLoadThreadProc();
private:
	CConfigMgr m_ConfigMgr;


	map<wstring,int> m_mapUpdateFolder;

};
