#include "stdafx.h"

BITMAPINFO* lpBitsInfo = NULL;// ����ָ��BITMAPINFO�ṹ��ָ�룬���ڴ洢λͼ��Ϣ
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

void Gray()
{
    int w = lpBitsInfo->bmiHeader.biWidth;
    int h = lpBitsInfo->bmiHeader.biHeight;
    int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
    BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int LineBytes_gray = (w * 8 + 31) / 32 * 4; // ÿ������8bit
    BITMAPINFO* lpBitsInfo_gray = (BITMAPINFO*)malloc(40 + 256 + LineBytes_gray * h);
    memcpy(lpBitsInfo_gray, lpBitsInfo, 40);
    lpBitsInfo_gray->bmiHeader.biBitCount = 8;
    lpBitsInfo_gray->bmiHeader.biClrUsed = 256;
	int i,j;
	for (i = 0; i < 256; i++) {
        lpBitsInfo_gray->bmiColors[i].rgbRed = i;
        lpBitsInfo_gray->bmiColors[i].rgbGreen = i;
        lpBitsInfo_gray->bmiColors[i].rgbBlue = i;
        lpBitsInfo_gray->bmiColors[i].rgbReserved = 0;
    }

    BYTE* lpBits_gray = (BYTE*)&lpBitsInfo_gray->bmiColors[256];

    BYTE *R, *G, *B,_B,_G,_R,avg, *pixel,*index,_index;
    
	switch (lpBitsInfo->bmiHeader.biBitCount) {
        case 1: { // 2ֵͼ��
         
            /*for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
					_index = (*lpBits+LineBytes*(h-i-1)+j/8 &(1 << (7 - j % 8) | (1 << (7 - j % 8));
                    _B = lpBitsInfo->bmiColors[_index].rgbBlue;
                    _G = lpBitsInfo->bmiColors[_index].rgbGreen;
                    _R = lpBitsInfo->bmiColors[_index].rgbRed;
                    avg = (_R +_G +_B) / 3;


                    pixel = lpBits_gray + LineBytes_gray * (h - 1 - i) + j / 8;
                    *pixel = (*pixel & ~(1 << (7 - j % 8))) | (((avg > 128) ? 1 : 0) << (7 - j % 8));
                }
            }*/
            break;
        }
ss
        case 4: { // 16ɫͼ��

            for (i = 0; i < h; i++) {
                for (j = 0; j < w; j++) {
					if (j % 2 == 0) {
                        _index = (*(lpBits+LineBytes*(h-1-i)+j/2) & 0xF0)>> 4;
                    } else {
                        _index = (*(lpBits+LineBytes*(h-1-i)+j/2) & 0xF0);
                    }
                    _B = lpBitsInfo->bmiColors[_index].rgbBlue;
                    _G = lpBitsInfo->bmiColors[_index].rgbGreen;
                    _R = lpBitsInfo->bmiColors[_index].rgbRed;
                    avg = (_R +_G + _B) / 3;

                    pixel = lpBits_gray + LineBytes_gray * (h - 1 - i) + j ;
                    *pixel=avg;
                    
                }
            }
            break;
        }

        case 8: { // 256ɫͼ��
            for (i = 0; i < h; i++) {
                for (j = 0; j < w; j++) {
					index= lpBits + LineBytes*(h-1-i)*j*1;
                    _B = lpBitsInfo->bmiColors[*index].rgbBlue;
                    _G = lpBitsInfo->bmiColors[*index].rgbGreen;
                    _R = lpBitsInfo->bmiColors[*index].rgbRed;
                    avg = (_R + _G + _B) / 3;

                    pixel = lpBits_gray + LineBytes_gray * (h - 1 - i) + j;
                    *pixel = avg;
                }
            }
            break;
        }

        case 24: { // 24λ���ɫͼ��
            for (i = 0; i < h; i++) {
                for (j = 0; j < w; j++) {
                    B = lpBits + LineBytes * (h - 1 - i) + j * 3;
                    G = B + 1;
                    R = G + 1;
                    avg = (*R + *G + *B) / 3;

                    pixel = lpBits_gray + LineBytes_gray * (h - 1 - i) + j;
                    *pixel = avg;
                }
            }
            break;
        }

        default:
            // Handle error or return without doing anything
            return;
    }

    free(lpBitsInfo);
    lpBitsInfo = lpBitsInfo_gray;
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



void pixel(int i,int j,char* str)//�������������ֵ
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