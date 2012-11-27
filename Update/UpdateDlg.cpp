// UpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Update.h"
#include "UpdateDlg.h"
#include "Environment.h"
#include "CsysPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "SingleDownload.h"

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CUpdateDlg dialog




CUpdateDlg::CUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateDlg::IDD, pParent)
	, m_txtUpdateLst(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TXTUPDATELST, m_txtUpdateLst);
	DDX_Control(pDX, IDC_LIST_LOCALUPDATELIST, m_UpdateLst);
}

BEGIN_MESSAGE_MAP(CUpdateDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CUpdateDlg::OnBnClickedOk)
	ON_MESSAGE(UM_UPDATE, OnUpdateList)//注意这条语句的后面没有分号
	ON_BN_CLICKED(IDC_BTN_RESTORE, &CUpdateDlg::OnBnClickedBtnRestore)
END_MESSAGE_MAP()


// CUpdateDlg message handlers

BOOL CUpdateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//Init();
	Init();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUpdateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUpdateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUpdateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUpdateDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	//g_Logger.Debug(__FILE__,__LINE__,"DDD%s","11");
	if (m_txtUpdateLst.IsEmpty())
	{
		return;
	}
	m_UpdateMgr.Update();
}

void CUpdateDlg::GetUpdateList(CStringArray& arr)
{

	CFileFind fileFinder;
	wstring CurDirectory;
	CString filePath = CEnvironment::Env_GetCurrentDirectory(CurDirectory).c_str();
	//CString filePath = str + _T("//*.*");
	int CurDirectoryLen = filePath.GetLength();
	filePath=filePath+="\\*.*";
	BOOL bFinished = fileFinder.FindFile(filePath);
	while(bFinished)  //每次循环对应一个类别目录
	{
		bFinished = fileFinder.FindNextFile();
		if(fileFinder.IsDirectory() && !fileFinder.IsDots())  //若是目录则递归调用此方法
		{
			// BayesCategoryTest(bt, fileFinder.GetFilePath());
			CString strFilePath = fileFinder.GetFilePath();
			
			if (strFilePath.Mid(CurDirectoryLen+1,lstrlen(UPDATEFOLDER)).CompareNoCase(UPDATEFOLDER) ==0)
			{
				wstring strPath = CsysPath::GetDirectoryName(strFilePath.GetBuffer(0));
				strFilePath.Replace(strPath.c_str(),_T(""));
				arr.Add(strFilePath.Right(strFilePath.GetLength()-1-lstrlen(UPDATEFOLDER)));
			}
			
		}
		else  //再判断是否为txt文件
		{
			
		
		}
	}
}

void CUpdateDlg::Init()
{
	g_Logger.StartSystem(UPDATE_PROPERTIES);

	//LOGRECORD("dd");
	g_Logger.Debug(__FILE__,__LINE__,"hello");

// 	CSingleDown
	
//m_ConfigMgr.GetConfigInfo();
	
	//UpdateMgr.ParseLocalXml()
	UPDATE_ERROR UpdateErrCode = (UPDATE_ERROR)m_UpdateMgr.IsNeedUpdate();
	switch(UpdateErrCode)
	{
	case NEEDUPDATE:
		{
 			vector<MULTI_DOWNLOAD_INFO*> UpdateLst = m_UpdateMgr.GetUpdateList();
 			vector<MULTI_DOWNLOAD_INFO*>::iterator Iter;
 			for (Iter = UpdateLst.begin();Iter != UpdateLst.end();++Iter)
 			{
 				LPMULTI_DOWNLOAD_INFO lpUpdateInfo = (MULTI_DOWNLOAD_INFO*)*Iter;
 				//m_txtUpdateLst+=lpServerXmlInfo->fileName.c_str();
				m_txtUpdateLst+=lpUpdateInfo->filename;
 				m_txtUpdateLst+=NEWLINE;
 			}
 			//UpdateMgr.Update();
			break;
		}
	}
	CStringArray arrUpdateLst;
	GetUpdateList(arrUpdateLst);
	for (int i=0,sz=arrUpdateLst.GetSize();i<sz;i++)
	{
		m_UpdateLst.AddString(arrUpdateLst.GetAt(i));
	}
	m_UpdateLst.SetCurSel(0);
	UpdateData(FALSE);


	//BOOL bret = m_UpdateMgr.DownLoadServerUpdateXmlFile();


	
}

LRESULT CUpdateDlg::OnUpdateList(WPARAM wParam,LPARAM lParam){
	

	vector<MULTI_DOWNLOAD_INFO*> UpdateLst = m_UpdateMgr.GetUpdateList();
	vector<MULTI_DOWNLOAD_INFO*>::iterator Iter;
	for (Iter = UpdateLst.begin();Iter != UpdateLst.end();++Iter)
	{
		LPMULTI_DOWNLOAD_INFO lpUpdateInfo = (MULTI_DOWNLOAD_INFO*)*Iter;
		//m_txtUpdateLst+=lpServerXmlInfo->fileName.c_str();
		m_txtUpdateLst+=lpUpdateInfo->filename;
		m_txtUpdateLst+="\r\n";
	}
	//UpdateMgr.Update();
	UpdateData(FALSE);
	return 0;
}

void CUpdateDlg::OnBnClickedBtnRestore()
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_UpdateLst.GetCurSel();
	CString strRestoreFolder;
	if (nCurSel<0)
		return;
	m_UpdateLst.GetText(nCurSel,strRestoreFolder);
	wstring wsCurDirectory;
	wstring wsRestoreDirectory;
	CEnvironment::Env_GetCurrentDirectory(wsCurDirectory);
	wsRestoreDirectory = wsCurDirectory + L"\\" + UPDATEFOLDER +  strRestoreFolder.GetBuffer(0);
	m_UpdateMgr.RestoreFile(wsRestoreDirectory,wsCurDirectory);


}
