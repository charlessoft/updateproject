#pragma once
#include "BrowseDir.h"
#include "TypeDefine.h"
#include <map>
class CStatMd5Dir :public CBrowseDir
{
public:
	CStatMd5Dir(void);
	virtual bool ProcessFile(const char *filename); 

	//����BrowseDirÿ����һ��Ŀ¼,�͵���ProcessDir 
	//�������ڴ����Ŀ¼������һ��Ŀ¼����Ϊ�������ݹ�ȥ 
	//������ڴ�����ǳ�ʼĿ¼,��parentdir=NULL  
	//�û����Ը�д�ú���,�����Լ��Ĵ������   
	//�����û�����������ͳ����Ŀ¼�ĸ��� 
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
	map<string,int> m_ExceptMap;//���������ļ��б�
};
