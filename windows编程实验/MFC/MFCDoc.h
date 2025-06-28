// MFCDoc.h : interface of the CMFCDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCDOC_H__002B8F0C_C789_4B71_90C6_743620643F3D__INCLUDED_)
#define AFX_MFCDOC_H__002B8F0C_C789_4B71_90C6_743620643F3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMFCDoc : public CDocument
{
protected: // create from serialization only
	CMFCDoc();
	DECLARE_DYNCREATE(CMFCDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFCDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMFCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMFCDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCDOC_H__002B8F0C_C789_4B71_90C6_743620643F3D__INCLUDED_)
