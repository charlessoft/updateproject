#include "stdafx.h"
#include "MultiDownload.h"

//扩展定义
#define MD_CURLE_ErrorReport(A)						\
	if(CURLE_OK != A)  /* we failed */				\
	{												\
		_ftprintf(stderr, _T("ERROR:%s,%d,curl told us %s\n"), __FILE__,__LINE__,curl_easy_strerror(A));	\
		g_Logger.Error(__FILE__,__LINE__,"ERROR:curl told us %s\n",curl_easy_strerror(A));	\
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
	str.Format(_T("%s %g / %g (%g %%)\r\n"), (WCHAR*)clientp, dltotal, ultotal, dlnow*100.0/ultotal);
	OutputDebugString(str);

	//注意这里返回 0为正常进程
	//返回 1为退出下载进程。直接下载失败(可以做“取消下载”使用)
	return 	0;
}

static size_t cb(char *d, size_t n, size_t l, void *p)
{
	/* take care of the data here, ignored in this example */
	(void)d;
	(void)p;
	return n*l;
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
	MD_curl_easy_setopt_EXT(eh, CURLOPT_WRITEDATA, info->fp,res);
	//设置写入回调函数
	MD_curl_easy_setopt_EXT(eh, CURLOPT_WRITEFUNCTION, MD_WriteFunc,res);

	MD_curl_easy_setopt_EXT(eh, CURLOPT_HEADER, 0L,res);
	//设置URL地址
	MD_curl_easy_setopt_EXT(eh, CURLOPT_URL, info->url,res);
	MD_curl_easy_setopt_EXT(eh, CURLOPT_PRIVATE, info->url,res);
	MD_curl_easy_setopt_EXT(eh, CURLOPT_VERBOSE, 0L,res);

	MD_curl_easy_setopt_EXT(eh, CURLOPT_WRITEDATA, info->fp,res);


	//设置无进程函数
	MD_curl_easy_setopt_EXT(eh, CURLOPT_NOPROGRESS, FALSE,res);
	//设置进程回调函数
	MD_curl_easy_setopt_EXT(eh, CURLOPT_PROGRESSFUNCTION, MD_ProgressFunc,res);
	MD_curl_easy_setopt_EXT(eh, CURLOPT_PROGRESSDATA, info->process_data,res);

	MD_curl_easy_setopt_EXT(eh,CURLOPT_CONNECTTIMEOUT,timeout,res);



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
	str.Format(_T("CQ_UPDATE_%s %g / %g (%g %%)\r\n"), progress_data, d, t, d*100.0/t);
	OutputDebugString(str);
	//printf(str.GetBuffer(0));
	printf("%s %g / %g (%g %%)\r\n", progress_data, d, t, d*100.0/t);
	return 0;
}
size_t save_sina_page(void *buffer, size_t size, size_t count, void *user_p)
{
	return fwrite(buffer, size, count, (FILE *)user_p);
}

size_t save_sohu_page(void *buffer, size_t size, size_t count, void *user_p)
{
	return fwrite(buffer, size, count, (FILE *)user_p);
}



static void init(CURLM *cm, MULTI_DOWNLOAD_INFO* info)
{
	CURL *eh = curl_easy_init();
	char *progress_data = "* ";

	FILE *outfile;
	//outfile = fopen("C:\\add.exe", "wb");
	//pfile[i]=fopen("C:")
	//MyFileInfo[i].pfile = fopen(MyFileInfo[i].szName,"wb");
	//MyFileInfo[i].process_data=MyFileInfo[i].szName;
	TCHAR szbuf[MAX_PATH]={0};
	_stprintf(szbuf,_T("%s%s"),info->fileSavePath,info->filename);
	info->fp = _tfopen(szbuf,_T("wb"));
	if (info->fp==NULL)
	{
		return;
	}
	info->process_data =info->filename;
	//设置写入的文件指针
	curl_easy_setopt(eh, CURLOPT_WRITEDATA, info->fp);
	//设置写入回调函数
	curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, my_write_func);

	curl_easy_setopt(eh, CURLOPT_HEADER, 0L);
	//设置URL地址
	curl_easy_setopt(eh, CURLOPT_URL, info->url);
	curl_easy_setopt(eh, CURLOPT_PRIVATE, info->url);
	curl_easy_setopt(eh, CURLOPT_VERBOSE, 0L);

	curl_easy_setopt(eh, CURLOPT_WRITEDATA, info->fp);


	//设置无进程函数
	curl_easy_setopt(eh, CURLOPT_NOPROGRESS, FALSE);
	//设置进程回调函数
	curl_easy_setopt(eh, CURLOPT_PROGRESSFUNCTION, my_progress_func);
	curl_easy_setopt(eh, CURLOPT_PROGRESSDATA, info->process_data);

	curl_easy_setopt(eh,CURLOPT_CONNECTTIMEOUT,10);



	curl_multi_add_handle(cm, eh);
}

bool MultiDownload::Download(vector<MULTI_DOWNLOAD_INFO*> lstInfo,long infocount,long sametimecount,long timeout)
{
 CURLM *cm;
	CURLMsg *msg;
	long L;
	unsigned int C=0;
	int M, Q, U = -1;
	fd_set fd_read, fd_write, fd_except;
	struct timeval T;

	curl_global_init(CURL_GLOBAL_ALL);

	cm = curl_multi_init();

	/* we can optionally limit the total amount of connections this multi handle
	uses */
	curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, (long)1);
	vector<LPMULTI_DOWNLOAD_INFO>::iterator iter;
	for (iter=lstInfo.begin();iter!=lstInfo.end();++iter)
	{
		LPMULTI_DOWNLOAD_INFO lpDownLaodInfo = (LPMULTI_DOWNLOAD_INFO)*iter;
		init(cm,lpDownLaodInfo);
	}

	

	while (U) {
		curl_multi_perform(cm, &U);

		if (U) {
			FD_ZERO(&fd_read);
			FD_ZERO(&fd_write);
			FD_ZERO(&fd_except);

			if (curl_multi_fdset(cm, &fd_read, &fd_write, &fd_except, &M)) {
				fprintf(stderr, "E: curl_multi_fdset\n");
				return EXIT_FAILURE;
			}

			if (curl_multi_timeout(cm, &L)) {
				fprintf(stderr, "E: curl_multi_timeout\n");
				return EXIT_FAILURE;
			}
			if (L == -1)
				L = 100;

			if (M == -1) {
#ifdef WIN32
				Sleep(L);
#else
				sleep(L / 1000);
#endif
			} else {
				T.tv_sec = L/1000;
				T.tv_usec = (L%1000)*1000;

				if (0 > select(M+1, &fd_read, &fd_write, &fd_except, &T)) {
					fprintf(stderr, "E: select(%i,,,,%li): %i: %s\n",
						M+1, L, errno, strerror(errno));
					return EXIT_FAILURE;
				}
			}
		}

		while ((msg = curl_multi_info_read(cm, &Q))) {
			if (msg->msg == CURLMSG_DONE) {
				char *url;
				CURL *e = msg->easy_handle;
				curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
				fprintf(stderr, "R: %d - %s <%s>\n",
					msg->data.result, curl_easy_strerror(msg->data.result), url);
				//将结果写入 返回结构体
				for(int j=0;j<infocount;j++)
				{
					//if(0==_tcscmp(s2ws(info[j].url).c_str(),s2ws(url).c_str()))
					if(0==strcmp(lstInfo[j]->url,url))
					{
						//关闭文件
						//fclose(info[j].fp);
						if(CURLE_OK == msg->data.result)
							lstInfo[j]->result=true;
						else
							lstInfo[j]->result=false;
					}
				}
				curl_multi_remove_handle(cm, e);
				curl_easy_cleanup(e);
			}
			else {
				fprintf(stderr, "E: CURLMsg (%d)\n", msg->msg);
			}
// 			if(C<lstInfo.size())
// 			{
// 				init(cm,lstInfo[C++]);
// 				U++;
// 			}
		}
	}

	vector<LPMULTI_DOWNLOAD_INFO>::iterator Iter;
	for (Iter=lstInfo.begin();Iter!=lstInfo.end();++Iter)
	{
		LPMULTI_DOWNLOAD_INFO lpDownInfo = (LPMULTI_DOWNLOAD_INFO)*Iter;
		
		if(lpDownInfo->result != true)
		{
			AfxMessageBox(_T("fail"));
		}
		fclose(lpDownInfo->fp);
		lpDownInfo->fp = NULL;
		//fclose(((LPMULTI_DOWNLOAD_INFO)*Iter)->fp);
	}

	curl_multi_cleanup(cm);
	curl_global_cleanup();
	
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
