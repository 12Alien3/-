// cw092Doc.cpp : implementation of the CCw092Doc class
//

#include "stdafx.h"
#include "cw092.h"

#include "cw092Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCw092Doc

IMPLEMENT_DYNCREATE(CCw092Doc, CDocument)

BEGIN_MESSAGE_MAP(CCw092Doc, CDocument)
	//{{AFX_MSG_MAP(CCw092Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCw092Doc construction/destruction

CCw092Doc::CCw092Doc()
{
	// TODO: add one-time construction code here

}

CCw092Doc::~CCw092Doc()
{
}

BOOL CCw092Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCw092Doc serialization

void CCw092Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCw092Doc diagnostics

#ifdef _DEBUG
void CCw092Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCw092Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCw092Doc commands
BOOL LoadBmpFile(char*);

BOOL CCw092Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	LoadBmpFile((char*)lpszPathName);
	return TRUE;
}
