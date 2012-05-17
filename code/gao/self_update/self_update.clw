; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSelf_updateDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "self_update.h"

ClassCount=3
Class1=CSelf_updateApp
Class2=CSelf_updateDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SELF_UPDATE_DIALOG

[CLS:CSelf_updateApp]
Type=0
HeaderFile=self_update.h
ImplementationFile=self_update.cpp
Filter=N

[CLS:CSelf_updateDlg]
Type=0
HeaderFile=self_updateDlg.h
ImplementationFile=self_updateDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CSelf_updateDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=self_updateDlg.h
ImplementationFile=self_updateDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SELF_UPDATE_DIALOG]
Type=1
Class=CSelf_updateDlg
ControlCount=8
Control1=IDC_BUTTON1,button,1342242816
Control2=IDC_PROGRESS1,msctls_progress32,1350565888
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,static,1342177294
Control7=IDC_BUTTON2,button,1342242816
Control8=IDC_STATIC,static,1342308352

