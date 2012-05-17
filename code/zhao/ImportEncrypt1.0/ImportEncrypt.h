#pragma once

#include "stdafx.h"
struct DllInfo
{
	PULONG FirstThunk;//当前内存中输入表的填充地址
	ULONG SumOfAPI;
	PCHAR DLLName;
};
struct APIInfo
{
	ULONG Ordinal;
	PCHAR APIName;//为0时，表示用序号查找//VA
};

#pragma pack(push) //保存对齐状态 
#pragma pack(1)//设定为1字节对齐 
struct FirstBinary
{//这个结构体在整个二进制的首部
	IMAGE_IMPORT_DESCRIPTOR Improt;										
	IMAGE_IMPORT_DESCRIPTOR EndOfImprot;//这个值是0						
	CHAR Kernel[13];//字符串kerner32.dll								
	ULONG OriginalFirstThunkLoadLibrary;//OriginalFirstThunk//21
	ULONG OriginalFirstThunkGetProAddress;
	ULONG EndOfOriginalFirstThunk;//这个值是0


	
	WORD HitOfLoadLibrary;//IMAGE_THUNK_DATA
	CHAR LoadLibrary[13];//字符串LoadLibraryA
	WORD HitOfGetProAddress;//IMAGE_THUNK_DATA
	CHAR GetProcAddress[15];//字符串GetProAddress
	char Data[250];//引擎需要的数据
	ULONG DataPtr;//指向数据的指针VA
	ULONG FirstThunkLoadLibrary;//FirstThunk 也就是说此处填充真实API地址
	ULONG FirstThunkGetProAddress;//
	ULONG EndOfFirstThunk;//这个值是
	DWORD pImport;//在PE文件中指向ImportInfo的VA
	ULONG MaxLen;//DLL名，和API名的最大长度
};

struct JmpBinary
{//转发二进制，当程序调用API会先执行到这里，跳向输入表解密函数，最后才会调用API
	UCHAR Push1;
	ULONG DllNum;//Dll序号
	UCHAR Push2;
	ULONG APINum;//API序号
	UCHAR Push3;
	ULONG DllKey;//Dll密钥
	UCHAR Push4;
	ULONG APIKey;//API密钥
	UCHAR Push5;
	ULONG FunCall;//解密函数地址
	UCHAR Ret;
};
#pragma pack(pop)//恢复对齐状态 
