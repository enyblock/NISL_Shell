/*
 * file name : update_tools.h
 * function  : declare all function
 * date      : 2012-4-10
 * author    : gaoxiang
 */

#ifndef __UPDATE_TOOLS_ENYBLOCK_2012_4_10__
#define __UPDATE_TOOLS_ENYBLOCK_2012_4_10__



/*定义dll infor结构体*/
/*限定：
 *      1.模块数不超过40
 *      2.模块名字不超过20
 */
typedef struct _dll_info{
	
	char dll_name[40][20];
	int  num;
	
}DLL_INFO;




extern "C" _declspec(dllexport) int my_sum(int a,int b);



/*获取shell_module目录下面dll文件名*/
extern "C" _declspec(dllexport) int find_all_dll(DLL_INFO *info);

/*对比新旧版本文件,判断是否升级*/
extern "C" _declspec(dllexport) int compare_log(char *log, char *log_new);

/*创建进程*/
extern "C" _declspec(dllexport) int create_process(char *file_name);

/*销毁进程*/
extern "C" _declspec(dllexport) int delete_process(char *file_name);

#endif