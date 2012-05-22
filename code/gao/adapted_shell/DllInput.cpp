// DllInput.cpp : implementation file
//

#include "stdafx.h"
#include "adapted_shell.h"
#include "DllInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDllInput dialog


CDllInput::CDllInput(CWnd* pParent /*=NULL*/)
	: CDialog(CDllInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDllInput)
	m_brief_introduction = _T("");
	//}}AFX_DATA_INIT
}


void CDllInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDllInput)
	DDX_Text(pDX, IDC_EDIT1, m_brief_introduction);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDllInput, CDialog)
	//{{AFX_MSG_MAP(CDllInput)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDllInput message handlers
