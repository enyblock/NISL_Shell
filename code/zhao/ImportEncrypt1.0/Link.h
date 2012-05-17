#pragma once 
#include <malloc.h>

typedef struct _List
{
	PVOID	pData;
	ULONG	Size;
	_List	*NextNode;
}List,*PList;

void WINAPI ListInit(List **pListHead);
PVOID WINAPI ListAddNode(List **pListHead,PVOID	pData,ULONG Size);
void WINAPI ListFree(List **pListHead);