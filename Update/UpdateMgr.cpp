#include "StdAfx.h"
#include "UpdateMgr.h"
#include "SingleDownload.h"
CUpdateMgr::CUpdateMgr(void)
{
	//m_Map.insert(pair<string,string>(FILE_PATH,""));
}

CUpdateMgr::~CUpdateMgr(void)
{
}


int CUpdateMgr::IsNeedUpdate()
{
	if (true == DownLoadServerUpdateXmlFile())
	{
		string ServerXml = m_SingleDownLoad.GetMemBuf();
		ParseXml(ServerXml,SERVER,PARSE_TYPE_MEMORY);
	}


	return DO_NOT_NEEDUPDATE;

}

bool CUpdateMgr::DownLoadServerUpdateXmlFile()
{

	return m_SingleDownLoad.DownLoadByMemory(UPDATEXML,5);
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
	TiXmlElement* rootElement = doc.RootElement();  //SchoolÔªËØ  
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
 	LPSERVER_XML_INFO lpServerXmlInfo = NULL;
// 			//g_Logger.Debug(__FILE__,__LINE__,"TiXmlNode::TINYXML_ELEMENT");
// 			TiXmlElement *pp= pchild->ToElement();
// 			//string strValue = pchild->ValueStr();
// 			string str = pchild->ToElement()->Value();
// 			//string strFirstValue = pp->FirstAttribute()->Name();
// 			//pp->NextSiblingElement()
// 			string strValue;
// 	
// 			lpServerXmlInfo = new SERVER_XML_INFO;
// 			memset(lpServerXmlInfo,0,sizeof(SERVER_XML_INFO));
// 			
// 			pp->QueryValueAttribute(FILE_PATH,lpServerXmlInfo->fileName);
// 			pp->QueryValueAttribute(FILE_URL,lpServerXmlInfo->url);
// 			pp->QueryValueAttribute(FILE_LASTVER,lpServerXmlInfo->lastVersion);
// 			pp->QueryValueAttribute(FILE_NEEDRESTART,lpServerXmlInfo->needRestart);
// 			pp->QueryValueAttribute(FILE_MD5,lpServerXmlInfo->needRestart);
// 
// 			//m_SerXmlList.push_back(lpServerXmlInfo);
// 			m_MapServerXmlInfo.insert(pair<string,SERVER_XML_INFO*>(lpServerXmlInfo->fileName,lpServerXmlInfo));

	while(xmlNode)
	{
		int t = xmlNode->Type();

		if(t == TiXmlNode::TINYXML_ELEMENT)
		{
			//g_Logger.Debug(__FILE__,__LINE__,"TiXmlNode::TINYXML_ELEMENT");
			TiXmlElement *pp= xmlNode->ToElement();
			//string strValue = pchild->ValueStr();
			string str = xmlNode->ToElement()->Value();
// 			if (str.compare(SERVER_FILES)==0)
// 			{
// 				ParseServerXml(pchild);
// 			}
			//string strFirstValue = pp->FirstAttribute()->Name();
			//pp->NextSiblingElement()
			string strValue;

			lpServerXmlInfo = new SERVER_XML_INFO;
			memset(lpServerXmlInfo,0,sizeof(SERVER_XML_INFO));

			pp->QueryValueAttribute(FILE_PATH,lpServerXmlInfo->fileName);
			pp->QueryValueAttribute(FILE_URL,lpServerXmlInfo->url);
			pp->QueryValueAttribute(FILE_LASTVER,lpServerXmlInfo->lastVersion);
			pp->QueryBoolAttribute(FILE_NEEDRESTART,&lpServerXmlInfo->needRestart);
			pp->QueryValueAttribute(FILE_MD5,lpServerXmlInfo->md5);

			//m_SerXmlList.push_back(lpServerXmlInfo);
			m_MapServerXmlInfo.insert(pair<string,SERVER_XML_INFO*>(lpServerXmlInfo->fileName,lpServerXmlInfo));

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

void CUpdateMgr::ParseLocalXml(TiXmlNode* xmlNode)
{
// 	TiXmlDocument doc;//( "demo.xml" );
// 	doc.Parse(xml.c_str());
// 	//TiXmlElement xmlElement = doc.GetChar()
// 	TiXmlElement* rootElement = doc.RootElement();  //SchoolÔªËØ  
// 	TiXmlElement* pElement = rootElement->ToElement();
// 	TiXmlNode* pchild = rootElement->FirstChild(); 
 	LPLOCAL_XML_INFO lpLocalXmlInfo = NULL;

	while(xmlNode)
	{
		int t = xmlNode->Type();

		if( t == TiXmlNode::TINYXML_ELEMENT)
		{
			//g_Logger.Debug(__FILE__,__LINE__,"TiXmlNode::TINYXML_ELEMENT");
			TiXmlElement *pp= xmlNode->ToElement();
			//string strValue = pchild->ValueStr();
			string str = xmlNode->ToElement()->Value();
			//string strFirstValue = pp->FirstAttribute()->Name();
			//pp->NextSiblingElement()
			string strValue;

			lpLocalXmlInfo = new LOCAL_XML_INFO;
			memset(lpLocalXmlInfo,0,sizeof(LOCAL_XML_INFO));

			pp->QueryValueAttribute(FILE_PATH,lpLocalXmlInfo->fileName);
// 			pp->QueryValueAttribute(FILE_URL,lpLocalXmlInfo->url);
// 			pp->QueryValueAttribute(FILE_LASTVER,lpLocalXmlInfo->lastVersion);
// 			pp->QueryValueAttribute(FILE_NEEDRESTART,lpLocalXmlInfo->needRestart);
// 			pp->QueryValueAttribute(FILE_MD5,lpLocalXmlInfo->needRestart);

			//m_SerXmlList.push_back(lpServerXmlInfo);
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

