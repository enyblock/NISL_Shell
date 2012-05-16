// self_update.h : main header file for the SELF_UPDATE application
//

#if !defined(AFX_SELF_UPDATE_H__173B9889_2CF7_4E09_9980_C03643E1E640__INCLUDED_)
#define AFX_SELF_UPDATE_H__173B9889_2CF7_4E09_9980_C03643E1E640__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSelf_updateApp:
// See self_update.cpp for the implementation of this class
//




class CSelf_updateApp : public CWinApp
{
public:
	CSelf_updateApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelf_updateApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSelf_updateApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELF_UPDATE_H__173B9889_2CF7_4E09_9980_C03643E1E640__INCLUDED_)
