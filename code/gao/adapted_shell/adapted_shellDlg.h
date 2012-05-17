// adapted_shellDlg.h : header file
//

#include "TabPage1.h"

#if !defined(AFX_ADAPTED_SHELLDLG_H__E37913C6_AE07_4D28_A818_5CC4B8C04AB8__INCLUDED_)
#define AFX_ADAPTED_SHELLDLG_H__E37913C6_AE07_4D28_A818_5CC4B8C04AB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAdapted_shellDlg dialog

class CAdapted_shellDlg : public CDialog
{
// Construction
public:
	CAdapted_shellDlg(CWnd* pParent = NULL);	// standard constructor
	bool Download(const CString& strFileURLInServer, const CString & strFileLocalFullPath);//存放到本地的路径 
	BOOL PreTranslateMessage(MSG* pMsg);
// Dialog Data
	//{{AFX_DATA(CAdapted_shellDlg)
	enum { IDD = IDD_ADAPTED_SHELL_DIALOG };
	CTabCtrl	m_tabctrl;
	CTabPage1   m_tabpage1;
	CTabPage1   m_tabpage2;
	CTabPage1   m_tabpage3;
	int         m_CurSelTab;
	CDialog     *pDialog[3];
	

	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdapted_shellDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAdapted_shellDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnUpdate();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnOpenFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAPTED_SHELLDLG_H__E37913C6_AE07_4D28_A818_5CC4B8C04AB8__INCLUDED_)
