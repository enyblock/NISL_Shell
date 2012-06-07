// DLLINFO.h: interface for the DLLINFO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLLINFO_H__D3F9D4F8_8A46_4501_AC15_0967BA298A3F__INCLUDED_)
#define AFX_DLLINFO_H__D3F9D4F8_8A46_4501_AC15_0967BA298A3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//插件类型


#define PLUGIN_CODEM 1
#define PLUGIN_CODEE 2
#define PLUGIN_TABLE 3



/*定义常量*/

/*DLL 名字长度 20*/
#define DLL_NAME  20

/*DLL 个数 40   */
#define DLL_NUM   40

/*cmd 名字长度 512*/
#define CMD_NAME  512

/*cmd 个数 10*/
#define CMD_NUM   10







/*
 * 说明：  定义dll info结构体 用来存放简略信息
 *		   存放dll名字与dll个数
 * 限定：
 *         1.模块数不超过40
 *         2.模块名字不超过20
 */
typedef struct _dll_info{
	
	char dll_name[DLL_NUM][DLL_NAME];
	int  num;
	
}DLL_INFO;




/* 
 * 说明：   定义DLL info结构体，用来存放dll详细信息
 *          1.dll名字
 *          2.dll类型
 *          3.dll的getbinary函数地址
 *          4.dll参数个数
 *          5.dll参数名字
 * 限定：   
 *          1.dll参数个数不超过10
 *          2.dll参数名字不超过512
 */
typedef struct Dll_Info {
	CHAR   DllName[DLL_NAME];
	ULONG  PluginType;
	ULONG  DLLGetBinary; 
	ULONG  CmdNum;
	ULONG  Cmd[CMD_NUM];
	char   **CmdStr;
}DLL_INFORMATION;



/*DLL COMMON INFO*/
typedef struct dllinfo{

	DLL_INFORMATION dll_information[DLL_NUM];
	int             dll_num;

}DLL_COMMON_INFO;



//函数指针
typedef BOOL  (WINAPI *GETBINARY)(
								  IN  PVOID  pFileData,					/*pE文件指针*/
								  IN  PUCHAR *pInBinary,				/*储存二进制代码指针*/
								  IN  ULONG  BinaryAddStartRva,			/*二进制代码加入点的RVA*/
								  OUT PLONG OffsetCode,				/*二进制中代码代码的相对偏移，用于第一次添加确定位置*/
								  OUT PLONG OffsetNextCode,			/*下一个模块的起始地址VA，用于连接所有模块*/
								  OUT PLONG Size,
								  IN  ULONG  ReserveNUM,				/*保留参数个数*/
								  IN  PLONG Reserve					/*带保留参数指针*/
								  );
typedef void  (WINAPI *GETFUNINFO)(
								   PULONG GetBinary,
								   PULONG PluginType,
								   PULONG CmdNum,
								   PCHAR **cmd);


typedef  BOOL  (WINAPI *DOSHELL)(
								 PCHAR FileName,//欲保护的文件路径
								 PCHAR SaveName,//加壳后的文件路径
								 ULONG DllInfuNum,//此次选取插件的个数
								 Dll_Info dll_info[]
							   );



class DLLINFO  
{
public:
	DLLINFO();
	int    GetDllName(void);
	int    GetDLLInformation(void);
	int    GetIndexFromDllName(char _dll_name[DLL_NUM]);

	//通过语种ID返回响应的语种字符串
	CString LoadLanguageFromId(UINT lId);

	virtual ~DLLINFO();

public:
	/*所有dll 通用详细信息*/
	DLL_COMMON_INFO	    m_dll_common_info;
	/*所有dll 简略名字信息*/
	DLL_INFO			m_df_dll_info;

};

#endif // !defined(AFX_DLLINFO_H__D3F9D4F8_8A46_4501_AC15_0967BA298A3F__INCLUDED_)
