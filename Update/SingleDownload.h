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
