// self_updateDlg.h : header file
//

#if !defined(AFX_SELF_UPDATEDLG_H__175E5FC6_371A_4750_B4EA_7F5143941957__INCLUDED_)
#define AFX_SELF_UPDATEDLG_H__175E5FC6_371A_4750_B4EA_7F5143941957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSelf_updateDlg dialog

class CSelf_updateDlg : public CDialog
{
// Construction
public:
	CSelf_updateDlg(CWnd* pParent = NULL);	// standard constructor
	bool Download(const CString& strFileURLInServer, const CString & strFileLocalFullPath);//存放到本地的路径 

// Dialog Data
	//{{AFX_DATA(CSelf_updateDlg)
	enum { IDD = IDD_SELF_UPDATE_DIALOG };
	CProgressCtrl	m_progress;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelf_updateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSelf_updateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelfUpdate();
	afx_msg void OnCancle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELF_UPDATEDLG_H__175E5FC6_371A_4750_B4EA_7F5143941957__INCLUDED_)
