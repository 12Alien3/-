// cw092View.cpp : implementation of the CCw092View class
//

#include "stdafx.h"
#include "cw092.h"

#include "cw092Doc.h"
#include "cw092View.h"

#include "HistogramDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCw092View

IMPLEMENT_DYNCREATE(CCw092View, CScrollView)

BEGIN_MESSAGE_MAP(CCw092View, CScrollView)
	//{{AFX_MSG_MAP(CCw092View)
	ON_COMMAND(ID_gray, Ongray)
	ON_UPDATE_COMMAND_UI(ID_gray, OnUpdategray)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_LINETRANS, OnLinetrans)
	ON_UPDATE_COMMAND_UI(ID_LINETRANS, OnUpdateLinetrans)
	ON_COMMAND(ID_HISTOGRAM, OnHistogram)
	ON_UPDATE_COMMAND_UI(ID_HISTOGRAM, OnUpdateHistogram)
	ON_COMMAND(ID_EQUALIZE, OnEqualize)
	ON_UPDATE_COMMAND_UI(ID_EQUALIZE, OnUpdateEqualize)
	ON_COMMAND(ID_FT, OnFt)
	ON_UPDATE_COMMAND_UI(ID_FT, OnUpdateFt)
	ON_COMMAND(ID_IFT, OnIft)
	ON_UPDATE_COMMAND_UI(ID_IFT, OnUpdateIft)
	ON_COMMAND(ID_FFT, OnFft)
	ON_UPDATE_COMMAND_UI(ID_FFT, OnUpdateFft)
	ON_COMMAND(ID_IFFT, OnIfft)
	ON_UPDATE_COMMAND_UI(ID_IFFT, OnUpdateIfft)
	ON_COMMAND(ID_RAPLAS_SHARP, OnRaplasSharp)
	ON_UPDATE_COMMAND_UI(ID_RAPLAS_SHARP, OnUpdateRaplasSharp)
	ON_COMMAND(ID_AVG_FLITER, OnAvgFliter)
	ON_UPDATE_COMMAND_UI(ID_AVG_FLITER, OnUpdateAvgFliter)
	ON_COMMAND(ID_MED_FLITER, OnMedFliter)
	ON_UPDATE_COMMAND_UI(ID_MED_FLITER, OnUpdateMedFliter)
	ON_COMMAND(ID_GRAD_SHARP, OnGradSharp)
	ON_UPDATE_COMMAND_UI(ID_GRAD_SHARP, OnUpdateGradSharp)
	ON_COMMAND(ID_FFT_FLITER, OnFftFliter)
	ON_UPDATE_COMMAND_UI(ID_FFT_FLITER, OnUpdateFftFliter)
	ON_COMMAND(ID_Butterworth_HighPass_FFT_Filter, OnButterworthHighPassFFTFilter)
	ON_UPDATE_COMMAND_UI(ID_Butterworth_HighPass_FFT_Filter, OnUpdateButterworthHighPassFFTFilter)
	ON_COMMAND(ID_Ideal_FFT_Filter, OnIdealFFTFilter)
	ON_UPDATE_COMMAND_UI(ID_Ideal_FFT_Filter, OnUpdateIdealFFTFilter)
	ON_COMMAND(ID_IH_FILTER, OnIhFilter)
	ON_UPDATE_COMMAND_UI(ID_IH_FILTER, OnUpdateIhFilter)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCw092View construction/destruction

CCw092View::CCw092View()
{
	// TODO: add construction code here

}

CCw092View::~CCw092View()
{
}

BOOL CCw092View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCw092View drawing
extern BITMAPINFO* lpBitsInfo;
extern BITMAPINFO* lpDIB_FT;
extern BITMAPINFO* lpDIB_IFT;
void CCw092View::OnDraw(CDC* pDC)
{
	CCw092Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if (NULL == lpBitsInfo)
		return;

	LPVOID lpBits =
		(LPVOID)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	StretchDIBits(
		pDC->GetSafeHdc(),
		0,0,lpBitsInfo->bmiHeader.biWidth, lpBitsInfo->bmiHeader.biHeight,
		0,0,lpBitsInfo->bmiHeader.biWidth, lpBitsInfo->bmiHeader.biHeight,
		lpBits,//bitmap bits
		lpBitsInfo,//bitmap data
		DIB_RGB_COLORS,
		SRCCOPY);

	if (lpDIB_FT)
	{
	lpBits = (LPVOID)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];

	StretchDIBits(
	pDC->GetSafeHdc(),
	600,0,lpDIB_FT->bmiHeader.biWidth,lpDIB_FT->bmiHeader.biHeight,
	0,0,lpDIB_FT->bmiHeader.biWidth,lpDIB_FT->bmiHeader.biHeight,
	lpBits,
	lpDIB_FT,
	DIB_RGB_COLORS,
	SRCCOPY);
	}
	if (lpDIB_IFT){
	lpBits = (LPVOID)&lpDIB_IFT->bmiColors[lpDIB_IFT->bmiHeader.biClrUsed];

	StretchDIBits(
	pDC->GetSafeHdc(),
	0,600,lpDIB_IFT->bmiHeader.biWidth,lpDIB_IFT->bmiHeader.biHeight,
	0,0,lpDIB_IFT->bmiHeader.biWidth,lpDIB_IFT->bmiHeader.biHeight,
	lpBits,
	lpDIB_IFT,
	DIB_RGB_COLORS,
	SRCCOPY);
	}
}

void CCw092View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 1200;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CCw092View printing

BOOL CCw092View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCw092View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCw092View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCw092View diagnostics

#ifdef _DEBUG
void CCw092View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CCw092View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CCw092Doc* CCw092View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCw092Doc)));
	return (CCw092Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCw092View message handlers
void Gray();
BOOL IsGray();
void CCw092View::Ongray() 
{
	// TODO: Add your command handler code here
	Gray();
	Invalidate();
}

void CCw092View::OnUpdategray(CCmdUI* pCmdUI)      //灰度化的按钮
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL && !IsGray());
}


void pixel(int i,int j,char* str);


void CCw092View::OnMouseMove(UINT nFlags, CPoint point) //显示xy坐标
{
	// TODO: Add your message handler code here and/or call default
	char xy[100];
	memset(xy,0,100);
	sprintf(xy,"x:%d y:%d    ",point.x,point.y);
	
	char rgb[100];
	memset(rgb,0,100);
	pixel(point.y,point.x,rgb);//i控制行，j控制列

	strcat(xy,rgb);

	((CFrameWnd*)GetParent())->SetMessageText(xy);//获取鼠标当前位置的xy坐标

	CScrollView::OnMouseMove(nFlags, point);
}



void LineTrans(float a,float b);
void CCw092View::OnLinetrans()        //对图像进行线性点运算
{
	// TODO: Add your command handler code here
	LineTrans(1.5,-50);               //Da=1.5 Db - 50
	Invalidate();
}
void CCw092View::OnUpdateLinetrans(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL );
}



void CCw092View::OnHistogram()        //画出直方图
{
	// TODO: Add your command handler code here
	CHistogramDlg dlg;
	dlg.DoModal();
	
}
void CCw092View::OnUpdateHistogram(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL);
}


void Equalize();
void CCw092View::OnEqualize()	    //	均衡化算法 
{
	// TODO: Add your command handler code here
	Equalize();
	Invalidate();
}

void CCw092View::OnUpdateEqualize(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL);
}


void Fourier();
void CCw092View::OnFt() //傅里叶变换
{
	// TODO: Add your command handler code here
	Fourier();
	Invalidate();
}

void CCw092View::OnUpdateFt(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}


void IFourier();
void CCw092View::OnIft()    //傅里叶反变换
{
	// TODO: Add your command handler code here
	IFourier();
	Invalidate();
}

BOOL is_gFD_OK();
void CCw092View::OnUpdateIft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL && is_gFD_OK());
}


void FFourier();
void CCw092View::OnFft()       //快速傅里叶变换
{
	// TODO: Add your command handler code here
	FFourier();
	Invalidate();
}

void CCw092View::OnUpdateFft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL && IsGray());
}


void IFFourier();
void CCw092View::OnIfft()     //快速傅里叶反变换
{
	// TODO: Add your command handler code here
	IFFourier();
	Invalidate();
}

void CCw092View::OnUpdateIfft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL && is_gFD_OK());
}



void AvgFliter();
void MedFliter();
void GradSharp();
void RaplasSharp();
void CCw092View::OnRaplasSharp() 
{
	// TODO: Add your command handler code here
	RaplasSharp();
	Invalidate();
}

void CCw092View::OnUpdateRaplasSharp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL && IsGray());
}

void CCw092View::OnAvgFliter() 
{
	// TODO: Add your command handler code here
	AvgFliter();
	Invalidate();
}

void CCw092View::OnUpdateAvgFliter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here

}

void CCw092View::OnMedFliter() 
{
	// TODO: Add your command handler code here
	MedFliter();
	Invalidate();
}

void CCw092View::OnUpdateMedFliter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here

}

void CCw092View::OnGradSharp() 
{
	// TODO: Add your command handler code here
	GradSharp();
	Invalidate();
}

void CCw092View::OnUpdateGradSharp(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

//巴特沃斯低通滤波器
void FFT_Fliter(int);        
void CCw092View::OnFftFliter() 
{
	// TODO: Add your command handler code here
	FFT_Fliter(10);
	Invalidate();
}

void CCw092View::OnUpdateFftFliter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL && is_gFD_OK());
}


//巴特沃斯高通
void Butterworth_HighPass_FFT_Filter(int);
void CCw092View::OnButterworthHighPassFFTFilter() 
{
	// TODO: Add your command handler code here
	Butterworth_HighPass_FFT_Filter(-10);
	Invalidate();
}

void CCw092View::OnUpdateButterworthHighPassFFTFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL && is_gFD_OK());
}

//理想低通滤波器
void Ideal_FFT_Filter(int);
void CCw092View::OnIdealFFTFilter() 
{
	// TODO: Add your command handler code here
	Ideal_FFT_Filter(10);
	Invalidate();
}

void CCw092View::OnUpdateIdealFFTFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL && is_gFD_OK());
}

//理想高通
void IH_Filter(int)；
void CCw092View::OnIhFilter() 
{
	// TODO: Add your command handler code here
	IH_Filter(int)；
	Invalidate();
}

void CCw092View::OnUpdateIhFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL && is_gFD_OK());
}
