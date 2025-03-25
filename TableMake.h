#pragma once
#include"total.h"

void tablemake(TreeNode* root)
{
	if (root == NULL)
	{
		return;
	}
	else
	{
		//root->child[0];
		declapartdeal(root->child[1]);
		procedeal(root->child[2]);
	}
}
void declapartdeal(TreeNode* root)
{
	if (root == NULL)
		return;
	for (int i = 0;root->child[i] != NULL;i++)
	{
		if (root->child[i]->name == "TypeDecPart")
		{
			typetablemake(root->child[i]->child[0]->child[1]);
		}
		else if (root->child[i]->name == "VarDecPart")
		{
			vartablemake(root->child[i]->child[0]->child[1]);
		}
		else if (root->child[i]->name == "ProcDecPart")
		{
			proctablemake(root->child[i]->child[0]);
		}
		else
			cout << root->child[i]->name;
	}
	return;
}
void typetablemake(TreeNode* root)//root = typedeclist
{
	if (root == NULL)
		return;
	string tmp_name;//要重载或者定义的类型名
	TreeNode* active;
	active = root;
	tmp_name = active->child[0]->name;
	TreeNode tmp_remain = active;
	for (int i = 0;i < totaltable.NOW_TYPE;i++)
	{
		if (totaltable.tyk[i].Inner->kind == tmp_name)
		{
			cout << "ERROR:TYPE REDEFINED " + tmp_name << endl;;
			return;
		}
	}
	active = active->child[2];//typedef := basetype ||structtype|| ID
	if (active->child[0]->name == "BaseType"||active->child[0]->tk->wd.tok==ID)
	{//本分支为基本类型与已经存在类型的重载部分
		Typetable* now=totaltable.tyk[totaltable.NOW_TYPE].Inner;
		TreeNode* tmp = active->child[0];
		if (tmp->name == "BaseType")
		{
			totaltable.tyk[totaltable.NOW_TYPE].typekind = "BaseType";
			tmp = tmp->child[0];
			bool tmp_flag = false;
			for (int i = 0;i < totaltable.NOW_TYPE;i++)
			{
				if (totaltable.tyk[i].Inner->kind == tmp->name)
				{
					now = &totaltable.tyk[i];
					tmp_flag = true;
				}
			}
			if (tmp_flag == false)
			{
				cout << "ERROR:TYPE NOT DEFINED " + tmp->name << endl;
				exit;
			}
		}
		else
		{
			bool tmp_flag = false;
			for (int i = 0;i < totaltable.NOW_TYPE;i++)
			{
				if (totaltable.tyk[i].typekind == tmp->name)
				{
					now = &totaltable.tyk[i];
					tmp_flag = true;
				}
			}
			if (tmp_flag == false)
			{
				cout << "ERROR:TYPE NOT DEFINED " + tmp->name << endl;
				exit;
			}
		}
			totaltable.tyk[totaltable.NOW_TYPE].typekind = tmp_name;
	}
	else if (active->child[0]->name == "StructureType")
	{//该分支为数组与记录类型
		active = active->child[0];
		if (active->child[0]->name == "RecType")
		{
			active = active->child[0];//active =rectype
			totaltable.tyk[totaltable.NOW_TYPE].typekind = "RecordType";
			totaltable.tyk[totaltable.NOW_TYPE].Inner->IsRecord = true;
			totaltable.tyk[totaltable.NOW_TYPE].Inner->IsArray = false;
			recordpmake(active->child[1]);//fielddeclist
		}
		else if (active->child[0]->name == "ArrayType")
		{
			active = active->child[0];//active = arraytype
			totaltable.tyk[totaltable.NOW_TYPE].typekind = "ArrayType";
			totaltable.tyk[totaltable.NOW_TYPE].Inner->IsRecord = false;
			totaltable.tyk[totaltable.NOW_TYPE].Inner->IsArray = true;
			arraypmake(active);
		}
	}
	totaltable.NOW_TYPE++;
	//typedecmore(tmp_remain := typelist)
	if(root->child[4]!=NULL)
		typetablemake(root->child[4]->child[0]);
	return;
}
void recordmake(TreeNode* root)//root = fielddeclist
{
	if (root == NULL)
		return;
	TreeNode* active = root->child[1];//active = fielddeclist
	//fielddeclist = basetype  IDlist; fielddecmore|| arraytype IDlist; fielddecmore
	//识别basetype
	TreeNode* tmp0 = active->child[0];//tmp0 = basetype
	TreeNode* tmp1 = active->child[1];//tmp1 = IDlist
	VarTable* now = totaltable.tyk[totaltable.NOW_TYPE].Inner->body;
	bool tmp_flag = false;//true表示integer，false表示char
	int NOW_BODY = &totaltable.tyk[totaltable.NOW_TYPE].Inner->NOW_BODY;
	if (tmp0->name == "integer")
	{
		now[NOW_BODY].inner = totaltable.tyk[0].Inner;
		now[NOW_BODY].access = true;
		tmp_flag = true;
	}
	else if (tmp0->name == "char")
	{
		now[NOW_BODY].inner = totaltable.tyk[1].Inner;
		now[NOW_BODY].access = true;
		tmp_flag = false;
	}
	else
	{
		cout << "UNKOMN ERROR(CODE1) EXIST IN " + tmp0->name << endl;
	}
	//IDlist = ID IDmpre
	//IDmore = NULL|| ,IDlist
	now[NOW_BODY].name = tmp1->child[0]->name;
	while (tmp1->child[1] != NULL)
	{//tmp1始终为IDlist
		tmp1 = tmp1->child[1]->child[1];
		NOW_BODY++;
		if (tmp_flag)
		{
			now[NOW_BODY].inner = totaltable.tyk[0].Inner;
			now[NOW_BODY].access = true;
		}
		else
		{
			now[NOW_BODY].inner = totaltable.tyk[1].Inner;
			now[NOW_BODY].access = true;
		}
		now[NOW_BODY].name = tmp1->child[0]->name;
	}
	NOW_BODY++;
	if(active->child[2]!=NULL)
	recordmake(active->child[2]->child[0]);//fielddecmore
}
void arraymake(TreeNode* root)//arraytype = ARRAY [low..top] OF basetype
{
	if (root == NULL)
		return;
	TreeNode* tmp0 = root->child[2]->child[0];//low
	TreeNode* tmp1 = root->child[4]->child[0];//top
	TreeNode* tmp2 = root->child[7]->child[0];//basetype
	TypeTable* now = totaltable.tyk[totaltable.NOW_TYPE].Inner->elemTy;
	if (tmp2->name == "integer")
		now = totaltable.tyk[0];
	else if(tmp2->name=="char")
		now = totaltable.tyk[1];
	else
		cout << "UNKOMN ERROR(CODE2) EXIST IN " + tmp2->name << endl;
	int st = 0;
	int ed = 0;
	for (auto tmp : tmp1->name)
	{
		st = st * 10 + tmp - '0';
	}
	for (auto tmp : tmp2->name)
	{
		ed = ed * 10 + tmp - '0';
	}
	totaltable.tyk[totaltable.NOW_TYPE].Inner->start = st;
	totaltable.tyk[totaltable.NOW_TYPE].Inner->end = ed;
	return;
}