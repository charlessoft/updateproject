#pragma once

class CConfigMgr
{
public:
	CConfigMgr(void);
public:
	~CConfigMgr(void);
	void GetConfigInfo(CONFIGINFO* pConfigInfoStruct );
	void ReadConfig();
	void FormatConfigInfo(TCHAR* pData);
	void FormatConfigInfo(wstring& pData);
	LPCONFIGINFO GetConfigInfo(){return &m_ConfigInfo;}
private:
	CONFIGINFO m_ConfigInfo;
};
