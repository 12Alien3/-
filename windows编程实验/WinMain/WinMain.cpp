#include <windows.h>
#include <stdio.h>
LRESULT CALLBACK MyWndProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
);

//1.入口函数
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	//2 创建窗口
	//2.1设计一个窗口类
	WNDCLASS MyWnd;
	MyWnd.cbClsExtra = NULL;
	MyWnd.cbWndExtra = NULL;
	MyWnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	MyWnd.hCursor = LoadCursor(NULL, IDC_CROSS);
	MyWnd.hIcon = LoadIcon(NULL,IDI_QUESTION);
	MyWnd.hInstance = hInstance;
	MyWnd.lpfnWndProc = MyWndProc;
	MyWnd.lpszClassName = "Hello";
	MyWnd.lpszMenuName = NULL;
	MyWnd.style = CS_HREDRAW | CS_VREDRAW;
	//2.2 对设计好的窗口类进行注册
	RegisterClass(&MyWnd);
	//2.3 创建窗口
	HWND hWnd;
	hWnd = CreateWindow("Hello","Windows编程",WS_OVERLAPPEDWINDOW,
						0,0,800,600,
						NULL,NULL,hInstance,NULL);
	//2.4 显示及更新窗口
	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);
	//3 消息循环
	MSG msg;
	while (GetMessage(&msg,NULL,0,0)) //从消息队列中获取消息
	{
		TranslateMessage(&msg); //消息解释
		
		//发送消息
		DispatchMessage(&msg);  //将消息发送到“窗口过程”
	}
	return 0;
}

//4.“窗口过程函数”（编写消息响应代码）
LRESULT CALLBACK MyWndProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (uMsg)
	{
	case WM_PAINT:
		PAINTSTRUCT ps;
		{HDC hpaintDC = BeginPaint(hwnd,&ps);
		TextOut(hpaintDC,255,50,"Hello World!",strlen("Hello World!"));
		EndPaint(hwnd,&ps);}
		break;
	case WM_CHAR:
		char str[255];
		sprintf(str,"char is %d",wParam);
		MessageBox(hwnd,str,"按键响应",0);
		break;
	case WM_LBUTTONDOWN:
		MessageBox(hwnd,"鼠标左键按下","鼠标响应",0);
		HDC hDC;
		hDC = GetDC(hwnd);
		TextOut(hDC,255,100,"Hello World!",strlen("Hello World!"));
		ReleaseDC(hwnd,hDC);

		break;
	case WM_CLOSE:
		if (IDYES == MessageBox(hwnd,"真的要退出吗","退出",MB_YESNO))
			DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd,uMsg,wParam,lParam);
	}
	return 0;
}