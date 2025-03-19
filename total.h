#pragma once
#define _CRT_SECURE_NO_WARNINGS

/****************************************************************/
//	
//	https://github.com/YiChuan0712/SNL-Compiler
//
//	һ�� 2021������
//
//	���������ĸ�ģ��
//		�ʷ�����ģ��
//		�﷨����ģ�飨�ݹ��½�������
//		�﷨����ģ�飨LL(1)������
//		�������ģ�飨���ű����������⣩
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
// ��������ļ�·��
///*
char SOURCE[100] = "..\\test.txt"; // SOURCE Դ����
char OUT1[100] = "..\\token.txt"; // �ʷ�����
char OUT2a[100] = "..\\tree.txt"; // �ݹ��½�
char OUT2b[100] = "..\\LL1.txt"; // LL1
char OUT3[100] = "..\\table.txt"; // ������� ���ű�
char OUTerror[100] = "..\\error.txt"; // ����
char OUTcode[100] = "..\\error.txt";//Ŀ���������
//*/
/****************************************************************/
/****************************************************************/
// ʹ�õĸ���ȫ�ֱ��������ݽṹ
// �ʷ�����
typedef enum
{
	ENDFILE, ERROR,
	PROGRAM, PROCEDURE, TYPE, VAR, IF,
	THEN, ELSE, FI, WHILE, DO, ENDWH,
	BEGIN, END1, READ, WRITE, ARRAY, OF,
	RECORD, RETURN1,
	INTEGER, CHAR1, //INTEGER ��integer
	ID, INTC, CHARC,
	ASSIGN, EQ, LT, PLUS, MINUS,
	TIMES, OVER, LPAREN, RPAREN, DOT,
	COLON, SEMI, COMMA, LMIDPAREN, RMIDPAREN,
	UNDERANGE
}LexType;// �ռ���

// ������
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
	int index = -1; // tokenlist�е�index����ʾ���к�
	Token()
	{
		line = 0;
	}
};// token

Token tokenList[1024];// token��
/****************************************************************/
// �﷨����
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
}NonTerminal; // ���ռ���

typedef LexType Terminal; //�ռ����ı���

// LL1����ջ��һ���ڵ�
struct StackNode
{
	int ntflag;

	NonTerminal n;//0
	Terminal t;//1

	TreeNode* p;

	struct StackNode* next;

};
// LL1����ջ
class AnalysisStack
{
public:
	AnalysisStack()
	{
		isEmpty = 1;
		top = NULL;
	}
	StackNode* top; // ע�� pop() �޷���ֵ �鿴ջ��Ԫ������top
	int isEmpty;
	void push(int ntflag, int num)
	{
		StackNode* p;
		p = new StackNode();
		p->next = NULL;
		p->ntflag = -1; //����ӦΪ 0 1
		p->n = (NonTerminal)-1;//�������ֵ���г�ʼ��
		p->t = (Terminal)-1;//�������ֵ���г�ʼ��


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
// �������
class SymbolTable;
// ���ű�ļ�¼�ڵ�
struct SymbolRecord
{
	string name; // ����
	string kind; // ��������
	string type; // ��������
	Token* tk; // ָ��tokenlist
	SymbolTable* next;
};

// ���ű�
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

// ȫ�ֱ��� LL1������� ��Ϊȫ�ֱ�����Ϊ�˷�������������м���ʹ��
TreeNode* treeroot = NULL;

/****************************************************************/
/****************************************************************/

int INDEX = 0; // ����tokenListʹ��
Token* currentToken; // �����﷨��ʱʹ��

string outstr[512]; // ��ӡ�﷨��ʱʹ�� ע���ҵݹ��½���LL1���﷨���Ƿֱ𱣴�� û���ظ�ʹ��ͬһ����
int strline = 0; //����Ϊ�˱����ڸ��Ƕ�дstringʱ����

string outstrLL1[512]; //
int strlineLL1 = 0;

char ch1[3] = "��"; // ȫ���ַ���������֦ ch1[0] ch1[1] �������ַ� ch1[2]��\0 
char ch2[3] = "��"; // ȫ���ַ���������ĺ����׳���...
char ch3[3] = "��";

int LL1Table[104][104]; // LL1�� ������������ϵ����� ��һ�������ĺ������г�ʼ��
TreeNode* currentTree; // LL1��

/****************************************************************/
void Main_Print(TreeNode*);
void Head_Print(TreeNode*);
void Data_Print(TreeNode*);
void Text_Print(TreeNode*);

//Ŀ��������ɵĸ���������������

string SNL_Record(TreeNode*);
string SNL_Int(TreeNode*);
string SNL_Char(TreeNode*);
/****************************************************************/
//SNL�����������岿��
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
void Lx_An(FILE*);//�ķ�������Ҫ���֣�����Ϊ���ı��ļ��ж�ȡ���������з��ŷ���
struct Word GetReserved(string s);
string GetString(int lexNum);
void PrintTokenList();

// �ݹ��½������﷨��ʱ���õĺ��� ��Ϊ�ݹ麯����һ����һ����õ�������Ҫ��ȫ����������
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