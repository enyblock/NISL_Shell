// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__78F8BD38_FB26_4734_A87E_A42AC7DDF224__INCLUDED_)
#define AFX_STDAFX_H__78F8BD38_FB26_4734_A87E_A42AC7DDF224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


/*定义dll infor结构体*/
/*限定：
 *      1.模块数不超过40
 *      2.模块名字不超过20
 */
typedef struct _dll_info{
	
	char dll_name[40][20];
	int  num;
	
}DLL_INFO;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__78F8BD38_FB26_4734_A87E_A42AC7DDF224__INCLUDED_)
