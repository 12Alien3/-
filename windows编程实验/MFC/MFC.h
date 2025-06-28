// MFC.h : main header file for the MFC application
//

#if !defined(AFX_MFC_H__745E017F_2BBA_4FE1_A704_03982E998F81__INCLUDED_)
#define AFX_MFC_H__745E017F_2BBA_4FE1_A704_03982E998F81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMFCApp:
// See MFC.cpp for the implementation of this class
//

class CMFCApp : public CWinApp
{
public:
	CMFCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMFCApp)
	afx_msg void OnAppAbout();
	afx_msg void OnTell();
	afx_msg void OnAdd();
	afx_msg void OnSubtract();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC_H__745E017F_2BBA_4FE1_A704_03982E998F81__INCLUDED_)
