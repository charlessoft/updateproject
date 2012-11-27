// UpdateDlg.h : header file
//

#pragma once
#include "UpdateMgr.h"
#include "ConfigMgr.h"
#include "afxwin.h"

// CUpdateDlg dialog
class CUpdateDlg : public CDialog
{
// Construction
public:
	CUpdateDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_UPDATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	void Init();
	void GetUpdateList(CStringArray& arr);
private:
	CUpdateMgr m_UpdateMgr;
	CConfigMgr m_ConfigMgr;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnUpdateList(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateResult(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	CString m_txtUpdateLst;

	//检查是否更新线程
	static UINT MyCheckUpdateThread(LPVOID lpParam);
	UINT MyCheckUpdateThreadProc();

public:
	CListBox m_UpdateLst;
public:
	afx_msg void OnBnClickedBtnRestore();
public:
	afx_msg void OnLbnDblclkListLocalupdatelist();
};
