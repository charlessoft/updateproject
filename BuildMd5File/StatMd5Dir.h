#pragma once
#include "BrowseDir.h"
#include "TypeDefine.h"
#include <map>
class CStatMd5Dir :public CBrowseDir
{
public:
	CStatMd5Dir(void);
	virtual bool ProcessFile(const char *filename); 

	//函数BrowseDir每进入一个目录,就调用ProcessDir 
	//并把正在处理的目录名及上一级目录名作为参数传递过去 
	//如果正在处理的是初始目录,则parentdir=NULL  
	//用户可以覆写该函数,加入自己的处理代码   
	//比如用户可以在这里统计子目录的个数 
	virtual void ProcessDir(const char *currentdir,const char *parentdir);  

	virtual bool CStatMd5Dir::SetInitDir(const char *dir);
	void BuildXmlInfo();

public:
	~CStatMd5Dir(void);
private:
	list<LPSERVER_XML_INFO> m_fileLst;
	string GetMd5Value(const char* file);
	string m_currDirectory;
	string m_Xmlbuf;
	map<string,int> m_ExceptMap;//不包含的文件列表
};
