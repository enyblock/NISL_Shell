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
	{//��Ϊ������������ͷָ��
		*pListHead = (List *)malloc(sizeof(List));
		pLastNode = *pListHead;
	}
	else
	{//��Ϊ�գ����ҵ����һ���ڵ�
		pLastNode = *pListHead;
		while (pLastNode->NextNode != NULL)
		{
			pLastNode = pLastNode->NextNode;
		}
		pLastNode->NextNode = (List *)malloc(sizeof(List));
		pLastNode = pLastNode->NextNode;
	}//����ڵ��ڴ�
	pLastNode->NextNode = NULL;
	//���������ڴ�
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