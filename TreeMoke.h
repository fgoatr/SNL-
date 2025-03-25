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
		ERROR1 += "\t";
		ERROR1 += to_string(tok);
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
//9.typedef = basetype ||structtype|| ID
TreeNode* typeDef()
{
	TreeNode* tydef = new TreeNode("TypeDef");
	if (tokenList[NOW_LIST].wd.tok == CHAR1 || tokenList[NOW_LIST].wd.tok == INTEGER)
		tydef->addChild(baseType());
	else if (tokenList[NOW_LIST].wd.tok == ARRAY || tokenList[NOW_LIST].wd.tok == RECORD)
		tydef->addChild(structureType());
	else if (tokenList[NOW_LIST].wd.tok == ID)
		tydef->addChild(check_token(ID));
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
//11. structtype = arraytype ||rectype
TreeNode* structureType()
{
	TreeNode* sttype = new TreeNode("StructureType");
	if(tokenList[NOW_LIST].wd.tok==ARRAY)
		sttype->addChild(arrayType());
	else if(tokenList[NOW_LIST].wd.tok==RECORD)
		sttype->addChild(recType());
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
	return sttype;
}
//12. arraytype = ARRAY[low..top] OF basetype
TreeNode* arrayType()
{
	TreeNode* arrayt = new TreeNode("ArrayType");
	arrayt->addChild(check_token(ARRAY));
	arrayt->addChild(check_token(LMIDPAREN));
	arrayt->addChild(low());
	arrayt->addChild(check_token(UNDERANGE));
	arrayt->addChild(top());
	arrayt->addChild(check_token(RMIDPAREN));
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
		fdmore->addChild(fieldDecList());
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
		TreeNode* IDm = new TreeNode("IDMore");
		IDm->addChild(check_token(COMMA));
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
	if (tokenList[NOW_LIST].wd.tok == ID ||
		tokenList[NOW_LIST].wd.tok == INTEGER ||
		tokenList[NOW_LIST].wd.tok == CHAR1 ||
		tokenList[NOW_LIST].wd.tok == ARRAY ||
		tokenList[NOW_LIST].wd.tok == RECORD)
	{
		TreeNode* vdm = new TreeNode("VarDecMore");
		vdm->addChild(varDecList());
		return vdm;
	}
	else
		return NULL;
}
//24. varIDlist=ID varIDmore
TreeNode* varIDList()
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
		TreeNode* vIDm = new TreeNode("VarIDMore");
		vIDm->addChild(check_token(COMMA));
		vIDm->addChild(varIDList());
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
//27. prodec = PRODECURE procname(paramlist);(书中原文是prodecpart，但此prodecpart与之前的重定义；1改为)declarepart  procbody procmore
TreeNode* procDec()
{
	TreeNode* pd = new TreeNode("ProcDec");
	pd->addChild(check_token(PROCEDURE));
	pd->addChild(procName());
	pd->addChild(check_token(LPAREN));
	pd->addChild(paramList());
	pd->addChild(check_token(RPAREN));
	pd->addChild(check_token(SEMI));
	pd->addChild(declarepart());
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
	if (tokenList[NOW_LIST].wd.tok == INTEGER ||
		tokenList[NOW_LIST].wd.tok == CHAR1 ||
		tokenList[NOW_LIST].wd.tok == ARRAY ||
		tokenList[NOW_LIST].wd.tok == ID||
		tokenList[NOW_LIST].wd.tok == VAR)
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
	else if (tokenList[NOW_LIST].wd.tok == CHAR1 ||
		tokenList[NOW_LIST].wd.tok == INTEGER ||
		tokenList[NOW_LIST].wd.tok == ARRAY ||
		tokenList[NOW_LIST].wd.tok == RECORD ||
		tokenList[NOW_LIST].wd.tok == ID)
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
/*TreeNode* procDecPart()
{
	TreeNode* pdp = new TreeNode("ProcDecPart");
	pdp->addChild(declarepart());
	return pdp;
}*/
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
//40. stm = conditionalsum||loopstm||inputstm||outputstm||returnstm||ID asscall||NULL(书中疑似缺了这一点)
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
		return NULL;
	}
	return s;
}
//41. asscall = assignmentrest||callstmrest
TreeNode* assCall()
{
	TreeNode* ac = new TreeNode("AssCall");
	if (tokenList[NOW_LIST].wd.tok == LMIDPAREN ||
		tokenList[NOW_LIST].wd.tok == DOT ||
		tokenList[NOW_LIST].wd.tok == ASSIGN)
	{
		ac->addChild(assignmentRest());
		return ac;
	}
	else if(tokenList[NOW_LIST].wd.tok== LPAREN)
	{ 
		ac->addChild(callStmRest());
		return ac;
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
//42. assignmentrest = varimore := exp
TreeNode* assignmentRest()
{ 
	TreeNode* asr = new TreeNode("AssignMentRest");
	asr->addChild(variMore());
	asr->addChild(check_token(ASSIGN));
	asr->addChild(exp());
	return asr;
}
//43. conditionalstm = IF relexp THEN stmlist ELSE stmlist FI
TreeNode* conditionalStm()
{
	TreeNode* cds = new TreeNode("ConditionalStm");
	cds->addChild(check_token(IF));
	cds->addChild(relExp());
	cds->addChild(check_token(THEN));
	cds->addChild(stmList());
	cds->addChild(check_token(ELSE));
	cds->addChild(stmList());
	cds->addChild(check_token(FI));
	return  cds;
}
//44. loopstm = WHILE relexp DO stmlist ENDWH
TreeNode* loopStm()
{
	TreeNode* lps = new TreeNode("LoopStm");
	lps->addChild(check_token(WHILE));
	lps->addChild(relExp());
	lps->addChild(check_token(DO));
	lps->addChild(stmList());
	lps->addChild(check_token(ENDWH));
	return lps;
}
//45. inputstm = READ (invar)
TreeNode* inputStm()
{
	TreeNode* istm = new TreeNode("InputStm");
	istm->addChild(check_token(READ));
	istm->addChild(check_token(LPAREN));
	istm->addChild(inVar());
	istm->addChild(check_token(RPAREN));
	return istm;
}
//46. invar = ID
TreeNode* inVar()
{
	TreeNode* IV = new TreeNode("InVar");
	IV->addChild(check_token(ID));
	return IV;
}
//47. outputstm = WRITE(exp)
TreeNode* outputStm()
{
	TreeNode* ops = new TreeNode("OutputStm");
	ops->addChild(check_token(WRITE));
	ops->addChild(check_token(LPAREN));
	ops->addChild(exp());
	ops->addChild(check_token(RPAREN));
	return ops;
}
//48. returnstm = RETUARN
TreeNode* returnStm()
{
	TreeNode* rts = new TreeNode("ReturnStm");
	rts->addChild(check_token(RETURN1));
	return rts;
}
//49. callstmrest = (actparamlist)
TreeNode* callStmRest()
{
	TreeNode* csr = new TreeNode("CallStmRest");
	csr->addChild(check_token(LPAREN));
	csr->addChild(actparamList());
	csr->addChild(check_token(RPAREN));
	return csr;
}
//50. actparamlist = NULL||exp actparammore
TreeNode* actparamList()
{
	if (tokenList[NOW_LIST].wd.tok == LPAREN ||
		tokenList[NOW_LIST].wd.tok == INTC ||
		tokenList[NOW_LIST].wd.tok == ID)
	{
		TreeNode* apl = new TreeNode("ActParamList");
		apl->addChild(exp());
		apl->addChild(actparamMore());
		return apl;
	}
	else
		return NULL;
}
//51. actparammore = NULL||, actparamlist
TreeNode* actparamMore()
{
	if (tokenList[NOW_LIST].wd.tok == COMMA)
	{
		TreeNode* apm = new TreeNode("ActParamMore");
		apm->addChild(check_token(COMMA));
		apm->addChild(actparamList());
		return apm;
	}
	else
		return NULL;
}
//52. relexp = exp otherrele
TreeNode* relExp()
{
	TreeNode* re = new TreeNode("RelExp");
	re->addChild(exp());
	re->addChild(otherRelE());
	return re;
}
//53. otherrele = cmpop exp
TreeNode* otherRelE()
{
	TreeNode* ore = new TreeNode("OtherRelE");
	ore->addChild(cmpOp());
	ore->addChild(exp());
	return ore;
}
//54. exp = term otherterm
TreeNode* exp()
{
	TreeNode* ex = new TreeNode("Exp");
	ex->addChild(term());
	ex->addChild(otherTerm());
	return ex;
}
//55. otherterm = NULL||addop exp
TreeNode* otherTerm()
{
	if (tokenList[NOW_LIST].wd.tok == PLUS ||
		tokenList[NOW_LIST].wd.tok == MINUS)
	{
		TreeNode* ot = new TreeNode("OtherTerm");
		ot->addChild(addOp());
		ot->addChild(exp());
		return ot;
	}
	else
		return NULL;
}
//56. term = factor otherfactor
TreeNode* term()
{
	TreeNode* t = new TreeNode("Term");
	t->addChild(factor());
	t->addChild(otherFactor());
	return t;
}
//57. otherfactor = NULL||multop term
TreeNode* otherFactor()
{
	if (tokenList[NOW_LIST].wd.tok == TIMES ||
		tokenList[NOW_LIST].wd.tok == OVER)
	{
		TreeNode* othf = new TreeNode("OtherFactor");
		othf->addChild(multOp());
		othf->addChild(term());
		return othf;
	}
	else
		return NULL;
}
//58. factor = (exp)||INTC||variable
TreeNode* factor()
{
	TreeNode* fa = new TreeNode("Factor");
	if (tokenList[NOW_LIST].wd.tok == LPAREN)
	{
		fa->addChild(check_token(LPAREN));
		fa->addChild(exp());
		fa->addChild(check_token(RPAREN));
		return fa;
	}
	else if (tokenList[NOW_LIST].wd.tok == INTC)
	{
		fa->addChild(check_token(INTC));
		return fa;
	}
	else if (tokenList[NOW_LIST].wd.tok == ID)
	{
		fa->addChild(variable());
		return fa;
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
//59. variable = ID fieldvarmore
TreeNode* variable()
{
	TreeNode* varb = new TreeNode("Variable");
	varb->addChild(check_token(ID));
	varb->addChild(fieldVarMore());
	return varb;
}
//60. varimore = NULL||[exp]||.fieldvar
TreeNode* variMore()
{
	TreeNode* vm = new TreeNode("VariMore");
	if (tokenList[NOW_LIST].wd.tok == LMIDPAREN)
	{
		vm->addChild(check_token(LMIDPAREN));
		vm->addChild(exp());
		vm->addChild(check_token(RMIDPAREN));
	}
	else if (tokenList[NOW_LIST].wd.tok == DOT)
	{
		vm->addChild(check_token(DOT));
		vm->addChild(fieldVar());
		return vm;
	}
	else
		return NULL;
}
//61. fieldvar = ID fieldvarmore
TreeNode* fieldVar()
{
	TreeNode* fv = new TreeNode("FieldVar");
	fv->addChild(check_token(ID));
	fv->addChild(fieldVarMore());
	return fv;
}
//62. fieldvarmore = NULL||[exp]
TreeNode* fieldVarMore()
{
	if (tokenList[NOW_LIST].wd.tok == LMIDPAREN)
	{
		TreeNode* fvm = new TreeNode("FieldVarMore");
		fvm->addChild(check_token(LMIDPAREN));
		fvm->addChild(exp());
		fvm->addChild(check_token(RMIDPAREN));
		return fvm;
	}
	else
		return NULL;
}
//63. cmpop = <||=
TreeNode* cmpOp()
{
	TreeNode* co = new TreeNode("CmpOp");
	if (tokenList[NOW_LIST].wd.tok == LT)
	{
		co->addChild(check_token(LT));
		return co;
	}
	else if (tokenList[NOW_LIST].wd.tok == EQ)
	{
		co->addChild(check_token(EQ));
		return co;
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
//64. addop = +||/
TreeNode* addOp()
{
	TreeNode* ao = new TreeNode("AddOp");
	if (tokenList[NOW_LIST].wd.tok == PLUS)
	{
		ao->addChild(check_token(PLUS));
		return ao;
	}
	else if (tokenList[NOW_LIST].wd.tok == MINUS)
	{
		ao->addChild(check_token(MINUS));
		return ao;
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
//63. multop = *||/
TreeNode* multOp()
{
	TreeNode* mo = new TreeNode("MutiOp");
	if (tokenList[NOW_LIST].wd.tok == TIMES)
	{
		mo->addChild(check_token(TIMES));
		return mo;
	}
	else if (tokenList[NOW_LIST].wd.tok == OVER)
	{
		mo->addChild(check_token(OVER));
		return mo;
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
//缺失： parammore = NULL||; paramdeclist
TreeNode* paramMore()
{
	TreeNode* pm = new TreeNode("ParamMore");
	if (tokenList[NOW_LIST].wd.tok == SEMI)
	{
		pm->addChild(check_token(SEMI));
		pm->addChild(paramDecList());
		return pm;
	}
	else
		return  NULL;
}