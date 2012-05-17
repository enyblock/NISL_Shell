// Link.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "Link.h"


void WINAPI ListInit(List **pListHead)
{
	*pListHead = NULL;
}


PVOID WINAPI ListAddNode(List **pListHead,PVOID	pData,ULONG Size)
{
	List *pLastNode;PVOID pTmpData;
	if (*pListHead == NULL)
	{//若为空链表则设置头指针
		*pListHead = (List *)malloc(sizeof(List));
		pLastNode = *pListHead;
	}
	else
	{//不为空，则找到最后一个节点
		pLastNode = *pListHead;
		while (pLastNode->NextNode != NULL)
		{
			pLastNode = pLastNode->NextNode;
		}
		pLastNode->NextNode = (List *)malloc(sizeof(List));
		pLastNode = pLastNode->NextNode;
	}//申请节点内存
	pLastNode->NextNode = NULL;
	//申请数据内存
	pTmpData = malloc(Size);
	if (pData == NULL)
	{
		memset(pTmpData,0,Size);
	}
	else
	{
		memcpy(pTmpData,pData,Size);
	}
	
	pLastNode->pData = pTmpData;
	pLastNode->Size = Size;
	return pTmpData;

}
void WINAPI ListFree(List **pListHead)
{
	List *pNode,*ptmpNode;
	PVOID pTmpData;
	pNode = *pListHead;
	while (pNode != NULL)
	{
		ptmpNode = pNode->NextNode;
		pTmpData = pNode->pData;
		free(pNode);
		free(pTmpData);
		pNode = ptmpNode;
	}
	*pListHead = NULL;
}