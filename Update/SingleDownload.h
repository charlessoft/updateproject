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
	bool Download(char *url,TCHAR *filename,long timeout);


	
private:
	CURL *curl;
	CURLcode res;
	FILE *outfile;

	static size_t Single_WriteFunc(void *ptr, size_t size, size_t nmemb, FILE *stream);
	static int Single_ProgressFunc(void *clientp,
		double dltotal, 
		double dlnow, 
		double ultotal,
		double ulnow);
};
