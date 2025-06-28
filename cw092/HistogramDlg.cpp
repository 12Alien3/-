// HistogramDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cw092.h"
#include "HistogramDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistogramDlg dialog


CHistogramDlg::CHistogramDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHistogramDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistogramDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistogramDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialog)
	//{{AFX_MSG_MAP(CHistogramDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistogramDlg message handlers

extern DWORD HR[256]; // ��ɫͨ��ֱ��ͼ
extern DWORD HG[256]; // ��ɫͨ��ֱ��ͼ
extern DWORD HB[256]; // ��ɫͨ��ֱ��ͼ
void Histogram();
BOOL CHistogramDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // �����Ի����С
    CRect rect;
    GetClientRect(&rect);
    rect.bottom = 700; // �µĸ߶�
    MoveWindow(rect, TRUE);

    // ����ֱ��ͼ
    Histogram();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
void CHistogramDlg::OnPaint() 
{
    CPaintDC dc(this); // device context for painting

    // ���Ʊ߿�
    dc.Rectangle(20, 20, 277, 221);

    // ����ÿ��ͨ�������ֵ
    DWORD maxR = 0, maxG = 0, maxB = 0;
    for (int i = 0; i < 256; i++) {
        if (HR[i] > maxR) maxR = HR[i];
        if (HG[i] > maxG) maxG = HG[i];
        if (HB[i] > maxB) maxB = HB[i];
    }

    // ���ƺ�ɫͨ��ֱ��ͼ
    dc.SetTextColor(RGB(255, 0, 0)); // ����������ɫΪ��ɫ
    dc.DrawText(_T("Red Channel"), CRect(20, 20, 100, 40), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    for (int r = 0; r < 256; r++) {
        dc.MoveTo(r + 20, 220);
        dc.LineTo(r + 20, 220 - (int)(HR[r] * 200 / maxR));
    }
	dc.Rectangle(20, 220, 277, 441);
    // ������ɫͨ��ֱ��ͼ
    dc.SetTextColor(RGB(0, 255, 0)); // ����������ɫΪ��ɫ
    dc.DrawText(_T("Green Channel"), CRect(20, 240, 100, 260), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    for (int j = 0; j < 256; j++) {
        dc.MoveTo(j + 20, 440);
        dc.LineTo(j + 20, 440 - (int)(HG[j] * 200 / maxG));
    }
	dc.Rectangle(20, 440, 277, 661);
    // ������ɫͨ��ֱ��ͼ
    dc.SetTextColor(RGB(0, 0, 255)); // ����������ɫΪ��ɫ
    dc.DrawText(_T("Blue Channel"), CRect(20, 460, 100, 480), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    for (int m = 0; m < 256; m++) {
        dc.MoveTo(m + 20, 660);
        dc.LineTo(m + 20, 660 - (int)(HB[m] * 200 / maxB));
    }

    // ���Ʊ߿�
    
    
}