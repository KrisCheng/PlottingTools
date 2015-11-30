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
#include <TIME.H>
#include<sstream>
using namespace std;
void TrySeveDeskBmp(HWND RiWnd = NULL);//需要隐藏的窗口句柄
void ChangColor(char*data, int szie, int BitsPixel);//变成黑白
void TrySeveDeskBmp(HWND RiWnd)
{
	if (RiWnd != NULL)
		ShowWindow(RiWnd, SW_HIDE);
	Sleep(100);
	HDC HDeskDC = GetDC(GetDesktopWindow());//桌面窗口绘图句柄
	HDC MemDC = CreateCompatibleDC(HDeskDC);//创建新设备
	RECT rc;
	GetClientRect(GetDesktopWindow(), &rc);//桌面窗口大小
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	HBITMAP hBmp = CreateCompatibleBitmap(HDeskDC, width, height);//以桌面为上下文创建位图
	SelectObject(MemDC, hBmp);
	BitBlt(MemDC, 0, 0, width, height, HDeskDC, 0, 0, SRCCOPY);//将桌面图像保存到新的绘图设备中
	BITMAP BmpInfo;
	GetObject(hBmp, sizeof(BITMAP), &BmpInfo);//获取位图信息
	int datasize = BmpInfo.bmWidthBytes*BmpInfo.bmHeight;
	char*data = (char*)malloc(datasize);
	if (data == NULL)
	{
		printf("malloc err!\n");
		return;
	}

	BITMAPINFO binfo = { 0 };//bmp信息

	binfo.bmiHeader.biBitCount = BmpInfo.bmBitsPixel;//颜色位数
	binfo.bmiHeader.biClrImportant = 0;
	binfo.bmiHeader.biCompression = 0;
	binfo.bmiHeader.biHeight = height;
	binfo.bmiHeader.biPlanes = BmpInfo.bmPlanes;//级别?必须为1
	binfo.bmiHeader.biSize = sizeof(BITMAPINFO);
	binfo.bmiHeader.biSizeImage = datasize;
	binfo.bmiHeader.biWidth = width;
	binfo.bmiHeader.biXPelsPerMeter = 0;
	binfo.bmiHeader.biYPelsPerMeter = 0;

	GetDIBits(MemDC, hBmp, 0, BmpInfo.bmHeight, data, &binfo, DIB_RGB_COLORS);//RGB数据获取
																			  //依次为设备句柄，位图句柄，开始扫描行，结束扫描行，要保存的数据区，位图信息，颜色方式
																			  //(以前都把颜色用数组保存,现在都24或32无需数组)
																			  //	ChangColor(data,datasize,BmpInfo.bmBitsPixel);//变成黑白
	BITMAPFILEHEADER bfh = { 0 };//bmp文件头
	bfh.bfType = MAKEWORD('B', 'M');
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO);//数据偏移
	bfh.bfSize = bfh.bfOffBits + datasize;//文件总大小
	FILE *fp;
	fopen_s(&fp, "screenshot.bmp", "wb");

	if (fp == NULL)
		return;
	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);
	fwrite(&binfo, 1, sizeof(BITMAPINFO), fp);
	fwrite(data, 1, BmpInfo.bmWidthBytes*BmpInfo.bmHeight, fp);
	fclose(fp);
	free(data);
	DeleteObject(hBmp);//删除创建的设备
	DeleteDC(MemDC);
	if (RiWnd != NULL)
		ShowWindow(RiWnd, SW_SHOW);
}
void ChangColor(char*data, int datazie, int BitsPixel)
{
	int colbit = BitsPixel / 8;

	byte*temp = (byte*)data;
	while (temp - (byte*)data<datazie)
	{
		//	temp[0]=temp[1]=temp[2]=(int)sqrt(temp[0]*temp[0]+temp[1]*temp[1]+temp[2]*temp[2])%256;
		temp[0] = temp[1] = temp[2] = (30 * temp[2] + 59 * temp[1] + 11 * temp[0]) / 100;
		temp += colbit;
	}
}