// DLLINFO.h: interface for the DLLINFO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLLINFO_H__D3F9D4F8_8A46_4501_AC15_0967BA298A3F__INCLUDED_)
#define AFX_DLLINFO_H__D3F9D4F8_8A46_4501_AC15_0967BA298A3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DLLINFO  
{
public:
	DLLINFO();
	int    GetDllName(void);
	virtual ~DLLINFO();

public:
	DLL_INFO m_df_dll_info;

};

#endif // !defined(AFX_DLLINFO_H__D3F9D4F8_8A46_4501_AC15_0967BA298A3F__INCLUDED_)
