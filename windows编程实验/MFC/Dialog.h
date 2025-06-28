#if !defined(AFX_DIALOG_H__C4DF6D50_7BF4_4AEC_AD8B_0BDCDA7E25EB__INCLUDED_)
#define AFX_DIALOG_H__C4DF6D50_7BF4_4AEC_AD8B_0BDCDA7E25EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Dialog dialog

class Dialog : public CDialog
{
// Construction
public:
	Dialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Dialog)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Dialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Dialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG_H__C4DF6D50_7BF4_4AEC_AD8B_0BDCDA7E25EB__INCLUDED_)
