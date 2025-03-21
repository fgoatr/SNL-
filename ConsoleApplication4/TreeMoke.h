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




//1.program : programhead declarepart programbody


TreeNode* program()
{
	TreeNode* head = new TreeNode("Program");
	NOW_LIST = 0;
	head->addChild(programhead());
	head->addChild(declarepart());
	head->addChild(programbody());
	return head;
}
//2. programhead= PROGRAM programname
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
//4.declarepart = typedecpart vardecpart procdecpart
TreeNode* declarepart()
{
	TreeNode* declarepart = new TreeNode("DeclarePart");
	declarepart->addChild(typedecpart());
	declarepart->addChild(vardecpart());
	declarepart->addChild(procDecPart());
	return declarepart;
}
//5. typedecpart= NULL||typedec
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
//6.typedec = TYPE typedeclist
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
	tlist->addChild(typeID());
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
	if (tokenList[NOW_LIST].wd.tok == ID )
	{
		TreeNode* tMore = new TreeNode("TypeDecMore");
		tMore->addChild(typedeclist());
		return tMore;
	}
	else
		return NULL;
}//声明部分的类型声明部分结束
//以下为类型部分？
//9.typedef = basetype structtype ID
TreeNode* typeDef()
{
	TreeNode* tydef = new TreeNode("TypeDef");
	tydef->addChild(baseType());
	tydef->addChild(structureType());
	tydef->addChild(check_token(ID));
	return tydef;
}
//10.basetype = CHAR1||INTEGER
TreeNode* baseType()
{
	TreeNode* basetype = new TreeNode("BaseType");
	if(tokenList[NOW_LIST].wd.tok==CHAR1)
	basetype->addChild(check_token(CHAR1));
	else if (tokenList[NOW_LIST].wd.tok == INTEGER)
	basetype->addChild(check_token(INTEGER));
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
	return basetype;
}
//11. structtype = arraytype rectype
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
//15. rectype = RECORD fielddeclist END
TreeNode* recType()
{ 
	TreeNode* rect = new TreeNode("RecType");
	rect->addChild(check_token(RECORD));
	rect->addChild(fieldDecList());
	rect->addChild(check_token(END1));
	return rect;
}
//16. fielddeclist = basetype  IDlist; fielddecmore|| arraytype IDlist; fielddecmore
TreeNode* fieldDecList()
{
	TreeNode* fdlist = new TreeNode("FieldDecList");
	if(tokenList[NOW_LIST].wd.tok==INTEGER||tokenList[NOW_LIST].wd.tok==CHAR1)
	{
		fdlist->addChild(baseType());
		fdlist->addChild(IDList());
		fdlist->addChild(check_token(SEMI));
		fdlist->addChild(fieldDecMore());
		return fdlist;
	}
	else if (tokenList[NOW_LIST].wd.tok == ARRAY)
	{
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
TreeNode* IDMore()
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
//20. vardecpart = NULL||vardec
TreeNode* vardecpart()
{
	if (tokenList[NOW_LIST].wd.tok == VAR)
	{
		TreeNode* vdp = new TreeNode("VarDecPart");
		vdp->addChild(varDec());
		return vdp;
	}
	else
		return NULL;
}
//21. vardec = VAR vardeclist
TreeNode* varDec()
{
	TreeNode* vdec = new TreeNode("VarDec");
	vdec->addChild(check_token(VAR));
	vdec->addChild(varDecList());
	return vdec;
}
//22. vardeclist = typedef varidlist; vardecmore
TreeNode* varDecList()
{
	TreeNode* vdl = new TreeNode("VarDecList");
	vdl->addChild(typeDef());
	vdl->addChild(varIDList());
	vdl->addChild(check_token(SEMI));
	vdl->addChild(varDecMore());
	return vdl;
}
//23. vardecmore = NULL||vardeclist
TreeNode* varDecMore()
{
	if (tokenList[NOW_LIST].wd.tok == ID)
	{
		TreeNode* vdm = new TreeNode("VarDecMore");
		vdm->addChild(varDecList());
		return vdm;
	}
	else
		return NULL;
}
//24. varIDlist=ID varIDmore
TreeNode* varIDlist()
{
	TreeNode* vIDL= new TreeNode("VarIDList");
	vIDL->addChild(check_token(ID));
	vIDL->addChild(varIDMore());
	return vIDL;
}
//25. varIDmore = NULL||,varIDlist
TreeNode* varIDMore()
{
	if (tokenList[NOW_LIST].wd.tok == COMMA)
	{
		NOW_LIST++;
		currentToken = &tokenList[NOW_LIST];
		TreeNode* vIDm = new TreeNode("VarIDMore");
		vIDm->addChild(check_token(COMMA));
		vIDm->addChild(varIDlist());
		return vIDm;
	}
	else
		return NULL;
}
//26. prodecpart = NULL||procdec
TreeNode* procDecPart()
{
	if (tokenList[NOW_LIST].wd.tok == PROCEDURE)
	{
		TreeNode* pdp = new TreeNode("ProcDecPart");
		pdp->addChild(procDec());
		return pdp;
	}
	else
		return NULL;
}
//27. prodec = PRODECURE procname(paramlist);prodecpart procbody procmore
TreeNode* procDec()
{
	TreeNode* pd = new TreeNode("ProcDec");
	pd->addChild(check_token(PROCEDURE));
	pd->addChild(procName());
	pd->addChild(check_token(LPAREN));
	pd->addChild(paramList());
	pd->addChild(check_token(RPAREN));
	pd->addChild(check_token(SEMI));
	pd->addChild(procDecPart());
	pd->addChild(procBody());
	pd->addChild(procDecMore());
	return pd;
}
//28. prodecmore = NULL||procdec
TreeNode* procDecMore()
{
	if (tokenList[NOW_LIST].wd.tok == PROCEDURE)
	{
		TreeNode* pdm = new TreeNode("ProcDecMore");
		pdm->addChild(procDec());
		return pdm;
	}
	else
		return NULL;
}
//29. procname = ID
TreeNode* procName()
{
	return check_token(ID);
}
//30. paramlist = NULL||paramdeclist
TreeNode* paramList()
{
	if (tokenList[NOW_LIST].wd.tok == INTEGER || tokenList[NOW_LIST].wd.tok == CHAR1 || tokenList[NOW_LIST].wd.tok == ARRAY || tokenList[NOW_LIST].wd.tok == ID)
	{
		TreeNode* pl = new TreeNode("ParamList");
		pl->addChild(paramDecList());
		return pl;
	}
	else
		return NULL;
}
//31. paramdeclist = param parammore
TreeNode* paramDecList()
{
	TreeNode* pdl = new TreeNode("ParamDecList");
	pdl->addChild(param());
	pdl->addChild(paramMore());
	return pdl;
}
//32. param = typedef formlist||VAR typedef formlist
TreeNode* param()
{
	TreeNode* p = new TreeNode("Param");
	if (tokenList[NOW_LIST].wd.tok == VAR)
	{
		p->addChild(check_token(VAR));
		p->addChild(typeDef());
		p->addChild(formList());
	}
	else if(tokenList[NOW_LIST].wd.tok==CHAR1||tokenList[NOW_LIST].wd.tok==INTEGER)
	{
		p->addChild(typeDef());
		p->addChild(formList());	
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
	return p;
}
//33. formList = ID fidmore
TreeNode* formList()
{
	TreeNode* fl = new TreeNode("FormList");
	fl->addChild(check_token(ID));
	fl->addChild(fidMore());
	return fl;
}
//34. fidmore = NULL||,formlist
TreeNode* fidMore()
{
	if (tokenList[NOW_LIST].wd.tok == COMMA)
	{
		TreeNode* fm = new TreeNode("FidMore");
		fm->addChild(check_token(COMMA));
		fm->addChild(formList());
		return fm;
	}
	else
		return NULL;
}
//35. procdecpart = declarepart
TreeNode* procDecPart()
{
	TreeNode* pdp = new TreeNode("ProcDecPart");
	pdp->addChild(declarepart());
	return pdp;
}
//36. procbody = programbody
TreeNode* procBody()
{
	TreeNode* pb = new TreeNode("ProcBody");
	pb->addChild(programbody());
	return pb;
}
//37. programbody = BEGIN stmList END
TreeNode* programbody()
{
	TreeNode* pb = new TreeNode("ProgramBody");
	pb->addChild(check_token(BEGIN));
	pb->addChild(stmList());
	pb->addChild(check_token(END1));
	return pb;
}
//38. stmList = stm stmmore
TreeNode* stmList()
{
	TreeNode* sl = new TreeNode("StmList");
	sl->addChild(stm());
	sl->addChild(stmMore());
	return sl;
}
///39. stmmore = NULL||; stmList
TreeNode* stmMore()
{
	if (tokenList[NOW_LIST].wd.tok == SEMI)
	{
		TreeNode* sm = new TreeNode("StmMore");
		sm->addChild(check_token(SEMI));
		sm->addChild(stmList());
		return sm;
	}
	else
		return NULL;
}
//40. stm = conditionalsum||loopstm||inputstm||outputstm||returnstm||ID asscall
TreeNode* stm()
{
	TreeNode* s = new TreeNode("Stm");
	if (tokenList[NOW_LIST].wd.tok == IF)
	{
		s->addChild(conditionalStm());
	}
	else if (tokenList[NOW_LIST].wd.tok == WHILE)
	{
		s->addChild(loopStm());
	}
	else if (tokenList[NOW_LIST].wd.tok == READ)
	{
		s->addChild(inputStm());
	}
	else if (tokenList[NOW_LIST].wd.tok == WRITE)
	{
		s->addChild(outputStm());
	}
	else if (tokenList[NOW_LIST].wd.tok == RETURN1)
	{
		s->addChild(returnStm());
	}
	else if (tokenList[NOW_LIST].wd.tok == ID)
	{
		s->addChild(check_token(ID));
		s->addChild(assCall());
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
	return s;
}
//41. asscall = assignmentrest||callstmrest
TreeNode* assCall()
{
	TreeNode* ac = new TreeNode("AssCall");
	if (tokenList[NOW_LIST].wd.tok ==1 );//待续
}