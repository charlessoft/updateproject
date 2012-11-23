#include "StdAfx.h"
#include "SingleDownload.h"

CSingleDownload::CSingleDownload(void)
{
}

CSingleDownload::~CSingleDownload(void)
{
}



//��չ����
#define SD_CURLE_ErrorReport(A)						\
	if(CURLE_OK != A)  /* we failed */				\
{												\
	fprintf(stderr, "ERROR:%s,%d,curl told us %s\n", __FILE__,__LINE__,curl_easy_strerror(A));	\
	return false;								\
}


//��չ����
#define SD_curl_easy_setopt_EXT(A,B,C,D)			\
	D=curl_easy_setopt(A,B,C);						\
	SD_CURLE_ErrorReport(D);


size_t CSingleDownload::Single_WriteFunc_Mem(void *ptr, size_t size, size_t nmemb, string *writerData)
{
	unsigned long sizes = size * nmemb;
	if (writerData == NULL) return 0;
	writerData->append((char*)ptr, sizes);
	return sizes;

}

size_t CSingleDownload::Single_WriteFunc(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	
		return fwrite(ptr, size, nmemb, stream);
}


int CSingleDownload::Single_ProgressFunc(void *clientp,
					double dltotal, 
					double dlnow, 
					double ultotal,
					double ulnow)
{
	//	SignalDownload *s=(SignalDownload *)clientp;
	fprintf(stderr,"%g / %g (%g %%)\r\n", dlnow, dltotal, dlnow*100.0/dltotal);
	g_Logger.Debug(__FILE__,__LINE__,"%g / %g (%g %%)\r\n", dlnow, dltotal, dlnow*100.0/dltotal);
	//ע�����ﷵ�� 0Ϊ��������
	//���� 1Ϊ�˳����ؽ��̡�ֱ������ʧ��(��������ȡ�����ء�ʹ��)
	return 	0;
}

bool CSingleDownload::Download(CURL * curl,char* url,long timeout)
{
	//string strData;
	//����URL��ַ
	SD_curl_easy_setopt_EXT(curl, CURLOPT_URL, url,res);

	
	//�����޽��̺���
	SD_curl_easy_setopt_EXT(curl, CURLOPT_NOPROGRESS, FALSE,res);
	//���ý��̻ص�����
	SD_curl_easy_setopt_EXT(curl, CURLOPT_PROGRESSFUNCTION, Single_ProgressFunc,res);
	//���ý��̻ص����������Զ����
	SD_curl_easy_setopt_EXT(curl, CURLOPT_PROGRESSDATA, this,res);
	//���������ٶ�=0ʱ N���˳�
	SD_curl_easy_setopt_EXT(curl, CURLOPT_TIMEOUT, timeout,res);
	//����URL��ַ���� ����N�κ��Ƴ�
	SD_curl_easy_setopt_EXT(curl, CURLOPT_CONNECTTIMEOUT, timeout,res);
	return true;
}


//url		URL��ַ 
//filename	�ļ�·��
//timeout	�趨���س�ʱʱ��
//���		���سɹ���ʧ��
//ִ������
bool CSingleDownload::DownloadByFile(char *url,TCHAR *filename,long timeout=10)
{
	//��ʼ��
	curl = curl_easy_init();
	if(NULL == curl)
		return false;
	//�򿪴�д�ļ�
	outfile = _tfopen(filename, _T("wb"));
	if(NULL== outfile)
		return false;
	//����bufferָ��
	SD_curl_easy_setopt_EXT(curl, CURLOPT_WRITEDATA, outfile,res);
	//����д��ص�����
	SD_curl_easy_setopt_EXT(curl, CURLOPT_WRITEFUNCTION, Single_WriteFunc,res);
	Download(curl,url,timeout);
	

	//��ʼִ��
	res = curl_easy_perform(curl);
	if(CURLE_OK != res)  /* we failed */				
	{												
		fprintf(stderr, "curl result %s\n",curl_easy_strerror(res));	
		g_Logger.Error(__FILE__,__LINE__,"curl result %s\n",curl_easy_strerror(res));
		return false;								
	}

	//�ر��ļ�
	fclose(outfile);
	outfile=NULL;

	curl_easy_cleanup(curl);
	curl=NULL;
//AfxMessageBox(strData.c_str());

	return true;
}

bool CSingleDownload::DownLoadByMemory(char *url, long timeout)
{
 	curl = curl_easy_init();
 	if(NULL == curl)
 		return false;


	//����bufferָ��
	SD_curl_easy_setopt_EXT(curl, CURLOPT_WRITEDATA, &m_buffer,res);
	//����д��ص�����
	SD_curl_easy_setopt_EXT(curl, CURLOPT_WRITEFUNCTION, Single_WriteFunc_Mem,res);

	//������Ϣ
	Download(curl,url,timeout);

 
 	//��ʼִ��
  	res = curl_easy_perform(curl);
  	if(CURLE_OK != res)  /* we failed */				
  	{												
  		fprintf(stderr, "curl result %s\n",curl_easy_strerror(res));	
  		g_Logger.Error(__FILE__,__LINE__,"curl result %s\n",curl_easy_strerror(res));
  		return false;								
  	}

 	curl_easy_cleanup(curl);
 	curl=NULL;
	return true;
}
