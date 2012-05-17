// test_dll.h : main header file for the TEST_DLL application
//

#if !defined(AFX_TEST_DLL_H__9D5A0B69_6E31_4E0F_B927_18244A70E271__INCLUDED_)
#define AFX_TEST_DLL_H__9D5A0B69_6E31_4E0F_B927_18244A70E271__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTest_dllApp:
// See test_dll.cpp for the implementation of this class
//

class CTest_dllApp : public CWinApp
{
public:
	CTest_dllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest_dllApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTest_dllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_DLL_H__9D5A0B69_6E31_4E0F_B927_18244A70E271__INCLUDED_)
