
#pragma once
#include"total.h"
#include"axil.h"
#include<string>
void Lx_An(FILE* SNL_Code)
{
	if (SNL_Code == NULL)
	{
		cout << "FILE ERROR,PLEASE CHECK AGAIN" << endl;
		return;
	}
	int line = 1;//��ʾ��Դ�����е�����
	int Now_List = 0;//��ʾ�����б��е�λ��,token��¼��
	char tmp = fgetc(SNL_Code);//�ļ�ָ�룬ָ��ǰ���벿��
	string TmpC;//��ʱ������ܴ��ڵı����֡��м���
	//���Ĵ��벿��
	int times = 0;
	while (tmp != EOF)
	{
		if (IsDigit(tmp))
		{
			TmpC.clear();
			TmpC += tmp;
			tmp = fgetc(SNL_Code);
			bool tmp_flag = true;//��ʱ�ı�־���ڲ����������Ƿ�����ĸ
			while (IsDigit(tmp) || IsLetter(tmp))
			{
				if (IsLetter(tmp))
				{ 
					tmp_flag = false;
					tokenList[Now_List].wd.tok = ERROR;
					break;
				}
				TmpC += tmp;
				tmp = fgetc(SNL_Code);
			}
			if (tmp_flag)
				tokenList[Now_List].wd.tok = INTC;
			tokenList[Now_List].line = line;
			tokenList[Now_List].wd.str = TmpC;
			Now_List++;
		}
		else if (IsLetter(tmp)) 
		{
			TmpC.clear();
			TmpC += tmp;
			tmp = fgetc(SNL_Code);
			while (IsDigit(tmp) || IsLetter(tmp))
			{
				TmpC += tmp;
				tmp = fgetc(SNL_Code);
			}
		}
		else if (IsOperator(tmp))
		{
			switch (tmp)
			{
			case '+':
				tokenList[Now_List].wd.tok = PLUS;
				break;
			case'-':
				tokenList[Now_List].wd.tok = MINUS;
				break;
			case'*':
				tokenList[Now_List].wd.tok = TIMES;
				break;
			case'/':
				tokenList[Now_List].wd.tok = OVER;
				break;
			case'=':
				tokenList[Now_List].wd.tok = EQ;
				break;
			case'<':
				tokenList[Now_List].wd.tok = LT;
				break;
			}
			tokenList[Now_List].line = line;
			tokenList[Now_List].wd.str +=tmp;
			Now_List++;
			tmp = fgetc(SNL_Code);
				
		}
		else if (IsSeparater(tmp))
		{
			TmpC.clear();
			switch (tmp)
			{
			case '{':
			{
				while (tmp != '}')
				{
					tmp = fgetc(SNL_Code);
					if (tmp == '\n')
						line += 1;
				}
				tmp = fgetc(SNL_Code);
			};
			break;
			case '.':
			{
				TmpC += tmp;
				if ((tmp = fgetc(SNL_Code)) == '.') // ��¼'..'
				{
					/*
						INRANGE�����±����״̬
					*/
					TmpC += tmp;
					tokenList[Now_List].line = line;
					tokenList[Now_List].wd.str = TmpC;
					tokenList[Now_List].wd.tok = UNDERANGE;
					Now_List++;
					tmp = fgetc(SNL_Code);
				}
				else // ��¼'.'
				{
					tokenList[Now_List].line = line;
					tokenList[Now_List].wd.str = TmpC;
					tokenList[Now_List].wd.tok = DOT;
					Now_List++;
				}
			};
			break;
			case '\'':
			{
				tokenList[Now_List].line = line;
				tokenList[Now_List].wd.tok = CHARC;
				while ((tmp = fgetc(SNL_Code)) != '\'')
				{//����㣬ԭ��δ֪

					TmpC += tmp;
					cout << tmp << endl;
				}
				tokenList[Now_List].wd.str = TmpC;
				Now_List++;
				tmp = fgetc(SNL_Code);
			};
			break;
			case ':':
			{
				TmpC += tmp;
				if ((tmp = fgetc(SNL_Code)) == '=')
				{
					/*
						INASSIGN��ֵ״̬
					*/
					TmpC += tmp;
					tokenList[Now_List].line = line;
					tokenList[Now_List].wd.str = TmpC;
					tokenList[Now_List].wd.tok = ASSIGN;
					Now_List++;
					tmp = fgetc(SNL_Code);
				}
				else
				{
					tokenList[Now_List].line = line;
					tokenList[Now_List].wd.str = TmpC;
					tokenList[Now_List].wd.tok = COLON;
					Now_List++;

				}
			};
			break;
			default:
			{
				string temp;
				temp += tmp;
				tokenList[Now_List].line = line;
				tokenList[Now_List].wd.str += temp;
				
				if (temp == "(")
					tokenList[Now_List].wd.tok = LPAREN;
				else if (temp == ")")
					tokenList[Now_List].wd.tok = RPAREN;
				else if (temp == "[")
					tokenList[Now_List].wd.tok = LMIDPAREN;
				else if (temp == "]")
					tokenList[Now_List].wd.tok = RMIDPAREN;
				else if (temp == ";")
					tokenList[Now_List].wd.tok = SEMI;
				else if (temp == ",")
					tokenList[Now_List].wd.tok = COMMA;

				Now_List++;
				tmp = fgetc(SNL_Code);
			};
			};
		}
		else if (IsBlank(tmp))
		{

			if (tmp == '\n') // 
				line += 1;
			else
				;
			tmp = fgetc(SNL_Code);
		}
		else
		{
			string ERROR1 = "Line ";
			ERROR1 += to_string(line);
			ERROR1 += ' ';
			ERROR1 += "\"";
			ERROR1 += tmp;
			ERROR1 += "\"";
			ERROR1 += "TOKENERROR UNKOWN EXIST!!!\n";
			printErrorMsg(ERROR1);
			tmp = fgetc(SNL_Code);

		}
	}

}