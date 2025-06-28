// MFCView.h : interface of the CMFCView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCVIEW_H__16109B4A_4F98_48DE_9670_AC35A7A75801__INCLUDED_)
#define AFX_MFCVIEW_H__16109B4A_4F98_48DE_9670_AC35A7A75801__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMFCView : public CView
{
protected: // create from serialization only
	CMFCView();
	DECLARE_DYNCREATE(CMFCView)

// Attributes
public:
	CMFCDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFCView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMFCView)
	afx_msg void OnDrawRect();
	afx_msg void OnDrawText();
	afx_msg void OnMytoolbar();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    BOOL m_bShowRect;  // 控制是否显示矩形
    BOOL m_bShowText;  // 控制是否显示文本
	BOOL m_bShowHeart;  // 新增成员变量
};

#ifndef _DEBUG  // debug version in MFCView.cpp
inline CMFCDoc* CMFCView::GetDocument()
   { return (CMFCDoc*)m_pDocument; }

#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCVIEW_H__16109B4A_4F98_48DE_9670_AC35A7A75801__INCLUDED_)
