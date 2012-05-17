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
* ����:   �ҵ�����dllģ�������
* ����ֵ: 1 ����ɹ�,0 ����ʧ��
* �޶���
*         1.ģ����������40
*         2.ģ�����ֲ�����20
* ����:   2012-5-15
* ����:   ����
*/
int  _declspec(dllexport) find_all_dll(DLL_INFO *info)
{
	int i = 0;
	long Handle;
	struct _finddata_t FileInfo;
	char my_path[256];
	
	
	/*��ȡ��ǰ��������·��*/
	int pathlen = ::GetModuleFileName(NULL,my_path,200);
	
	
	
	/*ȥ�����.EXE��*/
	while (1){
		if (my_path[pathlen--] == '\\')
			break;
	}
	
	my_path[++pathlen] = 0x0;
	
	
	
	/*����Ѱ��·��*/
	strncat(my_path,"\\shell_module\\*.dll",56);
	
	
	/*��ӡ·��*/
	puts(my_path);
	
	
	/*Ѱ��Ŀ���ļ�*/
	if((Handle=_findfirst(my_path,&FileInfo))==-1L){
		
		fprintf(stderr,"no file\n");
		return 0;
	}
	else{
		
		/*����ռ����ļ���*/
		strncpy(info->dll_name[i],FileInfo.name,20);
		i++;
		
		
		/*Ѱ����һ��*/
		while( _findnext(Handle,&FileInfo)==0){

			strncpy(info->dll_name[i],FileInfo.name,20);
			i++;

		}
		
		/*�رվ��*/
		_findclose(Handle);
	}
	
	/*���ø���*/
	info->num = i;
	
	return 1;
}



/*
 * ����:   ���ٽ���
 * ����ֵ: 0 ����ɹ�,��0����ʧ��
 * ����:   2012-4-17
 * ����:   ����
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
 * ����:   ��������
 * ����ֵ: 1 ����ɹ�,0 ����ʧ��
 * ����:   2012-4-17
 * ����:   ����
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
 * ����:   �Ա��¾ɰ汾�ļ�������ֵ�����Ƿ����
 * ����ֵ: 1 �������,0 ��������
 * ����:   2012-4-16
 * ����:   ����
 */
int _declspec(dllexport) compare_log(char *log, char *log_new)
{
	/*���������ļ�ָ��*/
	FILE *pf_log = NULL;
	FILE *pf_log_new = NULL;
	
	/*���������־�汾�Ա�ָ��*/
	float log_v = 0.0;
	float log_v_new= 0.0;



	/*�򿪾ɰ汾�ļ�*/
	if ( NULL == (pf_log = fopen(log,"r")) )
	{
		printf("not open\n");
		exit(1);
	}
	
	
	/*��ȡ�ɰ汾��*/
	fscanf(pf_log,"%*s%f\n",&log_v);
	fclose(pf_log);
	
	
	
	/*���°汾�ļ�*/
	if ( NULL == (pf_log_new = fopen(log_new,"r")) )
	{
		printf("not open\n");
		exit(1);
	}
	
	
	/*��ȡ�°汾��*/
	fscanf(pf_log_new,"%*s%f\n",&log_v_new);
	fclose(pf_log_new);
	
	
	
	/*�жϰ汾��*/
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
