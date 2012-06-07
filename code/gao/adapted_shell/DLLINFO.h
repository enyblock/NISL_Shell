// DLLINFO.h: interface for the DLLINFO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLLINFO_H__D3F9D4F8_8A46_4501_AC15_0967BA298A3F__INCLUDED_)
#define AFX_DLLINFO_H__D3F9D4F8_8A46_4501_AC15_0967BA298A3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//�������


#define PLUGIN_CODEM 1
#define PLUGIN_CODEE 2
#define PLUGIN_TABLE 3



/*���峣��*/

/*DLL ���ֳ��� 20*/
#define DLL_NAME  20

/*DLL ���� 40   */
#define DLL_NUM   40

/*cmd ���ֳ��� 512*/
#define CMD_NAME  512

/*cmd ���� 10*/
#define CMD_NUM   10







/*
 * ˵����  ����dll info�ṹ�� ������ż�����Ϣ
 *		   ���dll������dll����
 * �޶���
 *         1.ģ����������40
 *         2.ģ�����ֲ�����20
 */
typedef struct _dll_info{
	
	char dll_name[DLL_NUM][DLL_NAME];
	int  num;
	
}DLL_INFO;




/* 
 * ˵����   ����DLL info�ṹ�壬�������dll��ϸ��Ϣ
 *          1.dll����
 *          2.dll����
 *          3.dll��getbinary������ַ
 *          4.dll��������
 *          5.dll��������
 * �޶���   
 *          1.dll��������������10
 *          2.dll�������ֲ�����512
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



//����ָ��
typedef BOOL  (WINAPI *GETBINARY)(
								  IN  PVOID  pFileData,					/*pE�ļ�ָ��*/
								  IN  PUCHAR *pInBinary,				/*��������ƴ���ָ��*/
								  IN  ULONG  BinaryAddStartRva,			/*�����ƴ��������RVA*/
								  OUT PLONG OffsetCode,				/*�������д����������ƫ�ƣ����ڵ�һ�����ȷ��λ��*/
								  OUT PLONG OffsetNextCode,			/*��һ��ģ�����ʼ��ַVA��������������ģ��*/
								  OUT PLONG Size,
								  IN  ULONG  ReserveNUM,				/*������������*/
								  IN  PLONG Reserve					/*����������ָ��*/
								  );
typedef void  (WINAPI *GETFUNINFO)(
								   PULONG GetBinary,
								   PULONG PluginType,
								   PULONG CmdNum,
								   PCHAR **cmd);


typedef  BOOL  (WINAPI *DOSHELL)(
								 PCHAR FileName,//���������ļ�·��
								 PCHAR SaveName,//�ӿǺ���ļ�·��
								 ULONG DllInfuNum,//�˴�ѡȡ����ĸ���
								 Dll_Info dll_info[]
							   );



class DLLINFO  
{
public:
	DLLINFO();
	int    GetDllName(void);
	int    GetDLLInformation(void);
	int    GetIndexFromDllName(char _dll_name[DLL_NUM]);

	//ͨ������ID������Ӧ�������ַ���
	CString LoadLanguageFromId(UINT lId);

	virtual ~DLLINFO();

public:
	/*����dll ͨ����ϸ��Ϣ*/
	DLL_COMMON_INFO	    m_dll_common_info;
	/*����dll ����������Ϣ*/
	DLL_INFO			m_df_dll_info;

};

#endif // !defined(AFX_DLLINFO_H__D3F9D4F8_8A46_4501_AC15_0967BA298A3F__INCLUDED_)
