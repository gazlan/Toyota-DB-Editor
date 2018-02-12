; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CToyotaDBDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "toyotadb.h"
LastPage=0

ClassCount=3
Class1=CToyotaDBApp
Class2=CToyotaDBDlg

ResourceCount=4
Resource1=IDD_TWINS
Class3=CTwins
Resource2=16
Resource3=IDR_POPUP_MENU
Resource4=IDD_TOYOTADB (English (U.S.))

[CLS:CToyotaDBApp]
Type=0
BaseClass=CWinApp
HeaderFile=ToyotaDB.h
ImplementationFile=ToyotaDB.cpp
LastObject=CToyotaDBApp

[CLS:CToyotaDBDlg]
Type=0
BaseClass=CDialog
HeaderFile=ToyotaDBDlg.h
ImplementationFile=ToyotaDBDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDT_BLOCK

[DLG:IDD_TOYOTADB_DIALOG]
Type=1
Class=CToyotaDBDlg

[DLG:IDD_TOYOTADB (English (U.S.))]
Type=1
Class=CToyotaDBDlg
ControlCount=26
Control1=IDC_REDT_TRANSLATION,RICHEDIT,1350631552
Control2=IDC_REDT_DST,RICHEDIT,1353777348
Control3=IDC_REDT_SRC,RICHEDIT,1353779396
Control4=IDC_BTN_FIRST,button,1342242816
Control5=IDC_BTN_UP,button,1342242816
Control6=IDC_BTN_PREV,button,1342242816
Control7=IDC_BTN_NEXT,button,1342242816
Control8=IDC_BTN_DN,button,1342242816
Control9=IDC_BTN_LAST,button,1342242816
Control10=IDC_EDT_GO,edit,1350631553
Control11=IDC_BTN_GO,button,1342242816
Control12=IDC_BTN_SEARCH,button,1342242816
Control13=IDC_BTN_TWINS,button,1342242816
Control14=IDC_BTN_COMPACT,button,1342242816
Control15=IDC_EDT_BLOCK,edit,1350633601
Control16=IDC_EDT_IDX,edit,1350633601
Control17=IDC_STATIC,static,1342308352
Control18=IDC_EDT_COMMENT,edit,1350631552
Control19=IDC_EDT_ORIGINAL,edit,1350631553
Control20=IDC_LIST_DIX,SysListView32,1350668701
Control21=IDC_CMB_TABLE,combobox,1344340226
Control22=IDC_CHK_TRANSLATED,button,1342242819
Control23=IDC_CHK_VERIFIED,button,1342242819
Control24=IDC_CHK_MARK,button,1342242819
Control25=IDC_BTN_UPDATE,button,1342242817
Control26=IDC_BTN_UPDATE_ALL,button,1342242816

[DLG:IDD_TWINS]
Type=1
Class=CTwins
ControlCount=19
Control1=IDC_LIST1,SysListView32,1350668685
Control2=IDC_LIST2,SysListView32,1350668685
Control3=IDC_EDT_FIND_L,edit,1350631552
Control4=IDC_BTN_PREV_L,button,1342242816
Control5=IDC_BTN_NEXT_L,button,1342242816
Control6=IDC_EDT_FIND_R,edit,1350631552
Control7=IDC_BTN_PREV_R,button,1342242816
Control8=IDC_BTN_NEXT_R,button,1342242816
Control9=IDC_BTN_SYNC,button,1342242817
Control10=IDC_BTN_UP,button,1342246784
Control11=IDC_BTN_DN,button,1342246784
Control12=IDC_BTN_FIRST,button,1342242816
Control13=IDC_BTN_UP2,button,1342242816
Control14=IDC_BTN_PREV2,button,1342242816
Control15=IDC_BTN_NEXT2,button,1342242816
Control16=IDC_BTN_DN2,button,1342242816
Control17=IDC_BTN_LAST2,button,1342242816
Control18=IDC_EDT_GO2,edit,1350631553
Control19=IDC_BTN_GO2,button,1342242816

[CLS:CTwins]
Type=0
HeaderFile=Twins.h
ImplementationFile=Twins.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_BTN_DN

[MNU:IDR_POPUP_MENU]
Type=1
Class=CToyotaDBDlg
Command1=ID_APP_ABOUT
Command2=ID_APP_OPTIONS
Command3=ID_APP_EXIT
CommandCount=3

[MNU:16]
Type=1
Class=?
Command1=ID_APP_ABOUT
Command2=ID_APP_OPTIONS
Command3=ID_APP_EXIT
CommandCount=3

