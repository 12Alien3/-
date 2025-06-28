#include "stdafx.h"
#include <complex>
using namespace std;
#define PI 3.1415926535
BITMAPINFO* lpDIB_FT = NULL;
BITMAPINFO* lpDIB_IFT = NULL;
BITMAPINFO* lpBitsInfo = NULL;// 定义指向BITMAPINFO结构的指针，用于存储位图信息

complex<double>* gFD = NULL;

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

void Gray()                  //图像灰度化
{
    // 获取图像的宽度和高度
    int w = lpBitsInfo->bmiHeader.biWidth;
    int h = lpBitsInfo->bmiHeader.biHeight;
    // 计算原图像每一行的字节数，确保字节数是4的倍数
    int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
    BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

    // 计算灰度图像每一行的字节数，灰度图像是8位的
    int LineBytes_gray = (w * 8 + 31) / 32 * 4;
    // 动态分配新的 BITMAPINFO 结构体，用于存储灰度图像的信息
    BITMAPINFO* lpBitsInfo_gray = (BITMAPINFO*)malloc(40 + 256 * sizeof(RGBQUAD) + LineBytes_gray * h);
    if (!lpBitsInfo_gray) return; // 检查内存分配是否成功

    // 复制原图像的头部信息到新图像
    memcpy(lpBitsInfo_gray, lpBitsInfo, 40);
    lpBitsInfo_gray->bmiHeader.biBitCount = 8; // 设置新图像的位深度为8位
    lpBitsInfo_gray->bmiHeader.biClrUsed = 256; // 设置颜色表的大小为256
    lpBitsInfo_gray->bmiHeader.biSizeImage = LineBytes_gray * h; // 设置图像数据的大小

    // 初始化灰度图像的颜色表，每个灰度值对应一个RGB值
    for (int i = 0; i < 256; i++) {
        lpBitsInfo_gray->bmiColors[i].rgbRed = i;
        lpBitsInfo_gray->bmiColors[i].rgbGreen = i;
        lpBitsInfo_gray->bmiColors[i].rgbBlue = i;
        lpBitsInfo_gray->bmiColors[i].rgbReserved = 0;
    }

    // 获取灰度图像的数据指针
    BYTE* lpBits_gray = (BYTE*)&lpBitsInfo_gray->bmiColors[256];

    // 根据不同的位深度进行灰度化处理
    switch (lpBitsInfo->bmiHeader.biBitCount) {
        case 1: {// 2值图像
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    // 计算当前像素在字节中的位置
                    int byteIndex = j / 8;
                    int bitIndex = 7 - (j % 8);
                    // 获取当前像素的颜色值
                    BYTE byte = lpBits[LineBytes * (h - 1 - i) + byteIndex];
                    int color = (byte >> bitIndex) & 1;
                    // 将颜色值转换为灰度值
                    BYTE grayValue = (color == 0) ? 0 : 255;
                    // 将灰度值写入目标图像
                    lpBits_gray[LineBytes_gray * (h - 1 - i) + j] = grayValue;
                }
            }
            break;
        }
        case 4: {// 16色图像
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    // 计算当前像素在字节中的位置
                    int byteIndex = (j / 2);
                    int bitShift = (j % 2) ? 0 : 4;
                    // 获取当前像素的颜色索引
                    int index = ((lpBits + LineBytes * (h - 1 - i))[byteIndex] >> bitShift) & 0x0F;
                    // 从颜色表中获取颜色值
                    BYTE _B = lpBitsInfo->bmiColors[index].rgbBlue;
                    BYTE _G = lpBitsInfo->bmiColors[index].rgbGreen;
                    BYTE _R = lpBitsInfo->bmiColors[index].rgbRed;
                    // 计算灰度值
                    BYTE avg = (_R + _G + _B) / 3;
                    // 将灰度值写入目标图像
                    lpBits_gray[LineBytes_gray * (h - 1 - i) + j] = avg;
                }
            }
            break;
        }
        case 8: {// 256色图像
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    // 获取当前像素的颜色索引
                    int index = lpBits[LineBytes * (h - 1 - i) + j];
                    // 从颜色表中获取颜色值
                    BYTE _B = lpBitsInfo->bmiColors[index].rgbBlue;
                    BYTE _G = lpBitsInfo->bmiColors[index].rgbGreen;
                    BYTE _R = lpBitsInfo->bmiColors[index].rgbRed;
                    // 计算灰度值
                    BYTE avg = (_R + _G + _B) / 3;
                    // 将灰度值写入目标图像
                    lpBits_gray[LineBytes_gray * (h - 1 - i) + j] = avg;
                }
            }
            break;
        }
        case 24: {// 24位真彩色图像
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    // 获取当前像素的红、绿、蓝通道值
                    BYTE* B = lpBits + LineBytes * (h - 1 - i) + j * 3;
                    BYTE* G = B + 1;
                    BYTE* R = G + 1;
                    // 计算灰度值
                    BYTE avg = (*R + *G + *B) / 3;
                    // 将灰度值写入目标图像
                    lpBits_gray[LineBytes_gray * (h - 1 - i) + j] = avg;
                }
            }
            break;
        }
        default:
            return; 
    }

    // 释放旧的 BITMAPINFO 并替换为新的
    if (lpBitsInfo != lpBitsInfo_gray) {
        free(lpBitsInfo);
        lpBitsInfo = lpBitsInfo_gray;
    }
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



void pixel(int i,int j,char* str)          //计算鼠标点的像素值
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



DWORD HR[256]; // 红色通道直方图
DWORD HG[256]; // 绿色通道直方图
DWORD HB[256]; // 蓝色通道直方图
void Histogram()                         // 绘制24位真彩直方图，如果是灰度图像则都相同
{
    // 获取图像的宽度和高度
    int w = lpBitsInfo->bmiHeader.biWidth;
    int h = lpBitsInfo->bmiHeader.biHeight;
    // 计算原图像每一行的字节数，确保字节数是4的倍数
    int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
    BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

    int i, j;
    BYTE* pixel;

    // 初始化直方图数组
    for(i = 0; i < 256; i++) {
        HR[i] = 0;
        HG[i] = 0;
        HB[i] = 0;
    }

    // 遍历图像的每一个像素
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            pixel = lpBits + LineBytes * (h - 1 - i) + j * 3; // 24位图像每像素3字节
            HR[pixel[2]]++; // 红色通道
            HG[pixel[1]]++; // 绿色通道
            HB[pixel[0]]++; // 蓝色通道
        }
    }
}



void LineTrans(float a, float b)             //对24位真彩和灰度图像进行线性点运算
{
    // 获取图像的宽度和高度
    int w = lpBitsInfo->bmiHeader.biWidth;
    int h = lpBitsInfo->bmiHeader.biHeight;
    // 计算原图像每一行的字节数，确保字节数是4的倍数
    int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;

    BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

    int i, j;
    BYTE* pixel;
    float temp;

    // 根据不同的位深度进行处理
    switch (lpBitsInfo->bmiHeader.biBitCount) {
        case 24: { // 24位真彩色图像
            for (i = 0; i < h; i++) {
                for (j = 0; j < w; j++) {
                    // 每个像素占3个字节（R, G, B），所以这里乘以3
                    pixel = lpBits + LineBytes * (h - 1 - i) + j * 3;

                    // 对每个颜色通道应用线性变换
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
        case 8: { // 8位灰度图像,256色则未处理
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
            // 其他位深度的图像处理（如果需要）
            break;
    }
}


void Equalize()            // 针对24位真彩图像和灰度图像的均衡化
{
    // 获取图像的宽度和高度
    int w = lpBitsInfo->bmiHeader.biWidth;
    int h = lpBitsInfo->bmiHeader.biHeight;
    // 计算原图像每一行的字节数，确保字节数是4的倍数
    int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
    BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

    if (IsGray())
    {
        // 初始化灰度直方图数组
        DWORD H[256] = {0};

        // 计算灰度直方图
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                BYTE* pixel = lpBits + LineBytes * i + j; // 灰度图像每像素1字节
                H[*pixel]++;
            }
        }

        // 创建映射表
        BYTE Map[256];
        DWORD temp = 0;

        for (i = 0; i < 256; i++) {
            temp += H[i];
            Map[i] = (BYTE)(255 * temp / (w * h));
        }

        // 应用映射表
        for (i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                BYTE* pixel = lpBits + LineBytes * i + j; // 灰度图像每像素1字节
                *pixel = Map[*pixel];
            }
        }
    }
    else
    {
        // 初始化直方图数组
        DWORD HR[256] = {0}; // 红色通道直方图
        DWORD HG[256] = {0}; // 绿色通道直方图
        DWORD HB[256] = {0}; // 蓝色通道直方图

        // 计算每个通道的直方图
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                BYTE* pixel = lpBits + LineBytes * (h - 1 - i) + j * 3; // 24位图像每像素3字节
                HR[pixel[2]]++; // 红色通道
                HG[pixel[1]]++; // 绿色通道
                HB[pixel[0]]++; // 蓝色通道
            }
        }

        // 创建映射表
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

        // 应用映射表
        for (i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                BYTE* pixel = lpBits + LineBytes * (h - 1 - i) + j * 3; // 24位图像每像素3字节
                pixel[2] = MapR[pixel[2]]; // 红色通道
                pixel[1] = MapG[pixel[1]]; // 绿色通道
                pixel[0] = MapB[pixel[0]]; // 蓝色通道
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
	
	// 填充TD
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
	// 转置
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

void FFT(complex<double> * TD, complex<double> * FD, int r)            //快速傅里叶变换
{
	// 计算付立叶变换点数
	LONG count = 1 << r;
	// 计算加权系数
	int i;
	double angle;
	complex<double>* W = new complex<double>[count / 2];
	for(i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2 / count;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	// 将时域点写入X1
	complex<double>* X1 = new complex<double>[count];
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// 采用蝶形算法进行快速付立叶变换，输出为频域值X2
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
	
	// 重新排序（码位倒序排列）
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
	
	// 释放内存
	delete W;
	delete X1;
	delete X2;
}

void FFourier()
{
	//图像的宽度和高度
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	//指向图像数据指针
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂数，即迭代次数
	int wp = 0;
	while(FFT_w * 2 <= width)
	{
		FFT_w *= 2;
		wp ++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂数，即迭代次数
	int hp = 0;
	while(FFT_h * 2 <= height)
	{
		FFT_h *= 2;
		hp ++;
	}

	// 分配内存
	complex<double>* TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* FD = new complex<double>[FFT_w * FFT_h];
	
	int i, j;
	BYTE* pixel;
	
	for(i = 0; i < FFT_h; i++)  // 行
	{
		for(j = 0; j < FFT_w; j++)  // 列
		{
			// 指向DIB第i行，第j个象素的指针
			pixel = lpBits + LineBytes * (height - 1 - i) + j;

			// 给时域赋值
			TD[j + FFT_w * i] = complex<double>(*pixel* pow(-1,i+j), 0);
		}
	}
	
	for(i = 0; i < FFT_h; i++)
	{
		// 对y方向进行快速付立叶变换
		FFT(&TD[FFT_w * i], &FD[FFT_w * i], wp);
	}
	
	// 保存中间变换结果
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			TD[i + FFT_h * j] = FD[j + FFT_w * i];
		}
	}
	
	for(i = 0; i < FFT_w; i++)
	{
		// 对x方向进行快速付立叶变换
		FFT(&TD[i * FFT_h], &FD[i * FFT_h], hp);
	}

	//生成频谱图像
	//为频域图像分配内存
	LONG size = 40 + 1024 + LineBytes * height;
	lpDIB_FT = (LPBITMAPINFO) malloc(size);
	if (NULL == lpDIB_FT)
		return;
	memcpy(lpDIB_FT, lpBitsInfo, size);

	//指向频域图像数据指针
	lpBits = (BYTE*)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];

	double temp;
	for(i = 0; i < FFT_h; i++) // 行
	{
		for(j = 0; j < FFT_w; j++) // 列
		{
			// 计算频谱幅度
			temp = sqrt(FD[j * FFT_h + i].real() * FD[j * FFT_h + i].real() + 
				        FD[j * FFT_h + i].imag() * FD[j * FFT_h + i].imag()) *2000;
			
			// 判断是否超过255
			if (temp > 255)
			{
				// 对于超过的，直接设置为255
				temp = 255;
			}
			
			pixel = lpBits + LineBytes * (height - 1 - i) + j;

			// 更新源图像
			*pixel = (BYTE)(temp);
		}
	}

	delete TD;
	gFD = FD;
}


//IFFT反变换
void IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	// 付立叶变换点数
	LONG	count;
	// 计算付立叶变换点数
	count = 1 << r;

	// 分配运算所需存储器
	complex<double> * X = new complex<double>[count];
	// 将频域点写入X
	memcpy(X, FD, sizeof(complex<double>) * count);
	
	// 求共轭
	for(int i = 0; i < count; i++)
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	
	// 调用快速付立叶变换
	FFT(X, TD, r);
	
	// 求时域点的共轭
	for(i = 0; i < count; i++)
		TD[i] = complex<double> (TD[i].real() * count, -TD[i].imag() * count);
	
	// 释放内存
	delete X;
}

void IFFourier()
{
	//图像的宽度和高度
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂数，即迭代次数
	int wp = 0;
	while(FFT_w * 2 <= width)
	{
		FFT_w *= 2;
		wp ++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂数，即迭代次数
	int hp = 0;
	while(FFT_h * 2 <= height)
	{
		FFT_h *= 2;
		hp ++;
	}

	// 分配内存
	complex<double>* TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* FD = new complex<double>[FFT_w * FFT_h];
	
	int i, j;
	for(i = 0; i < FFT_h; i++)  // 行
		for(j = 0; j < FFT_w; j++)  // 列
			FD[j + FFT_w * i] = gFD[i + FFT_h*j];
	
	// 沿水平方向进行快速付立叶变换
	for(i = 0; i < FFT_h; i++)
		IFFT(&FD[FFT_w * i], &TD[FFT_w * i], wp);
	
	// 保存中间变换结果
	for(i = 0; i < FFT_h; i++)
		for(j = 0; j < FFT_w; j++)
			FD[i + FFT_h * j] = TD[j + FFT_w * i];
	
	// 沿垂直方向进行快速付立叶变换
	for(i = 0; i < FFT_w; i++)
		IFFT(&FD[i * FFT_h], &TD[i * FFT_h], hp);

	//为反变换图像分配内存
	LONG size = 40 + 1024 + LineBytes * height;

	lpDIB_IFT = (LPBITMAPINFO) malloc(size);
	if (NULL == lpDIB_IFT)
		return;
	memcpy(lpDIB_IFT, lpBitsInfo, size);

	//指向反变换图像数据指针
	BYTE* lpBits = (BYTE*)&lpDIB_IFT->bmiColors[256];
	BYTE* pixel;
	double temp;
	for(i = 0; i < FFT_h; i++) // 行
	{
		for(j = 0; j < FFT_w; j++) // 列
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

	// 删除临时变量
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

	//重新生成一个图像
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


void AvgFliter()				//均值滤波
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
void MedFliter()				//中值滤波
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	//重新生成一个图像
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


void GradSharp()				//梯度锐化
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	//重新生成一个图像
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
void RaplasSharp()				//拉普拉斯锐化
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
			if(D > 0)         //按巴特沃斯低通
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


//巴特沃斯高通滤波器
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
				// 距离小于等于 D 时频率保留
				gFD[i * w + j] *= 1.0;
			} else {
				// 距离大于 D 时直接设为 0
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
				// 距离小于等于 D 时设为 0
				gFD[i * w + j] = 0.0;
			} else {
				// 距离大于 D 时保留
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
