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
	ULONG SectionNowSize;//记录上区段已加入二进制的大小

};

typedef void (WINAPI *GETFUNINFO)(
								PULONG GetBinary,
								PULONG PluginType,
								PULONG CmdNum,
								PCHAR **cmd);

typedef BOOL  (WINAPI *GETBINARY)(
					  IN  PVOID  pFileData,					/*pE文件指针*/
					  IN  PUCHAR *pInBinary,				/*储存二进制代码指针*/
					  IN  ULONG  BinaryAddStartRva,			/*二进制代码加入点的RVA*/
					  OUT PLONG OffsetCode,				/*二进制中代码代码的相对偏移，用于第一次添加确定位置*/
					  OUT PLONG OffsetNextCode,			/*下一个模块的起始地址VA，用于连接所有模块*/
					  OUT PULONG Size,
					  IN  ULONG  ReserveNUM,				/*保留参数个数*/
					  IN  PULONG Reserve					/*带保留参数指针*/
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

BOOL WINAPI	DoShell(PCHAR FileName,//欲保护的文件路径
					PCHAR SaveName,//加壳后的文件路径
					ULONG DllInfuNum,//此次选取插件的个数
					Dll_Info *dll_info)//插件的信息
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
	LONG FrontIndex = -1;//记录上一次需要执行的二进制的序号
	LONG NowIndex;
	LONG BaseIndex=0;
	ULONG PluginTypeNum[3]={0};
	ULONG SumOfSize = 0;

	//载入文件，获取PE头
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
	//获取插件二进制

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
			{//成功返回，但是生成二进制失败了。
				MessageBoxA(0,dll_info[i].DllName,"Make Error",MB_OK);
				goto ERR;
			}
		}
		__except(EXCEPTION_CONTINUE_EXECUTION)
		{//调用错误，可能是GetBinary函数地址错误，也可能是GetBinary函数内部错误
			MessageBoxA(0,dll_info[i].DllName,"Run Error",MB_OK);
			goto ERR;
		}

	}
	//整合插件，并复制到区段
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

	//复制至区段
	for (i=0;i<DllInfuNum;i++)
	{
		if (Plugin_Type > dll_info[i].PluginType)
		{//没有按顺序排列插件
			MessageBoxA(0,"没有按顺序排列插件","Error",MB_OK);
			goto ERR;
		}
		else
		{
			Plugin_Type = dll_info[i].PluginType;
			PluginTypeNum[Plugin_Type-1]++;//记录每个类型插件的个数
		}
		memcpy(&pLastSection[NowSumOfSize],pluginInfo[i].pBinary,pluginInfo[i].Size);
		pluginInfo[i].SectionNowSize = NowSumOfSize;
		NowSumOfSize+=pluginInfo[i].Size;
		
	}

	//将插件连接起来
	for (i=0;i<3;i++)
	{//不同类型的插件是 PLUGIN_CODEM 1->PLUGIN_CODEE 2->PLUGIN_TABLE 3的顺序连接起来
		for (j=PluginTypeNum[i];j>0;j--)
		{//同类型插件则按照生成的逆顺序连接
			NowIndex = BaseIndex + j - 1;//计算此次连接的插件序号
			if ( pluginInfo[NowIndex].OffsetCode >= 0)
			{//每次判断是否是需要执行的二进制，若是则将上一个需要执行的二进制连接起来
				if (IsFirst)
				{//重新设置OEP
					
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
	//让最后一个跳向旧OEP
	if (FrontIndex != -1)
	{
		*(PULONG)&pLastSection[pluginInfo[FrontIndex].SectionNowSize + pluginInfo[FrontIndex].OffsetNextCode] = OldOEP;
	}
	

	//将第一个区段设置为可写
	Info.pSectionHeader[0].Characteristics = 0xE0000020;



	//保存文件,释放内存
	free(pluginInfo);
	SaveFile(pFileData,SaveName);
	UnLoadFile(pFileData);
	return true;
	
	
ERR:
	//释放内存
	free(pluginInfo);
	UnLoadFile(pFileData);
	return false;
	
}
