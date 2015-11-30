#pragma once
#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cstdio>
#include <cmath>

using namespace std;

/**
* 比较运算符的优先级
* 返回高true
* 或低false
*/
bool cmpPRI(char a, char b)
{
	//构造优先级
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

	if (a == b && a == '^') return true;	//右结合运算符的特例
	if (priList[a] < priList[b]) return true;
	else return false;
}

/**
* 将函数转换为单字节运算符
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
* 执行运算操作
* 读入运算符和数字栈
* 修改栈内数字
*/
void OpC(char op, stack<double> &rExpr)
{
	double a, b, c;	//需要运算的数字
					//弹出数字
					//根据运算符进行运算
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
* 将前缀式转化为后缀式并计算
* 读入表达式串及寄存栈
* 返回表达式的计算结果
*/
float Compute(string expr)
{

	stack<double> rExpr;	//转换后的表达式
	stack<char> opS;		//符号
	opS.push(0);

	string::size_type eLen(expr.size());	//记录读入表达式的长度

	bool isDigit(false);	//记录表达式是否是数字的组成部分
	bool isFun(false);//记录表达式是否是函数的组成部分
	bool isRBracket(false);//记录表达式位置上是否为右括号
	string dTmp;			//暂存数字
	string fTmp;			//暂存函数名

							//循环读入表达式，并对应入栈
	for (string::size_type i(0); i < eLen; ++i)
	{
		//预处理字符串
		if (expr[i] == ' ' || expr[i] == '\n') continue;
		if (expr[i] == '-' && !isDigit && !isRBracket) rExpr.push(0.0);
		//这里来判断是否读入了一个数字（的一部分）
		if (isdigit(expr[i]) || expr[i] == '.') isDigit = true;
		else isDigit = false;
		//这里判断是否读入了一个函数（的一部分）
		if (islower(expr[i])) isFun = true;
		else isFun = false;
		//这里判断当前位置是否为右括号
		if (expr[i] == ')') isRBracket = true;
		else isRBracket = false;

		if (isDigit) dTmp += expr[i];	//表达式中的数字组合暂存
		else if (isFun) fTmp += expr[i];	//表达式中的函数组合暂存
		else
		{
			if (!dTmp.empty()) rExpr.push(strtod(dTmp.c_str(), 0));	//上一个数字入表达式栈
			dTmp = "";
			if (expr[i] == ')')
			{
				//若为右括号，则向左弹出所有运算符
				while (opS.top() != '(')
				{
					OpC(opS.top(), rExpr);	//符号运算
					opS.pop();
				}
				opS.pop();	//弃用左括号
			}
			else if (cmpPRI(expr[i], opS.top()) || opS.top() == '(')	//如果当前运算符优先级高则入符号栈
			{
				if (!fTmp.empty())
				{
					opS.push(Fun2Op(fTmp));	//存入函数运算
					opS.push(expr[i]);	//符号入运算符栈
					fTmp = "";
				}
				else
					opS.push(expr[i]);	//符号入运算符栈
			}
			else
			{
				OpC(opS.top(), rExpr);	//符号运算
				opS.pop();
				//根据运算符栈的前一个运算符优先级进行运算
				if (!cmpPRI(expr[i], opS.top()) && opS.top() != '(')
				{
					OpC(opS.top(), rExpr);	//符号运算
					opS.pop();
				}
				opS.push(expr[i]);	//新符号入运算符栈
			}
		}
	}
	if (!dTmp.empty()) rExpr.push(strtod(dTmp.c_str(), 0));	//表达式末尾数字入栈
	if (opS.top())
		do
		{
			OpC(opS.top(), rExpr);	//符号运算
			opS.pop();
		} while (opS.top());
		return rExpr.top();
}