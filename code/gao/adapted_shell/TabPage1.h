#if !defined(AFX_TABPAGE1_H__00B9CEA3_6278_4964_83DF_929D3168B7FD__INCLUDED_)
#define AFX_TABPAGE1_H__00B9CEA3_6278_4964_83DF_929D3168B7FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabPage1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabPage1 dialog





#define  WM_MY_MESSAGE  5000

class CAdapted_shellDlg;


class CTabPage1 : public CDialog
{
// Construction
public:
	CTabPage1(CWnd* pParent = NULL);   // standard constructor
	CCheckListBox m_CheckListBox;
	CAdapted_shellDlg *m_main_dialog;

// Dialog Data
	//{{AFX_DATA(CTabPage1)
	enum { IDD = IDD_DIALOG_PAG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabPage1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabPage1)
	virtual BOOL OnInitDialog();
	afx_msg void OnCancle();
	afx_msg void OnSelcancelListDllBox();
	afx_msg void OnSelchangeListDllBox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABPAGE1_H__00B9CEA3_6278_4964_83DF_929D3168B7FD__INCLUDED_)
