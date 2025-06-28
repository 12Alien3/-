// cw092.h : main header file for the CW092 application
//

#if !defined(AFX_CW092_H__E74082BC_EC24_4CE0_B821_0F8AFD234697__INCLUDED_)
#define AFX_CW092_H__E74082BC_EC24_4CE0_B821_0F8AFD234697__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCw092App:
// See cw092.cpp for the implementation of this class
//

class CCw092App : public CWinApp
{
public:
	CCw092App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCw092App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCw092App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CW092_H__E74082BC_EC24_4CE0_B821_0F8AFD234697__INCLUDED_)
