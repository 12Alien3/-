#include "stdafx.h"
#include <complex>
using namespace std;
#define PI 3.1415926535
BITMAPINFO* lpDIB_FT = NULL;
BITMAPINFO* lpDIB_IFT = NULL;
BITMAPINFO* lpBitsInfo = NULL;// ����ָ��BITMAPINFO�ṹ��ָ�룬���ڴ洢λͼ��Ϣ

complex<double>* gFD = NULL;

BOOL LoadBmpFile(char* BmpFileName)// ����BMP�ļ��ĺ���������BOOL���ͱ�ʾ�ɹ���ʧ��
{
	FILE* fp;
	if (NULL==(fp=fopen(BmpFileName,"rb")))//�Զ����ƶ�ģʽ��ָ����BMP�ļ�
		return false;

	BITMAPFILEHEADER bf;// λͼ�ļ�ͷ�ṹ�� 
	BITMAPINFOHEADER bi;// λͼ��Ϣͷ�ṹ��

	fread(&bf,14,1,fp);//�ļ�ͷ14�ֽ�
	fread(&bi,40,1,fp);//��Ϣͷ40�ֽ�

	//��ɫ��ռ�ô洢�ռ��С
	DWORD NumColors;
	if (bi.biClrUsed !=0)
		NumColors =bi.biClrUsed;// ���biClrUsed��Ϊ0����ֱ��ʹ��biClrUsed��ֵ
	else {
		switch(bi.biBitCount) // �������λ���ȷ����ɫ���С
		{
		case 1:
			NumColors = 2; // 1λɫ���ɫ���СΪ2
			break;
		case 4:
			NumColors = 16;
			break;
		case 8:
			NumColors = 256;
			break;
		case 24:
			NumColors = 0;//24λɫ��Ϊ��ʣ���ʹ�õ�ɫ��
			break;
		}
	}

	DWORD PalSize = NumColors * 4;// �����ɫ��ռ�õĴ洢�ռ��С��ÿ����ɫ4�ֽڣ�
	DWORD ImgSize = (bi.biWidth * bi.biBitCount + 31)/32 * 4 * bi.biHeight;// ����ͼ������ռ�õĴ洢�ռ��С�����ǵ��ж��루ÿ�����ݰ�32λ���룩
	DWORD Size = 40 +PalSize+ImgSize;//�ܵĴ�С = ��Ϣͷ+��ɫ��+ͼ������
		
	if(NULL==(lpBitsInfo=(BITMAPINFO*)malloc(Size)))// ΪBITMAPINFO�ṹ������㹻���ڴ�ռ�
		return FALSE;

	fseek(fp,14,SEEK_SET);
	fread((char*)lpBitsInfo,Size,1,fp);//��ȡ����λͼ��Ϣ��������Ϣͷ����ɫ���ͼ�����ݣ�

	lpBitsInfo->bmiHeader.biClrUsed=NumColors;// ����BITMAPINFO�ṹ���е�biClrUsed�ֶΣ�ȷ������ȷ��ӳ�˵�ɫ���е���ɫ����  

	return true;
}

void Gray()                  //ͼ��ҶȻ�
{
    // ��ȡͼ��Ŀ�Ⱥ͸߶�
    int w = lpBitsInfo->bmiHeader.biWidth;
    int h = lpBitsInfo->bmiHeader.biHeight;
    // ����ԭͼ��ÿһ�е��ֽ�����ȷ���ֽ�����4�ı���
    int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
    BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

    // ����Ҷ�ͼ��ÿһ�е��ֽ������Ҷ�ͼ����8λ��
    int LineBytes_gray = (w * 8 + 31) / 32 * 4;
    // ��̬�����µ� BITMAPINFO �ṹ�壬���ڴ洢�Ҷ�ͼ�����Ϣ
    BITMAPINFO* lpBitsInfo_gray = (BITMAPINFO*)malloc(40 + 256 * sizeof(RGBQUAD) + LineBytes_gray * h);
    if (!lpBitsInfo_gray) return; // ����ڴ�����Ƿ�ɹ�

    // ����ԭͼ���ͷ����Ϣ����ͼ��
    memcpy(lpBitsInfo_gray, lpBitsInfo, 40);
    lpBitsInfo_gray->bmiHeader.biBitCount = 8; // ������ͼ���λ���Ϊ8λ
    lpBitsInfo_gray->bmiHeader.biClrUsed = 256; // ������ɫ��Ĵ�СΪ256
    lpBitsInfo_gray->bmiHeader.biSizeImage = LineBytes_gray * h; // ����ͼ�����ݵĴ�С

    // ��ʼ���Ҷ�ͼ�����ɫ��ÿ���Ҷ�ֵ��Ӧһ��RGBֵ
    for (int i = 0; i < 256; i++) {
        lpBitsInfo_gray->bmiColors[i].rgbRed = i;
        lpBitsInfo_gray->bmiColors[i].rgbGreen = i;
        lpBitsInfo_gray->bmiColors[i].rgbBlue = i;
        lpBitsInfo_gray->bmiColors[i].rgbReserved = 0;
    }

    // ��ȡ�Ҷ�ͼ�������ָ��
    BYTE* lpBits_gray = (BYTE*)&lpBitsInfo_gray->bmiColors[256];

    // ���ݲ�ͬ��λ��Ƚ��лҶȻ�����
    switch (lpBitsInfo->bmiHeader.biBitCount) {
        case 1: {// 2ֵͼ��
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    // ���㵱ǰ�������ֽ��е�λ��
                    int byteIndex = j / 8;
                    int bitIndex = 7 - (j % 8);
                    // ��ȡ��ǰ���ص���ɫֵ
                    BYTE byte = lpBits[LineBytes * (h - 1 - i) + byteIndex];
                    int color = (byte >> bitIndex) & 1;
                    // ����ɫֵת��Ϊ�Ҷ�ֵ
                    BYTE grayValue = (color == 0) ? 0 : 255;
                    // ���Ҷ�ֵд��Ŀ��ͼ��
                    lpBits_gray[LineBytes_gray * (h - 1 - i) + j] = grayValue;
                }
            }
            break;
        }
        case 4: {// 16ɫͼ��
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    // ���㵱ǰ�������ֽ��е�λ��
                    int byteIndex = (j / 2);
                    int bitShift = (j % 2) ? 0 : 4;
                    // ��ȡ��ǰ���ص���ɫ����
                    int index = ((lpBits + LineBytes * (h - 1 - i))[byteIndex] >> bitShift) & 0x0F;
                    // ����ɫ���л�ȡ��ɫֵ
                    BYTE _B = lpBitsInfo->bmiColors[index].rgbBlue;
                    BYTE _G = lpBitsInfo->bmiColors[index].rgbGreen;
                    BYTE _R = lpBitsInfo->bmiColors[index].rgbRed;
                    // ����Ҷ�ֵ
                    BYTE avg = (_R + _G + _B) / 3;
                    // ���Ҷ�ֵд��Ŀ��ͼ��
                    lpBits_gray[LineBytes_gray * (h - 1 - i) + j] = avg;
                }
            }
            break;
        }
        case 8: {// 256ɫͼ��
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    // ��ȡ��ǰ���ص���ɫ����
                    int index = lpBits[LineBytes * (h - 1 - i) + j];
                    // ����ɫ���л�ȡ��ɫֵ
                    BYTE _B = lpBitsInfo->bmiColors[index].rgbBlue;
                    BYTE _G = lpBitsInfo->bmiColors[index].rgbGreen;
                    BYTE _R = lpBitsInfo->bmiColors[index].rgbRed;
                    // ����Ҷ�ֵ
                    BYTE avg = (_R + _G + _B) / 3;
                    // ���Ҷ�ֵд��Ŀ��ͼ��
                    lpBits_gray[LineBytes_gray * (h - 1 - i) + j] = avg;
                }
            }
            break;
        }
        case 24: {// 24λ���ɫͼ��
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    // ��ȡ��ǰ���صĺ졢�̡���ͨ��ֵ
                    BYTE* B = lpBits + LineBytes * (h - 1 - i) + j * 3;
                    BYTE* G = B + 1;
                    BYTE* R = G + 1;
                    // ����Ҷ�ֵ
                    BYTE avg = (*R + *G + *B) / 3;
                    // ���Ҷ�ֵд��Ŀ��ͼ��
                    lpBits_gray[LineBytes_gray * (h - 1 - i) + j] = avg;
                }
            }
            break;
        }
        default:
            return; 
    }

    // �ͷžɵ� BITMAPINFO ���滻Ϊ�µ�
    if (lpBitsInfo != lpBitsInfo_gray) {
        free(lpBitsInfo);
        lpBitsInfo = lpBitsInfo_gray;
    }
}


BOOL IsGray()//�ж���256ɫ���ǻҶ�ͼ��
{
	int r,g,b;
	if(8==lpBitsInfo->bmiHeader.biBitCount)
	{
		r=lpBitsInfo->bmiColors[150].rgbRed;
		g=lpBitsInfo->bmiColors[150].rgbGreen;
		b=lpBitsInfo->bmiColors[150].rgbBlue;

		if(r==b&&r==g)//����һ��rgb��ȣ���Ϊ�Ҷ�ͼ��
			return TRUE;

	}
	return FALSE;

}



void pixel(int i,int j,char* str)          //�������������ֵ
{
	if(NULL==lpBitsInfo)//����ͼ��Χ�ڣ�return
		return;

	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	if(i>=h || j>=w)//i������,������Χ��return
		return;
	
	BYTE* pixel,bv;
	int r,g,b;

	switch(lpBitsInfo->bmiHeader.biBitCount){
	case 8://�Ҷ�
		pixel=lpBits + LineBytes *(h-1-i)+j;
		if (IsGray()){
			sprintf(str,"�Ҷ�ֵ��%d",*pixel);
		}
		else{
			r=lpBitsInfo->bmiColors[*pixel].rgbRed;
			g=lpBitsInfo->bmiColors[*pixel].rgbGreen;
			b=lpBitsInfo->bmiColors[*pixel].rgbBlue;
			sprintf(str,"RGB(%d,%d,%d)",r,g,b);//256ɫ�����RGB
		}
		break;
	case 24://24λ
		pixel = lpBits + LineBytes * (h - 1 - i) + j * 3;
        r = pixel[2]; // BGR˳��洢
        g = pixel[1];
        b = pixel[0];
        sprintf(str, "RGB(%d,%d,%d)", r, g, b); // ֱ�����RGB
        break;
	case 4://16ɫ
        pixel = lpBits + LineBytes * (h - 1 - i) + j / 2;
        if (j % 2 == 0) { // �����ż������ȡ��4λ
            int value = (*pixel & 0xF0) >> 4;
            r = lpBitsInfo->bmiColors[value].rgbRed;
            g = lpBitsInfo->bmiColors[value].rgbGreen;
            b = lpBitsInfo->bmiColors[value].rgbBlue;
            sprintf(str, "RGB(%d,%d,%d)", r, g, b);
        } else { // ����ȡ��4λ
            int value = (*pixel & 0x0F);
            r = lpBitsInfo->bmiColors[value].rgbRed;
            g = lpBitsInfo->bmiColors[value].rgbGreen;
            b = lpBitsInfo->bmiColors[value].rgbBlue;
            sprintf(str, "RGB(%d,%d,%d)", r, g, b);
        }
		break;
	case 1://2ֵͼ��
		bv=*(lpBits + LineBytes *(h-1-i)+j/8)&(1<<j%8);
		if(0==bv)
			strcpy(str,"������");
		else
			strcpy(str,"ǰ����");
		break;
	}
}



DWORD HR[256]; // ��ɫͨ��ֱ��ͼ
DWORD HG[256]; // ��ɫͨ��ֱ��ͼ
DWORD HB[256]; // ��ɫͨ��ֱ��ͼ
void Histogram()                         // ����24λ���ֱ��ͼ������ǻҶ�ͼ������ͬ
{
    // ��ȡͼ��Ŀ�Ⱥ͸߶�
    int w = lpBitsInfo->bmiHeader.biWidth;
    int h = lpBitsInfo->bmiHeader.biHeight;
    // ����ԭͼ��ÿһ�е��ֽ�����ȷ���ֽ�����4�ı���
    int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
    BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

    int i, j;
    BYTE* pixel;

    // ��ʼ��ֱ��ͼ����
    for(i = 0; i < 256; i++) {
        HR[i] = 0;
        HG[i] = 0;
        HB[i] = 0;
    }

    // ����ͼ���ÿһ������
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            pixel = lpBits + LineBytes * (h - 1 - i) + j * 3; // 24λͼ��ÿ����3�ֽ�
            HR[pixel[2]]++; // ��ɫͨ��
            HG[pixel[1]]++; // ��ɫͨ��
            HB[pixel[0]]++; // ��ɫͨ��
        }
    }
}



void LineTrans(float a, float b)             //��24λ��ʺͻҶ�ͼ��������Ե�����
{
    // ��ȡͼ��Ŀ�Ⱥ͸߶�
    int w = lpBitsInfo->bmiHeader.biWidth;
    int h = lpBitsInfo->bmiHeader.biHeight;
    // ����ԭͼ��ÿһ�е��ֽ�����ȷ���ֽ�����4�ı���
    int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;

    BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

    int i, j;
    BYTE* pixel;
    float temp;

    // ���ݲ�ͬ��λ��Ƚ��д���
    switch (lpBitsInfo->bmiHeader.biBitCount) {
        case 24: { // 24λ���ɫͼ��
            for (i = 0; i < h; i++) {
                for (j = 0; j < w; j++) {
                    // ÿ������ռ3���ֽڣ�R, G, B���������������3
                    pixel = lpBits + LineBytes * (h - 1 - i) + j * 3;

                    // ��ÿ����ɫͨ��Ӧ�����Ա任
                    for (int c = 0; c < 3; c++) { // R, G, B
                        temp = a * (float)pixel[c] + b;

                        if (temp > 255)
                            pixel[c] = 255;
                        else if (temp < 0)
                            pixel[c] = 0;
                        else
                            pixel[c] = (BYTE)(temp + 0.5);
                    }
                }
            }
            break;
        }
        case 8: { // 8λ�Ҷ�ͼ��,256ɫ��δ����
			if(IsGray())
			{
				for (i = 0; i < h; i++) {
					for (j = 0; j < w; j++) {
						pixel = lpBits + LineBytes * (h - 1 - i) + j;
						temp = a * (*pixel) + b;

						if (temp > 255)
							*pixel = 255;
						else if (temp < 0)
							*pixel = 0;
						else
							*pixel = (BYTE)(temp + 0.5);
					}
				}
			}
            break;
        }
        default:
            // ����λ��ȵ�ͼ���������Ҫ��
            break;
    }
}


void Equalize()            // ���24λ���ͼ��ͻҶ�ͼ��ľ��⻯
{
    // ��ȡͼ��Ŀ�Ⱥ͸߶�
    int w = lpBitsInfo->bmiHeader.biWidth;
    int h = lpBitsInfo->bmiHeader.biHeight;
    // ����ԭͼ��ÿһ�е��ֽ�����ȷ���ֽ�����4�ı���
    int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
    BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

    if (IsGray())
    {
        // ��ʼ���Ҷ�ֱ��ͼ����
        DWORD H[256] = {0};

        // ����Ҷ�ֱ��ͼ
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                BYTE* pixel = lpBits + LineBytes * i + j; // �Ҷ�ͼ��ÿ����1�ֽ�
                H[*pixel]++;
            }
        }

        // ����ӳ���
        BYTE Map[256];
        DWORD temp = 0;

        for (i = 0; i < 256; i++) {
            temp += H[i];
            Map[i] = (BYTE)(255 * temp / (w * h));
        }

        // Ӧ��ӳ���
        for (i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                BYTE* pixel = lpBits + LineBytes * i + j; // �Ҷ�ͼ��ÿ����1�ֽ�
                *pixel = Map[*pixel];
            }
        }
    }
    else
    {
        // ��ʼ��ֱ��ͼ����
        DWORD HR[256] = {0}; // ��ɫͨ��ֱ��ͼ
        DWORD HG[256] = {0}; // ��ɫͨ��ֱ��ͼ
        DWORD HB[256] = {0}; // ��ɫͨ��ֱ��ͼ

        // ����ÿ��ͨ����ֱ��ͼ
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                BYTE* pixel = lpBits + LineBytes * (h - 1 - i) + j * 3; // 24λͼ��ÿ����3�ֽ�
                HR[pixel[2]]++; // ��ɫͨ��
                HG[pixel[1]]++; // ��ɫͨ��
                HB[pixel[0]]++; // ��ɫͨ��
            }
        }

        // ����ӳ���
        BYTE MapR[256], MapG[256], MapB[256];
        DWORD tempR = 0, tempG = 0, tempB = 0;

        for (i = 0; i < 256; i++) {
            tempR += HR[i];
            tempG += HG[i];
            tempB += HB[i];

            MapR[i] = (BYTE)(255 * tempR / (w * h));
            MapG[i] = (BYTE)(255 * tempG / (w * h));
            MapB[i] = (BYTE)(255 * tempB / (w * h));
        }

        // Ӧ��ӳ���
        for (i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                BYTE* pixel = lpBits + LineBytes * (h - 1 - i) + j * 3; // 24λͼ��ÿ����3�ֽ�
                pixel[2] = MapR[pixel[2]]; // ��ɫͨ��
                pixel[1] = MapG[pixel[1]]; // ��ɫͨ��
                pixel[0] = MapB[pixel[0]]; // ��ɫͨ��
            }
        }
    }
}


void FT(complex<double>* TD,complex<double>* FD,int m){
	int x,u;
	double angle;
	for (u = 0;u < m;u ++){
		FD[u] = 0;
		for (x = 0; x < m; x++){
			angle = -2 * PI * u * x / m; // TODO
			FD[u] += TD[x] * complex<double>(cos(angle),sin(angle));
		}
		FD[u] /= m;
	}
}


void IFT(complex<double>* FD,complex<double>* TD,int m){
	int x,u;
	double angle;
	for (x = 0;x < m;x ++){
		TD[x] = 0;
		for (u = 0; u < m; u++){
			angle = 2 * PI * u * x / m;
			TD[x] += FD[u] * complex<double>(cos(angle),sin(angle));
		}
	}
}


void Fourier(){
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	complex<double>* TD = new complex<double>[w * h];
	complex<double>* FD = new complex<double>[w * h];
	
	// ���TD
	int i,j;
	BYTE* pixel;
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			pixel = lpBits + LineBytes * (h -1 -i) + j;
			TD[i * w + j] = complex<double>(*pixel * pow(-1, i + j),0);
		}
	}
	
	// 
	for (i = 0; i < h; i ++)
		FT(&TD[w * i], &FD[w * i], w);
	// ת��
	for(i=0;i<h;i++)
		for(j=0;j<w;j++)
			TD[h * j + i] = FD[i * w + j];

	for (i = 0; i < w; i ++)
		FT(&TD[h * i], &FD[h * i], h);

	DWORD Size = 40 + 1024 + LineBytes * h;

	lpDIB_FT = (BITMAPINFO*)malloc(Size);
	memcpy(lpDIB_FT,lpBitsInfo,Size);

	lpBits = (BYTE*)&lpDIB_FT->bmiColors[256];

	double temp;
	for (i=0;i<h;i++)
	{
		for (j = 0;j < w; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			temp = sqrt(FD[j * h + i].real() * FD[j * h + i].real() + FD[j * h + i].imag() * FD[j * h + i].imag()) * 2000;
			if (temp > 255)
				temp = 255;
			*pixel = (BYTE)(temp);

		}
	}
	delete TD;
//	delete FD;
	gFD = FD;
}


// TODO
void IFourier(){
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	complex<double>* TD = new complex<double>[w * h];

	int i,j;
	
	for (i = 0; i < w; i ++)
		IFT(&gFD[h * i], &TD[h * i], h);

	for(i=0;i<h;i++)
		for(j=0;j<w;j++)
			gFD[h * j + i] = TD[i * w + j];

	for (i = 0; i < h; i ++)
		IFT(&gFD[h * i], &TD[h * i], w);

	DWORD Size = 40 + 1024 + LineBytes * h;

	lpDIB_IFT = (BITMAPINFO*)malloc(Size);
	memcpy(lpDIB_IFT,lpBitsInfo,Size);

	lpBits = (BYTE*)&lpDIB_IFT->bmiColors[256];

	BYTE* pixel;
	for (i=0;i<h;i++)
	{
		for (j = 0;j < w; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			*pixel = (BYTE)(TD[w * i + j].real() /  pow(-1, i + j));
		}
	}
	delete TD;
	delete gFD;
	gFD = NULL;
}

BOOL is_gFD_OK(){
	return (gFD != NULL);
}

void FFT(complex<double> * TD, complex<double> * FD, int r)            //���ٸ���Ҷ�任
{
	// ���㸶��Ҷ�任����
	LONG count = 1 << r;
	// �����Ȩϵ��
	int i;
	double angle;
	complex<double>* W = new complex<double>[count / 2];
	for(i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2 / count;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	// ��ʱ���д��X1
	complex<double>* X1 = new complex<double>[count];
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// ���õ����㷨���п��ٸ���Ҷ�任�����ΪƵ��ֵX2
	complex<double>* X2 = new complex<double>[count]; 

	int k,j,p,size;
	complex<double>* temp;
	for (k = 0; k < r; k++)
	{
		for (j = 0; j < 1 << k; j++)
		{
			size = 1 << (r-k);
			for (i = 0; i < size/2; i++)
			{
				p = j * size;
				X2[i + p] = X1[i + p] + X1[i + p + size/2];
				X2[i + p + size/2] = (X1[i + p] - X1[i + p + size/2]) * W[i * (1<<k)];
			}
		}
		temp  = X1;
		X1 = X2;
		X2 = temp;
	}
	
	// ����������λ�������У�
	for (j = 0; j < count; j++)
	{
		p = 0;
		for (i = 0; i < r; i++)
		{
			if (j & (1<<i))
			{
				p += 1<<(r-i-1);
			}
		}
		FD[j]=X1[p];
		FD[j] /= count;
	}
	
	// �ͷ��ڴ�
	delete W;
	delete X1;
	delete X2;
}

void FFourier()
{
	//ͼ��Ŀ�Ⱥ͸߶�
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	//ָ��ͼ������ָ��
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT��ȣ�����Ϊ2�������η���
	int FFT_w = 1;
	// FFT��ȵ�����������������
	int wp = 0;
	while(FFT_w * 2 <= width)
	{
		FFT_w *= 2;
		wp ++;
	}

	// FFT�߶ȣ�����Ϊ2�������η���
	int FFT_h = 1;
	// FFT�߶ȵ�����������������
	int hp = 0;
	while(FFT_h * 2 <= height)
	{
		FFT_h *= 2;
		hp ++;
	}

	// �����ڴ�
	complex<double>* TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* FD = new complex<double>[FFT_w * FFT_h];
	
	int i, j;
	BYTE* pixel;
	
	for(i = 0; i < FFT_h; i++)  // ��
	{
		for(j = 0; j < FFT_w; j++)  // ��
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			pixel = lpBits + LineBytes * (height - 1 - i) + j;

			// ��ʱ��ֵ
			TD[j + FFT_w * i] = complex<double>(*pixel* pow(-1,i+j), 0);
		}
	}
	
	for(i = 0; i < FFT_h; i++)
	{
		// ��y������п��ٸ���Ҷ�任
		FFT(&TD[FFT_w * i], &FD[FFT_w * i], wp);
	}
	
	// �����м�任���
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			TD[i + FFT_h * j] = FD[j + FFT_w * i];
		}
	}
	
	for(i = 0; i < FFT_w; i++)
	{
		// ��x������п��ٸ���Ҷ�任
		FFT(&TD[i * FFT_h], &FD[i * FFT_h], hp);
	}

	//����Ƶ��ͼ��
	//ΪƵ��ͼ������ڴ�
	LONG size = 40 + 1024 + LineBytes * height;
	lpDIB_FT = (LPBITMAPINFO) malloc(size);
	if (NULL == lpDIB_FT)
		return;
	memcpy(lpDIB_FT, lpBitsInfo, size);

	//ָ��Ƶ��ͼ������ָ��
	lpBits = (BYTE*)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];

	double temp;
	for(i = 0; i < FFT_h; i++) // ��
	{
		for(j = 0; j < FFT_w; j++) // ��
		{
			// ����Ƶ�׷���
			temp = sqrt(FD[j * FFT_h + i].real() * FD[j * FFT_h + i].real() + 
				        FD[j * FFT_h + i].imag() * FD[j * FFT_h + i].imag()) *2000;
			
			// �ж��Ƿ񳬹�255
			if (temp > 255)
			{
				// ���ڳ����ģ�ֱ������Ϊ255
				temp = 255;
			}
			
			pixel = lpBits + LineBytes * (height - 1 - i) + j;

			// ����Դͼ��
			*pixel = (BYTE)(temp);
		}
	}

	delete TD;
	gFD = FD;
}


//IFFT���任
void IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	// ����Ҷ�任����
	LONG	count;
	// ���㸶��Ҷ�任����
	count = 1 << r;

	// ������������洢��
	complex<double> * X = new complex<double>[count];
	// ��Ƶ���д��X
	memcpy(X, FD, sizeof(complex<double>) * count);
	
	// ����
	for(int i = 0; i < count; i++)
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	
	// ���ÿ��ٸ���Ҷ�任
	FFT(X, TD, r);
	
	// ��ʱ���Ĺ���
	for(i = 0; i < count; i++)
		TD[i] = complex<double> (TD[i].real() * count, -TD[i].imag() * count);
	
	// �ͷ��ڴ�
	delete X;
}

void IFFourier()
{
	//ͼ��Ŀ�Ⱥ͸߶�
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;

	// FFT��ȣ�����Ϊ2�������η���
	int FFT_w = 1;
	// FFT��ȵ�����������������
	int wp = 0;
	while(FFT_w * 2 <= width)
	{
		FFT_w *= 2;
		wp ++;
	}

	// FFT�߶ȣ�����Ϊ2�������η���
	int FFT_h = 1;
	// FFT�߶ȵ�����������������
	int hp = 0;
	while(FFT_h * 2 <= height)
	{
		FFT_h *= 2;
		hp ++;
	}

	// �����ڴ�
	complex<double>* TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* FD = new complex<double>[FFT_w * FFT_h];
	
	int i, j;
	for(i = 0; i < FFT_h; i++)  // ��
		for(j = 0; j < FFT_w; j++)  // ��
			FD[j + FFT_w * i] = gFD[i + FFT_h*j];
	
	// ��ˮƽ������п��ٸ���Ҷ�任
	for(i = 0; i < FFT_h; i++)
		IFFT(&FD[FFT_w * i], &TD[FFT_w * i], wp);
	
	// �����м�任���
	for(i = 0; i < FFT_h; i++)
		for(j = 0; j < FFT_w; j++)
			FD[i + FFT_h * j] = TD[j + FFT_w * i];
	
	// �ش�ֱ������п��ٸ���Ҷ�任
	for(i = 0; i < FFT_w; i++)
		IFFT(&FD[i * FFT_h], &TD[i * FFT_h], hp);

	//Ϊ���任ͼ������ڴ�
	LONG size = 40 + 1024 + LineBytes * height;

	lpDIB_IFT = (LPBITMAPINFO) malloc(size);
	if (NULL == lpDIB_IFT)
		return;
	memcpy(lpDIB_IFT, lpBitsInfo, size);

	//ָ�򷴱任ͼ������ָ��
	BYTE* lpBits = (BYTE*)&lpDIB_IFT->bmiColors[256];
	BYTE* pixel;
	double temp;
	for(i = 0; i < FFT_h; i++) // ��
	{
		for(j = 0; j < FFT_w; j++) // ��
		{
			pixel = lpBits + LineBytes * (height - 1 - i) + j;
			temp= (TD[j*FFT_h + i].real() / pow(-1, i+j));
			if (temp < 0)
				temp = 0;
			else if (temp >255)
				temp = 255;
			*pixel = (BYTE)temp;
		}
	}

	// ɾ����ʱ����
	delete FD;
	delete TD;
	delete gFD;
	gFD = NULL;
}



void Template(int* Array,float coef)
{
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int h=height;
	int w=width;
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	//��������һ��ͼ��
	LONG size = 40 + 1024 + LineBytes * height;
	BITMAPINFO* new_lpBitsInfo = (LPBITMAPINFO) malloc(size);
	if (NULL == new_lpBitsInfo)
		return;
	memcpy(new_lpBitsInfo, lpBitsInfo, size);
	BYTE* new_lpBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i,j,m,n;
	BYTE *pixel, *new_pixel;
	float result;
	for (i=1;i<h-1;i++)
	{
		for (j = 1;j < w-1; j++)
		{
			new_pixel = new_lpBits + LineBytes * (h - 1 - i) + j;
			result = 0;
			for(m=0;m<3;m++)
			{
				for(n=0;n<3;n++)
				{
					pixel = lpBits + LineBytes * (h - i - m) + j -1 + n;
					result+=(*pixel) * Array[m*3 + n];
				}
			}
			result *=coef;
			if(result<0)
				*new_pixel=0;
			else if(result>255)
				*new_pixel=255;
			else
				*new_pixel=(BYTE)(result+0.5);
		}
	}
	free(lpBitsInfo);
	lpBitsInfo = new_lpBitsInfo;

}


void AvgFliter()				//��ֵ�˲�
{
	int Array[9];
	Array[0]=1;Array[1]=2;Array[2]=1;
	Array[3]=2;Array[4]=4;Array[5]=2;
	Array[6]=1;Array[7]=2;Array[8]=1;
	Template(Array,(float)1/16);
}


BYTE GetMedianNum(BYTE* Array)
{
	int i,j;
	BYTE temp;

	for(j=0;j<8;j++)
	{
		for(i=0;i<8-j;i++)
		{
			if(Array[i] >Array[i+1])
			{
				temp = Array[i];
				Array[i]=Array[i+1];
				Array[i+1]=temp;
			}
		}
	}
	return Array[4];
}
void MedFliter()				//��ֵ�˲�
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	//��������һ��ͼ��
	LONG size = 40 + 1024 + LineBytes * h;
	BITMAPINFO* new_lpBitsInfo = (LPBITMAPINFO) malloc(size);
	if (NULL == new_lpBitsInfo)
		return;
	memcpy(new_lpBitsInfo, lpBitsInfo, size);
	BYTE* new_lpBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i,j,m,n;
	BYTE *pixel, *new_pixel;
	BYTE Array[9];
	for (i=1;i<h-1;i++)
	{
		for (j = 1;j < w-1; j++)
		{
			for(m=0;m<3;m++)
			{
				for(n=0;n<3;n++)
				{
					pixel = lpBits + LineBytes * (h - i - m) + j -1 + n;
					Array[m*3 +n]= *pixel;
				}
			}
			new_pixel = new_lpBits + LineBytes * (h - 1 - i) + j;
			*new_pixel = GetMedianNum(Array);
		}
	}
	free(lpBitsInfo);
	lpBitsInfo = new_lpBitsInfo;
}


void GradSharp()				//�ݶ���
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	//��������һ��ͼ��
	LONG size = 40 + 1024 + LineBytes * h;
	BITMAPINFO* new_lpBitsInfo = (LPBITMAPINFO) malloc(size);
	if (NULL == new_lpBitsInfo)
		return;
	memcpy(new_lpBitsInfo, lpBitsInfo, size);
	BYTE* new_lpBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed];

	int i,j;
	BYTE *pixel,*pixel_right,*pixel_down,*new_pixel;
	for (i=1;i<h-1;i++)
	{
		for (j = 1;j < w-1; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			pixel_right = lpBits + LineBytes * (h - 1 - i) + j + 1;
			pixel_down = lpBits + LineBytes * (h - 2 - i) + j;
			
			new_pixel = new_lpBits + LineBytes * (h - 1 - i) + j;
			*new_pixel = abs(*pixel- *pixel_right) + abs(*pixel - *pixel_down); 
		}
	}
	free(lpBitsInfo);
	lpBitsInfo = new_lpBitsInfo;
}
void RaplasSharp()				//������˹��
{
	int Array[9];
	Array[0]=0;Array[1]=-1;Array[2]=0;
	Array[3]=-1;Array[4]=5;Array[5]=-1;
	Array[6]=0;Array[7]=-1;Array[8]=0;
	Template(Array,(float)1);
}


void FFT_Fliter(int D)
{
	int w = lpDIB_FT->bmiHeader.biWidth;
	int h = lpDIB_FT->bmiHeader.biHeight;
	int LineBytes = (w * lpDIB_FT->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];

	int i,j;
	double dis;
	complex<double> * Backup_FD = new complex<double>[w * h];
	for(i=0; i<w*h;i++)
	{
		Backup_FD[i] = gFD[i];
	}
	for (i=0;i<h;i++)
	{
		for (j = 0;j < w; j++)
		{
			dis = sqrt((i - h/2)*(i-h/2)+ (j-w/2)*(j-w/2));
			if(D > 0)         //��������˹��ͨ
				gFD[i*w + j] *= 1/(1+ pow(dis/D,4));
			else
				gFD[i*w + j] *= 1/(1+ pow(-D/dis,4));
		}
	}

	double temp;
	BYTE* pixel;
	for (i=0;i<h;i++)
	{
		for (j = 0;j < w; j++)
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			temp = sqrt(gFD[j * h + i].real() * gFD[j * h + i].real() + gFD[j * h + i].imag() * gFD[j * h + i].imag()) * 2000;
			if (temp > 255)
				temp = 255;
			*pixel = (BYTE)(temp);

		}
	}
	IFFourier();

	delete gFD;
	gFD = Backup_FD;
}


//������˹��ͨ�˲���
// ============================================================================
void BH_Filter(int D){
	int width = lpDIB_FT->bmiHeader.biWidth;
	int height = lpDIB_FT->bmiHeader.biHeight;
	int w = width;
	int h = height;
	int LineBytes = (width * lpDIB_FT->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];
	
	int i,j;
	double dis;
	complex<double>* Backup_FD = new complex<double>[w * h];
	for ( i = 0;i < w * h;i ++)
		Backup_FD[i] = gFD[i];

	for (i = 0;i < h; i++){
		for (j = 0 ;j < w; j ++){
			dis = sqrt((i - h/2)*(i - h/2) + (j - w/2)*(j - w/2));
			if (D > 0)
				gFD[i * w + j] *= 1 - (1/(1 + pow(dis/D,4)));
			else
				gFD[i * w + j] *= 1 - (1/(1 + pow(-D/dis,4)));
		}
	}
	
	double temp;
	BYTE* pixel;
	for (i = 0;i <h;i++){
		for (j=0;j<w;j++){
			pixel = lpBits + LineBytes * (h -1 -i) + j;
			temp = sqrt(gFD[j*h+i].real() * gFD[j * h+i].real() + 
						gFD[j*h+i].imag() * gFD[j * h+i].imag()) * 2000;
//			temp = sqrt(gFD[i*w+j].real() * gFD[i * w+j].real() + 
//						gFD[i*w+j].imag() * gFD[i * w+j].imag()) * 2000;

			if (temp > 255)
				temp = 255;
			*pixel = (BYTE)(temp);
		}
	}

	IFFourier();

	delete gFD;
	gFD = Backup_FD;
}


void IL_Filter(int D)
{
    int width = lpDIB_FT->bmiHeader.biWidth;
    int height = lpDIB_FT->bmiHeader.biHeight;
    int w = width;
    int h = height;
    int LineBytes = (width * lpDIB_FT->bmiHeader.biBitCount + 31) / 32 * 4;
    BYTE* lpBits = (BYTE*)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];

    int i, j;
    double dis;
    complex<double>* Backup_FD = new complex<double>[w * h];
    for (i = 0; i < w * h; i++)
        Backup_FD[i] = gFD[i];

	for (i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			double dis = sqrt(pow(i - h / 2, 2) + pow(j - w / 2, 2));
			if (dis <= D) {
				// ����С�ڵ��� D ʱƵ�ʱ���
				gFD[i * w + j] *= 1.0;
			} else {
				// ������� D ʱֱ����Ϊ 0
				gFD[i * w + j] = 0.0;
			}
		}
	}

    double temp;
    BYTE* pixel;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            pixel = lpBits + LineBytes * (h - 1 - i) + j;
            temp = sqrt(gFD[j * h + i].real() * gFD[j * h + i].real() +
                        gFD[j * h + i].imag() * gFD[j * h + i].imag()) * 2000;
            if (temp > 255)
                temp = 255;
            *pixel = (BYTE)(temp);
        }
    }

    IFFourier();

	delete gFD;
	gFD = Backup_FD;
}

void IH_Filter(int D)
{
    int width = lpDIB_FT->bmiHeader.biWidth;
    int height = lpDIB_FT->bmiHeader.biHeight;
    int w = width;
    int h = height;
    int LineBytes = (width * lpDIB_FT->bmiHeader.biBitCount + 31) / 32 * 4;
    BYTE* lpBits = (BYTE*)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];

    int i, j;
    double dis;
    complex<double>* Backup_FD = new complex<double>[w * h];
    for (i = 0; i < w * h; i++)
        Backup_FD[i] = gFD[i];

	for (i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			double dis = sqrt(pow(i - h / 2, 2) + pow(j - w / 2, 2));
			if (dis <= D) {
				// ����С�ڵ��� D ʱ��Ϊ 0
				gFD[i * w + j] = 0.0;
			} else {
				// ������� D ʱ����
				gFD[i * w + j] *= 1.0;
			}
		}
	}

    double temp;
    BYTE* pixel;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            pixel = lpBits + LineBytes * (h - 1 - i) + j;
            temp = sqrt(gFD[j * h + i].real() * gFD[j * h + i].real() +
                        gFD[j * h + i].imag() * gFD[j * h + i].imag()) * 2000;
            if (temp > 255)
                temp = 255;
            *pixel = (BYTE)(temp);
        }
    }

    IFFourier();

	delete gFD;
	gFD = Backup_FD;
}
