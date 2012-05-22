#if !defined(AFX_DLLINPUT_H__9F414BA8_83C2_4821_B771_5D6129A3FD55__INCLUDED_)
#define AFX_DLLINPUT_H__9F414BA8_83C2_4821_B771_5D6129A3FD55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DllInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDllInput dialog

class CDllInput : public CDialog
{
// Construction
public:
	CDllInput(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDllInput)
	enum { IDD = IDD_DLL_INPUT };
	CString	m_brief_introduction;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDllInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDllInput)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLINPUT_H__9F414BA8_83C2_4821_B771_5D6129A3FD55__INCLUDED_)
