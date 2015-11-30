#pragma once
#include<windows.h>
#include<stdio.h>
#include <iostream>
#include <stack>
#include <string.h>
#include <cctype>
#include <cstdio>
#include <cmath>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include<sstream>
#include"Expression.h"
using namespace std;

TCHAR *chr2wch(const char *buffer) {
	size_t len = strlen(buffer);
	size_t wlen = MultiByteToWideChar(CP_ACP, 0, (const char*)buffer, int(len), NULL, 0);
	wchar_t *wBuf  = new wchar_t[wlen+1];
	MultiByteToWideChar(CP_ACP, 0, (const char*)buffer, int(len), wBuf, int(wlen));
	return wBuf;
}

string TCHAR2STRING(TCHAR *STR)
{
int iLen = WideCharToMultiByte(CP_ACP, 0,STR, -1, NULL, 0, NULL, NULL);
char* chRtn = new char[iLen*sizeof(char)];
WideCharToMultiByte(CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL);
std::string str(chRtn);
return str;
}

boolean Parse(TCHAR *buf1, TCHAR *buf2, TCHAR *buf3, TCHAR *buf4, TCHAR *buf5, HWND hWnd,boolean judge) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	TCHAR buf[256];
	HPEN hpenXY, hpenXYOld;
	//重新绘制控制台
	if(judge){
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
	}
	//词法解析部分,逐个函数进行解析
	//将TCHAR传入string中
	
	if (buf1[0] != NULL) {
		string expr = TCHAR2STRING(buf1);
		string temp3 = "";//最终转换字符串
		string temp2 = expr;//初始字符串
		float num = 0;//临时变量
		float cursor[200][2];
		int k = 0;
		for (float j = -10.0; j <= 10.0; j += 0.1, k++) {
			for (int i = 0; i < expr.length(); i++) {
				if ('x' == expr[i]) {
					char str[10];
					int err;
					if ((-0.1 > j) || (j > 0.1)) {
						err = _gcvt_s(str, 10, j, 5);
						string s = str;
						for (int m = 0; m < s.length(); m++) {
							temp3 = temp3 + s[m];
						}
					}
					else {
						temp3 = temp3 + '0';
					}
				}
				else {
					temp3 = temp3 + expr[i];
				}
			}
			num = Compute(temp3);
			expr = temp2;
			temp3 = "";
			//坐标到像素点的转换
			cursor[k][0] = 25 * j + 270;
			cursor[k][1] = -25 * num + 300;
			//fprintf(fp, "x=%f y=%f\n", cursor[k][0], cursor[k][1]);//同输出printf一样，以格式方式输出到文本中
		}
		hpenXY = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));//蓝色线段
		hpenXYOld = (HPEN)SelectObject(hdc, hpenXY);
		//考虑作图，200个点的连接
		bool temp = true;
		int number = 0, tmp = 0;
		float t1 = 0, t2 = 0;
		POINT apt[200];//无非法点
		POINT apt1[200];
		for (int x = 0; x < 200; x++) {
			if ((20 <= cursor[x][0]) && (cursor[x][0] <= 520) && (50 <= cursor[x][1]) && (cursor[x][1] <= 550)) {//合法点
				apt[x].x = cursor[x][0];
				apt[x].y = cursor[x][1];
				number = x;
			}
			else {//非法点
				apt[x].x = 1000;
				apt[x].y = 1000;
			}
		}
		for (int z = 0; z < 200; z++) {
			if (!((1000 == apt[z].x) && (1000 == apt[z].y))) {
				apt1[tmp].x = apt[z].x;
				apt1[tmp].y = apt[z].y;
				tmp++;
			}
		}
		Polyline(hdc, apt1, tmp);
		expr = "F(x)=" + expr;
		char a[20] = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
		for (int y = 0; y <= expr.length(); y++) {
			a[y] = expr[y];
		}
		TCHAR *text = chr2wch(a);
		TextOut(hdc, cursor[number][0], cursor[number][1], text, expr.length());
		//fclose(fp);//关闭流
	}



if (buf2[0] != NULL) {
		string expr = TCHAR2STRING(buf2);
		string temp3 = "";//最终转换字符串
		string temp2 = expr;//初始字符串
		float num = 0;//临时变量
		float cursor[200][2];
		int k = 0;
		for (float j = -10.0; j <= 10.0; j += 0.1, k++) {
			for (int i = 0; i < expr.length(); i++) {
				if ('x' == expr[i]) {
					char str[10];
					int err;
					if ((-0.1 > j) || (j > 0.1)) {
						err = _gcvt_s(str, 10, j, 5);
						string s = str;
						for (int m = 0; m < s.length(); m++) {
							temp3 = temp3 + s[m];
						}
					}
					else {
						temp3 = temp3 + '0';
					}
				}
				else {
					temp3 = temp3 + expr[i];
				}
			}
			num = Compute(temp3);
			expr = temp2;
			temp3 = "";
			//坐标到像素点的转换
			cursor[k][0] = 25 * j + 270;
			cursor[k][1] = -25 * num + 300;
			//fprintf(fp, "x=%f y=%f\n", cursor[k][0], cursor[k][1]);//同输出printf一样，以格式方式输出到文本中
		}
		hpenXY = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));//红色线段
		hpenXYOld = (HPEN)SelectObject(hdc, hpenXY);
		//考虑作图，200个点的连接
		bool temp = true;
		int number = 0, tmp = 0;
		float t1 = 0, t2 = 0;
		POINT apt[200];//无非法点
		POINT apt1[200];
		for (int x = 0; x < 200; x++) {
			if ((20 <= cursor[x][0]) && (cursor[x][0] <= 520) && (50 <= cursor[x][1]) && (cursor[x][1] <= 550)) {//合法点
				apt[x].x = cursor[x][0];
				apt[x].y = cursor[x][1];
				number = x;
			}
			else {//非法点
				apt[x].x = 1000;
				apt[x].y = 1000;
			}
		}
		for (int z = 0; z < 200; z++) {
			if (!((1000 == apt[z].x) && (1000 == apt[z].y))) {
				apt1[tmp].x = apt[z].x;
				apt1[tmp].y = apt[z].y;
				tmp++;
			}
		}
		Polyline(hdc, apt1, tmp);
		expr = "F(x)=" + expr;
		char a[20] = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
		for (int y = 0; y <= expr.length(); y++) {
			a[y] = expr[y];
		}
		TCHAR *text = chr2wch(a);
		TextOut(hdc, cursor[number][0], cursor[number][1], text, expr.length());
		//fclose(fp);//关闭流
	}


	if (buf3[0] != NULL) {
		string expr = TCHAR2STRING(buf3);
		string temp3 = "";//最终转换字符串
		string temp2 = expr;//初始字符串
		float num = 0;//临时变量
		float cursor[200][2];
		int k = 0;
		for (float j = -10.0; j <= 10.0; j += 0.1, k++) {
			for (int i = 0; i < expr.length(); i++) {
				if ('x' == expr[i]) {
					char str[10];
					int err;
					if ((-0.1 > j) || (j > 0.1)) {
						err = _gcvt_s(str, 10, j, 5);
						string s = str;
						for (int m = 0; m < s.length(); m++) {
							temp3 = temp3 + s[m];
						}
					}
					else {
						temp3 = temp3 + '0';
					}
				}
				else {
					temp3 = temp3 + expr[i];
				}
			}
			num = Compute(temp3);
			expr = temp2;
			temp3 = "";
			//坐标到像素点的转换
			cursor[k][0] = 25 * j + 270;
			cursor[k][1] = -25 * num + 300;
			//fprintf(fp, "x=%f y=%f\n", cursor[k][0], cursor[k][1]);//同输出printf一样，以格式方式输出到文本中
		}
		hpenXY = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));//绿色线段
		hpenXYOld = (HPEN)SelectObject(hdc, hpenXY);
		//考虑作图，200个点的连接
		bool temp = true;
		int number = 0, tmp = 0;
		float t1 = 0, t2 = 0;
		POINT apt[200];//无非法点
		POINT apt1[200];
		for (int x = 0; x < 200; x++) {
			if ((20 <= cursor[x][0]) && (cursor[x][0] <= 520) && (50 <= cursor[x][1]) && (cursor[x][1] <= 550)) {//合法点
				apt[x].x = cursor[x][0];
				apt[x].y = cursor[x][1];
				number = x;
			}
			else {//非法点
				apt[x].x = 1000;
				apt[x].y = 1000;
			}
		}
		for (int z = 0; z < 200; z++) {
			if (!((1000 == apt[z].x) && (1000 == apt[z].y))) {
				apt1[tmp].x = apt[z].x;
				apt1[tmp].y = apt[z].y;
				tmp++;
			}
		}
		Polyline(hdc, apt1, tmp);
		expr = "F(x)=" + expr;
		char a[20] = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
		for (int y = 0; y <= expr.length(); y++) {
			a[y] = expr[y];
		}
		TCHAR *text = chr2wch(a);
		TextOut(hdc, cursor[number][0], cursor[number][1], text, expr.length());
		//fclose(fp);//关闭流
	}


	if (buf4[0] != NULL) {
		string expr = TCHAR2STRING(buf4);
		string temp3 = "";//最终转换字符串
		string temp2 = expr;//初始字符串
		float num = 0;//临时变量
		float cursor[200][2];
		int k = 0;
		for (float j = -10.0; j <= 10.0; j += 0.1, k++) {
			for (int i = 0; i < expr.length(); i++) {
				if ('x' == expr[i]) {
					char str[10];
					int err;
					if ((-0.1 > j) || (j > 0.1)) {
						err = _gcvt_s(str, 10, j, 5);
						string s = str;
						for (int m = 0; m < s.length(); m++) {
							temp3 = temp3 + s[m];
						}
					}
					else {
						temp3 = temp3 + '0';
					}
				}
				else {
					temp3 = temp3 + expr[i];
				}
			}
			num = Compute(temp3);
			expr = temp2;
			temp3 = "";
			//坐标到像素点的转换
			cursor[k][0] = 25 * j + 270;
			cursor[k][1] = -25 * num + 300;
			//fprintf(fp, "x=%f y=%f\n", cursor[k][0], cursor[k][1]);//同输出printf一样，以格式方式输出到文本中
		}
		hpenXY = CreatePen(PS_SOLID, 3, RGB(128, 0, 128));//紫色线段
		hpenXYOld = (HPEN)SelectObject(hdc, hpenXY);
		//考虑作图，200个点的连接
		bool temp = true;
		int number = 0, tmp = 0;
		float t1 = 0, t2 = 0;
		POINT apt[200];//无非法点
		POINT apt1[200];
		for (int x = 0; x < 200; x++) {
			if ((20 <= cursor[x][0]) && (cursor[x][0] <= 520) && (50 <= cursor[x][1]) && (cursor[x][1] <= 550)) {//合法点
				apt[x].x = cursor[x][0];
				apt[x].y = cursor[x][1];
				number = x;
			}
			else {//非法点
				apt[x].x = 1000;
				apt[x].y = 1000;
			}
		}
		for (int z = 0; z < 200; z++) {
			if (!((1000 == apt[z].x) && (1000 == apt[z].y))) {
				apt1[tmp].x = apt[z].x;
				apt1[tmp].y = apt[z].y;
				tmp++;
			}
		}
		Polyline(hdc, apt1, tmp);
		expr = "F(x)=" + expr;
		char a[20] = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
		for (int y = 0; y <= expr.length(); y++) {
			a[y] = expr[y];
		}
		TCHAR *text = chr2wch(a);
		TextOut(hdc, cursor[number][0], cursor[number][1], text, expr.length());
		//fclose(fp);//关闭流
	}


  if (buf5[0] != NULL) {
		string expr = TCHAR2STRING(buf5);
		string temp3 = "";//最终转换字符串
		string temp2 = expr;//初始字符串
		float num = 0;//临时变量
		float cursor[200][2];
		int k = 0;
		for (float j = -10.0; j <= 10.0; j += 0.1, k++) {
			for (int i = 0; i < expr.length(); i++) {
				if ('x' == expr[i]) {
					char str[10];
					int err;
					if ((-0.1 > j) || (j > 0.1)) {
						err = _gcvt_s(str, 10, j, 5);
						string s = str;
						for (int m = 0; m < s.length(); m++) {
							temp3 = temp3 + s[m];
						}
					}
					else {
						temp3 = temp3 + '0';
					}
				}
				else {
					temp3 = temp3 + expr[i];
				}
			}
			num = Compute(temp3);
			expr = temp2;
			temp3 = "";
			//坐标到像素点的转换
			cursor[k][0] = 25 * j + 270;
			cursor[k][1] = -25 * num + 300;
			//fprintf(fp, "x=%f y=%f\n", cursor[k][0], cursor[k][1]);//同输出printf一样，以格式方式输出到文本中
		}
		hpenXY = CreatePen(PS_SOLID, 3, RGB(160, 160, 164));//灰色线段
		hpenXYOld = (HPEN)SelectObject(hdc, hpenXY);
		//考虑作图，200个点的连接
		bool temp = true;
		int number = 0, tmp = 0;
		float t1 = 0, t2 = 0;
		POINT apt[200];//无非法点
		POINT apt1[200];
		for (int x = 0; x < 200; x++) {
			if ((20 <= cursor[x][0]) && (cursor[x][0] <= 520) && (50 <= cursor[x][1]) && (cursor[x][1] <= 550)) {//合法点
				apt[x].x = cursor[x][0];
				apt[x].y = cursor[x][1];
				number = x;
			}
			else {//非法点
				apt[x].x = 1000;
				apt[x].y = 1000;
			}
		}
		for (int z = 0; z < 200; z++) {
			if (!((1000 == apt[z].x) && (1000 == apt[z].y))) {
				apt1[tmp].x = apt[z].x;
				apt1[tmp].y = apt[z].y;
				tmp++;
			}
		}
		Polyline(hdc, apt1, tmp);
		expr = "F(x)=" + expr;
		char a[20] = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ' };
		for (int y = 0; y <= expr.length(); y++) {
			a[y] = expr[y];
		}
		TCHAR *text = chr2wch(a);
		TextOut(hdc, cursor[number][0], cursor[number][1], text, expr.length());
		//fclose(fp);//关闭流
	}
	return true;
}