/*
 * file name : update_tools.h
 * function  : declare all function
 * date      : 2012-4-10
 * author    : gaoxiang
 */

#ifndef __UPDATE_TOOLS_ENYBLOCK_2012_4_10__
#define __UPDATE_TOOLS_ENYBLOCK_2012_4_10__



/*����dll infor�ṹ��*/
/*�޶���
 *      1.ģ����������40
 *      2.ģ�����ֲ�����20
 */
typedef struct _dll_info{
	
	char dll_name[40][20];
	int  num;
	
}DLL_INFO;




extern "C" _declspec(dllexport) int my_sum(int a,int b);



/*��ȡshell_moduleĿ¼����dll�ļ���*/
extern "C" _declspec(dllexport) int find_all_dll(DLL_INFO *info);

/*�Ա��¾ɰ汾�ļ�,�ж��Ƿ�����*/
extern "C" _declspec(dllexport) int compare_log(char *log, char *log_new);

/*��������*/
extern "C" _declspec(dllexport) int create_process(char *file_name);

/*���ٽ���*/
extern "C" _declspec(dllexport) int delete_process(char *file_name);

#endif