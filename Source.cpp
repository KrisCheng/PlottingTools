#include<windows.h>
#include<Windowsx.h>
#include<vector>
#include<stdio.h>
#include"string.h"
#include <cstring>
#include <iostream>   
#include <string>   
#include<fstream>   
#include<vector>
#include"shlwapi.h"
#include "Parse.h"
#include"Zoom.h"
#include"Bitmap.h"
#include"Read.h"
using namespace std;
HINSTANCE ghInst, hInst;
//POINTS ptOld;
HDC hMem;
HDC a;//static的使用
float xPos = 0, yPos = 0;
float zoom = 1;//缩放指数

LRESULT __stdcall MyWinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	switch (Msg) {

		static POINTS ptOld;
		static HWND plot, reset, save, read, fun1, fun2, fun3, fun4, fun5;

		PAINTSTRUCT ps;

	case WM_CREATE:
		fun1 = CreateWindow(L"EDIT", TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 700, 50, 150, 30, hWnd, (HMENU)5, hInst, NULL);
		fun2 = CreateWindow(L"EDIT", TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 700, 100, 150, 30, hWnd, (HMENU)6, hInst, NULL);
		fun3 = CreateWindow(L"EDIT", TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 700, 150, 150, 30, hWnd, (HMENU)7, hInst, NULL);
		fun4 = CreateWindow(L"EDIT", TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 700, 200, 150, 30, hWnd, (HMENU)8, hInst, NULL);
		fun5 = CreateWindow(L"EDIT", TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 700, 250, 150, 30, hWnd, (HMENU)9, hInst, NULL);
		plot = CreateWindow(L"BUTTON", TEXT("PLOT"), WS_CHILD | WS_VISIBLE, 600, 350, 100, 60, hWnd, (HMENU)1, hInst, NULL);
		reset = CreateWindow(L"BUTTON", TEXT("RESET"), WS_CHILD | WS_VISIBLE, 740, 350, 100, 60, hWnd, (HMENU)2, hInst, NULL);
		save = CreateWindow(L"BUTTON", TEXT("SAVE"), WS_CHILD | WS_VISIBLE, 600, 450, 100, 60, hWnd, (HMENU)3, hInst, NULL);
		read = CreateWindow(L"BUTTON", TEXT("READ"), WS_CHILD | WS_VISIBLE, 740, 450, 100, 60, hWnd, (HMENU)4, hInst, NULL);
		break;

	case WM_COMMAND://button响应机制
	{
		int wmId = LOWORD(wParam);
		switch (wmId) {
		case 1: {
			//获取文本框信息
			TCHAR buf1[10];
			TCHAR buf2[10];
			TCHAR buf3[10];
			TCHAR buf4[10];
			TCHAR buf5[10];
			::SendMessage(fun1, WM_GETTEXT, 10, (LPARAM)buf1);
			::SendMessage(fun2, WM_GETTEXT, 10, (LPARAM)buf2);
			::SendMessage(fun3, WM_GETTEXT, 10, (LPARAM)buf3);
			::SendMessage(fun4, WM_GETTEXT, 10, (LPARAM)buf4);
			::SendMessage(fun5, WM_GETTEXT, 10, (LPARAM)buf5);
			::wsprintf(buf1, L"%s", buf1);
			::wsprintf(buf2, L"%s", buf2);
			::wsprintf(buf3, L"%s", buf3);
			::wsprintf(buf4, L"%s", buf4);
			::wsprintf(buf5, L"%s", buf5);
			InvalidateRect(hWnd, NULL, TRUE);//刷新
			Parse(buf1,buf2,buf3,buf4,buf5,hWnd,true);//作图
			break;
		}
		case 2: {//重置
			SendMessage(fun1, WM_SETTEXT, 10, NULL);
			SendMessage(fun2, WM_SETTEXT, 10, NULL);
			SendMessage(fun3, WM_SETTEXT, 10, NULL);
			SendMessage(fun4, WM_SETTEXT, 10, NULL);
			SendMessage(fun5, WM_SETTEXT, 10, NULL);
			InvalidateRect(hWnd, NULL, TRUE);//刷新
			break;
		}
		case 3: {//存为位图
			TrySeveDeskBmp();
			MessageBox(hWnd, L"已保存为位图.", L"Message", MB_OK);
			break;
		}
				
		case 4: {//读取文件并连线
			OPENFILENAME ofn;       // common dialog box structure
			char szFile[200];       // buffer for file name
			HWND hwnd = NULL;              // owner window
			HANDLE hf;              // file handle

									// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile  = (LPWSTR)szFile;
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = L"\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			// Display the Open dialog box. 
			//MessageBox(hWnd, L"已保存为位图.", L"Message", MB_OK);

			//TCHAR szPath2[] = TEXT("c:\\windows\\system32\\directx\\dinput\\joystick.ini");
			if (GetOpenFileName(&ofn) == TRUE){
				//PathStripPath(szFile);//strFile得到文件		
				//PathStripPath(szPath2);
			hf = CreateFile(L"das.txt",
					GENERIC_READ,
					0,
					(LPSECURITY_ATTRIBUTES)NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					(HANDLE)NULL);
			}
			//TCHAR szPath2[] = TEXT("D:\\workspace\\PlottingTool\\PlottingTools1\\PlottingTools1");
			//PathStripPath(szPath2);
			/*//获取文件名
			char  aa[200];
			int count1 = 0;
			int m = 0;
			for (int i = 10; i < 200; i++) {
				//if (szFile[i] == 'v' && szFile[i-1] == 's' && szFile[i-2] == 'c') {
				if( '\0' != szFile[i] ){
				//count1++;
						aa[m] = szFile[i];
						m++;
				}
			}
			int k = 0,mm = 0;
			char* openFile = (char *)malloc(sizeof(char)*m);
			for (int i = 0; i <= m; i++) {
				if ('.' == aa[i]) {
					 k = i;
					while ('\\' != aa[k]) {
						//openFile[mm] = aa[k];
						k--;
						mm++;
					}
					for (int w = 0; w < mm; w++) {
							openFile[w] = aa[k+1];
							k++;
					}
				}
			}
			int cd = 0;
			while ('.' != openFile[cd]) {
				cd++;
			}
			char* openName = (char *)malloc(sizeof(char)*cd+4);
			for (int t = 0; t < cd; t++) {
				openName[t] = openFile[t];
				
			}
			openName[cd] = '.';
			openName[cd+1] = 't';
			openName[cd+2] = 'x';
			openName[cd+3] = 't';
			char *bb = "adas.txt";
			*/
			
			/*
			while (*pFile.good())
			{
				getline(pFile, value, ','); // read a string until next comma:      
									   //cout << string(value, 0, value.length()) << ","; 
										   //if(1 == (i+1)%2){
				nums.push_back(string(value, 0, value.length()));
			}
			float f1 = 0;
			float temp[100];
			for (int i = 0; i < nums.size(); i++) {
				if (i > 0) {
					char *a = zhuanhuan(nums[i]);
					char* pEnd = NULL;
					f1 = strtof(a, &pEnd);
					temp[i - 1] = f1;
				}
			}
			*/

			InvalidateRect(hWnd, NULL, TRUE);//刷新

			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			TCHAR buf[256];
			HPEN hpenXY, hpenXYOld;
			TextOut(hdc, 265, 10, L"Y", 1);
			TextOut(hdc, 535, 300, L"X", 1);
			RECT rect;
			HBRUSH hBrush = (HBRUSH)GetStockObject(DC_BRUSH);
			rect.left = 600;   //向下竖排
			rect.right = 630;
			SetDCBrushColor(hdc, RGB(0, 0, 255));//蓝色
			rect.bottom = 80;
			rect.top = 50;
			FillRect(hdc, &rect, hBrush);
			SetDCBrushColor(hdc, RGB(255, 0, 0));//红色
			rect.bottom = 130;
			rect.top = 100;
			FillRect(hdc, &rect, hBrush);
			SetDCBrushColor(hdc, RGB(0, 255, 0));//绿色
			rect.bottom = 180;
			rect.top = 150;
			FillRect(hdc, &rect, hBrush);
			SetDCBrushColor(hdc, RGB(128, 0, 128));//紫色
			rect.bottom = 230;
			rect.top = 200;
			FillRect(hdc, &rect, hBrush);
			SetDCBrushColor(hdc, RGB(160, 160, 164));//灰色
			rect.bottom = 280;
			rect.top = 250;
			FillRect(hdc, &rect, hBrush);
			WCHAR fx[10] = L"F(x)=";
			for (int i = 0; i < 5; i++) {
				::wsprintf(buf, L"%d", i + 1);
				buf[1] = '.';
				TextOut(hdc, 640, 60 + i * 50, buf, 2);
				TextOut(hdc, 660, 60 + i * 50, fx, 5);
			}
			//以上为5个函数输入框
			//绘制坐标系

			hpenXY = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
			hpenXYOld = (HPEN)SelectObject(hdc, hpenXY);
			MoveToEx(hdc, 10, 300, NULL);
			LineTo(hdc, 530, 300);
			MoveToEx(hdc, 530, 300, NULL);
			LineTo(hdc, 525, 295);
			MoveToEx(hdc, 530, 300, NULL);
			LineTo(hdc, 525, 305);

			MoveToEx(hdc, 270, 30, NULL);
			LineTo(hdc, 270, 550);
			MoveToEx(hdc, 270, 30, NULL);
			LineTo(hdc, 275, 35);
			MoveToEx(hdc, 270, 30, NULL);
			LineTo(hdc, 265, 35);

			TextOut(hdc, 275, 305, L"0", 1);//坐标原点

			SelectObject(hdc, hpenXYOld);
			DeleteObject(hpenXY);

			//绘制坐标系中的虚线
			HPEN hpen, hpenOld;

			//网格
			hpen = CreatePen(PS_DASH, 2, RGB(216, 216, 216));
			hpenOld = (HPEN)SelectObject(hdc, hpen);
			for (int i = 0; i < 21; i++) {
				if (i != 10) {
					MoveToEx(hdc, 10, 550 - 25 * i, NULL);
					LineTo(hdc, 530, 550 - 25 * i);
					MoveToEx(hdc, 20 + i * 25, 30, NULL);
					LineTo(hdc, 20 + i * 25, 550);
				}
			}
			int count = 0;
			while (count < 21)
			{
				hpenXY = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
				hpenXYOld = (HPEN)SelectObject(hdc, hpenXY);

				MoveToEx(hdc, 20 + count * 25, 295, NULL);
				LineTo(hdc, 20 + count * 25, 305);

				SelectObject(hdc, hpenXYOld);
				DeleteObject(hpenXY);

				::wsprintf(buf, L"%d", count - 10);
				if (count == 0)
					TextOut(hdc, 15, 305, buf, 3);
				else if (count > 0 && count < 10 || count == 20)
					TextOut(hdc, 15 + count * 25, 305, buf, 2);
				else if (count != 10)
					TextOut(hdc, 15 + count * 25, 305, buf, 1);

				hpenXY = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
				hpenXYOld = (HPEN)SelectObject(hdc, hpenXY);

				MoveToEx(hdc, 265, 550 - count * 25, NULL);
				LineTo(hdc, 275, 550 - count * 25);


				SelectObject(hdc, hpenXYOld);
				DeleteObject(hpenXY);

				::wsprintf(buf, L"%d", count - 10);
				if (count == 0)
					TextOut(hdc, 250, 540, buf, 3);
				else if (count > 0 && count < 10 || count == 20)
					TextOut(hdc, 250, 545 - count * 25, buf, 2);
				else if (count != 10)
					TextOut(hdc, 250, 545 - count * 25, buf, 1);
				count++;
			}
			int i, n = 0;
			float x[100], val;
			FILE *fp;
			fopen_s(&fp, "test.csv", "r");

			while (1)
			{
				if (fscanf_s(fp, "%f,", &val) == EOF) break;
				x[n] = val;
				n++;
			}
			fclose(fp);
			float xPos[100],yPos[100];
			int mm = 0;
			for (int i = 0; i < n; i+=2) {
				xPos[mm] = x[i];
				yPos[mm] = x[i+1];
				mm++;
			}
		
			paixu(xPos,yPos,n/2);

			POINT apt1[100];
			int tmp = 0;
			for (int z = 0; z < n/2; z++) {
					apt1[tmp].x = (int)(25 * xPos[z] + 270);
					apt1[tmp].y = (int)(-25 * yPos[z] + 300);
					tmp++;
				}
			hpenXY = CreatePen(PS_SOLID, 3, RGB(128, 0, 128));//紫色线段
			hpenXYOld = (HPEN)SelectObject(hdc, hpenXY);
			//for (int i = 0; i <= n; i++) {
		//		temp1[i] = 25 * temp1[i] + 270;
		//	}
			//POINT apt2[] = {50,398,85,432,255,304,409,264,435,129,509,264};
			Polyline(hdc,apt1,tmp);
			break;
		}
		}
		break;
	}


	case WM_MOUSEMOVE: {//实现拖拽
		xPos = LOWORD(lParam);
		yPos = HIWORD(lParam);
		break;
	}


	case WM_MOUSEWHEEL: {//缩放的实现,每次滚动放大/缩小1.2倍
		//InvalidateRect(hWnd, NULL, TRUE);//刷新
		int fwKeys = HIWORD(wParam);
		//float a = 1;
		//InvalidateRect(hWnd, NULL, TRUE);//刷新
		TCHAR buf1[10];
		TCHAR buf2[10];
		TCHAR buf3[10];
		TCHAR buf4[10];
		TCHAR buf5[10];
		::SendMessage(fun1, WM_GETTEXT, 10, (LPARAM)buf1);
		::SendMessage(fun2, WM_GETTEXT, 10, (LPARAM)buf2);
		::SendMessage(fun3, WM_GETTEXT, 10, (LPARAM)buf3);
		::SendMessage(fun4, WM_GETTEXT, 10, (LPARAM)buf4);
		::SendMessage(fun5, WM_GETTEXT, 10, (LPARAM)buf5);
		::wsprintf(buf1, L"%s", buf1);
		::wsprintf(buf2, L"%s", buf2);
		::wsprintf(buf3, L"%s", buf3);
		::wsprintf(buf4, L"%s", buf4);
		::wsprintf(buf5, L"%s", buf5);
		//FILE *fp;
		//fopen_s(&fp, "test.txt", "w+");
		if (120 == fwKeys) {//放大
			//MessageBox(hWnd, L"das",L"s", MB_OK);
			//fprintf(fp, "x=%f y=%f\n", xPos, yPos);//同输出printf一样，以格式方式输出到文本中
			zoom += 0.2;
			InvalidateRect(hWnd, NULL, TRUE);//刷新
			//Parse(buf1, buf2, buf3, buf4, buf5,hWnd,false);//作图
			Zoom(buf1, buf2, buf3, buf4, buf5,hWnd,zoom);

		}
		else  {//缩小
			//MessageBox(hWnd, L"das",L"b" , MB_OK);
			//fprintf(fp, "x=%f y=%f\n", xPos, yPos);//同输出printf一样，以格式方式输出到文本中
			zoom -= 0.2;
			InvalidateRect(hWnd, NULL, TRUE);//刷新
			//Parse(buf1, buf2, buf3, buf4, buf5,hWnd,false);//作图
			Zoom(buf1, buf2, buf3, buf4, buf5, hWnd, zoom);

		}
		//fclose(fp);//关闭流
		break;
	}
	case WM_PAINT://基本图形框架
	{
		// TODO: 在此处添加使用 hdc 的任何绘图代码...

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		TCHAR buf[256];

		//绘制控制台
		RECT rect;
		HBRUSH hBrush = (HBRUSH)GetStockObject(DC_BRUSH);
		rect.left = 600;   //向下竖排
		rect.right = 630;

		SetDCBrushColor(hdc, RGB(0, 0, 255));//蓝色
		rect.bottom = 80;
		rect.top = 50;
		FillRect(hdc, &rect, hBrush);

		SetDCBrushColor(hdc, RGB(255, 0, 0));//红色
		rect.bottom = 130;
		rect.top = 100;
		FillRect(hdc, &rect, hBrush);

		SetDCBrushColor(hdc, RGB(0, 255, 0));//绿色
		rect.bottom = 180;
		rect.top = 150;
		FillRect(hdc, &rect, hBrush);

		SetDCBrushColor(hdc, RGB(128, 0, 128));//紫色
		rect.bottom = 230;
		rect.top = 200;
		FillRect(hdc, &rect, hBrush);

		SetDCBrushColor(hdc, RGB(160, 160, 164));//灰色
		rect.bottom = 280;
		rect.top = 250;
		FillRect(hdc, &rect, hBrush);

		WCHAR fx[10] = L"F(x)=";

		for (int i = 0; i < 5; i++) {
			::wsprintf(buf, L"%d", i + 1);
			buf[1] = '.';
			TextOut(hdc, 640, 60 + i * 50, buf, 2);
			TextOut(hdc, 660, 60 + i * 50, fx, 5);
		}
		//以上为5个函数输入框

		//绘制坐标系
		HPEN hpenXY, hpenXYOld;
		hpenXY = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		hpenXYOld = (HPEN)SelectObject(hdc, hpenXY);
		MoveToEx(hdc, 10, 300, NULL);
		LineTo(hdc, 530, 300);
		MoveToEx(hdc, 530, 300, NULL);
		LineTo(hdc, 525, 295);
		MoveToEx(hdc, 530, 300, NULL);
		LineTo(hdc, 525, 305);

		MoveToEx(hdc, 270, 30, NULL);
		LineTo(hdc, 270, 550);
		MoveToEx(hdc, 270, 30, NULL);
		LineTo(hdc, 275, 35);
		MoveToEx(hdc, 270, 30, NULL);
		LineTo(hdc, 265, 35);

		TextOut(hdc, 275, 305, L"0", 1);//坐标原点
		TextOut(hdc, 265, 10, L"Y",1);
		TextOut(hdc, 535, 300, L"X", 1);
		SelectObject(hdc, hpenXYOld);
		DeleteObject(hpenXY);

		//绘制坐标系中的虚线
		HPEN hpen, hpenOld;

		//网格
		hpen = CreatePen(PS_DASH, 2, RGB(216, 216, 216));
		hpenOld = (HPEN)SelectObject(hdc, hpen);
		for (int i = 0; i < 21; i++) {
			if (i != 10) {
				MoveToEx(hdc, 10, 550 - 25 * i, NULL);
				LineTo(hdc, 530, 550 - 25 * i);
				MoveToEx(hdc, 20 + i * 25, 30, NULL);
				LineTo(hdc, 20 + i * 25, 550);
			}
		}
		int count = 0;

		while (count < 21)
		{
			hpenXY = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
			hpenXYOld = (HPEN)SelectObject(hdc, hpenXY);

			MoveToEx(hdc, 20 + count * 25, 295, NULL);
			LineTo(hdc, 20 + count * 25, 305);

			SelectObject(hdc, hpenXYOld);
			DeleteObject(hpenXY);

			::wsprintf(buf, L"%d", count - 10);
			if (count == 0)
				TextOut(hdc, 15, 305, buf, 3);
			else if (count > 0 && count < 10 || count == 20)
				TextOut(hdc, 15 + count * 25, 305, buf, 2);
			else if (count != 10)
				TextOut(hdc, 15 + count * 25, 305, buf, 1);

			hpenXY = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
			hpenXYOld = (HPEN)SelectObject(hdc, hpenXY);

			MoveToEx(hdc, 265, 550 - count * 25, NULL);
			LineTo(hdc, 275, 550 - count * 25);

			SelectObject(hdc, hpenXYOld);
			DeleteObject(hpenXY);

			::wsprintf(buf, L"%d", count - 10);
			if (count == 0)
				TextOut(hdc, 250, 540, buf, 3);
			else if (count > 0 && count < 10 || count == 20)
				TextOut(hdc, 250, 545 - count * 25, buf, 2);
			else if (count != 10)
				TextOut(hdc, 250, 545 - count * 25, buf, 1);

			count++;
		}

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return ::DefWindowProc(hWnd, Msg, wParam, lParam);
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,//判断是否有一个相同类型程序打开
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow) {
	WNDCLASSEX wc;
	ATOM atom;
	HWND hwnd;
	MSG msg;

	::ZeroMemory(&wc, sizeof(wc));
	wc.style = CS_HREDRAW;
	wc.cbSize = sizeof(wc);
	wc.hInstance = hInstance;
	wc.lpszClassName = L"HELLO";
	wc.hbrBackground = (HBRUSH)(CreatePen(PS_DASH, 5, RGB(238, 229, 222)));
	wc.lpfnWndProc = MyWinProc;

	atom = ::RegisterClassEx(&wc);
	hwnd = CreateWindow(L"HELLO", L"PlottingTool", WS_OVERLAPPEDWINDOW, 200, 200, 900, 610, NULL, NULL, hInstance, NULL);
	::ShowWindow(hwnd, SW_SHOWNORMAL);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);//与键盘的交互
		DispatchMessage(&msg);
	}
	return 0;
}