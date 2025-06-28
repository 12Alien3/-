#include "stdafx.h"

BITMAPINFO* lpBitsInfo = NULL;// 定义指向BITMAPINFO结构的指针，用于存储位图信息
BOOL LoadBmpFile(char* BmpFileName)// 加载BMP文件的函数，返回BOOL类型表示成功或失败
{
	FILE* fp;
	if (NULL==(fp=fopen(BmpFileName,"rb")))//以二进制读模式打开指定的BMP文件
		return false;

	BITMAPFILEHEADER bf;// 位图文件头结构体 
	BITMAPINFOHEADER bi;// 位图信息头结构体

	fread(&bf,14,1,fp);//文件头14字节
	fread(&bi,40,1,fp);//信息头40字节

	//调色板占用存储空间大小
	DWORD NumColors;
	if (bi.biClrUsed !=0)
		NumColors =bi.biClrUsed;// 如果biClrUsed不为0，则直接使用biClrUsed的值
	else {
		switch(bi.biBitCount) // 否则根据位深度确定调色板大小
		{
		case 1:
			NumColors = 2; // 1位色深，调色板大小为2
			break;
		case 4:
			NumColors = 16;
			break;
		case 8:
			NumColors = 256;
			break;
		case 24:
			NumColors = 0;//24位色深为真彩，不使用调色板
			break;
		}
	}

	DWORD PalSize = NumColors * 4;// 计算调色板占用的存储空间大小（每个颜色4字节）
	DWORD ImgSize = (bi.biWidth * bi.biBitCount + 31)/32 * 4 * bi.biHeight;// 计算图像数据占用的存储空间大小，考虑到行对齐（每行数据按32位对齐）
	DWORD Size = 40 +PalSize+ImgSize;//总的大小 = 信息头+调色板+图像数据
		
	if(NULL==(lpBitsInfo=(BITMAPINFO*)malloc(Size)))// 为BITMAPINFO结构体分配足够的内存空间
		return FALSE;

	fseek(fp,14,SEEK_SET);
	fread((char*)lpBitsInfo,Size,1,fp);//读取整个位图信息（包括信息头、调色板和图像数据）

	lpBitsInfo->bmiHeader.biClrUsed=NumColors;// 设置BITMAPINFO结构体中的biClrUsed字段，确保它正确反映了调色板中的颜色数量  

	return true;
}

void Gray()
{
    int w = lpBitsInfo->bmiHeader.biWidth;
    int h = lpBitsInfo->bmiHeader.biHeight;
    int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
    BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int LineBytes_gray = (w * 8 + 31) / 32 * 4; // 每个像素8bit
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
        case 1: { // 2值图像
         
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
        case 4: { // 16色图像

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

        case 8: { // 256色图像
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

        case 24: { // 24位真彩色图像
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

BOOL IsGray()//判断是256色还是灰度图像
{
	int r,g,b;
	if(8==lpBitsInfo->bmiHeader.biBitCount)
	{
		r=lpBitsInfo->bmiColors[150].rgbRed;
		g=lpBitsInfo->bmiColors[150].rgbGreen;
		b=lpBitsInfo->bmiColors[150].rgbBlue;

		if(r==b&&r==g)//任意一点rgb相等，则为灰度图像
			return TRUE;

	}
	return FALSE;

}



void pixel(int i,int j,char* str)//计算鼠标点的像素值
{
	if(NULL==lpBitsInfo)//不在图像范围内，return
		return;

	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	if(i>=h || j>=w)//i控制行,超过范围就return
		return;
	
	BYTE* pixel,bv;
	int r,g,b;

	switch(lpBitsInfo->bmiHeader.biBitCount){
	case 8://灰度
		pixel=lpBits + LineBytes *(h-1-i)+j;
		if (IsGray()){
			sprintf(str,"灰度值：%d",*pixel);
		}
		else{
			r=lpBitsInfo->bmiColors[*pixel].rgbRed;
			g=lpBitsInfo->bmiColors[*pixel].rgbGreen;
			b=lpBitsInfo->bmiColors[*pixel].rgbBlue;
			sprintf(str,"RGB(%d,%d,%d)",r,g,b);//256色，输出RGB
		}
		break;
	case 24://24位
		pixel = lpBits + LineBytes * (h - 1 - i) + j * 3;
        r = pixel[2]; // BGR顺序存储
        g = pixel[1];
        b = pixel[0];
        sprintf(str, "RGB(%d,%d,%d)", r, g, b); // 直接输出RGB
        break;
	case 4://16色
        pixel = lpBits + LineBytes * (h - 1 - i) + j / 2;
        if (j % 2 == 0) { // 如果是偶数，则取高4位
            int value = (*pixel & 0xF0) >> 4;
            r = lpBitsInfo->bmiColors[value].rgbRed;
            g = lpBitsInfo->bmiColors[value].rgbGreen;
            b = lpBitsInfo->bmiColors[value].rgbBlue;
            sprintf(str, "RGB(%d,%d,%d)", r, g, b);
        } else { // 否则取低4位
            int value = (*pixel & 0x0F);
            r = lpBitsInfo->bmiColors[value].rgbRed;
            g = lpBitsInfo->bmiColors[value].rgbGreen;
            b = lpBitsInfo->bmiColors[value].rgbBlue;
            sprintf(str, "RGB(%d,%d,%d)", r, g, b);
        }
		break;
	case 1://2值图像
		bv=*(lpBits + LineBytes *(h-1-i)+j/8)&(1<<j%8);
		if(0==bv)
			strcpy(str,"背景点");
		else
			strcpy(str,"前景点");
		break;
	}
}