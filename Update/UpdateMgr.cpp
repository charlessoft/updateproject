#include "StdAfx.h"
#include "UpdateMgr.h"
#include "SingleDownload.h"
#include "Environment.h"
CUpdateMgr::CUpdateMgr(void)
{
	//m_Map.insert(pair<string,string>(FILE_PATH,""));
	CURLcode res=curl_global_init(CURL_GLOBAL_ALL);
	if(CURLE_OK != res) 
	{												
		fprintf(stderr, "curl told us %s\n", curl_easy_strerror(res));	
		return ;								
	}
	m_UpdateERR = NEEDUPDATE;
}

CUpdateMgr::~CUpdateMgr(void)
{
	ClearServerXmlMap();
	ClearLocalXmlMap();
	curl_global_cleanup();
}


int CUpdateMgr::IsNeedUpdate()
{
	AfxBeginThread(MyCheckUpdateThread,this);

	return 1;

}

bool CUpdateMgr::DownLoadServerUpdateXmlFile()
{

	return m_SingleDownLoad.DownLoadByMemory(UPDATEXMLA,5);
}

string CUpdateMgr::GetServerUpdateXmlBuf()
{
	return m_SingleDownLoad.GetMemBuf();
}


//void CUpdateMgr::ParseXml(string xml)
void CUpdateMgr::ParseXml(string xml,ClientType nClientType,ParseType nParseType)
{
	TiXmlDocument doc;//( "demo.xml" );
	switch(nParseType)
	{
	case PARSE_TYPE_FILE:
		doc.LoadFile(xml);
		break;
	case PARSE_TYPE_MEMORY:
		doc.Parse(xml.c_str());
		break;;
	}

	
	//TiXmlElement xmlElement = doc.GetChar()
	TiXmlElement* rootElement = doc.RootElement();  //SchoolԪ��  
	TiXmlElement* pElement = rootElement->ToElement();
	TiXmlNode* pchild = rootElement->FirstChild(); 
	switch(nClientType)
	{
	case SERVER:
		ParseServerXml(pchild);
		break;
	case LOCAL:
		ParseLocalXml(pchild);
		break;
	}

	
}
void CUpdateMgr::ParseServerXml(TiXmlNode* xmlNode)
{
// 
	ClearServerXmlMap();
 	LPSERVER_XML_INFO lpServerXmlInfo = NULL;

	while(xmlNode)
	{
		int t = xmlNode->Type();

		if(t == TiXmlNode::TINYXML_ELEMENT)
		{
			//g_Logger.Debug(__FILE__,__LINE__,"TiXmlNode::TINYXML_ELEMENT");
			TiXmlElement *pp= xmlNode->ToElement();
			//string strValue = pchild->ValueStr();
			string str = xmlNode->ToElement()->Value();

			string strValue;

			lpServerXmlInfo = new SERVER_XML_INFO;
			memset(lpServerXmlInfo,0,sizeof(SERVER_XML_INFO));

			pp->QueryValueAttribute(FILE_PATH,&lpServerXmlInfo->fileName);
			pp->QueryValueAttribute(FILE_URL,&lpServerXmlInfo->url);
			pp->QueryValueAttribute(FILE_LASTVER,&lpServerXmlInfo->lastVersion);
			pp->QueryBoolAttribute(FILE_NEEDRESTART,&lpServerXmlInfo->needRestart);
			pp->QueryValueAttribute(FILE_MD5,&lpServerXmlInfo->md5);

			m_MapServerXmlInfo.insert(pair<string,SERVER_XML_INFO*>(lpServerXmlInfo->fileName,lpServerXmlInfo));


		}
		else if( t == TiXmlNode::TINYXML_TEXT)
		{               
			string str =xmlNode->Value();
		}       
		xmlNode = xmlNode->NextSibling();                                          

	}

}

void CUpdateMgr::ParseLocalXml(TiXmlNode* xmlNode)
{
	ClearLocalXmlMap();
 	LPLOCAL_XML_INFO lpLocalXmlInfo = NULL;

	while(xmlNode)
	{
		int t = xmlNode->Type();

		if( t == TiXmlNode::TINYXML_ELEMENT)
		{
			TiXmlElement *pp= xmlNode->ToElement();
			string str = xmlNode->ToElement()->Value();
			string strValue;

			lpLocalXmlInfo = new LOCAL_XML_INFO;
			memset(lpLocalXmlInfo,0,sizeof(LOCAL_XML_INFO));
			
			pp->QueryValueAttribute(FILE_PATH,&lpLocalXmlInfo->fileName);
 			pp->QueryValueAttribute(FILE_LASTVER,&lpLocalXmlInfo->lastVersion);
 			pp->QueryValueAttribute(FILE_MD5,&lpLocalXmlInfo->md5);

			m_MapLocalXmlInfo.insert(pair<string,LOCAL_XML_INFO*>(lpLocalXmlInfo->fileName,lpLocalXmlInfo));

			// 			TiXmlText *pTxt  = pchild->ToText();
			// 			string sss = pTxt->Value();
		}
		else if( t == TiXmlNode::TINYXML_TEXT)
		{               
			string str =xmlNode->Value();
		}       
		xmlNode = xmlNode->NextSibling();                                          

	}
}



void CUpdateMgr::ClearServerXmlMap()
{
	//ɾ�������� map ����ɾ��
	map<string,SERVER_XML_INFO*>::iterator iter;
	for (iter=m_MapServerXmlInfo.begin();iter!=m_MapServerXmlInfo.end();)
	{
		m_MapServerXmlInfo.erase(iter++);
	}
}

void CUpdateMgr::ClearLocalXmlMap()
{
	//ɾ������ map ����ɾ��
	map<string,LOCAL_XML_INFO*>::iterator iter;
	for (iter=m_MapLocalXmlInfo.begin();iter!=m_MapLocalXmlInfo.end();)
	{
		m_MapLocalXmlInfo.erase(iter++);
	}
}

BOOL CUpdateMgr::Update()
{
//return TRUE;

	//MULTI_DOWNLOAD_INFO data[2]=
	//{
	//	{_T("a.html"),"http://dow111111111.com",NULL,false},
	//	{_T("a.html"),"http://www.baidu.com",NULL,false},
	//};


	bool bRet = m_MultiDownLoad.Download(m_UpdateList,m_UpdateList.size(),m_UpdateList.size(),10);
	if (bRet)
	{
		AfxMessageBox(_T("true"));
	}
	else
		AfxMessageBox(_T("fail"));
	return TRUE;
}




UINT CUpdateMgr::MyCheckUpdateThread(LPVOID lpParam)
{
	CUpdateMgr* pUpdateMgr = (CUpdateMgr*)lpParam;
	return pUpdateMgr->MyCheckUpdateThreadProc();
}


UINT CUpdateMgr::MyCheckUpdateThreadProc()
{
	if (true == DownLoadServerUpdateXmlFile())
	{
		string ServerXml = m_SingleDownLoad.GetMemBuf();
		ParseXml(ServerXml,SERVER,PARSE_TYPE_MEMORY);
	}
#ifdef _UNICODE
	USES_CONVERSION;
	ParseXml(W2A(LOCALXML),LOCAL,PARSE_TYPE_FILE);
#else
	ParseXml(LOCALXML,LOCAL,PARSE_TYPE_FILE);
#endif


	map<string,SERVER_XML_INFO*>::iterator ServerIter;
	map<string,LOCAL_XML_INFO*>::iterator LocalIter;
	BOOL bAppendLst=FALSE;
	for(ServerIter = m_MapServerXmlInfo.begin(); ServerIter != m_MapServerXmlInfo.end(); ServerIter++)
	{
		SERVER_XML_INFO* lpSerXmlInfo =(SERVER_XML_INFO*)ServerIter->second;
		LOCAL_XML_INFO* lpLocalXmlInfo =NULL;
		LocalIter = m_MapLocalXmlInfo.find(ServerIter->first);
		if (LocalIter != m_MapLocalXmlInfo.end())
		{
			//�ҵ�
			lpLocalXmlInfo = (LOCAL_XML_INFO*)LocalIter->second;
			if(strcmp(lpSerXmlInfo->md5.c_str(),lpLocalXmlInfo->md5.c_str())!=0)
			{
				bAppendLst = TRUE;
			}
		}
		else
		{		
			//˵��û�ҵ�,��ӵ������б���
			//m_UpdateList.push_back(ServerIter->second);
			bAppendLst = TRUE;
		}
		if (bAppendLst)
		{
			USES_CONVERSION;
			//˵��md5��һ��,��Ҫ����,��ӵ������б���
			//SERVER_XML_INFO* lpServerXmlInfo = (SERVER_XML_INFO*)ServerIter->second;
			LPMULTI_DOWNLOAD_INFO lpMultiDownLoad = new MULTI_DOWNLOAD_INFO;
			memset(lpMultiDownLoad,0,sizeof(MULTI_DOWNLOAD_INFO));

			//����ʱ��
			SYSTEMTIME st;
			::GetLocalTime(&st);
			TCHAR buf[32];
			_stprintf_s(buf,32,_T("%04d-%02d-%02d-%02d-%02d-%02d"), st.wYear, st.wMonth, st.wDay,st.wHour,st.wMinute,st.wSecond);

			wstring wsCurDirectory;
			CEnvironment::Env_GetCurrentDirectoryW(wsCurDirectory);
			CString strCurDirectory;
			strCurDirectory.Format(_T("UpDate_%s\\%s\\"),wsCurDirectory.c_str(),buf);
			MakeSureDirectoryPathExists(W2A(strCurDirectory.GetBuffer(0)));

#ifdef _UNICODE
			_tcscpy(lpMultiDownLoad->filename,A2W(lpSerXmlInfo->fileName.c_str()));
			//_tcscpy((lpMultiDownLoad->url).c_str(),A2W(lpSerXmlInfo->url.c_str()));
			strcpy(lpMultiDownLoad->url,lpSerXmlInfo->url.c_str());
			_tcscpy(lpMultiDownLoad->fileSavePath,strCurDirectory.GetBuffer(0));
#else
			_tcscpy(lpMultiDownLoad->filename,(lpSerXmlInfo->fileName.c_str()));
			_tcscpy(lpMultiDownLoad->url,(lpSerXmlInfo->url.c_str()));
			_tcscpy(lpMultiDownLoad->fileSavePath,strCurDirectory.GetBuffer(0));
#endif
			m_UpdateList.push_back(lpMultiDownLoad);
			bAppendLst=FALSE;
		}
	}

	if (m_UpdateList.size()>0)
	{
		return NEEDUPDATE;
	}
	return DO_NOT_NEEDUPDATE;
}