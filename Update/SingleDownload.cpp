#include "StdAfx.h"
#include "SingleDownload.h"

CSingleDownload::CSingleDownload(void)
{
}

CSingleDownload::~CSingleDownload(void)
{
}



//扩展定义
#define SD_CURLE_ErrorReport(A)						\
	if(CURLE_OK != A)  /* we failed */				\
{												\
	fprintf(stderr, "ERROR:%s,%d,curl told us %s\n", __FILE__,__LINE__,curl_easy_strerror(A));	\
	return false;								\
}


//扩展定义
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
	//注意这里返回 0为正常进程
	//返回 1为退出下载进程。直接下载失败(可以做“取消下载”使用)
	return 	0;
}

bool CSingleDownload::Download(CURL * curl,char* url,long timeout)
{
	//string strData;
	//设置URL地址
	SD_curl_easy_setopt_EXT(curl, CURLOPT_URL, url,res);

	
	//设置无进程函数
	SD_curl_easy_setopt_EXT(curl, CURLOPT_NOPROGRESS, FALSE,res);
	//设置进程回调函数
	SD_curl_easy_setopt_EXT(curl, CURLOPT_PROGRESSFUNCTION, Single_ProgressFunc,res);
	//设置进程回调函数传的自定义参
	SD_curl_easy_setopt_EXT(curl, CURLOPT_PROGRESSDATA, this,res);
	//设置下载速度=0时 N次退出
	SD_curl_easy_setopt_EXT(curl, CURLOPT_TIMEOUT, timeout,res);
	//设置URL地址错误 重连N次后推出
	SD_curl_easy_setopt_EXT(curl, CURLOPT_CONNECTTIMEOUT, timeout,res);
	return true;
}


//url		URL地址 
//filename	文件路径
//timeout	设定下载超时时限
//输出		下载成功或失败
//执行下载
bool CSingleDownload::DownloadByFile(char *url,TCHAR *filename,long timeout=10)
{
	//初始化
	curl = curl_easy_init();
	if(NULL == curl)
		return false;
	//打开待写文件
	outfile = _tfopen(filename, _T("wb"));
	if(NULL== outfile)
		return false;
	//设置buffer指针
	SD_curl_easy_setopt_EXT(curl, CURLOPT_WRITEDATA, outfile,res);
	//设置写入回调函数
	SD_curl_easy_setopt_EXT(curl, CURLOPT_WRITEFUNCTION, Single_WriteFunc,res);
	Download(curl,url,timeout);
	

	//开始执行
	res = curl_easy_perform(curl);
	if(CURLE_OK != res)  /* we failed */				
	{												
		fprintf(stderr, "curl result %s\n",curl_easy_strerror(res));	
		g_Logger.Error(__FILE__,__LINE__,"curl result %s\n",curl_easy_strerror(res));
		return false;								
	}

	//关闭文件
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


	//设置buffer指针
	SD_curl_easy_setopt_EXT(curl, CURLOPT_WRITEDATA, &m_buffer,res);
	//设置写入回调函数
	SD_curl_easy_setopt_EXT(curl, CURLOPT_WRITEFUNCTION, Single_WriteFunc_Mem,res);

	//其他信息
	Download(curl,url,timeout);

 
 	//开始执行
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
