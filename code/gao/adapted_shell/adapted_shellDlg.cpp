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
extern CAdapted_shellApp theApp;

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



	/*���Է��ֹ��캯������û�г�ʼ�������  2012-5-30 */
	m_CurSelTab = 0;
	pDialog[0] = NULL;
	pDialog[1] = NULL;
	pDialog[2] = NULL;

	m_dll_info.m_dll_common_info.dll_num = 0;

	m_dll_info.m_df_dll_info.num == 0;

	int i = 0;

	while (i < DLL_NUM){
		pDialog_input[i] = NULL;
		i++;
	}
}

void CAdapted_shellDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdapted_shellDlg)
	DDX_Control(pDX, IDC_COMBO_LANGUAGE_LIST, m_language_list);
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
	ON_BN_CLICKED(IDC_BUTTON6, OnProtect)
	ON_CBN_SELCHANGE(IDC_COMBO_LANGUAGE_LIST, OnSelchangeComboLanguageList)
	ON_BN_CLICKED(IDC_BUTTON_LIST_DLL, OnButtonListDll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdapted_shellDlg message handlers

BOOL CAdapted_shellDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.



//	h_dlgmain = this->m_hWnd;

	CString table1 = _T("");
	CString table2 = _T("");
	CString table3 = _T("");


	table1.LoadString(IDS_STRING_TABLE1);
	table2.LoadString(IDS_STRING_TABLE2);
	table3.LoadString(IDS_STRING_TABLE3);




	//���table
	m_tabctrl.InsertItem(0,_T(table3));
	m_tabctrl.InsertItem(1,_T(table2));
	m_tabctrl.InsertItem(2,_T(table1));




	
	//����dialog
 	m_tabpage[0].Create(IDD_DIALOG_PAG,&m_tabctrl);
 	m_tabpage[1].Create(IDD_DIALOG_PAG,&m_tabctrl);
	m_tabpage[2].Create(IDD_DIALOG_PAG,&m_tabctrl);



	m_tabpage[0].m_main_dialog = this;
	m_tabpage[1].m_main_dialog = this;
	m_tabpage[2].m_main_dialog = this;


	



	//�趨module list��ʾλ��
	CRect rc;
	m_tabctrl.GetClientRect(rc);
	rc.top += 20;
	rc.bottom -= 8;
	rc.left +=3;
	rc.right -= 350;

	m_tabpage[0].MoveWindow(&rc);
	m_tabpage[1].MoveWindow(&rc);
	m_tabpage[2].MoveWindow(&rc);


	//����ָ��
	pDialog[0] = &m_tabpage[0];
	pDialog[1] = &m_tabpage[1];
	pDialog[2] = &m_tabpage[2];

	//��ʾ����
	pDialog[0]->ShowWindow(SW_SHOW);
	pDialog[1]->ShowWindow(SW_HIDE);
	pDialog[2]->ShowWindow(SW_HIDE);


	//Ĭ�ϵ�һҳ
	m_tabctrl.SetCurSel(0); 


	m_CurSelTab = 0;


	//�趨dll input��ʾλ��
	CRect rc1;
	m_tabctrl.GetClientRect(rc1);
	rc1.top += 20;
	rc1.bottom -= 8;
	rc1.left +=220;
	rc1.right -= 8;
	
	int i = 0;
	while(i < DLL_NUM){
		//����dialog
		m_dll_input[i].Create(IDD_DLL_INPUT,&m_tabctrl);
		m_dll_input[i].MoveWindow(&rc1);
		pDialog_input[i] = &m_dll_input[i];
		pDialog_input[i]->ShowWindow(SW_HIDE);
		i++;
	}


	pDialog_input[0]->ShowWindow(SW_SHOW);



	char     my_path[MAX_PATH];
	CString  self_update_path;
	
	
	/*��ȡ��ǰ��������·��*/
	int pathlen = ::GetModuleFileName(NULL,my_path,MAX_PATH);
	
	
	
	/*ȥ�����.EXE��*/
	while (1){
		if (my_path[pathlen--] == '\\')
			break;
	}
	
	my_path[++pathlen] = 0x0;
	
	
	
	/*��������·��*/
	self_update_path = my_path;
	
	//	update_list_log_path = my_path;
	
	
	
	/*��������Ŀ��*/	
	self_update_path += "\\self_update.exe";


	remove((LPSTR)(LPCTSTR)self_update_path);


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

	m_language_list.SetCurSel(theApp.m_curLang);
	OnSelchangeComboLanguageList();

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


	char     my_path[MAX_PATH];
	CString  version_log_path;
	CString  version_log_path_old;
	CString  self_update_path;
	
	
	/*��ȡ��ǰ��������·��*/
	int pathlen = ::GetModuleFileName(NULL,my_path,MAX_PATH);
	
	
	
	/*ȥ�����.EXE��*/
	while (1){
		if (my_path[pathlen--] == '\\')
			break;
	}
	
	my_path[++pathlen] = 0x0;



	/*��������·��*/
	version_log_path = my_path;
	version_log_path_old = my_path;

	self_update_path = my_path;

//	update_list_log_path = my_path;



	/*��������Ŀ��*/
	version_log_path += "\\version_log_new.inf";
	version_log_path_old += "\\version_log.inf";
//	update_list_log_path += "\\update_list_log.inf";

	self_update_path += "\\self_update.exe";


//	AfxMessageBox(version_log_path);
//	AfxMessageBox(update_list_log_path);
//	AfxMessageBox(version_log_path_old);


	/*���ظ�����־*/
	
	CString DownloadUrl = _T("http://127.0.0.1:8000/version_log.inf");
	CString LocalDirectory = _T(version_log_path);
	
	if(!Download(DownloadUrl,LocalDirectory)){
		
		AfxMessageBox("oh my dear,donwload fail");	
	}else{
		//AfxMessageBox("download");
	}

	

	int status_comp = 0;

	typedef int (*p_create_process)(char *file_name);
	typedef int (*p_delete_process)(char *file_name);
	typedef int (*p_compare_log)(char *log, char *log_new);

	HINSTANCE hDLL;
	p_compare_log fun_comp = NULL;
	p_create_process fun_create = NULL;

	hDLL=LoadLibrary("update_tools.dll");//���ض�̬���ӿ�MyDll.dll�ļ���
	fun_comp=(p_compare_log)GetProcAddress(hDLL,"compare_log");
	status_comp=fun_comp((LPSTR)(LPCTSTR)version_log_path_old,(LPSTR)(LPCTSTR)version_log_path);	
	if (status_comp == 1)
	{	
		AfxMessageBox("update shell");
		remove((LPSTR)(LPCTSTR)version_log_path_old);
		rename((LPSTR)(LPCTSTR)version_log_path,(LPSTR)(LPCTSTR)version_log_path_old);


		/*�ͷ���������*/
		HRSRC hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(IDR_SELF_UPDATE2), TEXT("SELF_UPDATE"));
		
		if (NULL == hRsrc)
		{
			AfxMessageBox("no found");
			return;
		}
		
		//load resource to memory
		HGLOBAL hGlobal = ::LoadResource(NULL, hRsrc);
		if (NULL == hGlobal)
		{
			AfxMessageBox("load fail");
			return;
		}
		
		//calculate the size of buffer
		DWORD dwSize = ::SizeofResource(NULL, hRsrc);
		
		LPVOID pBuffer = ::LockResource(hGlobal);
		
		
		FILE *fp = _tfopen((LPSTR)(LPCTSTR)self_update_path, _T("wb"));
		
		if (fp != NULL)
		{
			if (dwSize != fwrite(pBuffer, sizeof(char), dwSize, fp))
			{
				AfxMessageBox("creat fail");
			}
			
			fclose(fp);
		}
		
		
	    ::FreeResource(hGlobal);
		Sleep(500);


		int status_create = 0;
		fun_create=(p_create_process)GetProcAddress(hDLL,"create_process");
		status_create = fun_create((LPSTR)(LPCTSTR)self_update_path);
		if (status_create == 1)
		{
			FreeLibrary(hDLL);
			//AfxMessageBox("success create");
		}else{
			FreeLibrary(hDLL);
			AfxMessageBox("oh my dear create faile");
		}

	}else{
		remove((LPSTR)(LPCTSTR)version_log_path);
		FreeLibrary(hDLL);
		AfxMessageBox("oh my dear not update");
	}
	
	
	
	
}

//painted eggshell by enyblock
BOOL CAdapted_shellDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == 'X' && IsCTRLPressed())
	{
		AfxMessageBox("Made By Enyblock\nEmail: enyblock@gmail.com\nQQ: 502525713\nVersion: 3.0 ");
		return TRUE;
	}else{
		return FALSE;
	}
}




/*���غ���*/
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
	 session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); //����֮��ĵȴ���ʱ
	 session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);   //���Դ���
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
			 NULL);  //���������ļ�
	   if(hFile == INVALID_HANDLE_VALUE)
	   {
		pHttpFile->Close();
		pHttpConnection->Close();
		session.Close();
		return false;
	   }
 
	   char szInfoBuffer[1000];  //������Ϣ
	   DWORD dwFileSize = 0;   //�ļ�����
	   DWORD dwInfoBufferSize = sizeof(szInfoBuffer);
	   BOOL bResult = FALSE;
	   bResult = pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH,
			   (void*)szInfoBuffer,&dwInfoBufferSize,NULL);

	   dwFileSize = atoi(szInfoBuffer);
	   const int BUFFER_LENGTH = 1024 * 10;
	   pszBuffer = new char[BUFFER_LENGTH];  //��ȡ�ļ��Ļ���
	   DWORD dwWrite, dwTotalWrite;
	   dwWrite = dwTotalWrite = 0;
	   UINT nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH); //��ȡ������������

	   while(nRead > 0)
	   {
		WriteFile(hFile, pszBuffer, nRead, &dwWrite, NULL);  //д�������ļ�
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


/*ʵ����ק����*/
void CAdapted_shellDlg::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default

	/*�и��*/
	int i_num = 0;
	
	/*�����ļ�·��*/
	char* pFilePathName = new char[512];
	
	::DragQueryFile(hDropInfo, 0, pFilePathName, 512);  // ��ȡ�Ϸ��ļ��������ļ���
	
	//m_strFilePathName.Format("%s", pFilePathName);
	
	SetDlgItemText(IDC_EDIT_LOAD_FILE,_T(pFilePathName));
	::DragFinish(hDropInfo);   // ע����������٣��������ͷ�Windows Ϊ�����ļ��ϷŶ�������ڴ�
	
	delete[] pFilePathName;


	CString OutPathName;
	
	GetDlgItemText(IDC_EDIT_LOAD_FILE,OutPathName);



	/*find the suffix*/
	i_num = OutPathName.ReverseFind('.');

	CString temp = OutPathName.Right(OutPathName.GetLength()-i_num-1);

	if (i_num == -1){

		OutPathName += "_nisl";
			
	}else{
		
		/*��Ӻ�׺*/
		OutPathName = OutPathName.Left(i_num)+"_nisl.";
		
		OutPathName+=temp;
	}

	
	
	
	/*��������ļ�·��*/
	SetDlgItemText(IDC_EDIT_OUT_FILE,_T(OutPathName));
	
	/*����ֻ��*/
	CWnd *pEdit = GetDlgItem(IDC_EDIT_OUT_FILE);
	if (pEdit)
		 pEdit->EnableWindow(FALSE);

	

	
	CDialog::OnDropFiles(hDropInfo);


}


/*���ļ��Ի���*/

void CAdapted_shellDlg::OnOpenFile() 
{
	// TODO: Add your control notification handler code here
	
	/*�и��*/
	int i_num = 0;
	
	/*���ù�����*/
	CString OpenFilter = "��ִ���ļ�(*.exe)|*.exe||";
	
	/*�������ļ��Ի���*/
	CFileDialog FileDlgOpen(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, OpenFilter);

	/*������ʾ�����ҵ��OK��*/
	if(IDOK == FileDlgOpen.DoModal())
	{
		/*��ȡ�ļ�·��*/
		CString PathName = FileDlgOpen.GetPathName();
		
		/*���������ļ�·��*/
		SetDlgItemText(IDC_EDIT_LOAD_FILE,_T(PathName));


		/*find the suffix*/
		i_num = PathName.ReverseFind('.');


		/*��Ӻ�׺*/
		CString OurPathName = PathName.Left(i_num)+"_nisl.exe";


		/*��������ļ�·��*/
		SetDlgItemText(IDC_EDIT_OUT_FILE,_T(OurPathName));

		/*����ֻ��*/
		CWnd *pEdit = GetDlgItem(IDC_EDIT_OUT_FILE);
		if (pEdit)
		 pEdit->EnableWindow(FALSE);


		
    }
}




/*����dll��Ϣ*/
void CAdapted_shellDlg::OnAddDll()
{
	m_tabpage[0].m_CheckListBox.AddString("hhe");
}



/*tab�л���Ӧ��Ϣ*/
void CAdapted_shellDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);
    m_CurSelTab = m_tabctrl.GetCurSel();
    pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);



//	if (m_tabpage[m_CurSelTab].m_CheckListBox.GetCount() <= 0)
//	{
//		if (m_dll_info.m_dll_common_info.dll_num == 0)
//		{
//			return;
//		}

//		int i = 0;
/*
		while (i < m_dll_info.m_df_dll_info.num){
			pDialog_input[i]->ShowWindow(SW_HIDE);
			i++;
		}
*/		
//		pDialog_input[m_dll_info.m_df_dll_info.num]->ShowWindow(SW_SHOW);
//		Set_Dll_Input_Dialog();
//		pDialog_input[m_dll_info.m_df_dll_info.num]->ShowWindow(SW_SHOW);

//		return;
		
	
//	}




	if (m_tabpage[m_CurSelTab].m_CheckListBox.GetCount()>0){

		CString strTemp;   
		m_tabpage[m_CurSelTab].m_CheckListBox.GetText(m_tabpage[m_CurSelTab].m_CheckListBox.GetCurSel(),strTemp); 
		Display_Dll_Input(strTemp);
//		Set_Dll_Input_Dialog();
	//	AfxMessageBox(strTemp);

	//	AfxMessageBox("����0");

	}else{
	//	CString strTemp = "";
	//	Display_Dll_Input(strTemp);
	//	pDialog_input[20]->ShowWindow(SW_SHOW);
		
	}



//	AfxMessageBox("change");

//  	CString strTemp;   
//  	m_tabpage[m_CurSelTab].m_CheckListBox.GetText(m_tabpage[m_CurSelTab].m_CheckListBox.GetCurSel(),strTemp); 
//  	AfxMessageBox(strTemp);
	
	*pResult = 0;
}

void CAdapted_shellDlg::OnListDll() 
{
	// TODO: Add your control notification handler code here

	if (m_dll_info.GetDllName())
	{
			MessageBox(_T(LoadLanguageFromId(IDS_STRING_LIST_SUCCESS)),"",MB_OK);
			
	}else{
			MessageBox(_T(LoadLanguageFromId(IDS_STRING_LIST_FAIL)),"",MB_OK);
	}
	
	/*���listbox*/
	m_tabpage[0].m_CheckListBox.ResetContent();
	m_tabpage[1].m_CheckListBox.ResetContent();
	m_tabpage[2].m_CheckListBox.ResetContent();
	


	
	m_dll_info.GetDLLInformation();


	int i = 0;
	while (i < m_dll_info.m_df_dll_info.num){
		m_tabpage[m_dll_info.m_dll_common_info.dll_information[i].PluginType-1].m_CheckListBox.AddString(_T(m_dll_info.m_df_dll_info.dll_name[i]));
	    i++;
	}

	m_tabpage[0].m_CheckListBox.SetCurSel(0);
	m_tabpage[1].m_CheckListBox.SetCurSel(0);
	m_tabpage[2].m_CheckListBox.SetCurSel(0);



	Set_Dll_Input_Dialog();
	
}


/*
 * ����  ��ͨ��DLL ������ʾdll input����
 * ����  ��2012-5-22
 * ����  ��gaoxiang
 */

void CAdapted_shellDlg::Display_Dll_Input(CString _dll_name)
{
	int index = 0;
	int i = 0;

//	if (_dll_name == ""){
//		AfxMessageBox("kong");
		
		
//		pDialog_input[m_dll_info.m_df_dll_info.num]->ShowWindow(SW_SHOW);
		
//		
//		return;
//	}

	
	index = Return_Index_For_Dll_Name(_dll_name);

//	AfxMessageBox(_dll_name);




	while (i < m_dll_info.m_df_dll_info.num){
		pDialog_input[i]->ShowWindow(SW_HIDE);
		i++;
	}
		
	pDialog_input[index]->ShowWindow(SW_SHOW);



//	Set_Dll_Input_Dialog();

}


/*
 * ����  ��ͨ��DLL ��������dll����
 * ����ֵ��-1 ���ش�������ֵΪ���ص�����ֵ
 * ����  ��2012-5-25
 * ����  ��gaoxiang
 */
int CAdapted_shellDlg::Set_Parameters(CString _dll_name)
{
	int index = -1;
	int i = 0;
	
	index = Return_Index_For_Dll_Name(_dll_name);




	Set_Parameters_From_Dll_Input_Dialog(m_dll_info.m_dll_common_info.dll_information[index].CmdNum,index);


	return index;

}


/*
 * ����  �������������������ʾ�༭�����
 * ����ֵ��1 ��ʾ�ɹ���0��ʾʧ��
 * ����  ��2012-5-22
 * ����  ��gaoxiang
 */

int CAdapted_shellDlg::Set_Parameters_From_Dll_Input_Dialog(int limit,int i)
{
	CString str1;
	CString str2;
	CString str3;
	CString str4;
	CString str5;
	CString str6;
	CString str7;
	CString str8;
	CString str9;
	CString str10;

	switch(limit){
		case 0:{
			;
			   }break;
		case 1:{
			
					m_dll_input[i].GetDlgItemText(IDC_EDIT1,str1);	


					m_dll_info.m_dll_common_info.dll_information[i].Cmd[0] = dstring_to_hex((LPSTR)(LPCTSTR)str1);

			   }break;
		case 2:{
					m_dll_input[i].GetDlgItemText(IDC_EDIT1,str1);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT2,str2);	



					m_dll_info.m_dll_common_info.dll_information[i].Cmd[0] = dstring_to_hex((LPSTR)(LPCTSTR)str1);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[1] = dstring_to_hex((LPSTR)(LPCTSTR)str2);


			   }break;
		case 3:{
					m_dll_input[i].GetDlgItemText(IDC_EDIT1,str1);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT2,str2);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT3,str3);



					
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[0] = dstring_to_hex((LPSTR)(LPCTSTR)str1);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[1] = dstring_to_hex((LPSTR)(LPCTSTR)str2);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[2] = dstring_to_hex((LPSTR)(LPCTSTR)str3);

			   }break;
		case 4:{
					m_dll_input[i].GetDlgItemText(IDC_EDIT1,str1);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT2,str2);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT3,str3);
					m_dll_input[i].GetDlgItemText(IDC_EDIT4,str4);




					
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[0] = dstring_to_hex((LPSTR)(LPCTSTR)str1);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[1] = dstring_to_hex((LPSTR)(LPCTSTR)str2);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[2] = dstring_to_hex((LPSTR)(LPCTSTR)str3);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[3] = dstring_to_hex((LPSTR)(LPCTSTR)str4);


			   }break;
		case 5:{
					m_dll_input[i].GetDlgItemText(IDC_EDIT1,str1);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT2,str2);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT3,str3);
					m_dll_input[i].GetDlgItemText(IDC_EDIT4,str4);
					m_dll_input[i].GetDlgItemText(IDC_EDIT5,str5);




					
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[0] = dstring_to_hex((LPSTR)(LPCTSTR)str1);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[1] = dstring_to_hex((LPSTR)(LPCTSTR)str2);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[2] = dstring_to_hex((LPSTR)(LPCTSTR)str3);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[3] = dstring_to_hex((LPSTR)(LPCTSTR)str4);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[4] = dstring_to_hex((LPSTR)(LPCTSTR)str5);

			   }break;
		case 6:{
					m_dll_input[i].GetDlgItemText(IDC_EDIT1,str1);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT2,str2);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT3,str3);
					m_dll_input[i].GetDlgItemText(IDC_EDIT4,str4);
					m_dll_input[i].GetDlgItemText(IDC_EDIT5,str5);
					m_dll_input[i].GetDlgItemText(IDC_EDIT6,str6);	
	



					
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[0] = dstring_to_hex((LPSTR)(LPCTSTR)str1);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[1] = dstring_to_hex((LPSTR)(LPCTSTR)str2);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[2] = dstring_to_hex((LPSTR)(LPCTSTR)str3);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[3] = dstring_to_hex((LPSTR)(LPCTSTR)str4);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[4] = dstring_to_hex((LPSTR)(LPCTSTR)str5);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[5] = dstring_to_hex((LPSTR)(LPCTSTR)str6);

			   }break;
		case 7:{
					m_dll_input[i].GetDlgItemText(IDC_EDIT1,str1);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT2,str2);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT3,str3);
					m_dll_input[i].GetDlgItemText(IDC_EDIT4,str4);
					m_dll_input[i].GetDlgItemText(IDC_EDIT5,str5);
					m_dll_input[i].GetDlgItemText(IDC_EDIT6,str6);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT7,str7);	



					
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[0] = dstring_to_hex((LPSTR)(LPCTSTR)str1);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[1] = dstring_to_hex((LPSTR)(LPCTSTR)str2);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[2] = dstring_to_hex((LPSTR)(LPCTSTR)str3);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[3] = dstring_to_hex((LPSTR)(LPCTSTR)str4);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[4] = dstring_to_hex((LPSTR)(LPCTSTR)str5);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[5] = dstring_to_hex((LPSTR)(LPCTSTR)str6);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[6] = dstring_to_hex((LPSTR)(LPCTSTR)str7);

			   }break;
		case 8:{
					m_dll_input[i].GetDlgItemText(IDC_EDIT1,str1);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT2,str2);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT3,str3);
					m_dll_input[i].GetDlgItemText(IDC_EDIT4,str4);
					m_dll_input[i].GetDlgItemText(IDC_EDIT5,str5);
					m_dll_input[i].GetDlgItemText(IDC_EDIT6,str6);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT7,str7);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT8,str8);


					
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[0] = dstring_to_hex((LPSTR)(LPCTSTR)str1);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[1] = dstring_to_hex((LPSTR)(LPCTSTR)str2);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[2] = dstring_to_hex((LPSTR)(LPCTSTR)str3);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[3] = dstring_to_hex((LPSTR)(LPCTSTR)str4);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[4] = dstring_to_hex((LPSTR)(LPCTSTR)str5);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[5] = dstring_to_hex((LPSTR)(LPCTSTR)str6);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[6] = dstring_to_hex((LPSTR)(LPCTSTR)str7);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[7] = dstring_to_hex((LPSTR)(LPCTSTR)str8);

			   }break;
		case 9:{
					m_dll_input[i].GetDlgItemText(IDC_EDIT1,str1);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT2,str2);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT3,str3);
					m_dll_input[i].GetDlgItemText(IDC_EDIT4,str4);
					m_dll_input[i].GetDlgItemText(IDC_EDIT5,str5);
					m_dll_input[i].GetDlgItemText(IDC_EDIT6,str6);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT7,str7);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT8,str8);
					m_dll_input[i].GetDlgItemText(IDC_EDIT9,str9);

					
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[0] = dstring_to_hex((LPSTR)(LPCTSTR)str1);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[1] = dstring_to_hex((LPSTR)(LPCTSTR)str2);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[2] = dstring_to_hex((LPSTR)(LPCTSTR)str3);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[3] = dstring_to_hex((LPSTR)(LPCTSTR)str4);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[4] = dstring_to_hex((LPSTR)(LPCTSTR)str5);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[5] = dstring_to_hex((LPSTR)(LPCTSTR)str6);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[6] = dstring_to_hex((LPSTR)(LPCTSTR)str7);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[7] = dstring_to_hex((LPSTR)(LPCTSTR)str8);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[8] = dstring_to_hex((LPSTR)(LPCTSTR)str9);

			   }break;
		case 10:{
					m_dll_input[i].GetDlgItemText(IDC_EDIT1,str1);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT2,str2);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT3,str3);
					m_dll_input[i].GetDlgItemText(IDC_EDIT4,str4);
					m_dll_input[i].GetDlgItemText(IDC_EDIT5,str5);
					m_dll_input[i].GetDlgItemText(IDC_EDIT6,str6);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT7,str7);	
					m_dll_input[i].GetDlgItemText(IDC_EDIT8,str8);
					m_dll_input[i].GetDlgItemText(IDC_EDIT9,str9);
					m_dll_input[i].GetDlgItemText(IDC_EDIT10,str10);
					
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[0] = dstring_to_hex((LPSTR)(LPCTSTR)str1);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[1] = dstring_to_hex((LPSTR)(LPCTSTR)str2);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[2] = dstring_to_hex((LPSTR)(LPCTSTR)str3);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[3] = dstring_to_hex((LPSTR)(LPCTSTR)str4);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[4] = dstring_to_hex((LPSTR)(LPCTSTR)str5);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[5] = dstring_to_hex((LPSTR)(LPCTSTR)str6);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[6] = dstring_to_hex((LPSTR)(LPCTSTR)str7);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[7] = dstring_to_hex((LPSTR)(LPCTSTR)str8);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[8] = dstring_to_hex((LPSTR)(LPCTSTR)str9);
					m_dll_info.m_dll_common_info.dll_information[i].Cmd[9] = dstring_to_hex((LPSTR)(LPCTSTR)str10);
			   }break;
		default:;
	}


	return 1;
			
}

/*
 * ����  ��ʵ���ַ���ת16����
 * ����ֵ��ת����dwordֵ
 * ����  ��2012-5-28
 * ����  ��gaoxiang
 */
DWORD CAdapted_shellDlg::dstring_to_hex (const char *str)
{


 	if (strcmp(str,"") == 0)
 	{
 		return  -1;
 	}

	int   i = 0;
	char  *index = "0123456789abcdef";				//��¼��������
	char  *temp  = strdup(str);						//copy str
	char  *lower = strlwr(temp);
	char  *find  = NULL;
	DWORD dword = 0;
	
	if (strstr(lower,"short ")) {						//���"short"���
		strcpy(lower,lower+6);
	}
	else if (strstr(lower,"far "))
	{
        strcpy(lower,lower+4);
	} 
	else
	{
		;
	}
	
	
	while (i < strlen(lower)) {
		
		find = strchr(index,lower[i]);
		
		dword = dword ^ (((DWORD)(find-index)) << ((strlen(lower)-1-i)*4));
		
		i++;
	}
	
	return dword;

}

//ͨ������ID������Ӧ�������ַ���
CString CAdapted_shellDlg::LoadLanguageFromId(UINT lId)
{
	CString temp;

	temp.LoadString(lId);

	return temp;
}



//�������������ı���ʾ
void CAdapted_shellDlg::__SetDlgLang(int langID)
{
	switch(langID) // �жϲ����õ�ǰ��������
	{
	case  0:SetThreadLocale(MAKELANGID(LANG_ENGLISH,SUBLANG_ENGLISH_US));break;
	case  1:SetThreadLocale(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED));break;
	default: break;
	}
	
	theApp.m_curLang = langID; // ͬʱ�޸�ȫ�ֱ���
	
	theApp.__SetItemText(this, IDC_BUTTON_UPDATE, IDC_BUTTON_UPDATE); // �޸İ�ť����
	theApp.__SetItemText(this, IDC_BUTTON_OPEN_FILE, IDC_BUTTON_OPEN_FILE);
	theApp.__SetItemText(this, IDC_BUTTON_ADD_DLL, IDC_BUTTON_ADD_DLL);
	theApp.__SetItemText(this, IDC_BUTTON_LIST_DLL, IDC_BUTTON_LIST_DLL);
	theApp.__SetItemText(this, IDC_BUTTON_PROTECT, IDC_BUTTON_PROTECT);
	theApp.__SetItemText(this, IDC_STATIC_LOAD_FILE, IDC_STATIC_LOAD_FILE);
	theApp.__SetItemText(this, IDC_STATIC_OUT_FILE, IDC_STATIC_OUT_FILE);
	theApp.__SetItemText(this, IDC_STATIC_LANGUAGES, IDC_STATIC_LANGUAGE);



	CString table1 = _T("");
	CString table2 = _T("");
	CString table3 = _T("");
	
	
	table1.LoadString(IDS_STRING_TABLE1);
	table2.LoadString(IDS_STRING_TABLE2);
	table3.LoadString(IDS_STRING_TABLE3);


	TCITEM tcItem1;
	tcItem1.mask = TCIF_TEXT;
	tcItem1.pszText = _T((LPSTR)(LPCTSTR)table3);
	m_tabctrl.SetItem(0,&tcItem1);

	TCITEM tcItem2;
	tcItem2.mask = TCIF_TEXT;
	tcItem2.pszText = _T((LPSTR)(LPCTSTR)table2);
	m_tabctrl.SetItem(1,&tcItem2);


	TCITEM tcItem3;
	tcItem3.mask = TCIF_TEXT;
	tcItem3.pszText = _T((LPSTR)(LPCTSTR)table1);
	m_tabctrl.SetItem(2,&tcItem3);


	
	//��ȡ��ǰѡ��tabҳ����
	m_CurSelTab = m_tabctrl.GetCurSel();


	//��ʾ����
	pDialog[0]->ShowWindow(SW_HIDE);
	pDialog[1]->ShowWindow(SW_HIDE);
	pDialog[2]->ShowWindow(SW_HIDE);


	//����module list ����
	theApp.__SetItemText(pDialog[0],IDC_STATIC_MODULE_LIST,IDC_STATIC_MODULE);
	theApp.__SetItemText(pDialog[1],IDC_STATIC_MODULE_LIST,IDC_STATIC_MODULE);
	theApp.__SetItemText(pDialog[2],IDC_STATIC_MODULE_LIST,IDC_STATIC_MODULE);







	pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);


	//��ʾinput dlg
	int i = 0;
	while(i < DLL_NUM){
		//����dialog
		pDialog_input[i]->ShowWindow(SW_HIDE);
		theApp.__SetItemText(pDialog_input[i],IDC_STATIC_MODULE_PARAMETERS,IDC_STATIC_MODULE_PARAMETER);
		theApp.__SetItemText(pDialog_input[i],IDC_STATIC_BRIEF_GROUP,IDC_STATIC_BRIEF_GROUP);
		i++;
	}
	
	
	pDialog_input[0]->ShowWindow(SW_SHOW);



}



/*
 * ����  �������������������ʾ�༭�����
 * ����ֵ��1 ��ʾ�ɹ���0��ʾʧ��
 * ����  ��2012-5-22
 * ����  ��gaoxiang
 */

int CAdapted_shellDlg::Show_Edit_And_Static(int limit,int i)
{
	switch(limit){
		case 0:{
				m_dll_input[i].GetDlgItem(IDC_EDIT1)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT2)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT3)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT4)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT5)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT6)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT7)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT8)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT10)->ShowWindow(false);

				m_dll_input[i].GetDlgItem(IDC_STATIC1)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC2)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC3)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC4)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC5)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC6)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC7)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC8)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC10)->ShowWindow(false);
				}break;
		case 1:{
				m_dll_input[i].GetDlgItem(IDC_EDIT1)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT2)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT3)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT4)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT5)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT6)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT7)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT8)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT10)->ShowWindow(false);
				
				m_dll_input[i].GetDlgItem(IDC_STATIC1)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC2)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC3)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC4)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC5)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC6)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC7)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC8)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC10)->ShowWindow(false);

				m_dll_input[i].SetDlgItemText(IDC_STATIC1,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[0]));

			   }break;
		case 2:{
				m_dll_input[i].GetDlgItem(IDC_EDIT1)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT2)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT3)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT4)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT5)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT6)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT7)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT8)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT10)->ShowWindow(false);
				
				m_dll_input[i].GetDlgItem(IDC_STATIC1)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC2)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC3)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC4)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC5)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC6)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC7)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC8)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC10)->ShowWindow(false);

				m_dll_input[i].SetDlgItemText(IDC_STATIC1,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[0]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC2,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[1]));
			   }break;
		case 3:{
				m_dll_input[i].GetDlgItem(IDC_EDIT1)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT2)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT3)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT4)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT5)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT6)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT7)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT8)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT10)->ShowWindow(false);
				
				m_dll_input[i].GetDlgItem(IDC_STATIC1)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC2)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC3)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC4)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC5)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC6)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC7)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC8)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC10)->ShowWindow(false);


				m_dll_input[i].SetDlgItemText(IDC_STATIC1,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[0]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC2,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[1]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC3,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[2]));
			   }break;
		case 4:{
				m_dll_input[i].GetDlgItem(IDC_EDIT1)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT2)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT3)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT4)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT5)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT6)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT7)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT8)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT10)->ShowWindow(false);
				
				m_dll_input[i].GetDlgItem(IDC_STATIC1)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC2)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC3)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC4)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC5)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC6)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC7)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC8)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC10)->ShowWindow(false);


				m_dll_input[i].SetDlgItemText(IDC_STATIC1,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[0]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC2,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[1]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC3,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[2]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC4,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[3]));
			   }break;
		case 5:{
				m_dll_input[i].GetDlgItem(IDC_EDIT1)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT2)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT3)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT4)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT5)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT6)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT7)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT8)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT10)->ShowWindow(false);
				
				m_dll_input[i].GetDlgItem(IDC_STATIC1)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC2)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC3)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC4)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC5)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC6)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC7)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC8)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC10)->ShowWindow(false);


				m_dll_input[i].SetDlgItemText(IDC_STATIC1,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[0]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC2,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[1]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC3,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[2]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC4,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[3]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC5,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[4]));	
			   }break;
			
		case 6:{
				m_dll_input[i].GetDlgItem(IDC_EDIT1)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT2)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT3)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT4)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT5)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT6)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT7)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT8)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT10)->ShowWindow(false);
				
				m_dll_input[i].GetDlgItem(IDC_STATIC1)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC2)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC3)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC4)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC5)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC6)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC7)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC8)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC10)->ShowWindow(false);


				
				m_dll_input[i].SetDlgItemText(IDC_STATIC1,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[0]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC2,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[1]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC3,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[2]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC4,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[3]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC5,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[4]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC6,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[5]));
			   }break;
		case 7:{
				m_dll_input[i].GetDlgItem(IDC_EDIT1)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT2)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT3)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT4)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT5)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT6)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT7)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT8)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT10)->ShowWindow(false);
				
				m_dll_input[i].GetDlgItem(IDC_STATIC1)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC2)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC3)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC4)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC5)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC6)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC7)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC8)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC10)->ShowWindow(false);


				m_dll_input[i].SetDlgItemText(IDC_STATIC1,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[0]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC2,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[1]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC3,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[2]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC4,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[3]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC5,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[4]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC6,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[5]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC7,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[6]));
			   }break;
		case 8:{
				m_dll_input[i].GetDlgItem(IDC_EDIT1)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT2)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT3)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT4)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT5)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT6)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT7)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT8)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_EDIT9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_EDIT10)->ShowWindow(false);
				
				m_dll_input[i].GetDlgItem(IDC_STATIC1)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC2)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC3)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC4)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC5)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC6)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC7)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC8)->ShowWindow(true);
				m_dll_input[i].GetDlgItem(IDC_STATIC9)->ShowWindow(false);
				m_dll_input[i].GetDlgItem(IDC_STATIC10)->ShowWindow(false);


				
				m_dll_input[i].SetDlgItemText(IDC_STATIC1,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[0]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC2,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[1]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC3,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[2]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC4,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[3]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC5,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[4]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC6,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[5]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC7,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[6]));
				m_dll_input[i].SetDlgItemText(IDC_STATIC8,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[7]));
			   }break;
		case 9:{
			m_dll_input[i].GetDlgItem(IDC_EDIT1)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT2)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT3)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT4)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT5)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT6)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT7)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT8)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT9)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT10)->ShowWindow(false);
			
			m_dll_input[i].GetDlgItem(IDC_STATIC1)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC2)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC3)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC4)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC5)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC6)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC7)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC8)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC9)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC10)->ShowWindow(false);


			m_dll_input[i].SetDlgItemText(IDC_STATIC1,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[0]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC2,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[1]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC3,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[2]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC4,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[3]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC5,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[4]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC6,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[5]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC7,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[6]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC8,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[7]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC9,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[8]));
			   }break;
		case 10:{
			m_dll_input[i].GetDlgItem(IDC_EDIT1)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT2)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT3)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT4)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT5)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT6)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT7)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT8)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT9)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_EDIT10)->ShowWindow(true);
			
			m_dll_input[i].GetDlgItem(IDC_STATIC1)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC2)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC3)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC4)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC5)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC6)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC7)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC8)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC9)->ShowWindow(true);
			m_dll_input[i].GetDlgItem(IDC_STATIC10)->ShowWindow(true);

			
			m_dll_input[i].SetDlgItemText(IDC_STATIC1,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[0]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC2,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[1]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC3,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[2]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC4,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[3]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC5,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[4]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC6,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[5]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC7,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[6]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC8,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[7]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC9,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[8]));
			m_dll_input[i].SetDlgItemText(IDC_STATIC10,_T(m_dll_info.m_dll_common_info.dll_information[i].CmdStr[9]));
			   }break;
	default:;
	}

	return 1;
}

/*
 * ����  ��ͨ������ֵ�趨dll input����
 * ����ֵ��1 ��ʾ�ɹ���0��ʾʧ��
 * ����  ��2012-5-22
 * ����  ��gaoxiang
 */

int CAdapted_shellDlg::Set_Dll_Input_Dialog()
{

	char str[CMD_NAME];
	int  i = 0;


	while (i < m_dll_info.m_df_dll_info.num){

		/*��������*/
		strcpy(str,m_dll_info.m_dll_common_info.dll_information[i].CmdStr[m_dll_info.m_dll_common_info.dll_information[i].CmdNum]);


		m_dll_input[i].SetDlgItemText(IDC_STATIC_BRIEF,_T(str));
		m_dll_input[i].SetDlgItemText(IDC_STATIC_DLL_NAME,_T(m_dll_info.m_df_dll_info.dll_name[i]));
		Show_Edit_And_Static(m_dll_info.m_dll_common_info.dll_information[i].CmdNum,i);
		i++;
	}
	return (i == m_dll_info.m_df_dll_info.num);
}



/*
 * ����  ������dll���ֶ�Ӧdll_infor������ֵ
 * ����	 ��dll����
 * ����ֵ��-1 ��ʾ����ʧ�ܣ�����ֵΪ�ҵ�dll������ֵ
 * ����  ��2012-5-22
 * ����  ��gaoxiang
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

/*
 * ����  ��ִ�б�������
 * ����	 ����
 * ����ֵ����
 * ����  ��2012-5-23
 * ����  ��gaoxiang
 */
void CAdapted_shellDlg::OnProtect() 
{
	// TODO: Add your control notification handler code here





	/*�ж�listbox�Ѿ�����dll*/
	int index = 0;
	int status = 0;

	while (index < 3){
		if (m_tabpage[index].m_CheckListBox.GetCount())
			status = 1;											
		index++;
	}
	/*���û���������ݣ���ʾ����shell module*/
	if (!status)
	{
		MessageBox(_T(LoadLanguageFromId(IDS_STRING_PLEASE_LOAD_MODULE)),"warning",MB_DEFBUTTON2);
		return;
	}



	/*�ж������ļ�edit�Ƿ��*/
	CString load_path;
	CString out_path;

	GetDlgItemText(IDC_EDIT_LOAD_FILE,load_path);



	if (load_path == "" ){
		MessageBox(_T(LoadLanguageFromId(IDS_STRING_NO_FILE_INPUT)),"warning",MB_DEFBUTTON2);
		return;
	}else{
		GetDlgItemText(IDC_EDIT_OUT_FILE,out_path);
	}






	/*���� ѡ�� dll ���֣�Ϊ������Ӳ�����׼��*/
	int k = 0;
	int j = 0;
	int i = 0;
	int dll_select_sum = 0;		/*��¼ѡ��dll����*/
	Dll_Info dll_info[DLL_NUM];

 	while (k < 3){

		if (m_tabpage[k].m_CheckListBox.GetCount()){						/*���listbox��dll��Ϣ������д���*/
		
			j = 0;
		
			while (j < m_tabpage[k].m_CheckListBox.GetCount()){
				
 				if (m_tabpage[k].m_CheckListBox.GetCheck(j)){

					dll_select_sum++;
 
 					CString strTemp;   

 					m_tabpage[k].m_CheckListBox.GetText(j,strTemp);   


					int temp_index = Set_Parameters(strTemp);

					dll_info[i] = m_dll_info.m_dll_common_info.dll_information[temp_index];

					i++;
 
 				}
 
 				j++;
 			}
 			
 			
 
 		}
 			
 			k++;
 		
 	}





	DOSHELL DoShell;
	char **cmdStr;
	BOOL Ret;













	//��ȡDoShell��ַ
	DoShell = (DOSHELL)GetProcAddress(LoadLibraryA("ShellMain.dll"),"DoShell");
	if (DoShell == NULL)
	{
		
		MessageBox(_T(LoadLanguageFromId(IDS_STRING_SUCCESS_LOAD_FAIL_SHELL_MAIN)),"warning",MB_DEFBUTTON2);
		return ;
	}else{
		MessageBox(_T(LoadLanguageFromId(IDS_STRING_SUCCESS_LOAD_SHELL_MAIN)),"warning",MB_DEFBUTTON2);
		
	}



	//ִ�м���
	Ret = DoShell((LPSTR)(LPCTSTR)load_path,(LPSTR)(LPCTSTR)out_path,dll_select_sum,dll_info);
	if (Ret)
	{
		MessageBox(_T(LoadLanguageFromId(IDS_STRING_PROTECT_FINISH)),"warning",MB_DEFBUTTON2);
	}
	else
	{
		MessageBox(_T(LoadLanguageFromId(IDS_STRING_PROTECT_FINISH_ERROR)),"warning",MB_DEFBUTTON2);
	}

	

	
}

//��Ӧ����
void CAdapted_shellDlg::OnSelchangeComboLanguageList() 
{
	// TODO: Add your control notification handler code here
	
	__SetDlgLang(m_language_list.GetCurSel());





//	::GetTabControl()
	
}

void CAdapted_shellDlg::OnButtonListDll() 
{
	// TODO: Add your control notification handler code here
	
}
