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
	// 初始化成员变量
	m_bShowRect = FALSE;
	m_bShowText = FALSE;
	m_bShowHeart = FALSE;  // 初始化爱心标志
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
	
	// 绘制矩形
	if(m_bShowRect)
	{
		pDC->Rectangle(50, 500, 250, 150);  // 绘制固定位置的矩形
	}
	
	// 绘制文本
	if(m_bShowText)
	{
		pDC->TextOut(100, 100, _T("这是通过菜单绘制的文本"));  // 绘制固定文本
	}
	// 新增爱心绘制逻辑
	// 修改后的爱心绘制代码
	if (m_bShowHeart) {
		// 保存旧画刷和画笔
		CBrush* pOldBrush = pDC->GetCurrentBrush();
		CPen* pOldPen = pDC->GetCurrentPen();
    
		// 创建红色实心画刷和红色画笔
		CBrush redBrush(RGB(255, 0, 0));
		CPen redPen(PS_SOLID, 1, RGB(255, 0, 0));
		pDC->SelectObject(&redBrush);
		pDC->SelectObject(&redPen);

		// 定义爱心路径（使用两个贝塞尔曲线组成）
		CPoint points[7];
		int centerX = 200;  // 爱心中心X坐标
		int centerY = 150;  // 爱心中心Y坐标
		int size = 50;      // 爱心大小
    
		points[0] = CPoint(centerX, centerY - size/2);  // 顶点
		points[1] = CPoint(centerX - size, centerY - size);
		points[2] = CPoint(centerX - size, centerY);
		points[3] = CPoint(centerX, centerY + size);
		points[4] = CPoint(centerX + size, centerY);
		points[5] = CPoint(centerX + size, centerY - size);
		points[6] = CPoint(centerX, centerY - size/2);  // 回到顶点

		// 开始路径绘制
		pDC->BeginPath();
		pDC->MoveTo(points[0]);
		pDC->PolyBezier(points, 7);  // 绘制贝塞尔曲线
		pDC->EndPath();
    
		// 填充并描边路径
		pDC->StrokeAndFillPath();

		// 恢复旧画刷和画笔
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
	m_bShowRect = !m_bShowRect;  // 切换矩形显示状态
	Invalidate();  // 重绘窗口
}

void CMFCView::OnDrawText() 
{
	m_bShowText = !m_bShowText;  // 切换文本显示状态
	Invalidate();  // 重绘窗口
}

void CMFCView::OnMytoolbar() 
{
	m_bShowHeart = !m_bShowHeart;  // 切换爱心显示状态
    Invalidate();                  // 触发重绘
	
}
