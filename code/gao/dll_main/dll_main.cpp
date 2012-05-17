#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <io.h>





/*定义dll infor结构体*/
/*限定：
 *      1.模块数不超过40
 *      2.模块名字不超过20
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
* 功能:   找到所有dll模块的名称
* 返回值: 1 代表成功,0 代表失败
* 限定：
*         1.模块数不超过40
*         2.模块名字不超过20
* 日期:   2012-5-15
* 作者:   高向
*/
int find_all_dll(DLL_INFO *info)
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
* 返回值: 1 代表成功,0 代表失败
* 日期:   2012-4-17
* 作者:   高向
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
* 功能:   创建进程
* 返回值: 1 代表成功,0 代表失败
* 日期:   2012-4-17
* 作者:   高向
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
* 功能:   对比新旧版本文件，返回值代表是否更新
* 返回值: 1 代表更新,0 代表不更新
* 日期:   2012-4-16
* 作者:   高向
*/
int compare_log(char *log, char *log_new)
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