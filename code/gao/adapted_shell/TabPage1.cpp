// TabPage1.cpp : implementation file
//

#include "stdafx.h"
#include "adapted_shell.h"
#include "TabPage1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabPage1 dialog


CTabPage1::CTabPage1(CWnd* pParent /*=NULL*/)
	: CDialog(CTabPage1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabPage1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CTabPage1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabPage1, CDialog)
	//{{AFX_MSG_MAP(CTabPage1)
	ON_BN_CLICKED(IDC_BUTTON3, OnCancle)
	ON_LBN_SELCANCEL(IDC_LIST_DLL_BOX, OnSelcancelListDllBox)
	ON_LBN_SELCHANGE(IDC_LIST_DLL_BOX, OnSelchangeListDllBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabPage1 message handlers

BOOL CTabPage1::OnInitDialog() 
{
	CDialog::OnInitDialog();


	
	// TODO: Add extra initialization here
	m_CheckListBox.SubclassDlgItem(IDC_LIST_DLL_BOX, this); // IDC_LIST1是ListBox控件的资源ID
    m_CheckListBox.SetCheckStyle(BS_AUTOCHECKBOX);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabPage1::OnCancle() 
{
	// TODO: Add your control notification handler code here
	EndDialog(NULL);
	
	m_CheckListBox.ResetContent();




	
	
}

void CTabPage1::OnSelcancelListDllBox() 
{
	// TODO: Add your control notification handler code here
	CString strTemp;   
	m_CheckListBox.GetText(m_CheckListBox.GetCurSel(),strTemp);   
	AfxMessageBox(strTemp);
	
}

/*item选中相应*/
void CTabPage1::OnSelchangeListDllBox() 
{
	// TODO: Add your control notification handler code here

//	CString strTemp;   
//	m_CheckListBox.GetText(m_CheckListBox.GetCurSel(),strTemp);   
//	AfxMessageBox(strTemp);
	
}
