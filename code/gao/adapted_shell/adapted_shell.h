// adapted_shell.h : main header file for the ADAPTED_SHELL application
//

#if !defined(AFX_ADAPTED_SHELL_H__CC84D677_DB70_4B82_8B98_A90608578141__INCLUDED_)
#define AFX_ADAPTED_SHELL_H__CC84D677_DB70_4B82_8B98_A90608578141__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAdapted_shellApp:
// See adapted_shell.cpp for the implementation of this class
//

class CAdapted_shellApp : public CWinApp
{
public:
	CAdapted_shellApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdapted_shellApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAdapted_shellApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAPTED_SHELL_H__CC84D677_DB70_4B82_8B98_A90608578141__INCLUDED_)
