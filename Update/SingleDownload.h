#pragma once

class CSingleDownload
{
public:
	CSingleDownload(void);
public:
	~CSingleDownload(void);
	//url		URL地址 
	//filename	文件路径
	//timeout	设定下载超时时限
	//输出		下载成功或失败
	//执行下载
	bool DownloadByFile(char *url,TCHAR *filename,long timeout);
	bool DownLoadByMemory(TCHAR*downUrl, long timeout);
	bool Download(CURL * curl,char* url,long timeout);


	//返回内存
	string GetMemBuf(){return m_buffer;}


	
private:
	CURL *curl;
	CURLcode res;
	FILE *outfile;
	string m_buffer;

	static size_t Single_WriteFunc(void *ptr, size_t size, size_t nmemb, FILE *stream);
	static size_t Single_WriteFunc_Mem(void *ptr, size_t size, size_t nmemb, string *writerData);
	static int Single_ProgressFunc(void *clientp,
		double dltotal, 
		double dlnow, 
		double ultotal,
		double ulnow);
};
