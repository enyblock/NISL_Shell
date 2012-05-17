// test_dllDlg.h : header file
//

#if !defined(AFX_TEST_DLLDLG_H__7E0ED810_DE71_42F4_8A10_F8ADB0991EEE__INCLUDED_)
#define AFX_TEST_DLLDLG_H__7E0ED810_DE71_42F4_8A10_F8ADB0991EEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTest_dllDlg dialog

#include "DLLINFO.h"


class CTest_dllDlg : public CDialog
{
// Construction
public:
	CTest_dllDlg(CWnd* pParent = NULL);	// standard constructor
	CCheckListBox m_CheckListBox;
	DLLINFO       m_dll_info;

// Dialog Data
	//{{AFX_DATA(CTest_dllDlg)
	enum { IDD = IDD_TEST_DLL_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest_dllDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTest_dllDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_DLLDLG_H__7E0ED810_DE71_42F4_8A10_F8ADB0991EEE__INCLUDED_)
