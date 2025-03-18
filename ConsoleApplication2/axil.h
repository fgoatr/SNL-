
#pragma once
#include"total.h"
bool IsDigit(char ch)
{
	if (ch >= '0' && ch <= '9')
		return true;
	else
		return false;
}

// letter check
// 检查是不是字母
bool IsLetter(char ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return true;
	else
		return false;
}

// blank check
// 检查是不是需要blank掉
bool IsBlank(char ch)
{

	if (ch == ' ') // 空格
		return true;
	else if (ch == '\n') // 换行
		return true;
	else if (ch == '\t') // 制表符
		return true;
	else if (ch == '\r') // 回车
		return true;
	else
		return false;
}

// key check
// 检查是不是保留字
bool IsReserved(string ch)
{
	for (int i = 0; i < 21; i++)
		if (ch == reservedWords[i].str)
			return true;
	return false;
}

// find the correspond reversed word
// 在保留字表中找到对应的编号
struct Word GetReserved(string s)
{
	for (int i = 0; i < 21; i++)
	{
		if (reservedWords[i].str == s)
			return reservedWords[i];
	}
}

// operator check
// 检查是不是运算符
bool IsOperator(char ch)
{
	if (ch == '+')
		return true;
	else if (ch == '-')
		return true;
	else if (ch == '*')
		return true;
	else if (ch == '/')
		return true;
	else if (ch == '<')
		return true;
	else if (ch == '=')
		return true;
	else
		return false;
}

// separater cehck
// 检查是不是分隔符
bool IsSeparater(char ch)
{
	if (ch == ';')
		return true;
	else if (ch == ',')
		return true;
	else if (ch == '{')
		return true;
	else if (ch == '}')
		return true;
	else if (ch == '[')
		return true;
	else if (ch == ']')
		return true;
	else if (ch == '(')
		return true;
	else if (ch == ')')
		return true;
	else if (ch == '.')
		return true;
	else if (ch == '\'')
		return true;
	else if (ch == ':')
		return true;
	else
		return false;
}

// output
// 输出到文件
void PrintTokenList()
{
	ofstream output(OUT1);

	int i = 0;
	while (tokenList[i].wd.tok != ENDFILE)
	{
		output << tokenList[i].line
			<< "\t" << std::left << setw(16) << GetString(tokenList[i].wd.tok)
			<< "\t" << tokenList[i].wd.str << endl;

		cout << tokenList[i].line
			<< "\t" << std::left << setw(16) << GetString(tokenList[i].wd.tok)
			<< "\t" << tokenList[i].wd.str << endl;

		i++;
	}
	output << tokenList[i].line
		<< "\t" << std::left << setw(16) << GetString(tokenList[i].wd.tok)
		<< "\t" << tokenList[i].wd.str << endl;

	cout << tokenList[i].line
		<< "\t" << std::left << setw(16) << GetString(tokenList[i].wd.tok)
		<< "\t" << tokenList[i].wd.str << endl;

	output.close();
}

// 把lex从数字转成字符串
string GetString(int lexNum)
{
	if (lexNum == 0)
		return "ENDFILE";
	else if (lexNum == 1)
		return "ERROR";
	/*reserved word*/
	else if (lexNum == 2)
		return "PROGRAM";
	else if (lexNum == 3)
		return "PROCEDURE";
	else if (lexNum == 4)
		return "TYPE";
	else if (lexNum == 5)
		return "VAR";
	else if (lexNum == 6)
		return "IF";
	else if (lexNum == 7)
		return "THEN";
	else if (lexNum == 8)
		return "ELSE";
	else if (lexNum == 9)
		return "FI";
	else if (lexNum == 10)
		return "WHILE";
	else if (lexNum == 11)
		return "DO";
	else if (lexNum == 12)
		return "ENDWH";
	else if (lexNum == 13)
		return "BEGIN";
	else if (lexNum == 14)
		return "END";
	else if (lexNum == 15)
		return "READ";
	else if (lexNum == 16)
		return "WRITE";
	else if (lexNum == 17)
		return "ARRAY";
	else if (lexNum == 18)
		return "OF";
	else if (lexNum == 19)
		return "RECORD";
	else if (lexNum == 20)
		return "RETURN";
	else if (lexNum == 21)
		return "INTEGER";
	else if (lexNum == 22)
		return "CHAR";
	/*multi char*/
	else if (lexNum == 23)
		return "ID";
	else if (lexNum == 24)
		return "INTC";
	else if (lexNum == 25)
		return "CHARC";
	/*special*/
	else if (lexNum == 26)
		return "ASSIGN";
	else if (lexNum == 27)
		return "EQ";
	else if (lexNum == 28)
		return "LT";
	else if (lexNum == 29)
		return "PLUS";
	else if (lexNum == 30)
		return "MINUS";
	else if (lexNum == 31)
		return "TIMES";
	else if (lexNum == 32)
		return "OVER";
	else if (lexNum == 33)
		return "LPAREN";
	else if (lexNum == 34)
		return "RPAREN";
	else if (lexNum == 35)
		return "DOT";
	else if (lexNum == 36)
		return "COLON";
	else if (lexNum == 37)
		return "SEMI";
	else if (lexNum == 38)
		return "COMMA";
	else if (lexNum == 39)
		return "LMIDPAREN";
	else if (lexNum == 40)
		return "RMIDPAREN";
	else if (lexNum == 41)
		return "UNDERANGE";
}

void printErrorMsg(string reason)
{
	reason += '\n';
	ofstream ERROROUT(OUTerror);
	ERROROUT << reason;
	cout << reason;
	ERROROUT.close();
	exit(0);
}