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
using namespace std;

//缩放实现函数4
void Zoom(TCHAR *buf1, TCHAR *buf2, TCHAR *buf3, TCHAR *buf4, TCHAR *buf5, HWND hWnd, float n) {//n表示放大倍数
	
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	TCHAR buf[256];
	//重新绘制控制台
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

	TextOut(hdc, 265, 10, L"Y", 1);
	TextOut(hdc, 535, 300, L"X", 1);
	//绘制坐标系,此处重画
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

	SelectObject(hdc, hpenXYOld);
	DeleteObject(hpenXY);

	
	float pos = 270, neg = 270, ypos = 300, yneg = 300;
	int x = 0, y = 0;
	for (; (pos < 500); pos += n * 25, neg -= n * 25, ypos -= n * 25, yneg += n * 25) {
		TextOut(hdc,275, 305, L"0", 1);//坐标原点
		
		hpenXY = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		hpenXYOld = (HPEN)SelectObject(hdc, hpenXY);
		//横正轴
		MoveToEx(hdc, pos, 295, NULL);
		LineTo(hdc, pos, 305);
		//纵正轴
		MoveToEx(hdc, 265, ypos, NULL);
		LineTo(hdc, 275, ypos);
		::wsprintf(buf,L"%d", x);
		if (0 != x ) {
			//以原点为中心描点
			TextOut(hdc, pos, 305, buf, 1);
			TextOut(hdc, 255, ypos, buf, 1);
			HPEN hpen, hpenOld;
			hpen = CreatePen(PS_DASH, 3, RGB(216, 216, 216));
			hpenOld = (HPEN)SelectObject(hdc, hpen);
			MoveToEx(hdc, 10, ypos, NULL);
			LineTo(hdc, 530, ypos);
			MoveToEx(hdc, 10, yneg, NULL);
			LineTo(hdc, 530, yneg);
			MoveToEx(hdc, pos, 30, NULL);
			LineTo(hdc, pos, 550);
			MoveToEx(hdc, neg, 30, NULL);
			LineTo(hdc, neg, 550);
			SelectObject(hdc, hpenOld);
			DeleteObject(hpen);
		}
		//横负轴
		MoveToEx(hdc, neg, 295, NULL);
		LineTo(hdc, neg, 305);
		//纵负轴
		MoveToEx(hdc, 265, yneg, NULL);
		LineTo(hdc, 275, yneg);
		::wsprintf(buf, L"%d", y);
		if (0 != y) {
			TextOut(hdc, neg, 305, buf, 2);
			TextOut(hdc, 255, yneg, buf, 2);
		}
		SelectObject(hdc, hpenXYOld);
		DeleteObject(hpenXY);
		x++;
		y--;
	}
	//下面部分重新绘线
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
			cursor[k][0] = 25*n * j + 270;
			cursor[k][1] = -25 * n * num + 300;
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
			cursor[k][0] = 25 * n * j + 270;
			cursor[k][1] = -25 * n * num + 300;
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
			cursor[k][0] = 25 * n* j + 270;
			cursor[k][1] = -25 * n * num + 300;
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
			cursor[k][0] = 25 * n* j + 270;
			cursor[k][1] = -25 * n * num + 300;
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
			cursor[k][0] = 25 * n * j + 270;
			cursor[k][1] = -25 * n* num + 300;
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
}