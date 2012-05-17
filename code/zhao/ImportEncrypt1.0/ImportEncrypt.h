#pragma once

#include "stdafx.h"
struct DllInfo
{
	PULONG FirstThunk;//��ǰ�ڴ�������������ַ
	ULONG SumOfAPI;
	PCHAR DLLName;
};
struct APIInfo
{
	ULONG Ordinal;
	PCHAR APIName;//Ϊ0ʱ����ʾ����Ų���//VA
};

#pragma pack(push) //�������״̬ 
#pragma pack(1)//�趨Ϊ1�ֽڶ��� 
struct FirstBinary
{//����ṹ�������������Ƶ��ײ�
	IMAGE_IMPORT_DESCRIPTOR Improt;										
	IMAGE_IMPORT_DESCRIPTOR EndOfImprot;//���ֵ��0						
	CHAR Kernel[13];//�ַ���kerner32.dll								
	ULONG OriginalFirstThunkLoadLibrary;//OriginalFirstThunk//21
	ULONG OriginalFirstThunkGetProAddress;
	ULONG EndOfOriginalFirstThunk;//���ֵ��0


	
	WORD HitOfLoadLibrary;//IMAGE_THUNK_DATA
	CHAR LoadLibrary[13];//�ַ���LoadLibraryA
	WORD HitOfGetProAddress;//IMAGE_THUNK_DATA
	CHAR GetProcAddress[15];//�ַ���GetProAddress
	char Data[250];//������Ҫ������
	ULONG DataPtr;//ָ�����ݵ�ָ��VA
	ULONG FirstThunkLoadLibrary;//FirstThunk Ҳ����˵�˴������ʵAPI��ַ
	ULONG FirstThunkGetProAddress;//
	ULONG EndOfFirstThunk;//���ֵ��
	DWORD pImport;//��PE�ļ���ָ��ImportInfo��VA
	ULONG MaxLen;//DLL������API������󳤶�
};

struct JmpBinary
{//ת�������ƣ����������API����ִ�е���������������ܺ��������Ż����API
	UCHAR Push1;
	ULONG DllNum;//Dll���
	UCHAR Push2;
	ULONG APINum;//API���
	UCHAR Push3;
	ULONG DllKey;//Dll��Կ
	UCHAR Push4;
	ULONG APIKey;//API��Կ
	UCHAR Push5;
	ULONG FunCall;//���ܺ�����ַ
	UCHAR Ret;
};
#pragma pack(pop)//�ָ�����״̬ 
