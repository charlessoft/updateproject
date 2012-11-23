#include "stdafx.h"
#include "MultiDownload.h"

//扩展定义
#define MD_CURLE_ErrorReport(A)						\
	if(CURLE_OK != A)  /* we failed */				\
	{												\
		_ftprintf(stderr, _T("ERROR:%s,%d,curl told us %s\n"), __FILE__,__LINE__,curl_easy_strerror(A));	\
		return false;								\
	}

//扩展定义
#define MD_curl_easy_setopt_EXT(A,B,C,D)			\
	D=curl_easy_setopt(A,B,C);						\
	MD_CURLE_ErrorReport(D);

//扩展定义
#define	MD_CURLM_ErrorReport(A)						\
	if(CURLM_OK != A)								\
	{												\
		_ftprintf(stderr,_T("ERROR:%s,%d,curlmulti told us %s\n"),__FILE__,__LINE__,curl_multi_strerror(A));	\
		return false;								\
	}
	
void init(CURLM *cm, int i);
//下载并写到文件中 回调函数
/*
typedef size_t (*curl_write_callback)(char *buffer,
                                      size_t size,
                                      size_t nitems,
                                      void *outstream);
*/
size_t MD_WriteFunc(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	return fwrite(ptr, size, nmemb, stream);
}

//实时下载或上传进程的回调函数
/*
typedef int (*curl_progress_callback)(void *clientp,
                                      double dltotal,
                                      double dlnow,
                                      double ultotal,
                                      double ulnow);
*/
int MD_ProgressFunc(void *clientp,
					 double dltotal, 
					 double dlnow, 
					 double ultotal,
					 double ulnow)
{
//	char *s=(char *)clientp;
	_ftprintf(stderr,_T("%g / %g (%g %%)\r\n"), dlnow, dltotal, dlnow*100.0/dltotal);
	//CString str;
	//str.Format(_T("%g / %g (%g %%)\r\n"), dlnow, dltotal, dlnow*100.0/dltotal);
	//OutputDebugString(str);

	CString str;
	str.Format(_T("%s %g / %g (%g %%)\r\n"), (char*)clientp, dltotal, ultotal, dlnow*100.0/ultotal);
	OutputDebugString(str);

	//注意这里返回 0为正常进程
	//返回 1为退出下载进程。直接下载失败(可以做“取消下载”使用)
	return 	0;
}

//MULTI_DOWNLOAD_INFO	1个结构体
//timeout				超时设置
//输出		初始化成功或失败
//初始化
bool MultiDownload::Load(MULTI_DOWNLOAD_INFO *info,long timeout)
{

	CURL *eh = curl_easy_init();
	TCHAR *progress_data = _T("* ");

	FILE *outfile;
	//outfile = fopen(_T("C:\\add.exe", "wb"));
	//pfile[i]=fopen(_T("C:"))
	//MyFileInfo[i].pfile = fopen(MyFileInfo[i].szName,_T("wb"));
	//MyFileInfo[i].process_data=MyFileInfo[i].szName;
	TCHAR szFilePath[MAX_PATH]={0};
	_stprintf(szFilePath,_T("%s\\%s"),info->fileSavePath,info->filename);
	//打开待写文件
	info->fp = _tfopen(szFilePath, _T("wb"));
	if(NULL== info->fp )
		return false;


	//设置写入的文件指针
	curl_easy_setopt(eh, CURLOPT_WRITEDATA, info->fp);
	//设置写入回调函数
	curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, MD_WriteFunc);

	curl_easy_setopt(eh, CURLOPT_HEADER, 0L);
	//设置URL地址
	curl_easy_setopt(eh, CURLOPT_URL, info->url);
	curl_easy_setopt(eh, CURLOPT_PRIVATE, info->url);
	curl_easy_setopt(eh, CURLOPT_VERBOSE, 0L);

	curl_easy_setopt(eh, CURLOPT_WRITEDATA, info->fp);


	//设置无进程函数
	curl_easy_setopt(eh, CURLOPT_NOPROGRESS, FALSE);
	//设置进程回调函数
	curl_easy_setopt(eh, CURLOPT_PROGRESSFUNCTION, MD_ProgressFunc);
	curl_easy_setopt(eh, CURLOPT_PROGRESSDATA, info->process_data);

	curl_easy_setopt(eh,CURLOPT_CONNECTTIMEOUT,10);



	curl_multi_add_handle(curlmulti, eh);
	return true;
}




size_t my_write_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	return fwrite(ptr, size, nmemb, stream);
}

size_t my_read_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	return fread(ptr, size, nmemb, stream);
}

int my_progress_func(char *progress_data,
					 double t, /* dltotal */
					 double d, /* dlnow */
					 double ultotal,
					 double ulnow)
{
	CString str;
	str.Format(_T("%s %g / %g (%g %%)\r\n"), progress_data, d, t, d*100.0/t);
	OutputDebugString(str);
	//printf(str.GetBuffer(0));
	printf("%s %g / %g (%g %%)\r\n", progress_data, d, t, d*100.0/t);
	return 0;
}


bool MultiDownload::Download(vector<MULTI_DOWNLOAD_INFO*> lstInfo,long infocount,long sametimecount,long timeout)
{

	long i,j;
	fd_set Reads, Writes, Execs;
	int maxfd;
	int run=1;
	int queue;
	long sleeptime=timeout;
	struct timeval T;
	char *url;
	CURL *tempcurl;

	//初始化
	curlmulti = curl_multi_init();
	if(NULL == curlmulti)
		return false;

	//如果总共要下的数量 小于 同时下载数，则同时下载数 设置成 总共要下的数量
	if(infocount < sametimecount)
		sametimecount=infocount;
	//设置同时下载数
	mes=curl_multi_setopt(curlmulti, CURLMOPT_MAXCONNECTS, sametimecount);
	MD_CURLM_ErrorReport(mes);

	//初始化载入
	for (i = 0;i < sametimecount; i++) 
	{
		if(false == Load(lstInfo[i],timeout))
			return false;
	}

	while (run) 
	{
		//执行下载列表
		mes=curl_multi_perform(curlmulti, &run);
		MD_CURLM_ErrorReport(mes);

		if (run)
		{
			FD_ZERO(&Reads);
			FD_ZERO(&Writes);
			FD_ZERO(&Execs);
			
			mes=curl_multi_fdset(curlmulti, &Reads, &Writes, &Execs, &maxfd);
			MD_CURLM_ErrorReport(mes);
			//启动执行。获取是否超时等待
			mes=curl_multi_timeout(curlmulti, &sleeptime);
			MD_CURLM_ErrorReport(mes);

			if (maxfd == -1)
			{
#ifdef WIN32
				Sleep(sleeptime);
#else
				sleep(sleeptime / 1000);
#endif
			}
			else 
			{
				T.tv_sec = sleeptime/1000;
				T.tv_usec = (sleeptime%1000)*1000;

				if (0 > select(maxfd+1, &Reads, &Writes, &Execs, &T)) 
				{
					_ftprintf(stderr, _T("curlmulti error: select(%i,,,,%li): %i: %s\n"),
						maxfd+1, sleeptime, errno, strerror(errno));
					return false;
				}
			}
		}

		while ((curlmultiSG = curl_multi_info_read(curlmulti, &queue)))
		{
			//下载完成 或 下载异常退出
			if ( CURLMSG_DONE == curlmultiSG->msg) 
			{
				tempcurl = curlmultiSG->easy_handle;

				res=curl_easy_getinfo(curlmultiSG->easy_handle, CURLINFO_PRIVATE, &url);
				MD_CURLE_ErrorReport(res);

				_ftprintf(stderr, _T("curlmulti result: %d - %s\n<%s>\n"),
					curlmultiSG->data.result, curl_easy_strerror(curlmultiSG->data.result), url);
					
				LPMULTI_DOWNLOAD_INFO lpDownLoadInfo = NULL;
				//将结果写入 返回结构体
				for(j=0;j<infocount;j++)
				{
					lpDownLoadInfo = lstInfo[j];
					if(0==strcmp(lpDownLoadInfo->url,url))
					{
						//关闭文件
						//fclose(lpDownLoadInfo->fp);
						if(CURLE_OK == curlmultiSG->data.result)
							lpDownLoadInfo->result=true;
						else
							lpDownLoadInfo->result=false;
					}
				}

				//从下载列表中删句柄
				mes=curl_multi_remove_handle(curlmulti, tempcurl);
				MD_CURLM_ErrorReport(mes);
				//清理
				curl_easy_cleanup(tempcurl);
			}
			else 
			{
				//未知异常
				_ftprintf(stderr, _T("curlmultiSG error: CURLMsg (%d)\n"), curlmultiSG->msg);
				return	false;
				
			}
			if (i < infocount) 
			{
				if(false == Load(lstInfo[i],timeout))
					return false;
				i++;
				run++; /* just to prevent it from remaining at 0 if there are more
					 URLs to get */
			}
		}
	}

	mes=curl_multi_cleanup(curlmulti);
	MD_CURLM_ErrorReport(mes);
	return true;

}

//MULTI_DOWNLOAD_INFO	N个结构体
//infocount				N
//sametimecount			同时下载文件的个数
//timeout				超时设置
//返回					执行成功或者失败
//执行下载
bool MultiDownload::Download(MULTI_DOWNLOAD_INFO *&info,long infocount,long sametimecount,long timeout=10)
{
	long i,j;
	fd_set Reads, Writes, Execs;
	int maxfd;
	int run=1;
	int queue;
	long sleeptime=timeout;
	struct timeval T;
	char *url;
	CURL *tempcurl;

	//初始化
	curlmulti = curl_multi_init();
	if(NULL == curlmulti)
		return false;

	//如果总共要下的数量 小于 同时下载数，则同时下载数 设置成 总共要下的数量
	if(infocount < sametimecount)
		sametimecount=infocount;
	//设置同时下载数
	mes=curl_multi_setopt(curlmulti, CURLMOPT_MAXCONNECTS, sametimecount);
	MD_CURLM_ErrorReport(mes);

	//初始化载入
	for (i = 0;i < sametimecount; i++) 
	{
		if(false == Load(&info[i],timeout))
			return false;
	}

	while (run) 
	{
		//执行下载列表
		mes=curl_multi_perform(curlmulti, &run);
		MD_CURLM_ErrorReport(mes);

		if (run)
		{
			FD_ZERO(&Reads);
			FD_ZERO(&Writes);
			FD_ZERO(&Execs);
			
			mes=curl_multi_fdset(curlmulti, &Reads, &Writes, &Execs, &maxfd);
			MD_CURLM_ErrorReport(mes);
			//启动执行。获取是否超时等待
			mes=curl_multi_timeout(curlmulti, &sleeptime);
			MD_CURLM_ErrorReport(mes);

			if (maxfd == -1)
			{
#ifdef WIN32
				Sleep(sleeptime);
#else
				sleep(sleeptime / 1000);
#endif
			}
			else 
			{
				T.tv_sec = sleeptime/1000;
				T.tv_usec = (sleeptime%1000)*1000;

				if (0 > select(maxfd+1, &Reads, &Writes, &Execs, &T)) 
				{
					_ftprintf(stderr, _T("curlmulti error: select(%i,,,,%li): %i: %s\n"),
						maxfd+1, sleeptime, errno, strerror(errno));
					return false;
				}
			}
		}

		while ((curlmultiSG = curl_multi_info_read(curlmulti, &queue)))
		{
			//下载完成 或 下载异常退出
			if ( CURLMSG_DONE == curlmultiSG->msg) 
			{
				tempcurl = curlmultiSG->easy_handle;

				res=curl_easy_getinfo(curlmultiSG->easy_handle, CURLINFO_PRIVATE, &url);
				MD_CURLE_ErrorReport(res);

				_ftprintf(stderr, _T("curlmulti result: %d - %s\n<%s>\n"),
					curlmultiSG->data.result, curl_easy_strerror(curlmultiSG->data.result), url);
					
				//将结果写入 返回结构体
				for(j=0;j<infocount;j++)
				{
					if(0==_tcscmp(s2ws(info[j].url).c_str(),s2ws(url).c_str()))
					{
						//关闭文件
						fclose(info[j].fp);
						if(CURLE_OK == curlmultiSG->data.result)
							info[j].result=true;
						else
							info[j].result=false;
					}
				}

				//从下载列表中删句柄
				mes=curl_multi_remove_handle(curlmulti, tempcurl);
				MD_CURLM_ErrorReport(mes);
				//清理
				curl_easy_cleanup(tempcurl);
			}
			else 
			{
				//未知异常
				_ftprintf(stderr, _T("curlmultiSG error: CURLMsg (%d)\n"), curlmultiSG->msg);
				return	false;
				
			}
			if (i < infocount) 
			{
				if(false == Load(&info[i],timeout))
					return false;
				i++;
				run++; /* just to prevent it from remaining at 0 if there are more
					 URLs to get */
			}
		}
	}

	mes=curl_multi_cleanup(curlmulti);
	MD_CURLM_ErrorReport(mes);

	return true;
}
