// PEplugin.cpp : 定义 DLL 应用程序的导出函数。
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
	// Rva2Raw:内存相对偏移转数据在文件中相对偏移
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
		// 如果该内存地址相对偏移落在节表第一个元素地址的相对偏移之前表示不是节表里的东西，地址不需要重定位
		Raw = Rva;
		return true;
	}
	for ( Index = 0; Index < pNtH->FileHeader.NumberOfSections; Index++ )
	{
		if ( Rva >= pSectH->VirtualAddress && Rva < pSectH->VirtualAddress + pSectH->Misc.VirtualSize )
		{// 地址落在该节表之内
			Raw = Rva - pSectH->VirtualAddress + pSectH->PointerToRawData;
			return true;
		}
		// 节表头文件相对偏移+该内存地址相对偏移-节表头内存相对偏移
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
		// 如果该文件地址相对偏移落在节表第一个元素地址的相对偏移之前表示不是节表里的东西，地址不需要重定位
		Rva = Raw;
		return true;
	}
	for ( Index = 0; Index < pNtH->FileHeader.NumberOfSections; Index++ )
	{
		if ( Raw >= pSectH->PointerToRawData && Raw < pSectH->PointerToRawData + pSectH->SizeOfRawData )
		{		// 地址落在该节表之内
			Rva = Raw + pSectH->VirtualAddress - pSectH->PointerToRawData;
			return true;
		}	// 节表头文件相对偏移-该内存地址相对偏移+节表头内存相对偏移
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
		//获取PE信息
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
		//2012年4月23日 赵培源
		//修正特殊的PE文件加区段问题  
		//原因：最后一个区段不用对齐
		//应先修正最后一个区段的SizeOfRawData使之对齐
		pLastSection->SizeOfRawData = (pLastSection->SizeOfRawData % FileAlign ? FileAlign*(pLastSection->SizeOfRawData/FileAlign+1):pLastSection->SizeOfRawData);
		//设置新区段属性
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
		//申请新的内存块
		pNewFileData = malloc(NewSection.PointerToRawData + VirtualSize);
		memset(pNewFileData,0,NewSection.PointerToRawData + VirtualSize);
		memcpy(pNewFileData,pFileData,OldFileSize);
		free(pFileData);
		if(!GetPEInfo(pNewFileData,&Info)) 
		{
			return false;
		}
		//修改PE文件信息
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
	DWORD   StartAddressOfRawData;		//数据起始的地址 可为0
	DWORD   EndAddressOfRawData;		//结束的地址 可为0
	DWORD   AddressOfIndex;             //TLS索引的地址 指向一整形数组
	DWORD   AddressOfCallBacks;         // 指向一组以NULL结尾的IMAGE_TLS_CALLBACK函数的地址数组
	DWORD   SizeOfZeroFill;				//明填充TLS变量区域的大小 可为0
	DWORD   Characteristics;			//Characteristics用作保留，可能用作TLS标志。 可为0
	} IMAGE_TLS_DIRECTORY32;
	以上都为地址,这里使用的是VA，而不是RVA了。当修改时要注意。
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
	{//清除输入表
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
				//清除OriginalFirstThunk数组
				if (IMAGE_SNAP_BY_ORDINAL32(pThunkOrigin->u1.Ordinal))//判断是否为序号输入表
				{
					pThunkOrigin->u1.Ordinal = 0;
				}
				else
				{//当为函数名输入表时
					PIMAGE_IMPORT_BY_NAME pByName = (PIMAGE_IMPORT_BY_NAME)Rva2MemAdr(pFileData,(ULONG)pThunkOrigin->u1.AddressOfData);
					pByName->Hint = 0;
					PUCHAR	pName = pByName->Name;
					while(*pName)
					{
						*pName = 0;
						pName++;
					}//清除函数名
					pThunkOrigin->u1.AddressOfData = 0;
				}
				//清除FirstThunk数组
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
			}//清除DLL名
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
	{//若DLL清除重定位表后将不能被加载
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
			//指向下一个重定位块
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