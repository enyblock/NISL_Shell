// adapted_shell.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "adapted_shell.h"
#include "adapted_shellDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdapted_shellApp

BEGIN_MESSAGE_MAP(CAdapted_shellApp, CWinApp)
	//{{AFX_MSG_MAP(CAdapted_shellApp)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdapted_shellApp construction

CAdapted_shellApp::CAdapted_shellApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_curLang = 1;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAdapted_shellApp object

CAdapted_shellApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAdapted_shellApp initialization

BOOL CAdapted_shellApp::InitInstance()
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

	CAdapted_shellDlg dlg;
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
	return FALSE;
}


//设置文本
bool CAdapted_shellApp::__SetItemText(CDialog *pDlg, int itemID, int stringID)
{
	CString s;
	
	if(!s.LoadString(stringID)) 
		return false; // 从资源文件取字符串
	
	pDlg->GetDlgItem(itemID)->SetWindowText(s); // 设置控件文字　  
	return true;
}