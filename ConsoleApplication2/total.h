#pragma once
#define _CRT_SECURE_NO_WARNINGS

/****************************************************************/
//	
//	https://github.com/YiChuan0712/SNL-Compiler
//
//	一川 2021年四月
//
//	包含以下四个模块
//		词法分析模块
//		语法分析模块（递归下降方法）
//		语法分析模块（LL(1)方法）
//		语义分析模块（符号表和语义错误检测）
//
/****************************************************************/

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <fstream> 
#include <string>

using namespace std;

/****************************************************************/
/****************************************************************/
// 输入输出文件路径
///*
char SOURCE[100] = "..\\test.txt"; // SOURCE 源代码
char OUT1[100] = "..\\token.txt"; // 词法分析
char OUT2a[100] = "..\\tree.txt"; // 递归下降
char OUT2b[100] = "..\\LL1.txt"; // LL1
char OUT3[100] = "..\\table.txt"; // 语义分析 符号表
char OUTerror[100] = "..\\error.txt"; // 报错
char OUTcode[100] = "..\\error.txt";//目标代码生成
//*/
/****************************************************************/
/****************************************************************/
// 使用的各种全局变量和数据结构
// 词法分析
typedef enum
{
	ENDFILE, ERROR,
	PROGRAM, PROCEDURE, TYPE, VAR, IF,
	THEN, ELSE, FI, WHILE, DO, ENDWH,
	BEGIN, END1, READ, WRITE, ARRAY, OF,
	RECORD, RETURN1,
	INTEGER, CHAR1, //INTEGER 是integer
	ID, INTC, CHARC,
	ASSIGN, EQ, LT, PLUS, MINUS,
	TIMES, OVER, LPAREN, RPAREN, DOT,
	COLON, SEMI, COMMA, LMIDPAREN, RMIDPAREN,
	UNDERANGE
}LexType;// 终极符

// 保留字
static struct Word
{
	string str;
	LexType tok;
	Word()
	{
		str.clear();
	}
	Word(const char* a, LexType b)
	{
		str = a;
		tok = b;
	}
}
reservedWords[21] =
{
	{"program",PROGRAM},
	{"type",TYPE},
	{"var",VAR},
	{"procedure",PROCEDURE},
	{"begin",BEGIN},
	{"end",END1},
	{"array",ARRAY},
	{"of",OF},
	{"record",RECORD},
	{"if",IF},
	{"then",THEN},
	{"else",ELSE},
	{"fi",FI},
	{"while",WHILE},
	{"do",DO},
	{"endwh",ENDWH},
	{"read",READ},
	{"write",WRITE},
	{"return",RETURN1},
	{"integer",INTEGER},
	{"char",CHAR1}
};

struct Token {
	int line;
	struct Word wd;
	int index = -1; // tokenlist中的index，表示序列号
	Token()
	{
		line = 0;
	}
};// token

Token tokenList[1024];// token表
/****************************************************************/
// 语法分析
class TreeNode
{
public:
	string name;
	Token* tk;
	TreeNode* child[32];
	int childIndex;
	TreeNode(string nm)
	{
		name = nm;
		tk = NULL;
		for (int i = 0; i < 31; i++)
		{
			child[i] = NULL;
		}
		childIndex = 0;
	}
	void addChild(TreeNode* t) {
		child[childIndex] = t;
		childIndex++;
	}
};

typedef enum
{
	Program, ProgramHead, ProgramName, DeclarePart,
	TypeDec, TypeDeclaration, TypeDecList, TypeDecMore,
	TypeId, TypeName, BaseType, StructureType,
	ArrayType, Low, Top, RecType,
	FieldDecList, FieldDecMore, IdList, IdMore,
	VarDec, VarDeclaration, VarDecList, VarDecMore,
	VarIdList, VarIdMore, ProcDec, ProcDeclaration,
	ProcDecMore, ProcName, ParamList, ParamDecList,
	ParamMore, Param, FormList, FidMore,
	ProcDecPart, ProcBody, ProgramBody, StmList,
	StmMore, Stm, AssCall, AssignmentRest,
	ConditionalStm, StmL, LoopStm, InputStm,
	InVar, OutputStm, ReturnStm, CallStmRest,
	ActParamList, ActParamMore, RelExp, OtherRelE,
	Exp, OtherTerm, Term, OtherFactor,
	Factor, Variable, VariMore, FieldVar,
	FieldVarMore, CmpOp, AddOp, MultOp
}NonTerminal; // 非终极符

typedef LexType Terminal; //终极符的别名

// LL1分析栈的一个节点
struct StackNode
{
	int ntflag;

	NonTerminal n;//0
	Terminal t;//1

	TreeNode* p;

	struct StackNode* next;

};
// LL1分析栈
class AnalysisStack
{
public:
	AnalysisStack()
	{
		isEmpty = 1;
		top = NULL;
	}
	StackNode* top; // 注意 pop() 无返回值 查看栈顶元素请用top
	int isEmpty;
	void push(int ntflag, int num)
	{
		StackNode* p;
		p = new StackNode();
		p->next = NULL;
		p->ntflag = -1; //正常应为 0 1
		p->n = (NonTerminal)-1;//按错误的值进行初始化
		p->t = (Terminal)-1;//按错误的值进行初始化


		if (ntflag == 0)
			p->n = (NonTerminal)num;
		else if (ntflag == 1)
			p->t = (Terminal)num;

		p->ntflag = ntflag;
		p->next = top;
		top = p;
		isEmpty = 0;
	}
	void pop()
	{
		top = top->next;
		if (top == NULL)
			isEmpty = 1;
	}
};

AnalysisStack* anlsstack = new AnalysisStack();
/****************************************************************/
// 语义分析
class SymbolTable;
// 符号表的记录节点
struct SymbolRecord
{
	string name; // 名字
	string kind; // 三种类型
	string type; // 具体类型
	Token* tk; // 指向tokenlist
	SymbolTable* next;
};

// 符号表
void printErrorMsg(string);
string outputstr;
class SymbolTable
{
public:
	SymbolTable()
	{
		index = 0;
		paramcount = 0;
	}
	SymbolRecord* table[512];
	int index;
	int paramcount;
	void addRecord(string name, string kind, string type, Token* tk, SymbolTable* next = NULL)
	{
		for (int i = 0; i < index; i++)
		{
			if (name == table[i]->name)
			{
				string temp = "Line ";
				temp += to_string(tk->line);
				temp += " ";
				temp += "\"";
				temp += name;
				temp += "\" ";
				temp += "RepeatDeclaration Error!\n";
				printErrorMsg(temp);
				return;
			}
		}
		table[index] = new SymbolRecord();
		char temp[100];

		table[index]->name = name;
		table[index]->kind = kind;
		table[index]->type = type;
		table[index]->tk = tk;
		table[index]->next = next;
		index++;
	}
	void printTable(int layer = 0)
	{
		for (int i = 0; i < index; i++)
		{
			for (int i = 0; i < layer; i++)
			{
				cout << "\t\t\t\t\t";
				outputstr += "\t\t\t\t\t";
			}

			cout << table[i]->name
				<< "\t"
				<< table[i]->kind
				<< "\t"
				<< table[i]->type << endl;

			outputstr += table[i]->name;
			outputstr += "\t";
			outputstr += table[i]->kind;
			outputstr += "\t";
			outputstr += table[i]->type;
			outputstr += "\n";

			if (table[i]->next)
			{
				//cout << endl;
				//outputstr += "\n";
				table[i]->next->printTable(layer + 1);
				cout << endl;
				outputstr += "\n";
			}
		}

		ofstream output(OUT3);
		output << outputstr;
	}
};

SymbolTable* smbltable = new SymbolTable();

// 全局变量 LL1树根结点 设为全局变量是为了方便在语义分析中继续使用
TreeNode* treeroot = NULL;

/****************************************************************/
/****************************************************************/

int INDEX = 0; // 搭配tokenList使用
Token* currentToken; // 建立语法树时使用

string outstr[512]; // 打印语法树时使用 注意我递归下降和LL1的语法树是分别保存的 没有重复使用同一函数
int strline = 0; //这是为了避免在覆盖读写string时出错

string outstrLL1[512]; //
int strlineLL1 = 0;

char ch1[3] = "├"; // 全角字符用来画树枝 ch1[0] ch1[1] 保存了字符 ch1[2]是\0 
char ch2[3] = "└"; // 全角字符用起来真的很容易出错...
char ch3[3] = "│";

int LL1Table[104][104]; // LL1表 这里采用了书上的做法 用一个单独的函数进行初始化
TreeNode* currentTree; // LL1用

/****************************************************************/
void Main_Print(TreeNode*);
void Head_Print(TreeNode*);
void Data_Print(TreeNode*);
void Text_Print(TreeNode*);

//目标代码生成的辅助函数声明部分

string SNL_Record(TreeNode*);
string SNL_Int(TreeNode*);
string SNL_Char(TreeNode*);
/****************************************************************/
//SNL辅助函数本体部分
string SNL_Record(TreeNode*)
{
	return "";
}

//**************************************************************************
bool IsSeparater(char ch);
bool IsOperator(char ch);
bool IsReserved(string ch);
bool IsBlank(char ch);
bool IsLetter(char ch);
bool IsDigit(char ch);
void Lx_An(FILE*);//文法分析主要部分，功能为从文本文件中读取符号来进行符号分析
struct Word GetReserved(string s);
string GetString(int lexNum);
void PrintTokenList();

// 递归下降建立语法树时所用的函数 因为递归函数是一层套一层调用的所以需要先全部声明出来
TreeNode* program();//
TreeNode* programHead();//
TreeNode* declarePart();
TreeNode* programBody();
TreeNode* typeDecPart();
TreeNode* varDecPart();
TreeNode* procDecpart();
TreeNode* typeDec();
TreeNode* typeDecList();
// TreeNode* typeID();
TreeNode* typeDef();
TreeNode* typeDecMore();
TreeNode* baseType();
TreeNode* structureType();
TreeNode* arrayType();
TreeNode* recType();
TreeNode* fieldDecList();
TreeNode* IDList();
TreeNode* fieldDecMore();
TreeNode* IDMore();
TreeNode* varDec();
TreeNode* varDecList();
TreeNode* varIDList();
TreeNode* varDecMore();
TreeNode* varIDMore();
TreeNode* procDec();
TreeNode* paramList();
TreeNode* procDecPart();
TreeNode* procBody();
TreeNode* paramDecList();
TreeNode* param();
TreeNode* paramMore();
TreeNode* formList();
TreeNode* fidMore();
TreeNode* stmList();
TreeNode* stm();
TreeNode* stmMore();
TreeNode* conditionalStm();
TreeNode* loopStm();
TreeNode* inputStm();
TreeNode* outputStm();
TreeNode* returnStm();
TreeNode* assCall();
TreeNode* assignmentRest();
TreeNode* callStmRest();
TreeNode* variMore();
TreeNode* exp();
TreeNode* actparamList();
TreeNode* actparamMore();
TreeNode* term();
TreeNode* otherTerm();
TreeNode* addOp();
TreeNode* factor();
TreeNode* otherFactor();
TreeNode* multOp();
TreeNode* variable();
TreeNode* fieldVar();
TreeNode* fieldVarMore();

void printErrorMsg(string reason);

void printTree(TreeNode* t, int layer, bool islastson, char out[100]);

void processAddChild(int istmnl, int lex, string name, int childindex);

void process(int num);

void InitLL1Table();


TreeNode* programLL1();