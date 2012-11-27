#pragma once

class CSingleDownload
{
public:
	CSingleDownload(void);
public:
	~CSingleDownload(void);
	//url		URL��ַ 
	//filename	�ļ�·��
	//timeout	�趨���س�ʱʱ��
	//���		���سɹ���ʧ��
	//ִ������
	bool DownloadByFile(char *url,TCHAR *filename,long timeout);
	bool DownLoadByMemory(TCHAR*downUrl, long timeout);
	bool Download(CURL * curl,char* url,long timeout);


	//�����ڴ�
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
