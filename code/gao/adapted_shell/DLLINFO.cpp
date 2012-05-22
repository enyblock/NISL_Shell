// DLLINFO.cpp: implementation of the DLLINFO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "test_dll.h"
#include "DLLINFO.h"
#include <windows.h>
#include <winbase.h>

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
	}else{
		AfxMessageBox("list faile");
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



/*获取dll详细信息*/
int DLLINFO::GetDLLInformation(void)
{

	/*定义getfuninfo函数指针类型*/
	typedef int (WINAPI *p_GetFunInfo)(PULONG _GetBinary,
					   PULONG PluginType,
					   PULONG CmdNum,
					   PCHAR **cmd);
	HINSTANCE hDLL;
	int num   = 0;
	int index = 0; 
	int i	  = 0;
	p_GetFunInfo fun = NULL;


	/*获取当前进行执行路径*/	
	LPTSTR   lpszModule   =   new   TCHAR[MAX_PATH]; 
	GetModuleFileName(AfxGetApp()-> m_hInstance,lpszModule,MAX_PATH); 

	CString   strPath   =   lpszModule; 




	/*去掉进程名*/
	index   =   strPath.ReverseFind('\\'); 
	strPath   =   strPath.Left(index); 

	/*添加完全路径*/
	strPath += "\\shell_module\\";


	m_dll_common_info.dll_num = m_df_dll_info.num;


	while (i < m_df_dll_info.num){

		index   =   strPath.ReverseFind('\\'); 
		strPath   =   strPath.Left(index); 
		strPath += "\\";
		strPath += m_df_dll_info.dll_name[i];

	//	AfxMessageBox(strPath);

		hDLL = NULL;
		fun  = NULL;
		 
		hDLL=LoadLibrary(strPath);//加载动态链接库MyDll.dll文件；
		
		//	HINSTANCE lib = LoadLibraryEx("F:\\program\\test_dll\\Debug\\shell_module\\ShiftEncrypt.dll",NULL,LOAD_WITH_ALTERED_SEARCH_PATH);
		
// 		LPTSTR szCurrentDir = new TCHAR[MAX_PATH];
// 		::GetCurrentDirectory(MAX_PATH, szCurrentDir);
// 		::SetCurrentDirectory("F:\\program\\test_dll\\Debug\\shell_module");
// 		::GetCurrentDirectory(MAX_PATH, szCurrentDir);
// 		hDLL = LoadLibrary("F:\\program\\test_dll\\Debug\\shell_module\\ShiftEncrypt.dll");
		
		
		
		
		fun=(p_GetFunInfo)GetProcAddress(hDLL,"GetFunInfo");
		num=fun(&m_dll_common_info.dll_information[i].DLLGetBinary,
			&m_dll_common_info.dll_information[i].PluginType,
			&m_dll_common_info.dll_information[i].CmdNum,
			&m_dll_common_info.dll_information[i].CmdStr);	
		
		strncpy(m_dll_common_info.dll_information[i].DllName,m_df_dll_info.dll_name[i],DLL_NAME);
		
		
		if (num)
		{
			//AfxMessageBox("call dll success");
		}else{
			AfxMessageBox("call dll faile");
		}

		i++;
	}
		
	


//	LPTSTR   dllname   =   new   TCHAR[strPath.GetLength()+1]; 
//	strncpy(dllname,strPath,strPath.GetLength()+1);

// 	CString m_path = ".\\shell_module\\";
// 
// 	m_path += m_df_dll_info.dll_name[0];





	if (i == 0){
		AfxMessageBox("no dll");
		return  0;
	}else{

		
	//	AfxMessageBox("list success");
		return 1;
	}

	
	
}



/*
 *　功能  ： 通过dll名字，获取dll的索引号
 *　返回值： -1 表示没有找到，其余的值为找到的索引值
 *　日期  ： 2012-5-21
 *  作者  ： 高向
 */
int DLLINFO::GetIndexFromDllName(char _dll_name[DLL_NUM])
{
	int i = 0;

	while (i < m_df_dll_info.num){
		if (strcmp(_dll_name,m_df_dll_info.dll_name[i]))
			return i;
		
		i++;
	}

	return -1;
}