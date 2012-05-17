// ImportEncrypt.cpp : ���� DLL Ӧ�ó���ĵ���������
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

//����ת��������
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


//pFileData�ڴ���ָ��PE�ļ���ָ��
//Va������ζ����ƽ����õ�Va��ַ
//Binary���ڷ���ָ������Ƶ�ָ��
//Size���ڷ��ض����ƵĴ�С
bool WINAPI GetBinary(IN PVOID pFileData,
					  IN OUT PUCHAR &Binary,
					  IN ULONG StartRva,//Ҫ����PE�ļ�ָ��Ŀ�ʼλ��RVA
					  OUT PLONG OffsetCode,//�����������Ҫִ�д�������ڶ����Ƶ�ƫ�ƣ���ִ����Ϊ-1
					  OUT PLONG OffsetNextCode,//�����������Ҫ������һ��ִ�д�������ڶ����Ƶ�ƫ��
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
		//����DLL��Ϣ
		SumOfDLL++;
		PCHAR pDLLName = (PCHAR)Rva2MemAdr(pFileData,pImportDirectory->Name);
			//����dll��
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
		{//����API��Ϣ
			SumOfAPI++;
			if (IMAGE_SNAP_BY_ORDINAL32(pThunk->u1.Ordinal))
			{//�ж��Ƿ�Ϊ��������
				apiinfo.Ordinal = pThunk->u1.Ordinal;
				apiinfo.APIName = NULL;
			}
			else
			{//�����������
				PIMAGE_IMPORT_BY_NAME pByName = (PIMAGE_IMPORT_BY_NAME)Rva2MemAdr(pFileData,(ULONG)pThunk->u1.AddressOfData);
				apiinfo.Ordinal = pByName->Hint;
				//����dll��
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
	}//���������ȫ����ȡ���
	
	/*-----------------��ʼ���ɶ�����------------------*/
	//������˳��
	//
	//FirstBinary		����Ϊ sizeof(FirstBinary)
	//��
	//���ܴ��������	����Ϊ CODESIZE
	//��
	//����IAT����		����Ϊ SumOfAPI * 8 + SumOfDLL * 8
	//��
	//DLL,API�ַ���		����Ϊ SumOfLen
	//��
	//ת��������		����Ϊ SumOfAPI * sizeof(JmpBinary)




	//FirstBinary ----- ���ɿ���Ҫ�ı�׼�����
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
	firstbinary.MaxLen =(MaxLen%4 ? 4*(MaxLen/4+1) : MaxLen);//��󳤶�Ϊ4������
	firstbinary.pImport = StartRva + Info.pOptinalHeader->ImageBase + sizeof(FirstBinary) + CODESIZE;
	firstbinary.DataPtr = StartRva + Info.pOptinalHeader->ImageBase + OFFSETOF(FirstBinary,Data);
	memcpy(firstbinary.Data,Data,DATASIZE);
	//���FirstBinary, ʹPE�ļ�ָ�����ǵ������ṹ
	if (!SetImport(pFileData,true,StartRva))
	{
		return false;
	}
	ListAddNode(&BinaryHead,&firstbinary,sizeof(FirstBinary));



	//ͨ��ģ�����ɽ�����������
	ULONG FunCallVA;
	ListAddNode(&BinaryHead,Code,CODESIZE);
	FunCallVA = StartRva + Info.pOptinalHeader->ImageBase + sizeof(FirstBinary);
	
	
	
	//����JmpBinary����ܺ��IAT,�޸�ԭ�����FirstThunk����
	List *pDllNode = DllInfoHead;
	List *pAPINode = APIInfoHead;
	DllInfo *pdllinfo;
	APIInfo *papiinfi;
	
	//���ڵ������������λ�գ�
	PULONG pCurrentIAT = (PULONG)ListAddNode(&BinaryHead,NULL,SumOfAPI * 8 + SumOfDLL * 8);
	PCHAR pCurrentString = (PCHAR)ListAddNode(&BinaryHead,NULL,SumOfLen);
	JmpBinary *pCurrentJMP = (JmpBinary *)ListAddNode(&BinaryHead,NULL,SumOfAPI * sizeof(JmpBinary));
	//�������
	PULONG pFirstThunk;
	ULONG dllkey;
	ULONG apikey;
	ULONG NextOffset;
	ULONG StringOffset = 0;
	ULONG JMPOffset = 0;
	for (ULONG i=0;i < SumOfDLL;i++)
	{	
		//����DLL��Ϣ
	 	pdllinfo = (DllInfo *)pDllNode->pData;
			//������һ������IATָ��
		NextOffset = 8 + pdllinfo->SumOfAPI * 8;
		*pCurrentIAT =  NextOffset ;
		pCurrentIAT++;
			//����DLL��������
		dllkey = MakeKey();

		memcpy(pCurrentString,pdllinfo->DLLName,strlen(pdllinfo->DLLName)+1);
		Xor(dllkey,pCurrentString,strlen(pdllinfo->DLLName)+1);
			//����DLL��ָ��
		*pCurrentIAT = StartRva + Info.pOptinalHeader->ImageBase + STRINGOFFSET;
		pCurrentIAT++;
		pCurrentString+=strlen(pdllinfo->DLLName)+1;
		StringOffset+=strlen(pdllinfo->DLLName)+1;
			//����pFirstThunk
		pFirstThunk = pdllinfo->FirstThunk;
		
		
		for (ULONG j=0;j < pdllinfo->SumOfAPI;j++)
		{
			//����API��Ϣ
			papiinfi = (APIInfo *)pAPINode->pData;
			//�������
			*pCurrentIAT = papiinfi->Ordinal & 0x0000FFFF;
			pCurrentIAT++;
			apikey = MakeKey();
			if (papiinfi->APIName != NULL)
			{
				//����API��
				memcpy(pCurrentString,papiinfi->APIName,strlen(papiinfi->APIName)+1);
				Xor(apikey,pCurrentString,strlen(papiinfi->APIName)+1);
				//����APIָ��
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
			//����JmpBinary
			MakeJmpBinary(pCurrentJMP, i+1,j+1,dllkey,apikey,FunCallVA);
			//�޸�pFirstThunk
			*pFirstThunk = StartRva + Info.pOptinalHeader->ImageBase + JMPOFFSET;
			JMPOffset+=sizeof(JmpBinary);
			pFirstThunk++;
			pCurrentJMP++;
	
			pAPINode = pAPINode->NextNode;
			
		}



		pDllNode = pDllNode->NextNode;


	}
	//��ȡ�ܴ�С
	PList plist = BinaryHead;
	Size = 0;
	while(plist != NULL)
	{
		Size += plist->Size;
		plist = plist->NextNode;
	}
	//���ɶ�����
	
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
	//���˶������ļ��������




	

	ListFree(&DllInfoHead);
	ListFree(&APIInfoHead);
	ListFree(&JmpBinaryHead);
	ListFree(&StringHead);
	ListFree(&BinaryHead);
	return true;
}

CHAR *MyCmd[] = {"����һ���������ܵ�ģ��"};

void WINAPI GetFunInfo(PULONG _GetBinary, 
				PULONG PluginType,
				PULONG  CmdNum,
				PCHAR   **cmd)

{	
		*_GetBinary = (ULONG)GetBinary;
		*CmdNum = 0;
		*cmd = MyCmd;
		*PluginType = PLUGIN_TABLE;//ָ��->���ݲ��
		
}