// cw092Doc.h : interface of the CCw092Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CW092DOC_H__5B68A208_FC7D_4105_B292_B2418C11AE0A__INCLUDED_)
#define AFX_CW092DOC_H__5B68A208_FC7D_4105_B292_B2418C11AE0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCw092Doc : public CDocument
{
protected: // create from serialization only
	CCw092Doc();
	DECLARE_DYNCREATE(CCw092Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCw092Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCw092Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCw092Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CW092DOC_H__5B68A208_FC7D_4105_B292_B2418C11AE0A__INCLUDED_)
