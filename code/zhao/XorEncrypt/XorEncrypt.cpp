// XorEncrypt.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "PEplugin.h"
#pragma pack(push) //�������״̬ 
#pragma pack(1)//�趨Ϊ1�ֽڶ��� 
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
#pragma pack(pop)//�ָ�����״̬ 
#define MAXLEN 43
UCHAR CodeBinary[]  = {
		0x56, 0x8B, 0x74, 0x24, 0x10, 0x33, 0xC0, 0x85, 0xF6, 0x76, 0x1C, 0x8B, 0x4C, 0x24, 0x0C, 0x53, 
		0x8A, 0x1C, 0x08, 0x8B, 0xD0, 0x83, 0xE2, 0x03, 0x8A, 0x54, 0x14, 0x0C, 0x32, 0xDA, 0x88, 0x1C, 
		0x08, 0x40, 0x3B, 0xC6, 0x72, 0xEA, 0x5B, 0x5E, 0xC2, 0x0C, 0x00
};

void WINAPI	Xor(ULONG Key,PUCHAR Data,ULONG Size)
{
	
	PUCHAR pKey = (PUCHAR)&Key;
	for (ULONG i=0;i < Size;i++)
	{
		Data[i] = Data[i] ^ pKey[i % 4];
	}
}

PUCHAR OldBinary = NULL;

BOOL WINAPI GetBinary(IN PVOID pFileData,//PE�ļ��ڴ�ָ��
					  IN  OUT PUCHAR *pInBinary,//������д��ָ��
					  IN ULONG BinaryAddStartRva,//��ǰ��������PE�ļ���RVA
					  OUT PULONG OffsetCode,//�����������Ҫִ�д�������ڶ����Ƶ�ƫ�ƣ���ִ����Ϊ-1
					  OUT PULONG OffsetNextCode,//�����������Ҫ������һ��ִ�д�������ڶ����Ƶ�ƫ��
					  OUT PULONG Size,
					  IN ULONG ReserveCmdNum,//��������
					  IN PULONG ReserveCmd)//��������ָ��				
{

	FirstBinary *pFirst;
	PEInfo Info;
	ULONG StartCodeDataRva,CodeDataSize;
	if (ReserveCmdNum != 2)
	{
		return false;
	}
	//���ò���
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
	OldBinary = (PUCHAR)malloc(MAXLEN + sizeof(FirstBinary));
	pFirst = (FirstBinary *)OldBinary;
	//�����븴��
	memcpy(&OldBinary[sizeof(FirstBinary)],CodeBinary,MAXLEN);
	//����FirstBinary
	pFirst->push1 = 0x68;
	pFirst->push2 = 0x68;
	pFirst->push3 = 0x68;
	pFirst->push4 = 0x68;
	pFirst->Call = 0xE8;
	pFirst->ret = 0xC3;
	pFirst->Key = 0x6E69736C;
	pFirst->pDataVA = StartCodeDataRva + Info.pOptinalHeader->ImageBase;
	pFirst->Size = CodeDataSize;
	pFirst->CallOffset = 6;

	*OffsetNextCode = 21;
	//����ǰ�ڴ������ݼ���
	PUCHAR pData =  (PUCHAR)Rva2MemAdr(pFileData,StartCodeDataRva);
	Xor(0x6E69736C,pData,CodeDataSize);

	*OffsetCode = 0;
    *Size = MAXLEN+sizeof(FirstBinary);

	*pInBinary = OldBinary;
	return true;
}



CHAR *MyCmd[] = {"Ҫ���ܵĴ��뿪ʼλ��RVA","���ܵĴ�С","�������������ܴ�"};

void WINAPI GetFunInfo(
					   PULONG _GetBinary, 
					   PULONG PluginType,
					   PULONG  CmdNum,
						PCHAR   **cmd
						)

{	
	*_GetBinary = (ULONG)GetBinary;//����GetBinary��ַ
	*CmdNum = 2;
	*cmd = MyCmd;
	*PluginType = PLUGIN_CODEE;//ָ��->���ݲ��

}

BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	PEpluginInit();
    return TRUE;
}