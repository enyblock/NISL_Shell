#ifndef PE_PLUGIN
#define PE_PLUGIN
#include <Windows.h>
#include <malloc.h>

#define PLUGIN_CODEM 1
#define PLUGIN_CODEE 2
#define PLUGIN_TABLE 3

#define NAKED __declspec(naked)

struct PEInfo
{
	PIMAGE_DOS_HEADER  				pDosHeader;
	PIMAGE_NT_HEADERS				pNtHeader;
	PIMAGE_FILE_HEADER  			pFileHeader;
	PIMAGE_OPTIONAL_HEADER  		pOptinalHeader;
	PIMAGE_DATA_DIRECTORY			pDataDriectory;
	PIMAGE_SECTION_HEADER			pSectionHeader;
};

typedef ULONG (WINAPI *f_Rva2MemAdr)(PVOID pFileData,ULONG Rva);
typedef BOOL (WINAPI *f_Rva2Raw)(PVOID pFileData, ULONG Rva ,OUT ULONG &Raw);
typedef BOOL (WINAPI *f_Raw2Rva)(PVOID pFileData, ULONG Raw ,OUT ULONG &Rva);
typedef BOOL (WINAPI *f_LoadFile)(IN CHAR * szFilePath,OUT PVOID &pFileData);
typedef ULONG (WINAPI *f_GetPEFileSize)(PVOID pFileData);
typedef BOOL (WINAPI *f_IsPEFile)(PVOID pFileData);
typedef BOOL (WINAPI *f_GetPEInfo)(IN PVOID pFileData,OUT PEInfo * pPEInfo);
typedef BOOL (WINAPI *f_AddSection)(IN OUT PVOID &pFileData,ULONG SectionSize , CHAR SectionName[8]);
typedef BOOL (WINAPI *f_SetTLS)(IN PVOID pFileData,ULONG TlsDirectoryOfRVA);
typedef BOOL (WINAPI *f_SetImport)(PVOID pFileData,BOOL  ClearOriginal,ULONG ImportDirectoryOfRVA);
typedef BOOL (WINAPI *f_SetReLocation)(PVOID pFileData,BOOL  ClearOriginal,ULONG ReLocationDirectoryOfRVA);
typedef BOOL (WINAPI *f_SaveFile)(PVOID pFileData,CHAR * szSaveFilePath);
typedef VOID (WINAPI *f_UnLoadFile)(PVOID pFileData);

f_Rva2MemAdr Rva2MemAdr ;
f_Rva2Raw Rva2Raw ;
f_Raw2Rva Raw2Rva ;
f_LoadFile LoadFile ;
f_GetPEFileSize GetPEFileSize ;
f_IsPEFile IsPEFile ;
f_GetPEInfo GetPEInfo ;
f_AddSection AddSection ;
f_SetTLS SetTLS ;
f_SetImport SetImport ;
f_SetReLocation SetReLocation ;
f_SaveFile SaveFile ;
f_UnLoadFile UnLoadFile ;

ULONG FunAdr[13]={0};
CHAR FunName[13][20] = {
	"Rva2MemAdr",
		"Rva2Raw",
		"Raw2Rva",
		"LoadFile",
		"GetPEFileSize",
		"IsPEFile",
		"GetPEInfo",
		"AddSection",
		"SetTLS",
		"SetImport",
		"SetReLocation",
		"SaveFile",
		"UnLoadFile",
		
};
BOOL IsInit = FALSE;
BOOL	PEpluginInit()
{
	if (IsInit)
	{
		return TRUE;
	}
	//获取顶级目录
	CHAR Path[MAX_PATH]={0};
	if (!GetModuleFileNameA(NULL,Path,MAX_PATH))
	{
		MessageBoxA(0,"获取PEplugin路径失败","Error",MB_OK);
		return FALSE;
	}
	ULONG Len = lstrlenA(Path);
	for (ULONG i=Len-1;i>=0;i--)
	{
		if (Path[i] == '\\')
		{
			Path[i+1] = 0;
			break;
		}
	}
	lstrcatA(Path,"PEplugin.dll");
	//加载PEplugin
	HMODULE hmod = LoadLibraryA(Path);
	if (!hmod)
	{
		MessageBoxA(0,"加载PEplugin失败","Error",MB_OK);
		return FALSE;
	}
	for (i=0;i<13;i++)
	{
		FunAdr[i] = (ULONG)GetProcAddress(hmod,FunName[i]);
		if (FunAdr[i] == NULL)
		{
			MessageBoxA(0,"获取PEplugin中函数失败","Error",MB_OK);
			return FALSE;
		}
	}
	Rva2MemAdr = (f_Rva2MemAdr)FunAdr[0];
	Rva2Raw = (f_Rva2Raw)FunAdr[1];
	Raw2Rva = (f_Raw2Rva)FunAdr[2];
	LoadFile = (f_LoadFile)FunAdr[3];
	GetPEFileSize = (f_GetPEFileSize)FunAdr[4];
	IsPEFile = (f_IsPEFile)FunAdr[5];
	GetPEInfo = (f_GetPEInfo)FunAdr[6];
	AddSection = (f_AddSection)FunAdr[7];
	SetTLS = (f_SetTLS)FunAdr[8];
	SetImport = (f_SetImport)FunAdr[9];
	SetReLocation = (f_SetReLocation)FunAdr[10];
	SaveFile = (f_SaveFile)FunAdr[11];
	UnLoadFile = (f_UnLoadFile)FunAdr[12];

	IsInit = TRUE;
	return TRUE;

};




#endif