// PEplugin.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "PEplugin.h"
ULONG	WINAPI Rva2MemAdr(PVOID pFileData,ULONG Rva)
{
	ULONG Raw;
	if( Rva2Raw(pFileData,Rva,Raw) ) 
	{
		return Raw +(ULONG)pFileData;
	}
	return 0;
}
BOOL	WINAPI Rva2Raw(PVOID pFileData, ULONG Rva, OUT ULONG &Raw)
	// Rva2Raw:�ڴ����ƫ��ת�������ļ������ƫ��
{
	PIMAGE_NT_HEADERS pNtH;
	PIMAGE_SECTION_HEADER pSectH;
	ULONG Index;
	PEInfo Info;
	if( !GetPEInfo(pFileData ,&Info) ) 
	{
		return false;
	}
	pNtH = Info.pNtHeader;
	pSectH = Info.pSectionHeader;

	if ( Rva >= 0 && Rva < pSectH->VirtualAddress )
	{
		// ������ڴ��ַ���ƫ�����ڽڱ��һ��Ԫ�ص�ַ�����ƫ��֮ǰ��ʾ���ǽڱ���Ķ�������ַ����Ҫ�ض�λ
		Raw = Rva;
		return true;
	}
	for ( Index = 0; Index < pNtH->FileHeader.NumberOfSections; Index++ )
	{
		if ( Rva >= pSectH->VirtualAddress && Rva < pSectH->VirtualAddress + pSectH->Misc.VirtualSize )
		{// ��ַ���ڸýڱ�֮��
			Raw = Rva - pSectH->VirtualAddress + pSectH->PointerToRawData;
			return true;
		}
		// �ڱ�ͷ�ļ����ƫ��+���ڴ��ַ���ƫ��-�ڱ�ͷ�ڴ����ƫ��
		pSectH++;
	}
	return false;
}
BOOL	WINAPI Raw2Rva(PVOID pFileData, ULONG Raw, OUT ULONG &Rva)
{
	PIMAGE_NT_HEADERS pNtH;
	PIMAGE_SECTION_HEADER pSectH;
	ULONG Index;
	PEInfo Info;
	if( !GetPEInfo(pFileData ,&Info) ) 
	{
		return false;
	}
	pNtH = Info.pNtHeader;
	pSectH = Info.pSectionHeader;

	if ( Raw >= 0 && Raw < pSectH->PointerToRawData )
	{
		// ������ļ���ַ���ƫ�����ڽڱ��һ��Ԫ�ص�ַ�����ƫ��֮ǰ��ʾ���ǽڱ���Ķ�������ַ����Ҫ�ض�λ
		Rva = Raw;
		return true;
	}
	for ( Index = 0; Index < pNtH->FileHeader.NumberOfSections; Index++ )
	{
		if ( Raw >= pSectH->PointerToRawData && Raw < pSectH->PointerToRawData + pSectH->SizeOfRawData )
		{		// ��ַ���ڸýڱ�֮��
			Rva = Raw + pSectH->VirtualAddress - pSectH->PointerToRawData;
			return true;
		}	// �ڱ�ͷ�ļ����ƫ��-���ڴ��ַ���ƫ��+�ڱ�ͷ�ڴ����ƫ��
		pSectH++;
	}
	return 0;
}
void	WINAPI OutPutError(char * ErrorMsg)
{
	char * ErrorStr = (char *)malloc(strlen(ErrorMsg)+100);
	sprintf(ErrorStr,"%s %s = %8X",ErrorMsg,"Error,Error Code",GetLastError());
	MessageBox(NULL,ErrorStr,"Error!",MB_OK);
	free(ErrorMsg);
}
BOOL 	WINAPI LoadFile(IN CHAR * szFilePath,OUT PVOID &pFileData)
{
	HANDLE hFile;
	ULONG ReadSize;
	ULONG FileSize;
	hFile = CreateFile((LPCTSTR)szFilePath, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ,NULL, OPEN_EXISTING, 0, NULL);
	if ( INVALID_HANDLE_VALUE == hFile)
	{
		OutPutError("CreateFile");
		return false;
	}
	FileSize = GetFileSize(hFile,NULL);
	if(GetFileSize(hFile,NULL) == INVALID_FILE_SIZE)
	{
		OutPutError("GetFileSize");
		return false;
	}

	pFileData = malloc(FileSize);
	if ( !ReadFile(hFile,pFileData,FileSize,&ReadSize,NULL) )
	{
		OutPutError("ReadFile");
		free(pFileData);
		CloseHandle(hFile);
		return false;
	}
	CloseHandle(hFile);
	return true;
}
BOOL	WINAPI GetPEInfo(IN PVOID pFileData,OUT PEInfo * pPEInfo)
{
	if( IsBadReadPtr(pPEInfo,sizeof(PEInfo))) 
	{
		MessageBox(NULL,"PEInfo point can't read","Error",MB_OK);
		return false;
	}
	__try
	{
		pPEInfo->pDosHeader = (PIMAGE_DOS_HEADER)pFileData;
		pPEInfo->pNtHeader = (PIMAGE_NT_HEADERS)((ULONG)pFileData + pPEInfo->pDosHeader->e_lfanew);
		pPEInfo->pFileHeader = &(pPEInfo->pNtHeader->FileHeader);
		pPEInfo->pOptinalHeader = &(pPEInfo->pNtHeader->OptionalHeader);
		pPEInfo->pDataDriectory = (PIMAGE_DATA_DIRECTORY)&(pPEInfo->pOptinalHeader->DataDirectory);
		pPEInfo->pSectionHeader = (PIMAGE_SECTION_HEADER)((ULONG)pPEInfo->pNtHeader + sizeof(IMAGE_NT_HEADERS));
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		MessageBox(NULL,"It's not a PE file","Error",MB_OK);
		return false;
	}
	return true;
}
BOOL 	WINAPI IsPEFile(PVOID pFileData)
{
	PEInfo Info;
	if( !GetPEInfo(pFileData ,&Info) ) 
	{
		return false;
	}
	if (Info.pDosHeader->e_magic != IMAGE_DOS_SIGNATURE 
		|| Info.pNtHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		MessageBox(NULL,"It's not a standard PE file","Error",MB_OK);
		return false;
	}
	return true;
}
BOOL	WINAPI AddSection(IN OUT PVOID &pFileData,ULONG SectionSize , CHAR SectionName[8])
{
	
	__try
	{
		ULONG n,SectionNum,FileAlign,SectionAlign,VirtualSize,OldFileSize;
		PEInfo Info;
		PVOID	pNewFileData;
		IMAGE_SECTION_HEADER NewSection = {0};
		//��ȡPE��Ϣ
		PIMAGE_SECTION_HEADER pLastSection ;
		if(!GetPEInfo(pFileData,&Info)) 
		{
			return false;
		}
		OldFileSize = GetPEFileSize(pFileData);
		SectionNum = Info.pFileHeader->NumberOfSections;
		FileAlign = Info.pOptinalHeader->FileAlignment;
		SectionAlign = Info.pOptinalHeader->SectionAlignment;
		pLastSection = (PIMAGE_SECTION_HEADER)&(Info.pSectionHeader[SectionNum-1]);
		//2012��4��23�� ����Դ
		//���������PE�ļ�����������  
		//ԭ�����һ�����β��ö���
		//Ӧ���������һ�����ε�SizeOfRawDataʹ֮����
		pLastSection->SizeOfRawData = (pLastSection->SizeOfRawData % FileAlign ? FileAlign*(pLastSection->SizeOfRawData/FileAlign+1):pLastSection->SizeOfRawData);
		//��������������
		NewSection.Characteristics = IMAGE_SCN_MEM_EXECUTE|IMAGE_SCN_MEM_READ|IMAGE_SCN_MEM_WRITE;
		NewSection.SizeOfRawData = (SectionSize % FileAlign ? FileAlign*(SectionSize/FileAlign+1):SectionSize);
		NewSection.PointerToRawData = pLastSection->PointerToRawData + pLastSection->SizeOfRawData;
		NewSection.Misc.VirtualSize = SectionSize;
		if (pLastSection->SizeOfRawData % SectionAlign)
		{
			VirtualSize = SectionAlign * (pLastSection->SizeOfRawData/SectionAlign+1);
		} 
		else
		{
			VirtualSize = pLastSection->SizeOfRawData;
		}
		NewSection.VirtualAddress = pLastSection->VirtualAddress + VirtualSize;
		for (n = 0;n < 8; n++)
		{
			NewSection.Name[n] = SectionName[n];
		}
		//�����µ��ڴ��
		pNewFileData = malloc(NewSection.PointerToRawData + VirtualSize);
		memset(pNewFileData,0,NewSection.PointerToRawData + VirtualSize);
		memcpy(pNewFileData,pFileData,OldFileSize);
		free(pFileData);
		if(!GetPEInfo(pNewFileData,&Info)) 
		{
			return false;
		}
		//�޸�PE�ļ���Ϣ
		Info.pFileHeader->NumberOfSections ++;
		Info.pOptinalHeader->SizeOfImage = NewSection.VirtualAddress + NewSection.Misc.VirtualSize;
		memcpy(&(Info.pSectionHeader[SectionNum]),&NewSection,sizeof(IMAGE_SECTION_HEADER));
		pFileData = pNewFileData;
	}
	
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}
	
	return true;

}
ULONG	WINAPI GetPEFileSize(PVOID pFileData)
{
	PEInfo Info;
	if(!GetPEInfo(pFileData,&Info)) 
	{
		return false;
	}
	ULONG LastSectionRaw,LastSectionRSize,SectionNum;
	SectionNum = Info.pFileHeader->NumberOfSections;
	LastSectionRaw = Info.pSectionHeader[SectionNum-1].PointerToRawData;
	LastSectionRSize = Info.pSectionHeader[SectionNum-1].SizeOfRawData;
	return LastSectionRaw + LastSectionRSize;

}
BOOL	WINAPI SetTLS(PVOID pFileData,ULONG TlsDirectoryOfRVA)
{
	//
	/*
	typedef struct _IMAGE_TLS_DIRECTORY32 {
	DWORD   StartAddressOfRawData;		//������ʼ�ĵ�ַ ��Ϊ0
	DWORD   EndAddressOfRawData;		//�����ĵ�ַ ��Ϊ0
	DWORD   AddressOfIndex;             //TLS�����ĵ�ַ ָ��һ��������
	DWORD   AddressOfCallBacks;         // ָ��һ����NULL��β��IMAGE_TLS_CALLBACK�����ĵ�ַ����
	DWORD   SizeOfZeroFill;				//�����TLS��������Ĵ�С ��Ϊ0
	DWORD   Characteristics;			//Characteristics������������������TLS��־�� ��Ϊ0
	} IMAGE_TLS_DIRECTORY32;
	���϶�Ϊ��ַ,����ʹ�õ���VA��������RVA�ˡ����޸�ʱҪע�⡣
	*/
	PEInfo Info;
	if(!GetPEInfo(pFileData,&Info)) 
	{
		return false;
	}
	Info.pDataDriectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress = TlsDirectoryOfRVA;
	Info.pDataDriectory[IMAGE_DIRECTORY_ENTRY_TLS].Size = sizeof(IMAGE_TLS_DIRECTORY);
	return true;
}
BOOL	WINAPI SetImport(PVOID pFileData,bool  ClearOriginal,ULONG ImportDirectoryOfRVA)
{
	PEInfo Info;
	if(!GetPEInfo(pFileData,&Info)) 
	{
		return false;
	}
	if (ClearOriginal)
	{//��������
		PIMAGE_IMPORT_DESCRIPTOR pImportDirectory;
		pImportDirectory = (PIMAGE_IMPORT_DESCRIPTOR)Rva2MemAdr(pFileData,Info.pDataDriectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
		Info.pDataDriectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = 0;
		Info.pDataDriectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size = 0;
		while (pImportDirectory->Characteristics != 0)
		{
			PIMAGE_THUNK_DATA pThunkOrigin = (PIMAGE_THUNK_DATA)(Rva2MemAdr(pFileData,pImportDirectory->OriginalFirstThunk));
			PIMAGE_THUNK_DATA pThunkFirst = (PIMAGE_THUNK_DATA)(Rva2MemAdr(pFileData,pImportDirectory->FirstThunk));
			while (*(PULONG)pThunkOrigin != 0)
			{
				//���OriginalFirstThunk����
				if (IMAGE_SNAP_BY_ORDINAL32(pThunkOrigin->u1.Ordinal))//�ж��Ƿ�Ϊ��������
				{
					pThunkOrigin->u1.Ordinal = 0;
				}
				else
				{//��Ϊ�����������ʱ
					PIMAGE_IMPORT_BY_NAME pByName = (PIMAGE_IMPORT_BY_NAME)Rva2MemAdr(pFileData,(ULONG)pThunkOrigin->u1.AddressOfData);
					pByName->Hint = 0;
					PUCHAR	pName = pByName->Name;
					while(*pName)
					{
						*pName = 0;
						pName++;
					}//���������
					pThunkOrigin->u1.AddressOfData = 0;
				}
				//���FirstThunk����
				IMAGE_THUNK_DATA TmpThunk = {0};
				*pThunkFirst = TmpThunk;
				*pThunkOrigin = TmpThunk;
				pThunkFirst++;
				pThunkOrigin++; 
			}
			pImportDirectory->OriginalFirstThunk = 0;
			pImportDirectory->TimeDateStamp = 0;
			pImportDirectory->ForwarderChain = 0;
			PUCHAR	pName = (PUCHAR)(Rva2MemAdr(pFileData,pImportDirectory->Name));
			while(*pName)
			{
				*pName = 0;
				pName++;
			}//���DLL��
			pImportDirectory->Name = 0;
			pImportDirectory->FirstThunk = 0;
			pImportDirectory++;
		}
		
	}
	Info.pDataDriectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = ImportDirectoryOfRVA;
	Info.pDataDriectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size = sizeof(IMAGE_IMPORT_DESCRIPTOR);
	return true;
}
BOOL	WINAPI SetReLocation(PVOID pFileData,bool  ClearOriginal,ULONG ReLocationDirectoryOfRVA)
{
	PEInfo Info;
	if(!GetPEInfo(pFileData,&Info)) 
	{
		return false;
	}
	if(ClearOriginal)
	{//��DLL����ض�λ��󽫲��ܱ�����
		PIMAGE_BASE_RELOCATION pReLocation;
		PIMAGE_BASE_RELOCATION pOldReLocation;
		ULONG Size;
		pReLocation = (PIMAGE_BASE_RELOCATION)Rva2MemAdr(pFileData,Info.pDataDriectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
		Info.pDataDriectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress = 0;
		Info.pDataDriectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size = 0;
		while(pReLocation->VirtualAddress == 0)
		{
			
			pOldReLocation = pReLocation;
			Size = pReLocation->SizeOfBlock;
			//ָ����һ���ض�λ��
			pReLocation = (PIMAGE_BASE_RELOCATION)((ULONG)pReLocation + Size);
			memset(pOldReLocation,0,Size);
		}
	}
	Info.pDataDriectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress = ReLocationDirectoryOfRVA;
	Info.pDataDriectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size = sizeof(IMAGE_TLS_DIRECTORY);
	return true;
}
BOOL 	WINAPI SaveFile(PVOID pFileData,CHAR * szSaveFilePath)
{
	HANDLE hFile;
	ULONG WriteSize;
	hFile = CreateFile((LPCSTR)szSaveFilePath,
						GENERIC_WRITE,
						NULL,
						NULL,
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL);
	if ( INVALID_HANDLE_VALUE == hFile)
	{
		OutPutError("CreateFile");
		return false;
	}
	if( !WriteFile(hFile,pFileData,GetPEFileSize(pFileData),&WriteSize,NULL) ) 
	{
		CloseHandle(hFile);
		return false;
	}
	CloseHandle(hFile);
	return true;
}
void	WINAPI UnLoadFile(PVOID pFileData)
{
	if (!IsBadReadPtr(pFileData,4))
	{
		free(pFileData);
	}
	 
	
}