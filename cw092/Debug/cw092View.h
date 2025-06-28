// cw092View.h : interface of the CCw092View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CW092VIEW_H__9D8347A9_6EA0_4B74_B384_F1BDEAAEB9FE__INCLUDED_)
#define AFX_CW092VIEW_H__9D8347A9_6EA0_4B74_B384_F1BDEAAEB9FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCw092View : public CScrollView
{
protected: // create from serialization only
	CCw092View();
	DECLARE_DYNCREATE(CCw092View)

// Attributes
public:
	CCw092Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCw092View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCw092View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCw092View)
	afx_msg void Ongray();
	afx_msg void OnUpdategray(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLinetrans();
	afx_msg void OnUpdateLinetrans(CCmdUI* pCmdUI);
	afx_msg void OnHistogram();
	afx_msg void OnUpdateHistogram(CCmdUI* pCmdUI);
	afx_msg void OnEqualize();
	afx_msg void OnUpdateEqualize(CCmdUI* pCmdUI);
	afx_msg void OnFt();
	afx_msg void OnUpdateFt(CCmdUI* pCmdUI);
	afx_msg void OnIft();
	afx_msg void OnUpdateIft(CCmdUI* pCmdUI);
	afx_msg void OnFft();
	afx_msg void OnUpdateFft(CCmdUI* pCmdUI);
	afx_msg void OnIfft();
	afx_msg void OnUpdateIfft(CCmdUI* pCmdUI);
	afx_msg void OnRaplasSharp();
	afx_msg void OnUpdateRaplasSharp(CCmdUI* pCmdUI);
	afx_msg void OnAvgFliter();
	afx_msg void OnUpdateAvgFliter(CCmdUI* pCmdUI);
	afx_msg void OnMedFliter();
	afx_msg void OnUpdateMedFliter(CCmdUI* pCmdUI);
	afx_msg void OnGradSharp();
	afx_msg void OnUpdateGradSharp(CCmdUI* pCmdUI);
	afx_msg void OnFftFliter();
	afx_msg void OnUpdateFftFliter(CCmdUI* pCmdUI);
	afx_msg void OnButterworthHighPassFFTFilter();
	afx_msg void OnUpdateButterworthHighPassFFTFilter(CCmdUI* pCmdUI);
	afx_msg void OnIdealFFTFilter();
	afx_msg void OnUpdateIdealFFTFilter(CCmdUI* pCmdUI);
	afx_msg void OnIhFilter();
	afx_msg void OnUpdateIhFilter(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in cw092View.cpp
inline CCw092Doc* CCw092View::GetDocument()
   { return (CCw092Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CW092VIEW_H__9D8347A9_6EA0_4B74_B384_F1BDEAAEB9FE__INCLUDED_)
