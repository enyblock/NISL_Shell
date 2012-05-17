// update_tools.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "update_tools.h"

#include <io.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

int _declspec(dllexport) my_sum(int a, int b) 
{ 

	return a+b; 

}



/*
* 功能:   找到所有dll模块的名称
* 返回值: 1 代表成功,0 代表失败
* 限定：
*         1.模块数不超过40
*         2.模块名字不超过20
* 日期:   2012-5-15
* 作者:   高向
*/
int  _declspec(dllexport) find_all_dll(DLL_INFO *info)
{
	int i = 0;
	long Handle;
	struct _finddata_t FileInfo;
	char my_path[256];
	
	
	/*获取当前进程所在路径*/
	int pathlen = ::GetModuleFileName(NULL,my_path,200);
	
	
	
	/*去掉最后.EXE名*/
	while (1){
		if (my_path[pathlen--] == '\\')
			break;
	}
	
	my_path[++pathlen] = 0x0;
	
	
	
	/*增加寻找路径*/
	strncat(my_path,"\\shell_module\\*.dll",56);
	
	
	/*打印路径*/
	puts(my_path);
	
	
	/*寻找目标文件*/
	if((Handle=_findfirst(my_path,&FileInfo))==-1L){
		
		fprintf(stderr,"no file\n");
		return 0;
	}
	else{
		
		/*分配空间存放文件名*/
		strncpy(info->dll_name[i],FileInfo.name,20);
		i++;
		
		
		/*寻找下一个*/
		while( _findnext(Handle,&FileInfo)==0){

			strncpy(info->dll_name[i],FileInfo.name,20);
			i++;

		}
		
		/*关闭句柄*/
		_findclose(Handle);
	}
	
	/*设置个数*/
	info->num = i;
	
	return 1;
}



/*
 * 功能:   销毁进程
 * 返回值: 0 代表成功,非0代表失败
 * 日期:   2012-4-17
 * 作者:   高向
 */
int _declspec(dllexport) delete_process(char *file_name)
{	

	DWORD n = 0;
	char cmd[256] = "cmd /c TASKKILL /F /IM ";
	
	
	strncat(cmd,file_name,200);
	
	
	PROCESS_INFORMATION   pi; 
	STARTUPINFO   si; 
	
	//   Set   up   the   start   up   info   struct. 
	ZeroMemory(&si,sizeof(STARTUPINFO)); 
	
	si.dwFlags   =   STARTF_USESHOWWINDOW; 
	si.wShowWindow   =     SW_HIDE; 
	
	if(CreateProcess(NULL, cmd,   NULL,   NULL,   0, 
		NULL,   NULL,   NULL,   &si,   &pi))
	{
		WaitForSingleObject(pi.hProcess,INFINITE);  
 		GetExitCodeProcess(pi.hProcess,&n); 
		CloseHandle(pi.hThread); 
		CloseHandle(pi.hProcess);
		
		return (int)n;
		
	}else{

		return 128;
	}
	
 	

}





/*
 * 功能:   创建进程
 * 返回值: 1 代表成功,0 代表失败
 * 日期:   2012-4-17
 * 作者:   高向
 */
int _declspec(dllexport) create_process(char *file_name)
{

	//CreateProcess parameters
    LPSECURITY_ATTRIBUTES processA = NULL;
    LPSECURITY_ATTRIBUTES threadA  = NULL;
    bool shareRights = true;        // Default
    DWORD creationMask  = CREATE_NEW_CONSOLE;   // Windows Per Process
    LPVOID enviroment = NULL;   // Default
    LPSTR  curDir = NULL;       // Default
    STARTUPINFO startInfo;      // Result
    PROCESS_INFORMATION procInfo;//Result



	// Create a new process to execute the command
	ZeroMemory(&startInfo,sizeof(startInfo));
    startInfo.cb = sizeof(startInfo);


	if (
		!CreateProcess(
		file_name,//File name of executable
		NULL,//command line
		processA,//Process inherited security
		threadA, //Thread inherited security
		shareRights,//Rights propagation
		creationMask,//various creation flags
		enviroment,//Enviroment variable
		curDir, //Child's current directory
		&startInfo,
		&procInfo
		)
		
		)
	{
		fprintf(stderr,"CreatProcess failed on error %d\n",GetLastError());
		return 0;
		
    }else{

		fprintf(stdout,"create process succes\n");
		return 1;

	}


}





/*
 * 功能:   对比新旧版本文件，返回值代表是否更新
 * 返回值: 1 代表更新,0 代表不更新
 * 日期:   2012-4-16
 * 作者:   高向
 */
int _declspec(dllexport) compare_log(char *log, char *log_new)
{
	/*声明两个文件指针*/
	FILE *pf_log = NULL;
	FILE *pf_log_new = NULL;
	
	/*声明另个日志版本对比指针*/
	float log_v = 0.0;
	float log_v_new= 0.0;



	/*打开旧版本文件*/
	if ( NULL == (pf_log = fopen(log,"r")) )
	{
		printf("not open\n");
		exit(1);
	}
	
	
	/*获取旧版本号*/
	fscanf(pf_log,"%*s%f\n",&log_v);
	fclose(pf_log);
	
	
	
	/*打开新版本文件*/
	if ( NULL == (pf_log_new = fopen(log_new,"r")) )
	{
		printf("not open\n");
		exit(1);
	}
	
	
	/*获取新版本号*/
	fscanf(pf_log_new,"%*s%f\n",&log_v_new);
	fclose(pf_log_new);
	
	
	
	/*判断版本号*/
	if (fabs(log_v - log_v_new) < 0.000001)
	{
		printf ("version equal\n");
		return 0;
		
	}else 
	{
		printf("update\n");
		return 1;
	}


}
