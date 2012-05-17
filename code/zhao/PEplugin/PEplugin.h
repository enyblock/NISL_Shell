#ifndef PE_PLUGIN
#define PE_PLUGIN
#include <Windows.h>
#include <malloc.h>
#include <stdio.h>
struct PEInfo
{
	PIMAGE_DOS_HEADER  				pDosHeader;
	PIMAGE_NT_HEADERS				pNtHeader;
	PIMAGE_FILE_HEADER  			pFileHeader;
	PIMAGE_OPTIONAL_HEADER  		pOptinalHeader;
	PIMAGE_DATA_DIRECTORY			pDataDriectory;
	PIMAGE_SECTION_HEADER			pSectionHeader;
};														
ULONG	WINAPI Rva2MemAdr(PVOID pFileData,ULONG Rva);
BOOL	WINAPI Rva2Raw(PVOID pFileData, ULONG Rva, OUT ULONG &Raw);
BOOL	WINAPI Raw2Rva(PVOID pFileData, ULONG Raw, OUT ULONG &Rva);
void	WINAPI OutPutError(char * ErrorMsg);
BOOL 	WINAPI LoadFile(IN CHAR * szFilePath,OUT PVOID &pFileData);
BOOL	WINAPI GetPEInfo(IN PVOID pFileData,OUT PEInfo * pPEInfo);
BOOL 	WINAPI IsPEFile(PVOID pFileData);
BOOL	WINAPI AddSection(IN OUT PVOID &pFileData,ULONG SectionSize , CHAR SectionName[8]);
ULONG	WINAPI GetPEFileSize(PVOID pFileData);
BOOL	WINAPI SetTLS(PVOID pFileData,ULONG TlsDirectoryOfRVA);
BOOL	WINAPI SetImport(PVOID pFileData,bool  ClearOriginal,ULONG ImportDirectoryOfRVA);
BOOL	WINAPI SetReLocation(PVOID pFileData,bool  ClearOriginal,ULONG ReLocationDirectoryOfRVA);
BOOL 	WINAPI SaveFile(PVOID pFileData,CHAR * szSaveFilePath);
void	WINAPI UnLoadFile(PVOID pFileData);
#endif