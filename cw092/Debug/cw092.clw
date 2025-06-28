; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCw092View
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "cw092.h"
LastPage=0

ClassCount=6
Class1=CCw092App
Class2=CCw092Doc
Class3=CCw092View
Class4=CMainFrame

ResourceCount=3
Resource1=IDD_ABOUTBOX
Class5=CAboutDlg
Resource2=IDR_MAINFRAME
Class6=CHistogramDlg
Resource3=IDD_DIALOG1

[CLS:CCw092App]
Type=0
HeaderFile=cw092.h
ImplementationFile=cw092.cpp
Filter=N

[CLS:CCw092Doc]
Type=0
HeaderFile=cw092Doc.h
ImplementationFile=cw092Doc.cpp
Filter=N
LastObject=CCw092Doc
BaseClass=CDocument
VirtualFilter=DC

[CLS:CCw092View]
Type=0
HeaderFile=cw092View.h
ImplementationFile=cw092View.cpp
Filter=C
BaseClass=CScrollView
VirtualFilter=VWC
LastObject=ID_GRAD_SHARP


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_IH_FILTER




[CLS:CAboutDlg]
Type=0
HeaderFile=cw092.cpp
ImplementationFile=cw092.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_gray
Command10=ID_LINETRANS
Command11=ID_HISTOGRAM
Command12=ID_EQUALIZE
Command13=ID_FT
Command14=ID_IFT
Command15=ID_FFT
Command16=ID_IFFT
Command17=ID_AVG_FLITER
Command18=ID_MED_FLITER
Command19=ID_GRAD_SHARP
Command20=ID_RAPLAS_SHARP
Command21=ID_FFT_FLITER
Command22=ID_Butterworth_HighPass_FFT_Filter
Command23=ID_Ideal_FFT_Filter
Command24=ID_IH_FILTER
CommandCount=24

[DLG:IDD_DIALOG1]
Type=1
Class=CHistogramDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[CLS:CHistogramDlg]
Type=0
HeaderFile=HistogramDlg.h
ImplementationFile=HistogramDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CHistogramDlg

