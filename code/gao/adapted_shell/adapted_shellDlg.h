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

#include "DLLINFO.h"
#include "DllInput.h"




class CAdapted_shellDlg : public CDialog
{
// Construction
public:
	CAdapted_shellDlg(CWnd* pParent = NULL);	// standard constructor
	bool Download(const CString& strFileURLInServer, const CString & strFileLocalFullPath);//存放到本地的路径 
	BOOL PreTranslateMessage(MSG* pMsg);
	void Display_Dll_Input(CString _dll_name);
	int  Set_Parameters(CString _dll_name);
	int  Return_Index_For_Dll_Name(CString _dll_name);
	int  Set_Dll_Input_Dialog();
	int  Show_Edit_And_Static(int limit,int i);
	int  Set_Parameters_From_Dll_Input_Dialog(int limit,int i);
	DWORD dstring_to_hex (const char *str);
	
    // Dialog Data
	//{{AFX_DATA(CAdapted_shellDlg)
	enum { IDD = IDD_ADAPTED_SHELL_DIALOG };

	//}}AFX_DATA

	/*tab control*/
	CTabCtrl	m_tabctrl;
	CTabPage1   m_tabpage[3];
	int         m_CurSelTab;
	CDialog     *pDialog[3];
	
	/*dll information*/
	DLLINFO     m_dll_info;
	
	
	/*dll input*/
	CDllInput   m_dll_input[DLL_NUM];
	CDialog     *pDialog_input[DLL_NUM];





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
	afx_msg void OnAddDll();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListDll();
	afx_msg void OnProtect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAPTED_SHELLDLG_H__E37913C6_AE07_4D28_A818_5CC4B8C04AB8__INCLUDED_)
