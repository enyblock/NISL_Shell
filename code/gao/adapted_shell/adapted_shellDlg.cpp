// adapted_shellDlg.cpp : implementation file
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
// CAboutDlg dialog used for App About


//define ctrl pressed
#define IsCTRLPressed() ((GetKeyState(VK_CONTROL) & (1<<(sizeof(SHORT)*8-1))) != 0)


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
// CAdapted_shellDlg dialog

CAdapted_shellDlg::CAdapted_shellDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdapted_shellDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdapted_shellDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAdapted_shellDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdapted_shellDlg)
	DDX_Control(pDX, IDC_TAB1, m_tabctrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAdapted_shellDlg, CDialog)
	//{{AFX_MSG_MAP(CAdapted_shellDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnUpdate)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON2, OnOpenFile)
	ON_BN_CLICKED(IDC_BUTTON4, OnAddDll)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON5, OnListDll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdapted_shellDlg message handlers

BOOL CAdapted_shellDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

//	h_dlgmain = this->m_hWnd;

	//添加table
	m_tabctrl.InsertItem(0,_T("Table"));
	m_tabctrl.InsertItem(0,_T("encryption"));
	m_tabctrl.InsertItem(0,_T("modify"));


	
	//创建dialog
 	m_tabpage[0].Create(IDD_DIALOG_PAG,&m_tabctrl);
 	m_tabpage[1].Create(IDD_DIALOG_PAG,&m_tabctrl);
 	m_tabpage[2].Create(IDD_DIALOG_PAG,&m_tabctrl);


	m_tabpage[0].m_main_dialog = this;
	m_tabpage[1].m_main_dialog = this;
	m_tabpage[2].m_main_dialog = this;


	



	//设定module list显示位置
	CRect rc;
	m_tabctrl.GetClientRect(rc);
	rc.top += 20;
	rc.bottom -= 8;
	rc.left +=3;
	rc.right -= 350;

	m_tabpage[0].MoveWindow(&rc);
	m_tabpage[1].MoveWindow(&rc);
	m_tabpage[2].MoveWindow(&rc);


	//保存指针
	pDialog[0] = &m_tabpage[0];
	pDialog[1] = &m_tabpage[1];
	pDialog[2] = &m_tabpage[2];

	//显示界面
	pDialog[0]->ShowWindow(SW_SHOW);
	pDialog[1]->ShowWindow(SW_HIDE);
	pDialog[2]->ShowWindow(SW_HIDE);


	//默认第一页
	m_tabctrl.SetCurSel(0); 


	m_CurSelTab = 0;


	//设定dll input显示位置
	CRect rc1;
	m_tabctrl.GetClientRect(rc1);
	rc1.top += 20;
	rc1.bottom -= 8;
	rc1.left +=220;
	rc1.right -= 8;
	
	int i = 0;
	while(i < DLL_NUM){
		//创建dialog
		m_dll_input[i].Create(IDD_DLL_INPUT,&m_tabctrl);
		m_dll_input[i].MoveWindow(&rc1);
		pDialog_input[i] = &m_dll_input[i];
		pDialog_input[i]->ShowWindow(SW_HIDE);
		i++;
	}


	pDialog_input[0]->ShowWindow(SW_SHOW);


//	GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE); 




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


	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAdapted_shellDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAdapted_shellDlg::OnPaint() 
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
HCURSOR CAdapted_shellDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAdapted_shellDlg::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	CString DownloadUrl = _T("http://117.22.127.117:8000/version_log.inf");
	
	CString LocalDirectory = _T("c:\\version_log_new.inf");
	
	Download(DownloadUrl,LocalDirectory);

	//AfxMessageBox("download");

	int status_comp = 0;

	typedef int (*p_create_process)(char *file_name);
	typedef int (*p_delete_process)(char *file_name);
	typedef int (*p_compare_log)(char *log, char *log_new);

	HINSTANCE hDLL;
	p_compare_log fun_comp = NULL;
	p_create_process fun_create = NULL;

	hDLL=LoadLibrary("update_tools.dll");//加载动态链接库MyDll.dll文件；
	fun_comp=(p_compare_log)GetProcAddress(hDLL,"compare_log");
	status_comp=fun_comp("c:\\version_log.inf","c:\\version_log_new.inf");	
	if (status_comp == 1)
	{	
		AfxMessageBox("update shell");
		int status_create = 0;
		fun_create=(p_create_process)GetProcAddress(hDLL,"create_process");
		status_create = fun_create("c:\\self_update.exe");
		if (status_create == 1)
		{
			//AfxMessageBox("success create");
		}else{
			AfxMessageBox("oh my dear create faile");
		}

	}else{
		AfxMessageBox("oh my dear not update");
	}
	
	

	FreeLibrary(hDLL);
	
	
}

//painted eggshell by enyblock
BOOL CAdapted_shellDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'X' && IsCTRLPressed())
	{
		AfxMessageBox("Made By Enyblock\nEmail: enyblock@gmail.com\nQQ: 502525713\nVersion: 4.00 ");
		return TRUE;
	}else{
		return FALSE;
	}
}




/*下载函数*/
bool CAdapted_shellDlg::Download(const CString& strFileURLInServer, const CString & strFileLocalFullPath)
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


/*实现拖拽功能*/
void CAdapted_shellDlg::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default

	/*切割点*/
	int i_num = 0;
	
	/*输入文件路径*/
	char* pFilePathName = new char[512];
	
	::DragQueryFile(hDropInfo, 0, pFilePathName, 512);  // 获取拖放文件的完整文件名
	
	//m_strFilePathName.Format("%s", pFilePathName);
	
	SetDlgItemText(IDC_EDIT_LOAD_FILE,_T(pFilePathName));
	::DragFinish(hDropInfo);   // 注意这个不能少，它用于释放Windows 为处理文件拖放而分配的内存
	
	delete[] pFilePathName;


	CString OutPathName;
	
	GetDlgItemText(IDC_EDIT_LOAD_FILE,OutPathName);



	/*find the suffix*/
	i_num = OutPathName.ReverseFind('.');

	CString temp = OutPathName.Right(OutPathName.GetLength()-i_num-1);

	if (i_num == -1){

		OutPathName += "_nisl";
			
	}else{
		
		/*添加后缀*/
		OutPathName = OutPathName.Left(i_num)+"_nisl.";
		
		OutPathName+=temp;
	}

	
	
	
	/*设置输出文件路径*/
	SetDlgItemText(IDC_EDIT_OUT_FILE,_T(OutPathName));
	
	/*设置只读*/
	CWnd *pEdit = GetDlgItem(IDC_EDIT_OUT_FILE);
	if (pEdit)
		 pEdit->EnableWindow(FALSE);

	

	
	CDialog::OnDropFiles(hDropInfo);


}


/*打开文件对话框*/

void CAdapted_shellDlg::OnOpenFile() 
{
	// TODO: Add your control notification handler code here
	
	/*切割点*/
	int i_num = 0;
	
	/*设置过滤器*/
	CString OpenFilter = "可执行文件(*.exe)|*.exe||";
	
	/*创建打开文件对话框*/
	CFileDialog FileDlgOpen(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, OpenFilter);

	/*正常显示，并且点击OK键*/
	if(IDOK == FileDlgOpen.DoModal())
	{
		/*获取文件路径*/
		CString PathName = FileDlgOpen.GetPathName();
		
		/*设置输入文件路径*/
		SetDlgItemText(IDC_EDIT_LOAD_FILE,_T(PathName));


		/*find the suffix*/
		i_num = PathName.ReverseFind('.');


		/*添加后缀*/
		CString OurPathName = PathName.Left(i_num)+"_nisl.exe";


		/*设置输出文件路径*/
		SetDlgItemText(IDC_EDIT_OUT_FILE,_T(OurPathName));

		/*设置只读*/
		CWnd *pEdit = GetDlgItem(IDC_EDIT_OUT_FILE);
		if (pEdit)
		 pEdit->EnableWindow(FALSE);


		
    }
}




/*增加dll信息*/
void CAdapted_shellDlg::OnAddDll()
{
	m_tabpage[0].m_CheckListBox.AddString("hhe");
}

void CAdapted_shellDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);
    m_CurSelTab = m_tabctrl.GetCurSel();
    pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);
	
	*pResult = 0;
}

void CAdapted_shellDlg::OnListDll() 
{
	// TODO: Add your control notification handler code here
	m_dll_info.GetDllName();
	
	/*清空listbox*/
	m_tabpage[0].m_CheckListBox.ResetContent();
	m_tabpage[1].m_CheckListBox.ResetContent();
	m_tabpage[2].m_CheckListBox.ResetContent();
	


	
	m_dll_info.GetDLLInformation();


	int i = 0;
	while (i < m_dll_info.m_df_dll_info.num){
		m_tabpage[m_dll_info.m_dll_common_info.dll_information[i].PluginType-1].m_CheckListBox.AddString(_T(m_dll_info.m_df_dll_info.dll_name[i]));
	    i++;
	}


	
}

void CAdapted_shellDlg::Display_Dll_Input(CString _dll_name)
{
	int index = 0;
	int i = 0;
	
	index = Return_Index_For_Dll_Name(_dll_name);

	AfxMessageBox(_dll_name);

	while (i < m_dll_info.m_df_dll_info.num){
		pDialog_input[i]->ShowWindow(SW_HIDE);
		i++;
	}
	
	pDialog_input[index]->ShowWindow(SW_SHOW);


	Set_Dll_Input_Dialog(index);

}


/*
 * 功能  ：通过索引值设定dll input界面
 * 参数	 ：dll 索引
 * 返回值：1 表示成功，0表示失败
 * 日期  ：2012-5-22
 * 作者  ：gaoxiang
 */

int CAdapted_shellDlg::Set_Dll_Input_Dialog(int _index)
{

//	pDialog_input[_index]->m_brief_introduction;
//	AfxMessageBox(m_dll_info.m_dll_common_info.dll_information[_index].Cmd[m_dll_info.m_dll_common_info.dll_information[_index].CmdNum]);

	return 1;
}



/*
 * 功能  ：返回dll名字对应dll_infor的索引值
 * 参数	 ：dll名字
 * 返回值：-1 表示查找失败，其他值为找到dll的索引值
 * 日期  ：2012-5-22
 * 作者  ：gaoxiang
 */

int CAdapted_shellDlg::Return_Index_For_Dll_Name(CString _dll_name)
{

	int  i = 0;

	while(i < m_dll_info.m_df_dll_info.num){
		
		if (!strcmp(_dll_name,m_dll_info.m_df_dll_info.dll_name[i]))
		{
			return i;
		}

		i++;
	}
	
	return -1;

}

