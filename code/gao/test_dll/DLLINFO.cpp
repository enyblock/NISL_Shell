// DLLINFO.cpp: implementation of the DLLINFO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "test_dll.h"
#include "DLLINFO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*get dll info*/
int DLLINFO::GetDllName(void)
{
	int num = 0;
	typedef int (*p_find_all_dll)(DLL_INFO *info);
	HINSTANCE hDLL;
	p_find_all_dll fun = NULL;
	hDLL=LoadLibrary("update_tools.dll");//加载动态链接库MyDll.dll文件；
	fun=(p_find_all_dll)GetProcAddress(hDLL,"find_all_dll");
	num=fun(&m_df_dll_info);	
	
	int i = 0;
	
	if (num == 1)
	{
		AfxMessageBox("list success");
	}
	
	FreeLibrary(hDLL);

	return 0;
}


DLLINFO::DLLINFO()
{

}

DLLINFO::~DLLINFO()
{

}
