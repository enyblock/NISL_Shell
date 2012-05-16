// self_updateDlg.cpp : implementation file
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
// CSelf_updateDlg dialog

CSelf_updateDlg::CSelf_updateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelf_updateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelf_updateDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	
}

void CSelf_updateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelf_updateDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSelf_updateDlg, CDialog)
	//{{AFX_MSG_MAP(CSelf_updateDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnSelfUpdate)
	ON_BN_CLICKED(IDC_BUTTON2, OnCancle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelf_updateDlg message handlers

BOOL CSelf_updateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

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
	SkinH_Attach();
//	SkinH_SetAero(1);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSelf_updateDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSelf_updateDlg::OnPaint() 
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
HCURSOR CSelf_updateDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


/*下载函数*/
bool CSelf_updateDlg::Download(const CString& strFileURLInServer, const CString & strFileLocalFullPath)
{
	 ASSERT(strFileURLInServer != "");
	 ASSERT(strFileLocalFullPath != "");
	 CInternetSession session;
	 CHttpConnection* pHttpConnection = NULL;
	 CHttpFile* pHttpFile = NULL;
	 CString strServer, strObject;
	 INTERNET_PORT wPort;

	 DWORD dwType;
	 const int nTimeOut = 2000;
	 session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //重试之间的等待延时
	 session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);   //重试次数
	 char* pszBuffer = NULL;  

	 try
	 {
	  AfxParseURL(strFileURLInServer, dwType, strServer, strObject, wPort);
	  pHttpConnection = session.GetHttpConnection(strServer, wPort);
	  pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject);
	  if(pHttpFile->SendRequest() == FALSE)
	   return false;
	  DWORD dwStateCode;

	  pHttpFile->QueryInfoStatusCode(dwStateCode);
	  if(dwStateCode == HTTP_STATUS_OK)
	  {
		HANDLE hFile = CreateFile(strFileLocalFullPath, GENERIC_WRITE,
			 FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
			 NULL);  //创建本地文件
	   if(hFile == INVALID_HANDLE_VALUE)
	   {
		pHttpFile->Close();
		pHttpConnection->Close();
		session.Close();
		return false;
	   }
 
	   char szInfoBuffer[1000];  //返回消息
	   DWORD dwFileSize = 0;   //文件长度
	   DWORD dwInfoBufferSize = sizeof(szInfoBuffer);
	   BOOL bResult = FALSE;
	   bResult = pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH,
			   (void*)szInfoBuffer,&dwInfoBufferSize,NULL);

	   dwFileSize = atoi(szInfoBuffer);
	   const int BUFFER_LENGTH = 1024 * 10;
	   pszBuffer = new char[BUFFER_LENGTH];  //读取文件的缓冲
	   DWORD dwWrite, dwTotalWrite;
	   dwWrite = dwTotalWrite = 0;
	   UINT nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH); //读取服务器上数据

	   while(nRead > 0)
	   {
		WriteFile(hFile, pszBuffer, nRead, &dwWrite, NULL);  //写到本地文件
		dwTotalWrite += dwWrite;
		nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH);
	   }

	   delete[]pszBuffer;
	   pszBuffer = NULL;
	   CloseHandle(hFile);
	  }
	  else
	  {
	   delete[]pszBuffer;
	   pszBuffer = NULL;
	   if(pHttpFile != NULL)
	   {
		pHttpFile->Close();
		delete pHttpFile;
		pHttpFile = NULL;
	   }
	   if(pHttpConnection != NULL)
	   {
		pHttpConnection->Close();
		delete pHttpConnection;
		pHttpConnection = NULL;
	   }
	   session.Close();
		return false;
	  }
	 }
	 catch(...)
	 {
	  delete[]pszBuffer;
	  pszBuffer = NULL;
	  if(pHttpFile != NULL)
	  {
	   pHttpFile->Close();
	   delete pHttpFile;
	   pHttpFile = NULL;
	  }
	  if(pHttpConnection != NULL)
	  {
	   pHttpConnection->Close();
	   delete pHttpConnection;
	   pHttpConnection = NULL;
	  }
	  session.Close();
	  return false;
 }

 if(pHttpFile != NULL)
  pHttpFile->Close();
 if(pHttpConnection != NULL)
  pHttpConnection->Close();
 session.Close();
 return true;
}

void CSelf_updateDlg::OnSelfUpdate() 
{
	// TODO: Add your control notification handler code here



	/*关闭主进程*/
	int status_create = 0;
	int status_delete = 0;
	
	typedef int (*p_create_process)(char *file_name);
	typedef int (*p_delete_process)(char *file_name);
	
	HINSTANCE hDLL;
	
	p_delete_process fun_delete = NULL;
	p_create_process fun_create = NULL;
	
	hDLL=LoadLibrary("update_tools1.dll");							   //加载动态链接库MyDll.dll文件；
	
	fun_delete=(p_delete_process)GetProcAddress(hDLL,"delete_process");
	
	status_delete = fun_delete("adapted_shell.exe");
	
	if (status_delete == 0)
	{	
		//AfxMessageBox("delete success");
		
	}else{
		AfxMessageBox("oh my dear,delete fail");
	}
	
	


	
	/*下载资源*/
	// TODO: Add your control notification handler code here
	CString DownloadUrl = _T("http://117.22.127.117:8000/README");
	
	CString LocalDirectory = _T("c:\\README");
	
	if(!Download(DownloadUrl,LocalDirectory))
	{
		AfxMessageBox("oh my dear,donwload fail");
	}else{
		
		//AfxMessageBox("download");
		
	}





	/*进度条控制*/
	m_progress.SetRange(0,100);
	int i = 0;
	while (i < 100)
	{
		Sleep(10);
		m_progress.SetPos(i+1);
		i++;
		
	}

	

	if ( IDOK == MessageBox("close me,and start main program","download finished",MB_DEFBUTTON2) )
	{

		/*删除和重命名日志文件*/
		remove("c:\\version_log.inf");
		rename("c:\\version_log_new.inf","c:\\version_log.inf");
	

		
		/*创建主进程*/
		fun_create=(p_create_process)GetProcAddress(hDLL,"create_process");
		
		status_create = fun_create("c:\\adapted_shell.exe");
		
		if (status_create == 1)
		{	
			//AfxMessageBox("create success");
			
		}else{
			AfxMessageBox("oh my dear,create fail");
		}



		/*结束自身进程*/
		status_delete = 0;
		status_delete = fun_delete("self_update.exe");
		
		if (status_delete == 0)
		{	
			//AfxMessageBox("delete success");
			
		}else{
			AfxMessageBox("oh my dear,delete fail");
		}
		
	
	}


	
	FreeLibrary(hDLL);

		
}

void CSelf_updateDlg::OnCancle() 
{
	// TODO: Add your control notification handler code here
	m_progress.SetRange(0,100);
	int i = 0;
	while (i < 100)
	{
		Sleep(20);
		m_progress.SetPos(i+1);
		i++;
		
	}
	
}
