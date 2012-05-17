#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <io.h>





/*����dll infor�ṹ��*/
/*�޶���
 *      1.ģ����������40
 *      2.ģ�����ֲ�����20
 */
typedef struct _dll_info{
	
	char dll_name[40][20];
	int  num;
	
}DLL_INFO;



int compare_log(char *log, char *log_new);
int create_process(char *file_name);
int delete_process(char *file_name);
int find_all_dll(DLL_INFO *info);





int main (void)
{
	DLL_INFO df_dll_info;

	int i = 0;
	find_all_dll(&df_dll_info);


	while (i < df_dll_info.num)
		puts(df_dll_info.dll_name[i++]);

/********************************************************/	
/******************  test update   **********************/

// 	char *file_name_log = "c:\\version_log.inf";
// 	char *file_name_log_new = "c:\\version_log_new.inf";
// 
// 
// 
// 	compare_log(file_name_log,file_name_log_new);
// 	getchar();
// 	getchar();
// 
// 	create_process("c:\\test.exe");
// 	getchar();
// 	getchar();
// 	printf("%d\n",delete_process("test.exe"));
// 	getchar();
// 	getchar();

	
/********************************************************/	
	
	

	return EXIT_SUCCESS;
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
int find_all_dll(DLL_INFO *info)
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
* ����ֵ: 1 ����ɹ�,0 ����ʧ��
* ����:   2012-4-17
* ����:   ����
*/
int delete_process(char *file_name)
{	
	char cmd[50] = "TASKKILL /F /IM ";


	strncat(cmd,file_name,10);


	if(0 != system(cmd))
	{
		fprintf(stderr,"delete process fail.\n");
		return 0;
	}else{
		fprintf(stdout,"delete process success.\n");
		return 1;

	}
	
}



/*
* ����:   ��������
* ����ֵ: 1 ����ɹ�,0 ����ʧ��
* ����:   2012-4-17
* ����:   ����
*/
int create_process(char *file_name)
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
int compare_log(char *log, char *log_new)
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