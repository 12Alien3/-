// MFCView.cpp : implementation of the CMFCView class
//

#include "stdafx.h"
#include "MFC.h"

#include "MFCDoc.h"
#include "MFCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCView

IMPLEMENT_DYNCREATE(CMFCView, CView)

BEGIN_MESSAGE_MAP(CMFCView, CView)
	//{{AFX_MSG_MAP(CMFCView)
	ON_COMMAND(ID_DRAW_RECT, OnDrawRect)
	ON_COMMAND(ID_DRAW_TEXT, OnDrawText)
	ON_COMMAND(IDR_MYTOOLBAR, OnMytoolbar)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCView construction/destruction

CMFCView::CMFCView()
{
	// ��ʼ����Ա����
	m_bShowRect = FALSE;
	m_bShowText = FALSE;
	m_bShowHeart = FALSE;  // ��ʼ�����ı�־
}

CMFCView::~CMFCView()
{
}

BOOL CMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMFCView drawing

void CMFCView::OnDraw(CDC* pDC)
{
	CMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// ���ƾ���
	if(m_bShowRect)
	{
		pDC->Rectangle(50, 500, 250, 150);  // ���ƹ̶�λ�õľ���
	}
	
	// �����ı�
	if(m_bShowText)
	{
		pDC->TextOut(100, 100, _T("����ͨ���˵����Ƶ��ı�"));  // ���ƹ̶��ı�
	}
	// �������Ļ����߼�
	// �޸ĺ�İ��Ļ��ƴ���
	if (m_bShowHeart) {
		// ����ɻ�ˢ�ͻ���
		CBrush* pOldBrush = pDC->GetCurrentBrush();
		CPen* pOldPen = pDC->GetCurrentPen();
    
		// ������ɫʵ�Ļ�ˢ�ͺ�ɫ����
		CBrush redBrush(RGB(255, 0, 0));
		CPen redPen(PS_SOLID, 1, RGB(255, 0, 0));
		pDC->SelectObject(&redBrush);
		pDC->SelectObject(&redPen);

		// ���尮��·����ʹ������������������ɣ�
		CPoint points[7];
		int centerX = 200;  // ��������X����
		int centerY = 150;  // ��������Y����
		int size = 50;      // ���Ĵ�С
    
		points[0] = CPoint(centerX, centerY - size/2);  // ����
		points[1] = CPoint(centerX - size, centerY - size);
		points[2] = CPoint(centerX - size, centerY);
		points[3] = CPoint(centerX, centerY + size);
		points[4] = CPoint(centerX + size, centerY);
		points[5] = CPoint(centerX + size, centerY - size);
		points[6] = CPoint(centerX, centerY - size/2);  // �ص�����

		// ��ʼ·������
		pDC->BeginPath();
		pDC->MoveTo(points[0]);
		pDC->PolyBezier(points, 7);  // ���Ʊ���������
		pDC->EndPath();
    
		// ��䲢���·��
		pDC->StrokeAndFillPath();

		// �ָ��ɻ�ˢ�ͻ���
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMFCView printing

BOOL CMFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CMFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CMFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CMFCView diagnostics

#ifdef _DEBUG
void CMFCView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCDoc* CMFCView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCDoc)));
	return (CMFCDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFCView message handlers

void CMFCView::OnDrawRect() 
{
	m_bShowRect = !m_bShowRect;  // �л�������ʾ״̬
	Invalidate();  // �ػ洰��
}

void CMFCView::OnDrawText() 
{
	m_bShowText = !m_bShowText;  // �л��ı���ʾ״̬
	Invalidate();  // �ػ洰��
}

void CMFCView::OnMytoolbar() 
{
	m_bShowHeart = !m_bShowHeart;  // �л�������ʾ״̬
    Invalidate();                  // �����ػ�
	
}
