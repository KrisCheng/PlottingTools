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
void TrySeveDeskBmp(HWND RiWnd = NULL);//��Ҫ���صĴ��ھ��
void ChangColor(char*data, int szie, int BitsPixel);//��ɺڰ�
void TrySeveDeskBmp(HWND RiWnd)
{
	if (RiWnd != NULL)
		ShowWindow(RiWnd, SW_HIDE);
	Sleep(100);
	HDC HDeskDC = GetDC(GetDesktopWindow());//���洰�ڻ�ͼ���
	HDC MemDC = CreateCompatibleDC(HDeskDC);//�������豸
	RECT rc;
	GetClientRect(GetDesktopWindow(), &rc);//���洰�ڴ�С
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	HBITMAP hBmp = CreateCompatibleBitmap(HDeskDC, width, height);//������Ϊ�����Ĵ���λͼ
	SelectObject(MemDC, hBmp);
	BitBlt(MemDC, 0, 0, width, height, HDeskDC, 0, 0, SRCCOPY);//������ͼ�񱣴浽�µĻ�ͼ�豸��
	BITMAP BmpInfo;
	GetObject(hBmp, sizeof(BITMAP), &BmpInfo);//��ȡλͼ��Ϣ
	int datasize = BmpInfo.bmWidthBytes*BmpInfo.bmHeight;
	char*data = (char*)malloc(datasize);
	if (data == NULL)
	{
		printf("malloc err!\n");
		return;
	}

	BITMAPINFO binfo = { 0 };//bmp��Ϣ

	binfo.bmiHeader.biBitCount = BmpInfo.bmBitsPixel;//��ɫλ��
	binfo.bmiHeader.biClrImportant = 0;
	binfo.bmiHeader.biCompression = 0;
	binfo.bmiHeader.biHeight = height;
	binfo.bmiHeader.biPlanes = BmpInfo.bmPlanes;//����?����Ϊ1
	binfo.bmiHeader.biSize = sizeof(BITMAPINFO);
	binfo.bmiHeader.biSizeImage = datasize;
	binfo.bmiHeader.biWidth = width;
	binfo.bmiHeader.biXPelsPerMeter = 0;
	binfo.bmiHeader.biYPelsPerMeter = 0;

	GetDIBits(MemDC, hBmp, 0, BmpInfo.bmHeight, data, &binfo, DIB_RGB_COLORS);//RGB���ݻ�ȡ
																			  //����Ϊ�豸�����λͼ�������ʼɨ���У�����ɨ���У�Ҫ�������������λͼ��Ϣ����ɫ��ʽ
																			  //(��ǰ������ɫ�����鱣��,���ڶ�24��32��������)
																			  //	ChangColor(data,datasize,BmpInfo.bmBitsPixel);//��ɺڰ�
	BITMAPFILEHEADER bfh = { 0 };//bmp�ļ�ͷ
	bfh.bfType = MAKEWORD('B', 'M');
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO);//����ƫ��
	bfh.bfSize = bfh.bfOffBits + datasize;//�ļ��ܴ�С
	FILE *fp;
	fopen_s(&fp, "screenshot.bmp", "wb");

	if (fp == NULL)
		return;
	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);
	fwrite(&binfo, 1, sizeof(BITMAPINFO), fp);
	fwrite(data, 1, BmpInfo.bmWidthBytes*BmpInfo.bmHeight, fp);
	fclose(fp);
	free(data);
	DeleteObject(hBmp);//ɾ���������豸
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