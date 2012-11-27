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
	ON_MESSAGE(UM_UPDATE_RESULT, OnUpdateResult)//注意这条语句的后面没有分号
	ON_BN_CLICKED(IDC_BTN_RESTORE, &CUpdateDlg::OnBnClickedBtnRestore)
	ON_LBN_DBLCLK(IDC_LIST_LOCALUPDATELIST, &CUpdateDlg::OnLbnDblclkListLocalupdatelist)
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
	m_UpdateMgr.GetUpdateList(arrUpdateLst);
	for (int i=0,sz=arrUpdateLst.GetSize();i<sz;i++)
	{
		m_UpdateLst.AddString(arrUpdateLst.GetAt(i));
	}
	m_UpdateLst.SetCurSel(0);
	UpdateData(FALSE);

	

	//BOOL bret = m_UpdateMgr.DownLoadServerUpdateXmlFile();


	
}


LRESULT CUpdateDlg::OnUpdateResult(WPARAM wParam,LPARAM lParam){
	AfxMessageBox(L"下载成功。是否更新文件？");
	//m_UpdateMgr.UpdateFiles()
	TCHAR* pUpdateFolder = (TCHAR*)wParam;
	int nCurSel=0;
	int nIndex = m_UpdateMgr.GetUpdateFolderIndex(pUpdateFolder);
	if (nIndex <0)
	{
		m_UpdateLst.AddString(pUpdateFolder);
		nCurSel = m_UpdateLst.GetCount()-1;
		m_UpdateLst.SetCurSel(nCurSel);
	}
	else
	{
		m_UpdateLst.SetCurSel(nIndex);
		nCurSel = nIndex ;
	}

	
	CString strValue ;
	CString strUpdateFolder;
	if (nCurSel!=-1)
		m_UpdateLst.GetText(nCurSel,strValue);
	else
		m_UpdateLst.GetText(nCurSel,strValue);
	
	strUpdateFolder.Format(_T("%s%s"),UPDATEFOLDER,strValue);
	m_UpdateMgr.UpdateFiles(strUpdateFolder.GetBuffer());
	
	return 0;
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
	if (m_txtUpdateLst.IsEmpty())
	{
		return 0;
	}
	m_UpdateMgr.Update();
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

void CUpdateDlg::OnLbnDblclkListLocalupdatelist()
{
	int nCurSel=0;
	CString strUpdateFolder;
	CString strValue;
	nCurSel = m_UpdateLst.GetCurSel();
	if (nCurSel!=-1)
		m_UpdateLst.GetText(nCurSel,strValue);
	else
		m_UpdateLst.GetText(nCurSel,strValue);

	strUpdateFolder.Format(_T("%s%s"),UPDATEFOLDER,strValue);
	m_UpdateMgr.UpdateFiles(strUpdateFolder.GetBuffer());

}
