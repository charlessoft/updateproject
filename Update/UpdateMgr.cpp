#include "StdAfx.h"
#include "UpdateMgr.h"
#include "SingleDownload.h"
//#include "Environment.h"
//#include "CsysFile.h"
//#include "CsysPath.h"
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
	m_ConfigMgr.ReadConfig();
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
	g_Logger.Debug(__FILE__,__LINE__,"下载服务端UpdateXml文件 DownLoadServerUpdateXmlFile");
	LPCONFIGINFO lpConfigInfo = m_ConfigMgr.GetConfigInfo();
	return m_SingleDownLoad.DownLoadByMemory((TCHAR*)lpConfigInfo->url.c_str(),5);
	//return m_SingleDownLoad.DownLoadByMemory(UPDATEXMLA,5);
}

string CUpdateMgr::GetServerUpdateXmlBuf()
{
	return m_SingleDownLoad.GetMemBuf();
}


//void CUpdateMgr::ParseXml(string xml)
bool CUpdateMgr::ParseXml(string xml,ClientType nClientType,ParseType nParseType)
{
	g_Logger.Debug(__FILE__,__LINE__,"ParseXml %s,ClientType = %d,ParseType=%d",xml.c_str(),nClientType,nParseType);
	bool bRet=true;
	TiXmlDocument doc;//( "demo.xml" );
	switch(nParseType)
	{
	case PARSE_TYPE_FILE:
		{
			bRet = doc.LoadFile(xml);
			if (false == bRet)
			{
				g_Logger.Debug(__FILE__,__LINE__,"加载文件失败 %s",xml.c_str());
				return bRet;
			}
			break;
		}
	case PARSE_TYPE_MEMORY:
		doc.Parse(xml.c_str());
		break;;
	}
	

	
	//TiXmlElement xmlElement = doc.GetChar()
	TiXmlElement* rootElement = doc.RootElement();  //School元素  
	TiXmlElement* pElement = rootElement->ToElement();
 	TiXmlNode* pchild = rootElement->FirstChild(); 
	switch(nClientType)
	{
	case SERVER:
		ParseServerXml(pElement);
		break;
	case LOCAL:
		ParseLocalXml(pchild);
		break;
	}

	
}
void CUpdateMgr::ParseServerXml(TiXmlNode* xmlNode)
{
	ClearServerXmlMap();
 	LPSERVER_XML_INFO lpServerXmlInfo = NULL;
	while(xmlNode)
	{
		int t = xmlNode->Type();

		if(t == TiXmlNode::TINYXML_ELEMENT)
		{
			TiXmlNode* xmlChildNode = xmlNode->FirstChild();
			if (xmlChildNode)
			{
				TiXmlElement *pp= xmlNode->ToElement();
				pp->QueryValueAttribute(FILE_UPDATETIME,&m_serverUpdateTime);
				ParseServerXml(xmlChildNode);
			}
			else
			{
				TiXmlElement *pp= xmlNode->ToElement();
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

		}
		else if( t == TiXmlNode::TINYXML_TEXT)
		{               
			string str =xmlNode->Value();
		}       
		xmlNode = xmlNode->NextSibling();                                          

	}

}

bool CUpdateMgr::ParseLocalXml(TiXmlNode* xmlNode)
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
	return true;
}



//删除服务器 map 迭代删除
void CUpdateMgr::ClearServerXmlMap()
{
	map<string,SERVER_XML_INFO*>::iterator iter;
	for (iter=m_MapServerXmlInfo.begin();iter!=m_MapServerXmlInfo.end();)
	{
		m_MapServerXmlInfo.erase(iter++);
	}
}


//删除本地 map 迭代删除
void CUpdateMgr::ClearLocalXmlMap()
{
	map<string,LOCAL_XML_INFO*>::iterator iter;
	for (iter=m_MapLocalXmlInfo.begin();iter!=m_MapLocalXmlInfo.end();)
	{
		m_MapLocalXmlInfo.erase(iter++);
	}
}

void CUpdateMgr::ClearLocalXml_Tmp()
{
	//m_LocalXmlTmpList.
	list<LOCAL_XML_INFO*>::iterator Iter;
	for(Iter=m_LocalXmlTmpList.begin();Iter!=m_LocalXmlTmpList.end();++Iter)
	{
		delete *Iter;
		Iter = m_LocalXmlTmpList.erase(Iter++);
	}
	m_LocalXmlTmpList.clear();
}

void CUpdateMgr::WriteLocxlXmlFile(TCHAR* path,list<LOCAL_XML_INFO*> localXmlList)
{
	wstring wsPath(path);
	string Updateconfig =ws2s(wsPath);
	FILE* pfile;
	pfile = fopen(Updateconfig.c_str(),"wb");
	if (pfile)
	{
		
		fwrite(XMLHEAD,sizeof(char) ,strlen(XMLHEAD),pfile);
		fwrite(NEWLINEA,sizeof(char) ,strlen(NEWLINEA),pfile);
		
		fwrite(LOCAL_XML_BEGIN_UPDATEFILELIST,sizeof(char) ,strlen(LOCAL_XML_BEGIN_UPDATEFILELIST),pfile);
		fwrite(NEWLINEA,sizeof(char),strlen(NEWLINEA),pfile);


		list<LOCAL_XML_INFO*>::iterator Iter;
		CString strBuf;
		for (Iter=localXmlList.begin();Iter!=localXmlList.end();++Iter)
		{
			LPLOCAL_XML_INFO lpWriteLocalXmlInfo = (LPLOCAL_XML_INFO)*Iter;
			strBuf.Format(_T(LOCAL_XML_DETAIL_INFO),
				s2ws(lpWriteLocalXmlInfo->fileName).c_str(),
				s2ws(lpWriteLocalXmlInfo->lastVersion).c_str(),
				lpWriteLocalXmlInfo->size,
				s2ws(lpWriteLocalXmlInfo->md5).c_str());

			fwrite(ws2s(strBuf.GetBuffer(0)).c_str(),sizeof(char),strBuf.GetLength(),pfile);
			fwrite(NEWLINEA,sizeof(char),strlen(NEWLINEA),pfile);
		}
		fwrite(LOCAL_XML_END_UPDATEFILELIST,sizeof(char),strlen(LOCAL_XML_END_UPDATEFILELIST),pfile);
		fclose(pfile);

	}

#if 0
	wstring	wsCurDirectory;
	CEnvironment::Env_GetCurrentDirectory(wsCurDirectory);
	wstring Updateconfig = wsCurDirectory + L"\\" + LOCALXMLCONFIG;   //_T("update.config_1");
	//wstring Updateconfig = wsCurDirectory + L"\\" + _T("update.config_1");
	CFile myFile;
	if (myFile.Open(Updateconfig.c_str(),CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary|CFile::modeNoTruncate))
	{
		WORD unicode = 0xFEFF;  //这句重要
		myFile.Write(&unicode,2);  //这句重要
		myFile.Write(XMLHEAD,lstrlen(XMLHEAD)*sizeof(TCHAR));
		myFile.Write(NEWLINE,lstrlen(NEWLINE)*sizeof(TCHAR));
		myFile.Write(LOCAL_XML_BEGIN_UPDATEFILELIST,lstrlen(LOCAL_XML_BEGIN_UPDATEFILELIST)*sizeof(TCHAR));
		myFile.Write(NEWLINE,lstrlen(NEWLINE)*sizeof(TCHAR));
		list<LOCAL_XML_INFO*>::iterator Iter;
 		CString strBuf;
 		for (Iter=localXmlList.begin();Iter!=localXmlList.end();++Iter)
 		{
 			LPLOCAL_XML_INFO lpWriteLocalXmlInfo = (LPLOCAL_XML_INFO)*Iter;
 			strBuf.Format(LOCAL_XML_DETAIL_INFO,s2ws(lpWriteLocalXmlInfo->fileName).c_str(),s2ws(lpWriteLocalXmlInfo->lastVersion).c_str(),lpWriteLocalXmlInfo->size,s2ws(lpWriteLocalXmlInfo->md5).c_str());
 			myFile.Write(strBuf,strBuf.GetLength()*sizeof(TCHAR));
			myFile.Write(NEWLINE,lstrlen(NEWLINE)*sizeof(TCHAR));
 		}
 		myFile.Write(LOCAL_XML_END_UPDATEFILELIST,lstrlen(LOCAL_XML_END_UPDATEFILELIST)*sizeof(TCHAR));
		myFile.Close();
	}
	else
	{
		DWORD dwErr = GetLastError();
		string strMsg;
		CGlobalFunction::AfxFormatMessageA(dwErr,strMsg);
		g_Logger.Error(__FILE__,__LINE__,"WriteLocalXmlFile Fail dw=%ld,%s",dwErr,strMsg.c_str());
	}
#endif
}

BOOL CUpdateMgr::Update()
{

	AfxBeginThread(MyUpdateDownLoadThread,this);

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
		g_Logger.Debug(__FILE__,__LINE__,"解析服务器端xml文件 %s",ServerXml.c_str());
		ParseXml(ServerXml,SERVER,PARSE_TYPE_MEMORY);
	}
#ifdef _UNICODE
	USES_CONVERSION;
	ParseXml(W2A(LOCALXMLCONFIG),LOCAL,PARSE_TYPE_FILE);
#else
	ParseXml(LOCALXMLCONFIG,LOCAL,PARSE_TYPE_FILE);
#endif


	map<string,SERVER_XML_INFO*>::iterator ServerIter;
	map<string,LOCAL_XML_INFO*>::iterator LocalIter;
	BOOL bAppendLst=FALSE;
	CString strCurDirectory;
	ClearLocalXml_Tmp();
	g_Logger.Debug(__FILE__,__LINE__,"m_MapServerXmlInfo Count %d",m_MapServerXmlInfo.size());
	g_Logger.Debug(__FILE__,__LINE__,"m_MapLocalXmlInfo Count %d",m_MapLocalXmlInfo.size());
	for(ServerIter = m_MapServerXmlInfo.begin(); ServerIter != m_MapServerXmlInfo.end(); ServerIter++)
	{
		SERVER_XML_INFO* lpSerXmlInfo =(SERVER_XML_INFO*)ServerIter->second;
		LOCAL_XML_INFO* lpLocalXmlInfo =NULL;
		LocalIter = m_MapLocalXmlInfo.find(ServerIter->first);
		if (LocalIter != m_MapLocalXmlInfo.end())
		{
			//找到
			lpLocalXmlInfo = (LOCAL_XML_INFO*)LocalIter->second;
			if(strcmp(lpSerXmlInfo->md5.c_str(),lpLocalXmlInfo->md5.c_str())!=0)
			{
				bAppendLst = TRUE;
			}
			else
			{
				g_Logger.Debug(__FILE__,__LINE__,"md5 值一致不需要加入更新列表 %s",lpLocalXmlInfo->fileName.c_str());
			}
		}
		else
		{		
			//说明没找到,添加到更新列表中
			//m_UpdateList.push_back(ServerIter->second);
			bAppendLst = TRUE;
		}

		if (bAppendLst)
		{
			USES_CONVERSION;
			//说明md5不一致,需要更新,添加到更新列表中
			//SERVER_XML_INFO* lpServerXmlInfo = (SERVER_XML_INFO*)ServerIter->second;
			LPMULTI_DOWNLOAD_INFO lpMultiDownLoad = new MULTI_DOWNLOAD_INFO;
			memset(lpMultiDownLoad,0,sizeof(MULTI_DOWNLOAD_INFO));

			wstring wsCurDirectory;
			CEnvironment::Env_GetCurrentDirectory(wsCurDirectory);
			strCurDirectory.Format(_T("%s\\%s%s\\"),wsCurDirectory.c_str(),UPDATEFOLDER, s2ws(m_serverUpdateTime).c_str());

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
			LOCAL_XML_INFO* lpNewLocalXmlInfo = new LOCAL_XML_INFO;
			memset(lpNewLocalXmlInfo,0,sizeof(LOCAL_XML_INFO));
			lpNewLocalXmlInfo->fileName =lpSerXmlInfo->fileName;
			lpNewLocalXmlInfo->lastVersion=lpSerXmlInfo->lastVersion;
			lpNewLocalXmlInfo->md5=lpSerXmlInfo->md5;
			lpNewLocalXmlInfo->size=lpSerXmlInfo->size;	

			g_Logger.Debug(__FILE__,__LINE__,"加入更新列表,%s",lpNewLocalXmlInfo->fileName);
			m_LocalXmlTmpList.push_back(lpNewLocalXmlInfo);

			m_UpdateList.push_back(lpMultiDownLoad);

			bAppendLst=FALSE;
		}
	}

	if (m_UpdateList.size()>0)
	{
		BOOL bRet = MakeSureDirectoryPathExists(W2A(strCurDirectory.GetBuffer(0)));
		if (!bRet)
		{
			DWORD dwErr = GetLastError();
			string strMsg;
			CGlobalFunction::AfxFormatMessageA(dwErr,strMsg);
			g_Logger.Debug(__FILE__,__LINE__,"创建更新目录 失败 dwErr=%ld,Msg=%s,strCurDirectory=%s",dwErr,strMsg.c_str(), W2A(strCurDirectory.GetBuffer()));
		}
		else
		{
			g_Logger.Debug(__FILE__,__LINE__,"创建更新目录 成功 %s", W2A(strCurDirectory.GetBuffer()));
		}
		
		return SendMessage(AfxGetMainWnd()->m_hWnd, UM_UPDATE,NULL,NULL);
	}
	else
	{
		//没有需要更新的文件,程序退出
		return SendMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,NULL,NULL);
	}
	
	//return DO_NOT_NEEDUPDATE;
}

UINT CUpdateMgr::MyUpdateDownLoadThread(LPVOID lpParam)
{
	CUpdateMgr* pUpdateMgr = (CUpdateMgr*)lpParam;
	return pUpdateMgr->MyUpdateDownLoadThreadProc();
}

BOOL CUpdateMgr::UpdateFiles(TCHAR* path)
{
	//if (bRet)
	{
		wstring wsCurDirectory;
		CEnvironment::Env_GetCurrentDirectory(wsCurDirectory);
		vector<MULTI_DOWNLOAD_INFO*>::iterator Iter;
		BOOL bRet = FALSE;		
		wstring wsSavePath;
		wstring wsUpdatePath(path) ;//下载的更新文件夹
		wstring wsUpdateFileDb = wsCurDirectory + L"\\" + path + L"\\" + UPDATE_FILE; //覆盖文件列表
		wstring wsOldFile; //旧版文件
		wstring wsOldFile_Bak;//备份文件
		wstring wsNewFile; //新版文件
	
		CFile myFile;

		bool isUnicode;
		//CString strBuf;
		TCHAR* UpdateFileBuf = NULL;
		if (myFile.Open(wsUpdateFileDb.c_str(),CFile::modeRead))
		{
			byte head[2];
			myFile.Read(head,2);
			if((head[0]==0xff&&head[1]==0xfe) || (head[0]==0xfe&&head[1]==0xff))
			{
				//AfxMessageBox(_T("File is Unicode!"));
				isUnicode = true;
			}
			if(isUnicode)
				myFile.Seek(2, CFile::begin);//0xfffe Unicode文件开头的头两个字节为0xfffe或0xfeff，跳过这两个字节
			int nLen = myFile.GetLength();
			nLen = nLen-2 +sizeof(TCHAR);
			UpdateFileBuf  = new TCHAR[nLen+1];
			memset(UpdateFileBuf,0,nLen);
			myFile.Read(UpdateFileBuf,nLen);
			UpdateFileBuf[nLen]='\0';

			myFile.Close();

		}
	

		
		TCHAR* pStrbuf = UpdateFileBuf;
		TCHAR* token = _tcstok(pStrbuf, _T("\r\n"));

		while( token != NULL )
		{
			wsOldFile = wsCurDirectory + L"\\" + token ;//+ L"_BAK"; //当前旧版文件
			wsOldFile_Bak = wsCurDirectory + L"\\" + wsUpdatePath + L"\\" + token + L"_BAK"; 
			wsNewFile = wsCurDirectory + L"\\" + wsUpdatePath + L"\\" + token ;
			bRet = CsysFile::Copy(wsOldFile ,wsOldFile_Bak );
			if (!bRet)
			{
				DWORD dwErr = CsysFile::SysGetlastError();
				string srtMsg = CsysFile::ErrMsgA(dwErr);
				g_Logger.Error(__FILE__,__LINE__,"旧版文件备份失败 %s,wsOldFile %s wsOldFile_Bak %s",srtMsg.c_str(),ws2s(wsOldFile).c_str(),ws2s(wsOldFile_Bak).c_str());
				//break;
				//continue;
			}

			bRet = CsysFile::Copy(wsNewFile ,wsOldFile );
			if (!bRet)
			{
				DWORD dwErr = CsysFile::SysGetlastError();
				string srtMsg = CsysFile::ErrMsgA(dwErr);
				g_Logger.Error(__FILE__,__LINE__,"新版文件覆盖旧版文件 失败 %s wsNewFile %s wsOldFile %s",srtMsg.c_str(),ws2s(wsNewFile).c_str(),ws2s(wsOldFile).c_str());
				
			}



			token = _tcstok( NULL, _T("\r\n"));
		}

		delete []UpdateFileBuf;
		

	}

	return 1;

}

UINT CUpdateMgr::MyUpdateDownLoadThreadProc()
{
//	Update();
	bool bRet = m_MultiDownLoad.Download(m_UpdateList,(long)m_UpdateList.size(),(long)m_UpdateList.size(),100);
	if (false == bRet)
	{
		return 1;
	}
	//wstring wsUpdatefolder = UPDATEFOLDER + s2ws(m_serverUpdateTime);
	MULTI_DOWNLOAD_INFO* lpDownLoadInfo = m_UpdateList[0];
	wstring wsConfigPath;
	wsConfigPath = lpDownLoadInfo->fileSavePath ;
	wsConfigPath += LOCALXMLCONFIG;

	wstring wsUpdateFileDb;
	wsUpdateFileDb = lpDownLoadInfo->fileSavePath;
	wsUpdateFileDb += UPDATE_FILE;

	wstring wsbuf;
	if (bRet)
	{
		CFile myFile;
		vector<MULTI_DOWNLOAD_INFO*>::iterator Iter;
		for (Iter=m_UpdateList.begin();Iter!=m_UpdateList.end();++Iter)
		{
			LPMULTI_DOWNLOAD_INFO lpDownInfo = *Iter;
			wsbuf += lpDownInfo->filename;
			wsbuf += NEWLINE;
		}
		wsbuf += LOCALXMLCONFIG; //加上配置文件
		wsbuf += NEWLINE;
		if (myFile.Open(wsUpdateFileDb.c_str(),CFile::modeReadWrite|CFile::modeCreate))
		{
			WORD unicode = 0xFEFF;  //这句重要
			myFile.Write(&unicode,2);  //这句重要
			myFile.Write(wsbuf.c_str(),(int)wsbuf.length()*sizeof(TCHAR));
			myFile.Close();
		}		

		WriteLocxlXmlFile((TCHAR*)wsConfigPath.c_str(),m_LocalXmlTmpList);

		return SendMessage(AfxGetMainWnd()->m_hWnd, UM_UPDATE_RESULT,(WPARAM)s2ws(m_serverUpdateTime).c_str(),NULL);
	}
	return 1;
}

void CUpdateMgr::RestoreFile(wstring strRestorePath,wstring wsCurDirectory)
{
	CFileFind fileFinder;
	CString filePath ;//= strRestorePath.c_str() + _T("//*.*");
	filePath.Format(_T("%s\\*.*"),strRestorePath.c_str());
	BOOL bFinished = fileFinder.FindFile(filePath);
	wstring wsDestfile;
	wstring wsRestoreFile;
	while(bFinished)  //每次循环对应一个类别目录
	{
		bFinished = fileFinder.FindNextFile();
		if(fileFinder.IsDirectory() && !fileFinder.IsDots())  //若是目录则递归调用此方法
		{
			// BayesCategoryTest(bt, fileFinder.GetFilePath());
		}
		else  //再判断是否为txt文件
		{
			//获取文件类型
			CString fileName = fileFinder.GetFileName();
			if(fileName.Right(4).CompareNoCase(_T("_BAK")) == 0)
			{
				wsRestoreFile = strRestorePath + L"\\" + fileName.GetBuffer(0);
				wsDestfile = wsCurDirectory + L"\\" + fileName.Left(fileName.GetLength()-4).GetBuffer(0);
				BOOL bret = CsysFile::Copy(wsRestoreFile,wsDestfile);
				if (FALSE == bret)
				{
					g_Logger.Error(__FILE__,__LINE__,"还原文件失败 restore %s to %s",ws2s(wsRestoreFile).c_str(),ws2s(wsDestfile).c_str());
				}
			}
		}
	}
}

void CUpdateMgr::GetUpdateList(CStringArray& arr)
{

	CFileFind fileFinder;
	wstring CurDirectory;
	CString filePath = CEnvironment::Env_GetCurrentDirectory(CurDirectory).c_str();
	//CString filePath = str + _T("//*.*");
	int CurDirectoryLen = filePath.GetLength();
	filePath=filePath+="\\*.*";
	BOOL bFinished = fileFinder.FindFile(filePath);
	CString strTmpFilePath;
	int nIndex =0;
	while(bFinished)  //每次循环对应一个类别目录
	{
		bFinished = fileFinder.FindNextFile();
		if(fileFinder.IsDirectory() && !fileFinder.IsDots())  //若是目录则递归调用此方法
		{
			// BayesCategoryTest(bt, fileFinder.GetFilePath());
			CString strFilePath = fileFinder.GetFilePath();
			
			if (strFilePath.Mid(CurDirectoryLen+1,lstrlen(UPDATEFOLDER)).CompareNoCase(UPDATEFOLDER) ==0)
			{
				wstring strPath = CsysPath::GetDirectoryName(strFilePath.GetBuffer(0));
				strFilePath.Replace(strPath.c_str(),_T(""));
				strTmpFilePath = strFilePath.Right(strFilePath.GetLength()-1-lstrlen(UPDATEFOLDER));
				arr.Add(strTmpFilePath);
				map<wstring,int>::iterator Iter;
				Iter = m_mapUpdateFolder.find(strTmpFilePath.GetBuffer());
				if (Iter == m_mapUpdateFolder.end())
				{
					m_mapUpdateFolder.insert(pair<wstring,int>(strTmpFilePath.GetBuffer() ,nIndex++));
				}
				
			}
			
		}
		else  //再判断是否为txt文件
		{
			
		
		}
	}
}

int CUpdateMgr::GetUpdateFolderIndex(wstring pUpdateFolder)
{
	map<wstring,int>::iterator Iter ;
	Iter = m_mapUpdateFolder.find(pUpdateFolder);
	if (Iter!= m_mapUpdateFolder.end())
	{
		return Iter->second;
	}
	return -1;
}