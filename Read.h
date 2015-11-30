#pragma once
#include <iostream>
#include <string>
using namespace std;
void tempFun(char* cArr)
{
	int iSpaceCounter = 0;
	int iSpaceMarker = -1;
	int iNotSpaceMarker = -1;
	int iNotSpace = -1;
	bool bMeetNotSpace = false;
	for (int i = 0; i<strlen(cArr); ++i)
	{
		if (cArr[i] == ' ')
		{
			++iSpaceCounter;

			if (iSpaceCounter == 2 && bMeetNotSpace)
			{
				iSpaceMarker = i;
			}
			else if (i == 0)
			{
				iSpaceMarker = 0;
			}
		}
		else
		{
			if (iSpaceCounter > 1 || !bMeetNotSpace)
			{
				char temp = cArr[i];
				cArr[i] = cArr[iSpaceMarker];
				cArr[iSpaceMarker] = temp;
				iNotSpace = iSpaceMarker;
				iSpaceCounter -= 2;
				if (iSpaceCounter < 0)
				{
					iSpaceCounter = 0;
				}
				iSpaceMarker += 2;
			}
			else
			{
				iNotSpace = i;
			}

			bMeetNotSpace = true;
		}
	}
	cArr[iNotSpace + 1] = '\0';
}

char* zhuanhuan(string src)//字符数组转换成string函数
{
	char *dst = new char[255];
	int i;
	for (i = 0; i <= src.length(); i++)
		dst[i] = src[i];
	dst[i] = '\0';
	return dst;
}

void paixu(float a[],float b[], int N)//排序函数
{
	double temp;
	int i, j;
	for (i = 0; i<N; i++)
		for (j = i + 1; j<N; j++)
		{
			if (a[i]>a[j])
			{
				temp = a[i]; a[i] = a[j]; a[j] = temp;
				temp = b[i]; b[i] = b[j]; b[j] = temp;
			}
		}
}