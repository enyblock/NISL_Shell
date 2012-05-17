// test_dllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test_dll.h"
#include "test_dllDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTest_dllDlg dialog

CTest_dllDlg::CTest_dllDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTest_dllDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTest_dllDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTest_dllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTest_dllDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTest_dllDlg, CDialog)
	//{{AFX_MSG_MAP(CTest_dllDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTest_dllDlg message handlers

BOOL CTest_dllDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_CheckListBox.SubclassDlgItem(IDC_LIST_BOX, this); // IDC_LIST1是ListBox控件的资源ID
    m_CheckListBox.SetCheckStyle(BS_AUTOCHECKBOX);

	// Add "About..." menu item to system menu.
// 	m_CheckListBox.AddString(_T("gao.dll"));
// 	m_CheckListBox.AddString(_T("shift.dll"));
// 	m_CheckListBox.AddString(_T("list.dll"));

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTest_dllDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTest_dllDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTest_dllDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTest_dllDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	int num = 0;
    typedef int (*p_sum)(int num1 , int num2);
	typedef int (*p_create_process)(char *file_name);
	typedef int (*p_delete_process)(char *file_name);
	typedef int (*p_compare_log)(char *log, char *log_new);
	HINSTANCE hDLL;
	p_sum fun = NULL;
	hDLL=LoadLibrary("update_tools.dll");//加载动态链接库MyDll.dll文件；
	fun=(p_sum)GetProcAddress(hDLL,"my_sum");
	num=fun(2,2);	
	if (num == 4)
	{
		AfxMessageBox("success");
	}

	FreeLibrary(hDLL);


}


void CTest_dllDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	int num = 0;
	typedef int (*p_delete_process)(char *file_name);
	HINSTANCE hDLL;
	p_delete_process fun = NULL;
	hDLL=LoadLibrary("update_tools.dll");//加载动态链接库MyDll.dll文件；
	fun=(p_delete_process)GetProcAddress(hDLL,"delete_process");
	num=fun("test.exe");	
	if (num == 0)
	{
		AfxMessageBox("delete success");
	}
		
	FreeLibrary(hDLL);
	
}

void CTest_dllDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	int num = 0;
	typedef int (*p_create_process)(char *file_name);
	HINSTANCE hDLL;
	p_create_process fun = NULL;
	hDLL=LoadLibrary("update_tools.dll");//加载动态链接库MyDll.dll文件；
	fun=(p_create_process)GetProcAddress(hDLL,"create_process");
	num=fun("c:\\test.exe");	
	if (num == 1)
	{
		AfxMessageBox("create success");
	}
	
	FreeLibrary(hDLL);
	
}

void CTest_dllDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	
	m_dll_info.GetDllName();
		
	int i = 0;
	while (i < m_dll_info.m_df_dll_info.num)
		m_CheckListBox.AddString(_T(m_dll_info.m_df_dll_info.dll_name[i++]));
	




}
