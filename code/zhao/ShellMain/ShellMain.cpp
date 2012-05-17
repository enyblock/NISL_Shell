// ShellMain.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ImportEncrypt.h"
#include "PEplugin.h"
#include "XorEncrypt.h"
#include "ShiftEncrypt.h"





struct Plugin_Info 
{
	PUCHAR pBinary;
	LONG OffsetCode;
	LONG OffsetNextCode;
	ULONG Size;
	ULONG SectionNowSize;//��¼�������Ѽ�������ƵĴ�С

};

typedef void (WINAPI *GETFUNINFO)(
								PULONG GetBinary,
								PULONG PluginType,
								PULONG CmdNum,
								PCHAR **cmd);

typedef BOOL  (WINAPI *GETBINARY)(
					  IN  PVOID  pFileData,					/*pE�ļ�ָ��*/
					  IN  PUCHAR *pInBinary,				/*��������ƴ���ָ��*/
					  IN  ULONG  BinaryAddStartRva,			/*�����ƴ��������RVA*/
					  OUT PLONG OffsetCode,				/*�������д����������ƫ�ƣ����ڵ�һ�����ȷ��λ��*/
					  OUT PLONG OffsetNextCode,			/*��һ��ģ�����ʼ��ַVA��������������ģ��*/
					  OUT PULONG Size,
					  IN  ULONG  ReserveNUM,				/*������������*/
					  IN  PULONG Reserve					/*����������ָ��*/
						);

struct Dll_Info 
{
	PCHAR DllName;
	ULONG PluginType;
	GETBINARY GetBinary; 
	ULONG CmdNum;
	ULONG *Cmd;
};

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	PEpluginInit();
    return TRUE;
}

ULONG GetStartRva(PVOID pFileData)
{
	PEInfo Info = {0};
	ULONG SectionAlign,VirtualSize,SectionNum,StartRva;
	PIMAGE_SECTION_HEADER pLastSection ;
	GetPEInfo(pFileData,&Info);
	SectionNum = Info.pFileHeader->NumberOfSections;
	SectionAlign = Info.pOptinalHeader->SectionAlignment;
	pLastSection = (PIMAGE_SECTION_HEADER)&(Info.pSectionHeader[SectionNum-1]);
	if (pLastSection->SizeOfRawData % SectionAlign)
	{
		VirtualSize = SectionAlign * (pLastSection->SizeOfRawData/SectionAlign+1);
	} 
	else
	{
		VirtualSize = pLastSection->SizeOfRawData;
	}
	StartRva = pLastSection->VirtualAddress + VirtualSize;
	return StartRva;
}

BOOL WINAPI	DoShell(PCHAR FileName,//���������ļ�·��
					PCHAR SaveName,//�ӿǺ���ļ�·��
					ULONG DllInfuNum,//�˴�ѡȡ����ĸ���
					Dll_Info *dll_info)//�������Ϣ
{
	PEInfo Info = {0};
	PVOID pFileData;
	Plugin_Info *pluginInfo = (Plugin_Info *)malloc(sizeof(Plugin_Info) * DllInfuNum);
	ULONG i,j,StartRva,SectionNum;

	ULONG Plugin_Type = 0;
	PUCHAR pLastSection ;
	ULONG NowSumOfSize = 0;
	BOOL IsFirst = TRUE;
	ULONG OldOEP;
	LONG FrontIndex = -1;//��¼��һ����Ҫִ�еĶ����Ƶ����
	LONG NowIndex;
	LONG BaseIndex=0;
	ULONG PluginTypeNum[3]={0};
	ULONG SumOfSize = 0;

	//�����ļ�����ȡPEͷ
	if ( !LoadFile(FileName,pFileData) )
	{
		return false;
	}
	if( !IsPEFile(pFileData) )
	{
		goto ERR;
	}
	if( !GetPEInfo(pFileData,&Info))
	{
		MessageBoxA(0,"GetPEInfo","Error",MB_OK);
		goto ERR;
	}
	StartRva = GetStartRva(pFileData);
	SectionNum = Info.pFileHeader->NumberOfSections;
	OldOEP = Info.pOptinalHeader->ImageBase + Info.pOptinalHeader->AddressOfEntryPoint;
	//��ȡ���������

	for (i=0;i<DllInfuNum;i++)
	{
		__try
		{
			
			if ( dll_info[i].GetBinary(
				pFileData,
				&pluginInfo[i].pBinary,
				StartRva + SumOfSize,
				&pluginInfo[i].OffsetCode,				
				&pluginInfo[i].OffsetNextCode,			
				&pluginInfo[i].Size,
				dll_info[i].CmdNum,				
				dll_info[i].Cmd					
				))
			{
				SumOfSize += pluginInfo[i].Size;
			}
			else
			{//�ɹ����أ��������ɶ�����ʧ���ˡ�
				MessageBoxA(0,dll_info[i].DllName,"Make Error",MB_OK);
				goto ERR;
			}
		}
		__except(EXCEPTION_CONTINUE_EXECUTION)
		{//���ô��󣬿�����GetBinary������ַ����Ҳ������GetBinary�����ڲ�����
			MessageBoxA(0,dll_info[i].DllName,"Run Error",MB_OK);
			goto ERR;
		}

	}
	//���ϲ���������Ƶ�����
	if ( !AddSection(pFileData,SumOfSize,"NISL") )
	{
		MessageBoxA(0,"AddSection","Error",MB_OK);
		goto ERR;
	}
	if( !GetPEInfo(pFileData,&Info))
	{
		MessageBoxA(0,"GetPEInfo","Error",MB_OK);
		goto ERR;
	}
	pLastSection = (PUCHAR)((ULONG)pFileData + Info.pSectionHeader[SectionNum].PointerToRawData);

	//����������
	for (i=0;i<DllInfuNum;i++)
	{
		if (Plugin_Type > dll_info[i].PluginType)
		{//û�а�˳�����в��
			MessageBoxA(0,"û�а�˳�����в��","Error",MB_OK);
			goto ERR;
		}
		else
		{
			Plugin_Type = dll_info[i].PluginType;
			PluginTypeNum[Plugin_Type-1]++;//��¼ÿ�����Ͳ���ĸ���
		}
		memcpy(&pLastSection[NowSumOfSize],pluginInfo[i].pBinary,pluginInfo[i].Size);
		pluginInfo[i].SectionNowSize = NowSumOfSize;
		NowSumOfSize+=pluginInfo[i].Size;
		
	}

	//�������������
	for (i=0;i<3;i++)
	{//��ͬ���͵Ĳ���� PLUGIN_CODEM 1->PLUGIN_CODEE 2->PLUGIN_TABLE 3��˳����������
		for (j=PluginTypeNum[i];j>0;j--)
		{//ͬ���Ͳ���������ɵ���˳������
			NowIndex = BaseIndex + j - 1;//����˴����ӵĲ�����
			if ( pluginInfo[NowIndex].OffsetCode >= 0)
			{//ÿ���ж��Ƿ�����Ҫִ�еĶ����ƣ���������һ����Ҫִ�еĶ�������������
				if (IsFirst)
				{//��������OEP
					
					Info.pOptinalHeader->AddressOfEntryPoint = 
						Info.pSectionHeader[SectionNum].VirtualAddress + 
						pluginInfo[NowIndex].SectionNowSize + 
						pluginInfo[NowIndex].OffsetCode;
					FrontIndex = NowIndex;
					IsFirst = FALSE;
				}
				else
				{
					*(PULONG)&pLastSection[pluginInfo[FrontIndex].SectionNowSize + pluginInfo[FrontIndex].OffsetNextCode] = 
						Info.pSectionHeader[SectionNum].VirtualAddress + 
						Info.pOptinalHeader->ImageBase + 
						pluginInfo[NowIndex].SectionNowSize + 
						pluginInfo[NowIndex].OffsetCode;
					FrontIndex = NowIndex;
					
					
				}				
		}
		}
		BaseIndex += PluginTypeNum[i];
	}
	//�����һ�������OEP
	if (FrontIndex != -1)
	{
		*(PULONG)&pLastSection[pluginInfo[FrontIndex].SectionNowSize + pluginInfo[FrontIndex].OffsetNextCode] = OldOEP;
	}
	

	//����һ����������Ϊ��д
	Info.pSectionHeader[0].Characteristics = 0xE0000020;



	//�����ļ�,�ͷ��ڴ�
	free(pluginInfo);
	SaveFile(pFileData,SaveName);
	UnLoadFile(pFileData);
	return true;
	
	
ERR:
	//�ͷ��ڴ�
	free(pluginInfo);
	UnLoadFile(pFileData);
	return false;
	
}
