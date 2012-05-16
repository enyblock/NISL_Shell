// self_update.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "self_update.h"
#include "self_updateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelf_updateApp

BEGIN_MESSAGE_MAP(CSelf_updateApp, CWinApp)
	//{{AFX_MSG_MAP(CSelf_updateApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()





/////////////////////////////////////////////////////////////////////////////
// CSelf_updateApp construction

CSelf_updateApp::CSelf_updateApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSelf_updateApp object

CSelf_updateApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSelf_updateApp initialization

BOOL CSelf_updateApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CSelf_updateDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.


// 	CButton * pBtn = (CButton *)::GetDlgItem(IDC_BUTTON1);
// 	pBtn->SetWindowText(_T("我自己的按钮"));

	
	

	return FALSE;
}
