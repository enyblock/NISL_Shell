; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAdapted_shellDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "adapted_shell.h"

ClassCount=5
Class1=CAdapted_shellApp
Class2=CAdapted_shellDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ADAPTED_SHELL_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CTabPage
Class5=CTabPage1
Resource4=IDD_DIALOG_PAG

[CLS:CAdapted_shellApp]
Type=0
HeaderFile=adapted_shell.h
ImplementationFile=adapted_shell.cpp
Filter=N
LastObject=IDC_BUTTON4
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CAdapted_shellDlg]
Type=0
HeaderFile=adapted_shellDlg.h
ImplementationFile=adapted_shellDlg.cpp
Filter=W
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_TAB1

[CLS:CAboutDlg]
Type=0
HeaderFile=adapted_shellDlg.h
ImplementationFile=adapted_shellDlg.cpp
Filter=D
LastObject=IDOK

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_ADAPTED_SHELL_DIALOG]
Type=1
Class=CAdapted_shellDlg
ControlCount=10
Control1=IDC_BUTTON1,button,1342242816
Control2=IDC_TAB1,SysTabControl32,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_LOAD_FILE,edit,1350633600
Control5=IDC_BUTTON2,button,1342242816
Control6=IDC_EDIT_OUT_FILE,edit,1350633600
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,button,1342177287
Control9=IDC_BUTTON4,button,1342242816
Control10=IDC_BUTTON5,button,1342242816

[CLS:CTabPage]
Type=0
HeaderFile=TabPage.h
ImplementationFile=TabPage.cpp
BaseClass=CDialog
Filter=D
LastObject=CTabPage

[DLG:IDD_DIALOG_PAG]
Type=1
Class=CTabPage1
ControlCount=4
Control1=IDC_STATIC,button,1342177287
Control2=IDC_BUTTON2,button,1342242816
Control3=IDC_BUTTON3,button,1342242816
Control4=IDC_LIST_DLL_BOX,listbox,1352728659

[CLS:CTabPage1]
Type=0
HeaderFile=TabPage1.h
ImplementationFile=TabPage1.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_LIST_DLL_BOX
VirtualFilter=dWC

