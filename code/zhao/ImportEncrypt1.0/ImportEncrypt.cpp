// ImportEncrypt.cpp : 定义 DLL 应用程序的导出函数。
//
#include "ImportEncrypt.h"
#include "Link.h"
#include "PEplugin.h"


#define OFFSETOF(type, field)  ((size_t)&(((type *)0)->field))
#define STRINGOFFSET (sizeof(FirstBinary) + CODESIZE + SumOfAPI * 8 + SumOfDLL * 8 + StringOffset)
#define JMPOFFSET (sizeof(FirstBinary) + CODESIZE + SumOfAPI * 8 + SumOfDLL * 8 + SumOfLen + JMPOffset)

PList DllInfoHead = NULL;
PList APIInfoHead = NULL;
PList JmpBinaryHead = NULL;
PList StringHead = NULL;
PList BinaryHead = NULL;
PUCHAR OldBinary = NULL;

#define DATASIZE 45
UCHAR Data[DATASIZE] = {0x75,0x73,0x65,0x72,0x33,0x32,0x2E,0x64,0x6C,0x6C,0x00,0x4D,0x65,0x73,0x73,0x61,0x67,0x65,0x42,0x6F,0x78,0x41,0x00,0xBC,0xEC,0xB2,0xE2,0xBD,0xE1,0xB9,0xFB,0x00,0xBC,0xEC,0xB2,0xE2,0xB5,0xBD,0xB5,0xF7,0xCA,0xD4,0xC6,0xF7,0x00};
#define CODESIZE 260
UCHAR Code[CODESIZE] = {0x55,0x8B,0xEC,0x60,0xE8,0x00,0x00,0x00,0x00,0x5B,0x8B,0x73,0xEF,0x8B,0x4D,0x10,0x83,0xF9,0x01,0x74,0x06,0x8B,0x3E,0x03,0xF7,0xE2,0xF5,0x83,0xC6,0x04,0x2B,0x63,0xF3,0x8B,0xFC,0x8B,0x43,0xF3,0xB9,0x04,0x00,0x00,0x00,0x33,0xD2,0xF7,0xF1,0x8B,0xC8,0x53,0x56,0x8B,0x06,0x8B,0xF0,0x8B,0x5D,0x08,0x8B,0x06,0x33,0xC3,0x89,0x07,0x83,0xC6,0x04,0x83,0xC7,0x04,0xE2,0xF2,0x5E,0x5B,0x8B,0x7B,0xE3,0x54,0xFF,0xD7,0x03,0x63,0xF3,0x8B,0xD0,0x8B,0x45,0x0C,0xB9,0x08,0x00,0x00,0x00,0x52,0xF7,0xE1,0x5A,0x03,0xF0,0x83,0xEE,0x04,0x8B,0x46,0x04,0x85,0xC0,0x75,0x0B,0x8B,0x0E,0x8B,0x7B,0xE7,0x51,0x52,0xFF,0xD7,0xEB,0x4D,0x2B,0x63,0xF3,0x8B,0xFC,0x83,0xC6,0x04,0x8B,0x43,0xF3,0xB9,0x04,0x00,0x00,0x00,0x52,0x33,0xD2,0xF7,0xF1,0x5A,0x8B,0xC8,0x53,0x56,0x8B,0x06,0x8B,0xF0,0x8B,0x5D,0x04,0x8B,0x06,0x33,0xC3,0x89,0x07,0x83,0xC6,0x04,0x83,0xC7,0x04,0xE2,0xF2,0x5E,0x5B,0x8B,0x7B,0xE7,0x54,0x52,0xFF,0xD7,0x03,0x63,0xF3,0x8B,0xF8,0xB9,0x01,0x00,0x00,0x00,0x0F,0xB6,0x17,0x80,0xFA,0xCC,0x74,0x0C,0x47,0xE2,0xF5,0x89,0x45,0x10,0x61,0x5D,0x83,0xC4,0x0C,0xC3,0x60,0x8B,0x7B,0xE3,0x8B,0x4B,0xDF,0x51,0xFF,0xD7,0x8B,0x7B,0xE7,0x8B,0x53,0xDF,0x8D,0x4A,0x0B,0x51,0x50,0xFF,0xD7,0x8B,0x53,0xDF,0x8D,0x4A,0x17,0x8D,0x5A,0x20,0x6A,0x00,0x51,0x53,0x6A,0x00,0xFF,0xD0,0x61,0xEB,0xCC};

BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	PEpluginInit();
	return TRUE;
}

//生成转发二进制
ULONG MakeKey()
{
	ULONG key;
	__asm
	{
		RDTSC
		MOV key,EAX
	}
	return (key * key) % 0x6E69736C;
}
void MakeJmpBinary(JmpBinary *pJBinary,ULONG DllNum,ULONG APINum,ULONG DllKey,ULONG APIKey,ULONG FunCallVA)
{
	pJBinary->Push1 = 0x68;
	pJBinary->Push2 = 0x68;
	pJBinary->Push3 = 0x68;
	pJBinary->Push4 = 0x68;
	pJBinary->Push5 = 0x68;
	pJBinary->Ret = 0xC3;
	pJBinary->APINum = APINum;
	pJBinary->DllNum = DllNum;
	pJBinary->APIKey = APIKey;
	pJBinary->DllKey = DllKey;
	pJBinary->FunCall = FunCallVA;
	
}
void Xor(ULONG Key,PCHAR Data,ULONG Size)
{
	
	PUCHAR pKey = (PUCHAR)&Key;
	for (ULONG i=0;i < Size;i++)
	{
		Data[i] = Data[i] ^ pKey[i % 4];
	}
}


//pFileData内存中指向PE文件的指针
//Va传入这段二进制将放置的Va地址
//Binary用于返回指向二进制的指针
//Size用于返回二进制的大小
bool WINAPI GetBinary(IN PVOID pFileData,
					  IN OUT PUCHAR &Binary,
					  IN ULONG StartRva,//要操作PE文件指令的开始位置RVA
					  OUT PLONG OffsetCode,//本插件的在需要执行代码相对于二进制的偏移，不执行则为-1
					  OUT PLONG OffsetNextCode,//本插件的在需要跳向下一个执行代码相对于二进制的偏移
					  OUT ULONG &Size,
					  IN ULONG ReserveCmdNum,
					  IN PULONG ReserveCmd)
{
	PEInfo	Info;
	PIMAGE_IMPORT_DESCRIPTOR pImportDirectory;
	APIInfo apiinfo;
	DllInfo dllinfo;
	ULONG MaxLen = 0;
	ULONG SumOfLen = 0;
	ULONG SumOfDLL = 0;
	ULONG SumOfAPI = 0;
	
	ListInit(&DllInfoHead);
	ListInit(&APIInfoHead);
	ListInit(&JmpBinaryHead);
	ListInit(&StringHead);
	ListInit(&BinaryHead);
	if (OldBinary)
	{
		free(OldBinary);
	}
// 	typedef struct _IMAGE_IMPORT_DESCRIPTOR {
// 		union {
// 			DWORD   Characteristics;            // 0 for terminating null import descriptor
// 			DWORD   OriginalFirstThunk;         // RVA to original unbound IAT (PIMAGE_THUNK_DATA)
// 		};
// 		DWORD   TimeDateStamp;                  // 0 if not bound,
// 		// -1 if bound, and real date\time stamp
// 		//     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
// 		// O.W. date/time stamp of DLL bound to (Old BIND)
// 		
// 		DWORD   ForwarderChain;                 // -1 if no forwarders
// 		DWORD   Name;
// 		DWORD   FirstThunk;                     // RVA to IAT (if bound this IAT has actual addresses)
// 	} IMAGE_IMPORT_DESCRIPTOR;
// 	typedef IMAGE_IMPORT_DESCRIPTOR UNALIGNED *PIMAGE_IMPORT_DESCRIPTOR;
	if(!GetPEInfo(pFileData,&Info))
	{
		return false;
	}
	pImportDirectory = (PIMAGE_IMPORT_DESCRIPTOR)Rva2MemAdr(pFileData,Info.pDataDriectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);


	while (pImportDirectory->Characteristics != 0)
	{
		//保存DLL信息
		SumOfDLL++;
		PCHAR pDLLName = (PCHAR)Rva2MemAdr(pFileData,pImportDirectory->Name);
			//保存dll名
		dllinfo.DLLName = (PCHAR)ListAddNode(&StringHead,pDLLName,strlen(pDLLName)+1);
		SumOfLen += strlen(pDLLName)+1;
		if (strlen(pDLLName)+1 > MaxLen)
		{
			MaxLen = strlen(pDLLName)+1;
		}
		dllinfo.FirstThunk = (PULONG)Rva2MemAdr(pFileData,pImportDirectory->FirstThunk);
		dllinfo.SumOfAPI = 0;

		

		PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)(Rva2MemAdr(pFileData,pImportDirectory->OriginalFirstThunk));
		while (*(PULONG)pThunk != 0)
		{//保存API信息
			SumOfAPI++;
			if (IMAGE_SNAP_BY_ORDINAL32(pThunk->u1.Ordinal))
			{//判断是否为序号输入表
				apiinfo.Ordinal = pThunk->u1.Ordinal;
				apiinfo.APIName = NULL;
			}
			else
			{//函数名输入表
				PIMAGE_IMPORT_BY_NAME pByName = (PIMAGE_IMPORT_BY_NAME)Rva2MemAdr(pFileData,(ULONG)pThunk->u1.AddressOfData);
				apiinfo.Ordinal = pByName->Hint;
				//保存dll名
				PCHAR pAPIName = (PCHAR)pByName->Name;
				apiinfo.APIName = (PCHAR)ListAddNode(&StringHead,pAPIName,strlen(pAPIName)+1);
				SumOfLen += strlen(pAPIName)+1;
				if (strlen(pAPIName)+1 > MaxLen)
				{
					
					MaxLen = strlen(pAPIName)+1;
				}
			}
			dllinfo.SumOfAPI++;
			ListAddNode(&APIInfoHead,&apiinfo,sizeof(APIInfo));
			pThunk++; 
		}
		ListAddNode(&DllInfoHead,&dllinfo,sizeof(DllInfo));
		pImportDirectory++;
	}//至此输入表全部获取完成
	
	/*-----------------开始生成二进制------------------*/
	//二进制顺序
	//
	//FirstBinary		长度为 sizeof(FirstBinary)
	//↓
	//解密代码二进制	长度为 CODESIZE
	//↓
	//加密IAT数据		长度为 SumOfAPI * 8 + SumOfDLL * 8
	//↓
	//DLL,API字符串		长度为 SumOfLen
	//↓
	//转发二进制		长度为 SumOfAPI * sizeof(JmpBinary)




	//FirstBinary ----- 生成壳需要的标准输入表
	FirstBinary firstbinary = {0};
	firstbinary.Improt.OriginalFirstThunk = StartRva + OFFSETOF(FirstBinary,OriginalFirstThunkLoadLibrary);
	firstbinary.Improt.FirstThunk = StartRva + OFFSETOF(FirstBinary,FirstThunkLoadLibrary);
	memcpy(firstbinary.Kernel,"kernel32.dll",13);
	firstbinary.Improt.Name = StartRva + OFFSETOF(FirstBinary,Kernel);
	memcpy(firstbinary.LoadLibrary,"LoadLibraryA",13);
	memcpy(firstbinary.GetProcAddress,"GetProcAddress",15);
	firstbinary.OriginalFirstThunkLoadLibrary = StartRva + OFFSETOF(FirstBinary,HitOfLoadLibrary);
	firstbinary.OriginalFirstThunkGetProAddress = StartRva + OFFSETOF(FirstBinary,HitOfGetProAddress);
	firstbinary.FirstThunkGetProAddress = firstbinary.OriginalFirstThunkGetProAddress;
	firstbinary.FirstThunkLoadLibrary = firstbinary.OriginalFirstThunkLoadLibrary;
	firstbinary.MaxLen =(MaxLen%4 ? 4*(MaxLen/4+1) : MaxLen);//最大长度为4的整数
	firstbinary.pImport = StartRva + Info.pOptinalHeader->ImageBase + sizeof(FirstBinary) + CODESIZE;
	firstbinary.DataPtr = StartRva + Info.pOptinalHeader->ImageBase + OFFSETOF(FirstBinary,Data);
	memcpy(firstbinary.Data,Data,DATASIZE);
	//完成FirstBinary, 使PE文件指向我们的输入表结构
	if (!SetImport(pFileData,true,StartRva))
	{
		return false;
	}
	ListAddNode(&BinaryHead,&firstbinary,sizeof(FirstBinary));



	//通过模板生成解密输入表代码
	ULONG FunCallVA;
	ListAddNode(&BinaryHead,Code,CODESIZE);
	FunCallVA = StartRva + Info.pOptinalHeader->ImageBase + sizeof(FirstBinary);
	
	
	
	//生成JmpBinary与加密后的IAT,修改原程序的FirstThunk数组
	List *pDllNode = DllInfoHead;
	List *pAPINode = APIInfoHead;
	DllInfo *pdllinfo;
	APIInfo *papiinfi;
	
	//将节点加入链表（数据位空）
	PULONG pCurrentIAT = (PULONG)ListAddNode(&BinaryHead,NULL,SumOfAPI * 8 + SumOfDLL * 8);
	PCHAR pCurrentString = (PCHAR)ListAddNode(&BinaryHead,NULL,SumOfLen);
	JmpBinary *pCurrentJMP = (JmpBinary *)ListAddNode(&BinaryHead,NULL,SumOfAPI * sizeof(JmpBinary));
	//填充数据
	PULONG pFirstThunk;
	ULONG dllkey;
	ULONG apikey;
	ULONG NextOffset;
	ULONG StringOffset = 0;
	ULONG JMPOffset = 0;
	for (ULONG i=0;i < SumOfDLL;i++)
	{	
		//设置DLL信息
	 	pdllinfo = (DllInfo *)pDllNode->pData;
			//设置下一个加密IAT指针
		NextOffset = 8 + pdllinfo->SumOfAPI * 8;
		*pCurrentIAT =  NextOffset ;
		pCurrentIAT++;
			//保存DLL名并加密
		dllkey = MakeKey();

		memcpy(pCurrentString,pdllinfo->DLLName,strlen(pdllinfo->DLLName)+1);
		Xor(dllkey,pCurrentString,strlen(pdllinfo->DLLName)+1);
			//设置DLL名指针
		*pCurrentIAT = StartRva + Info.pOptinalHeader->ImageBase + STRINGOFFSET;
		pCurrentIAT++;
		pCurrentString+=strlen(pdllinfo->DLLName)+1;
		StringOffset+=strlen(pdllinfo->DLLName)+1;
			//设置pFirstThunk
		pFirstThunk = pdllinfo->FirstThunk;
		
		
		for (ULONG j=0;j < pdllinfo->SumOfAPI;j++)
		{
			//设置API信息
			papiinfi = (APIInfo *)pAPINode->pData;
			//设置序号
			*pCurrentIAT = papiinfi->Ordinal & 0x0000FFFF;
			pCurrentIAT++;
			apikey = MakeKey();
			if (papiinfi->APIName != NULL)
			{
				//加密API名
				memcpy(pCurrentString,papiinfi->APIName,strlen(papiinfi->APIName)+1);
				Xor(apikey,pCurrentString,strlen(papiinfi->APIName)+1);
				//设置API指针
				*pCurrentIAT = StartRva + Info.pOptinalHeader->ImageBase + STRINGOFFSET;
				pCurrentIAT++;
				pCurrentString+=strlen(papiinfi->APIName)+1;
				StringOffset+=strlen(papiinfi->APIName)+1;
			}
			else
			{
				*pCurrentIAT = 0;
				pCurrentIAT++;
			}
			//生成JmpBinary
			MakeJmpBinary(pCurrentJMP, i+1,j+1,dllkey,apikey,FunCallVA);
			//修改pFirstThunk
			*pFirstThunk = StartRva + Info.pOptinalHeader->ImageBase + JMPOFFSET;
			JMPOffset+=sizeof(JmpBinary);
			pFirstThunk++;
			pCurrentJMP++;
	
			pAPINode = pAPINode->NextNode;
			
		}



		pDllNode = pDllNode->NextNode;


	}
	//获取总大小
	PList plist = BinaryHead;
	Size = 0;
	while(plist != NULL)
	{
		Size += plist->Size;
		plist = plist->NextNode;
	}
	//生成二进制
	
	OldBinary = (PUCHAR)malloc(Size);

	plist = BinaryHead;
	ULONG offset = 0;
	while(plist != NULL)
	{
		memcpy((PVOID)((ULONG)OldBinary+offset),plist->pData,plist->Size);
		offset += plist->Size;
		plist = plist->NextNode;
	}
	Binary = OldBinary;
	//至此二进制文件生成完成




	

	ListFree(&DllInfoHead);
	ListFree(&APIInfoHead);
	ListFree(&JmpBinaryHead);
	ListFree(&StringHead);
	ListFree(&BinaryHead);
	return true;
}

CHAR *MyCmd[] = {"这是一个输入表加密的模块"};

void WINAPI GetFunInfo(PULONG _GetBinary, 
				PULONG PluginType,
				PULONG  CmdNum,
				PCHAR   **cmd)

{	
		*_GetBinary = (ULONG)GetBinary;
		*CmdNum = 0;
		*cmd = MyCmd;
		*PluginType = PLUGIN_TABLE;//指令->数据插件
		
}