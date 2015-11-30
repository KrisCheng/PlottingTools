#pragma once
#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cstdio>
#include <cmath>

using namespace std;

/**
* �Ƚ�����������ȼ�
* ���ظ�true
* ���false
*/
bool cmpPRI(char a, char b)
{
	//�������ȼ�
	short priList[129];
	priList['\0'] = 16; priList['('] = 1;
	priList['s'] = 1;	//sin
	priList['c'] = 1;	//cos
	priList['t'] = 1;	//tan
	priList['q'] = 1;	//sqrt
	priList['l'] = 1;	//ln
	priList['i'] = 1;	//if *
	priList['^'] = 2; priList['*'] = 3; priList['/'] = 3;
	priList['+'] = 4; priList['-'] = 4;
	priList['<'] = 6;
	priList[','] = 15;

	if (a == b && a == '^') return true;	//�ҽ�������������
	if (priList[a] < priList[b]) return true;
	else return false;
}

/**
* ������ת��Ϊ���ֽ������
*/
char Fun2Op(string fun)
{
	if (fun == "sin") return 's';
	else if (fun == "cos") return 'c';
	else if (fun == "tan") return 't';
	else if (fun == "sqrt") return 'q';
	else if (fun == "log") return 'l';
	else if (fun == "if") return 'i';
	else return ' ';
}

/**
* ִ���������
* ���������������ջ
* �޸�ջ������
*/
void OpC(char op, stack<double> &rExpr)
{
	double a, b, c;	//��Ҫ���������
					//��������
					//�����������������
	switch (op)
	{
	case '+':
		b = rExpr.top(); rExpr.pop();
		a = rExpr.top(); rExpr.pop();
		rExpr.push(a + b);
		break;

	case '-':
		b = rExpr.top(); rExpr.pop();
		a = rExpr.top(); rExpr.pop();
		rExpr.push(a - b);
		break;

	case '*':
		b = rExpr.top(); rExpr.pop();
		a = rExpr.top(); rExpr.pop();
		rExpr.push(a * b);
		break;

	case '/':
		b = rExpr.top(); rExpr.pop();
		a = rExpr.top(); rExpr.pop();
		rExpr.push(a / b);
		break;

	case '^':
		b = rExpr.top(); rExpr.pop();
		a = rExpr.top(); rExpr.pop();
		rExpr.push(pow(a, b));
		break;

	case '<':
		b = rExpr.top(); rExpr.pop();
		a = rExpr.top(); rExpr.pop();
		rExpr.push(a < b);
		break;

	case 's':
		a = rExpr.top(); rExpr.pop();
		rExpr.push(sin(a));
		break;

	case 'c':
		a = rExpr.top(); rExpr.pop();
		rExpr.push(cos(a));
		break;

	case 't':
		a = rExpr.top(); rExpr.pop();
		rExpr.push(tan(a));
		break;

	case 'q':
		a = rExpr.top(); rExpr.pop();
		rExpr.push(sqrt(a));
		break;

	case 'l':
		a = rExpr.top(); rExpr.pop();
		rExpr.push(log(a));
		break;

	case 'i':
		c = rExpr.top(); rExpr.pop();
		b = rExpr.top(); rExpr.pop();
		a = rExpr.top(); rExpr.pop();
		rExpr.push(a ? b : c);
		break;

	default:
		break;
	}
}

/**
* ��ǰ׺ʽת��Ϊ��׺ʽ������
* ������ʽ�����Ĵ�ջ
* ���ر��ʽ�ļ�����
*/
float Compute(string expr)
{

	stack<double> rExpr;	//ת����ı��ʽ
	stack<char> opS;		//����
	opS.push(0);

	string::size_type eLen(expr.size());	//��¼������ʽ�ĳ���

	bool isDigit(false);	//��¼���ʽ�Ƿ������ֵ���ɲ���
	bool isFun(false);//��¼���ʽ�Ƿ��Ǻ�������ɲ���
	bool isRBracket(false);//��¼���ʽλ�����Ƿ�Ϊ������
	string dTmp;			//�ݴ�����
	string fTmp;			//�ݴ溯����

							//ѭ��������ʽ������Ӧ��ջ
	for (string::size_type i(0); i < eLen; ++i)
	{
		//Ԥ�����ַ���
		if (expr[i] == ' ' || expr[i] == '\n') continue;
		if (expr[i] == '-' && !isDigit && !isRBracket) rExpr.push(0.0);
		//�������ж��Ƿ������һ�����֣���һ���֣�
		if (isdigit(expr[i]) || expr[i] == '.') isDigit = true;
		else isDigit = false;
		//�����ж��Ƿ������һ����������һ���֣�
		if (islower(expr[i])) isFun = true;
		else isFun = false;
		//�����жϵ�ǰλ���Ƿ�Ϊ������
		if (expr[i] == ')') isRBracket = true;
		else isRBracket = false;

		if (isDigit) dTmp += expr[i];	//���ʽ�е���������ݴ�
		else if (isFun) fTmp += expr[i];	//���ʽ�еĺ�������ݴ�
		else
		{
			if (!dTmp.empty()) rExpr.push(strtod(dTmp.c_str(), 0));	//��һ����������ʽջ
			dTmp = "";
			if (expr[i] == ')')
			{
				//��Ϊ�����ţ������󵯳����������
				while (opS.top() != '(')
				{
					OpC(opS.top(), rExpr);	//��������
					opS.pop();
				}
				opS.pop();	//����������
			}
			else if (cmpPRI(expr[i], opS.top()) || opS.top() == '(')	//�����ǰ��������ȼ����������ջ
			{
				if (!fTmp.empty())
				{
					opS.push(Fun2Op(fTmp));	//���뺯������
					opS.push(expr[i]);	//�����������ջ
					fTmp = "";
				}
				else
					opS.push(expr[i]);	//�����������ջ
			}
			else
			{
				OpC(opS.top(), rExpr);	//��������
				opS.pop();
				//���������ջ��ǰһ����������ȼ���������
				if (!cmpPRI(expr[i], opS.top()) && opS.top() != '(')
				{
					OpC(opS.top(), rExpr);	//��������
					opS.pop();
				}
				opS.push(expr[i]);	//�·����������ջ
			}
		}
	}
	if (!dTmp.empty()) rExpr.push(strtod(dTmp.c_str(), 0));	//���ʽĩβ������ջ
	if (opS.top())
		do
		{
			OpC(opS.top(), rExpr);	//��������
			opS.pop();
		} while (opS.top());
		return rExpr.top();
}