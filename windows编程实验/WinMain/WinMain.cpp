#include <windows.h>
#include <stdio.h>
LRESULT CALLBACK MyWndProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
);

//1.��ں���
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	//2 ��������
	//2.1���һ��������
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
	//2.2 ����ƺõĴ��������ע��
	RegisterClass(&MyWnd);
	//2.3 ��������
	HWND hWnd;
	hWnd = CreateWindow("Hello","Windows���",WS_OVERLAPPEDWINDOW,
						0,0,800,600,
						NULL,NULL,hInstance,NULL);
	//2.4 ��ʾ�����´���
	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);
	//3 ��Ϣѭ��
	MSG msg;
	while (GetMessage(&msg,NULL,0,0)) //����Ϣ�����л�ȡ��Ϣ
	{
		TranslateMessage(&msg); //��Ϣ����
		
		//������Ϣ
		DispatchMessage(&msg);  //����Ϣ���͵������ڹ��̡�
	}
	return 0;
}

//4.�����ڹ��̺���������д��Ϣ��Ӧ���룩
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
		MessageBox(hwnd,str,"������Ӧ",0);
		break;
	case WM_LBUTTONDOWN:
		MessageBox(hwnd,"����������","�����Ӧ",0);
		HDC hDC;
		hDC = GetDC(hwnd);
		TextOut(hDC,255,100,"Hello World!",strlen("Hello World!"));
		ReleaseDC(hwnd,hDC);

		break;
	case WM_CLOSE:
		if (IDYES == MessageBox(hwnd,"���Ҫ�˳���","�˳�",MB_YESNO))
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