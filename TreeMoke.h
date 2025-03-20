#pragma once
#include"total.h"
#include"axil.h"


//以下为生成语法树子节点的辅助函数
// check_token为检测出现语法错误的关键，如果出现语法错误会报错，不出现会建树
TreeNode* check_token(LexType tok)
{
	if (tokenList[NOW_LIST].wd.tok == tok)
	{
		TreeNode* now = new TreeNode(tokenList[NOW_LIST].wd.str);
		currentToken = &tokenList[NOW_LIST];
		now->tk = currentToken;
		NOW_LIST++;
		return now;

	}
	else
	{
		//语法错误，弹出错误
		string ERROR1;
		ERROR1 += "SYNTAX ERROR EXIST IN LINE";
		ERROR1 += to_string(currentToken->line);
		printErrorMsg(ERROR1);
		return NULL;
	}
}




//1.program : programhead + declarepart+programbody


TreeNode* program()
{
	TreeNode* head = new TreeNode("Program");
	NOW_LIST = 0;
	head->addChild(programhead());
	head->addChild(declarepart());
	head->addChild(programbody());
	return head;
}
//2. programhead= "PrOGRAM"+ programname
TreeNode* programhead()
{
	TreeNode* programhead = new TreeNode("ProgramHead");
	programhead->addChild(check_token(PROGRAM));
	programhead->addChild(programname());
	return programhead;
}
//3. programname=ID
TreeNode* programname()
{
	TreeNode* programname = check_token(ID);
	return programname;
}
//****************至此程序头已经生成完成
//4.declarepart = typedecpart + vardecpart + procdecpart
TreeNode* declarepart()
{
	TreeNode* declarepart = new TreeNode("DeclarePart");
	declarepart->addChild(typedecpart());
	declarepart->addChild(vardecpart());
	declarepart->addChild(procDecPart());
	return declarepart;
}
//5. typedecpart= NULL//typedec
TreeNode* typedecpart()
{
	if (tokenList[NOW_LIST].wd.tok == TYPE)
	{
		NOW_LIST++;
		currentToken = &tokenList[NOW_LIST];
		TreeNode* now = new TreeNode("TypeDecPart");
		now->addChild(typedec());
		return now;
	}
	else
		return NULL;
}
//6.typedec = TYPE + typedeclist
TreeNode* typedec()
{
	TreeNode* typedec = new TreeNode("TypeDec");
	typedec->addChild(check_token(TYPE));
	typedec->addChild(typedeclist());
	return typedec;
}
//7.typedeclist = typeID = typedef ; typedecmore(关于该类型的详细说明)
TreeNode* typedeclist()
{
	TreeNode* tlist = new TreeNode("TypeDecList");
	tlist->addChild(typeID()));
	tlist->addChild(check_token(EQ));
	tlist->addChild(typeDef());
	tlist->addChild(check_token(SEMI));
	tlist->addChild(typeDecMore());
	return tlist;
}
// typeID = ID
TreeNode* typeID()
{
	return check_token(ID);
}
//8.typedecmore = NULL || typedeclist
TreeNode* typeDecMore()
{
	if (tokenList[NOW_LIST].wd.tok == ID || tokenList[NOW_LIST].wd.tok == INTEGER || tokenList[NOW_LIST].wd.tok == CHAR1 || tokenList[NOW_LIST].wd.tok == ARRAY || tokenList[NOW_LIST].wd.tok == RECORD)
	{
		NOW_LIST++;
		currentToken = tokenList[NOW_LIST];
		TreeNode* tMore = new TreeNode("TypeDecMore");
		tMore->addChild(typedeclist());
		return tMore;
	}
	else
		return NULL;
}//声明部分的类型声明部分结束
//以下为类型部分？
//9.typedef = basetype + structtype + ID
TreeNode* typeDef()
{
	TreeNode* tydef = new TreeNode("TypeDef");
	tydef->addChild(baseType());
	tydef->addChild(structureType());
	tydef->addChild(check_token(ID)));
	return tydef;
}
//10.basetype = CHAR1||INTEGER
TreeNode* baseType()
{
	TreeNode* basetype = new TreeNode("BaseType");
	basetype->addChild(check_token(CHAR1));
	basetype->addChild(check_token(INTEGER));
	return basetype;
}
//11. structtype = arraytype + rectype
TreeNode* structureType()
{
	TreeNode* sttype = new TreeNode("StructureType");
	sttype->addChild(arrayType());
	sttype->addChild(recType());
	return sttype;
}
//12. arraytype = ARRAY[low..top] OF basetype
TreeNode* arrayType()
{
	TreeNode* arrayt = new TreeNode("ArrayType");
	arrayt->addChild(check_token(ARRAY));
	arrayt->addChild(check_token(LMIDPAREN));
	arrayt->addChild(low());
	arrayt->addChild(top());
	arrayt->addChild(check_token(UNDERANGE));
	arrayt->addChild(check_token(OF));
	arrayt->addChild(baseType());
	return arrayt;
}
//13. low = INTC
TreeNode* low()
{
	TreeNode* lo = new TreeNode("Low");
	lo->addChild(check_token(INTC));
	return lo;
}
//14. top = INTC
TreeNode* top()
{
	TreeNode* to = new TreeNode("Top");
	to->addChild(check_token(INTC));
	return to;
}
//15. rectype = RECORD + fielddeclist + END
TreeNode* recType()
{ 
	TreeNode* rect = new TreeNode("RecType");
	rect->addChild(check_token(RECORD));
	rect->addChild(fieldDecList());
	rect->addChild(check_token(END));
	return rect;
}
//16. fielddeclist = basetype + IDlist; fielddecmore|| arraytype IDlist; fielddecmore
TreeNode* fieldDecList()
{
	TreeNode* fdlist = new TreeNode("FieldDecList");
	if(tokenList[NOW_LIST].wd.tok==INTEGER||tokenList[NOW_LIST].wd.tok==CHAR1)
	{
		NOW_LIST++;
		currentToken = &tokenList[NOW_LIST];
		fdlist->addChild(baseType());
		fdlist->addChild(check_token(EQ));
		fdlist->addChild(IDList());
		fdlist->addChild(check_token(SEMI));
		fdlist->addChild(FieldDecMore());
		return fdlist;
	}
	else if (tokenList[NOW_LIST].wd.tok == ARRAY)
	{
		NOW_LIST++;
		currentToken = &tokenList[NOW_LIST];
		fdlist->addChild(arrayType());
		fdlist->addChild(IDList());
		fdlist->addChild(check_token(SEMI));
		fdlist->addChild(fieldDecMore());
		return fdlist;
	}
	else
	{
		string ERROR1;
		ERROR1 += "ERROR EXIST IN LINE ";
		ERROR1 += to_string(tokenList[NOW_LIST].line);
		ERROR1 += " :";
		ERROR1 += tokenList[NOW_LIST].wd.str;
		printErrorMsg(ERROR1);
		return NULL;
	}
}
//17. fielddecmore = NULL||fielddeclist
TreeNode* fieldDecMore()
{
	if (tokenList[NOW_LIST].wd.tok == INTEGER || tokenList[NOW_LIST].wd.tok == CHAR1 || tokenList[NOW_LIST].wd.tok == ARRAY)
	{
		TreeNode* fdmore = new TreeNode("FieldDecMore");
		fdmore->addChild(fieldDecMore());
		return fdmore;
	}
	else
		return NULL;
}
//18. IDlist = ID IDmore
TreeNode* IDList()
{
	TreeNode* IDl = new TreeNode("IDList");
	IDl->addChild(check_token(ID));
	IDl->addChild(IDMore());
	return IDl;
}
//19. IDmore = NULL|| ,IDlist
TreeNode* IDmore()
{
	if (tokenList[NOW_LIST].wd.tok == COMMA)
	{
		NOW_LIST++;
		currentToken = &tokenList[NOW_LIST];
		TreeNode* IDm = new TreeNode("IDMore");
		IDm->addChild(new TreeNode("COMMA"));
		IDm->addChild(IDList());
		return IDm;
	}
	else
		return NULL;
}