// XorEncrypt.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ShiftEncrypt.h"
#include "PEplugin.h"
#pragma pack(push) //保存对齐状态 
#pragma pack(1)//设定为1字节对齐 
struct FirstBinary
{
	UCHAR push1;
	ULONG Size;
	UCHAR push2;
	ULONG pDataVA;
	UCHAR push3;
	ULONG Key;
	UCHAR Call;
	ULONG CallOffset;
	UCHAR push4;
	ULONG NextCode;
	UCHAR ret;
};
#pragma pack(pop)//恢复对齐状态 
#define MAXLEN 43
UCHAR CodeBinary[]  = {
	0x56, 0x36, 0x8B, 0x74, 0x24, 0x10, 0x33, 0xC0, 0x85, 0xF6, 0x76, 0x1B, 0x36, 0x8B, 0x54, 0x24, 
	0x0C, 0x36, 0x8A, 0x4C, 0x24, 0x08, 0x53, 0x3E, 0x8A, 0x1C, 0x10, 0xD2, 0xC3, 0x3E, 0x88, 0x1C, 
	0x10, 0x40, 0x3B, 0xC6, 0x72, 0xF1, 0x5B, 0x5E, 0xC2, 0x0C, 0x00
};

void __declspec(naked) WINAPI RShift(UCHAR Key,PUCHAR Data,ULONG Size)
{
	__asm
	{
		push esi
		mov esi,dword ptr ss:[esp+0x10]
		xor eax,eax
		test esi,esi
		jbe L2
		mov edx,dword ptr ss:[esp+0xC]
		mov cl,byte ptr ss:[esp+0x8]
		push ebx
L1:		mov bl,byte ptr ds:[eax+edx]
		ror bl,cl
		mov byte ptr ds:[eax+edx],bl
		inc eax
		cmp eax,esi
		jb L1
		pop ebx
L2:		pop esi
		ret 0xc
	}
}


PUCHAR OldBinary = NULL;

BOOL WINAPI GetBinary(IN PVOID pFileData,//PE文件内存指针
					  IN  OUT PUCHAR *pInBinary,//二进制写出指针
					  IN ULONG BinaryAddStartRva,//当前二进制在PE文件的RVA
					  OUT PULONG OffsetCode,//本插件的在需要执行代码相对于二进制的偏移，不执行则为-1
					  OUT PULONG OffsetNextCode,//本插件的在需要跳向下一个执行代码相对于二进制的偏移
					  OUT PULONG Size,
					  IN ULONG ReserveCmdNum,//参数个数
					  IN PULONG ReserveCmd)//参数数组指针
{
	
	FirstBinary *pFirst;
	PEInfo Info;
	ULONG StartCodeDataRva,CodeDataSize;
	if (ReserveCmdNum != 2)
	{
		return false;
	}
	//设置参数
	StartCodeDataRva = ReserveCmd[0];
	CodeDataSize = ReserveCmd[1];
	if (OldBinary)
	{
		free(OldBinary);
	}
	if (!GetPEInfo(pFileData,&Info))
	{
		return false;
	}


	
	//将代码复制
	OldBinary = (PUCHAR)malloc(MAXLEN + sizeof(FirstBinary));
	pFirst = (FirstBinary *)OldBinary;
	memcpy(&OldBinary[sizeof(FirstBinary)],CodeBinary,MAXLEN);
	//设置FirstBinary
	pFirst->push1 = 0x68;
	pFirst->push2 = 0x68;
	pFirst->push3 = 0x68;
	pFirst->push4 = 0x68;
	pFirst->Call = 0xE8;
	pFirst->ret = 0xC3;
	pFirst->Key = 0x3;
	pFirst->pDataVA = StartCodeDataRva + Info.pOptinalHeader->ImageBase;
	pFirst->Size = CodeDataSize;
	pFirst->CallOffset = 6;

	*OffsetNextCode = 21;         
	//将当前内存中数据加密
	PUCHAR pData =  (PUCHAR)Rva2MemAdr(pFileData,StartCodeDataRva);
	RShift(3,pData,CodeDataSize);
	*OffsetCode = 0;
	*OffsetNextCode = 21;
	*Size = MAXLEN + sizeof(FirstBinary);

	*pInBinary = OldBinary;
	return true;
}

CHAR *MyCmd[] = {"要加密的代码开始位置RVA","加密的大小","这个插件是移位加密代"};

void WINAPI GetFunInfo(PULONG _GetBinary,
					   PULONG PluginType,
					   PULONG CmdNum,
					   PCHAR **cmd)
{
	*_GetBinary = (ULONG)GetBinary;//返回GetBinary地址
	*CmdNum = 2;
	*cmd = MyCmd;
	*PluginType = PLUGIN_CODEE;//指令->数据插件


}

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	PEpluginInit();
    return TRUE;
}

